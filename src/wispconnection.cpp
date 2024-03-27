#include <cstdint>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXWebSocketMessageType.h>

#include "wispconnection.hpp"
#include "wispbuffer.hpp"
#include "wisppacket.hpp"

WispStream::WispStream(WispConnection* connection, uint8_t type, uint32_t stream_id) {
  this->connection = connection;
  this->type = type;
  this->stream_id = stream_id;
}

void WispStream::close(uint8_t reason) {
  this->connection->close_stream(this->stream_id, reason);
}

void WispStream::send(WispBuffer* data) {
  if (this->buffer_remaining > 0) {
    this->send_now(data);
    this->buffer_remaining --;
  }
  else {
    WispBuffer* buffer = new WispBuffer(data->len);
    buffer->set_buffer(0, data);
    this->send_queue.push(buffer);
  }
}

void WispStream::on_continue(uint32_t buffer_remaining) {
  this->buffer_remaining = buffer_remaining;
  while (this->buffer_remaining > 0 && !this->send_queue.empty()) {
    WispBuffer* data = this->send_queue.front();
    this->send_queue.pop();
    this->send_now(data);
    delete data;
  }
}

void WispStream::send_now(WispBuffer* data) {
  WispPacket packet = WispPacket(PacketTypes::DATA, this->stream_id, data);
  WispBuffer* buffer = packet.pack();
  this->connection->ws_send(buffer);
  delete buffer;
}

WispConnection::WispConnection(std::string url) {
  this->url = url;
  this->ws.setUrl(url);
  this->ws.setOnMessageCallback([this](const ix::WebSocketMessagePtr& msg){
    this->on_ws_event(msg);
  });
}

void WispConnection::connect() {
  this->ws.start();
}

void WispConnection::close() {
  this->ws.close();
  this->open = false;
  this->cleanup_all();
}

void WispConnection::cleanup_all() {
  std::map<uint32_t, WispStream*>::iterator iterator = this->streams.begin();
  std::vector<uint32_t> stream_ids;
  while (iterator != this->streams.end()) {
    stream_ids.push_back(iterator->first);
  }
  for (uint32_t& stream_id : stream_ids) {
    this->close_stream(stream_id, 0x03);
  }
}

void WispConnection::close_stream(uint32_t stream_id, uint8_t close_reason) {
  ClosePayload payload = ClosePayload(close_reason);
  WispPacket packet = WispPacket(PacketTypes::CLOSE, stream_id, payload.pack());
  WispBuffer* buffer = packet.pack();
  this->ws_send(buffer);
  delete buffer;
  this->cleanup_stream(stream_id, close_reason);
}

void WispConnection::cleanup_stream(uint32_t stream_id, uint8_t close_reason) {
  if (this->streams.count(stream_id) == 0) {
    return;
  }
  WispStream* stream = streams[stream_id];
  stream->on_close(close_reason);
  this->streams.erase(stream_id);
  delete stream;
}

void WispConnection::ws_send(WispBuffer* buffer) {
  this->ws.sendBinary(std::string(buffer->content, buffer->len));
}

void WispConnection::on_ws_event(const ix::WebSocketMessagePtr& msg) {
  if (msg->type == ix::WebSocketMessageType::Message) {
    this->on_ws_msg(msg);
  }
  else if (msg->type == ix::WebSocketMessageType::Close) {
    this->on_ws_close(msg);
  }
  else if (msg->type == ix::WebSocketMessageType::Error) {
    this->on_ws_close(msg);
  }
}

void WispConnection::on_ws_msg(const ix::WebSocketMessagePtr& msg) {
  if (!msg->binary) {
    return;
  }
  
  //todo: validate packet lengths
  WispBuffer* buffer = new WispBuffer(msg->str.size(), (char*) msg->str.c_str());
  WispPacket* packet = new WispPacket(buffer);
  free(buffer);

  //stream does not exist
  if (this->streams.count(packet->stream_id) == 0) {
    //todo: put console warning here?
  }

  //handle first continue packet
  else if (packet->type == PacketTypes::CONTINUE && packet->stream_id == 0) {
    ContinuePayload payload = ContinuePayload(packet->payload);
    this->buffer_size = payload.buffer_remaining;
    this->on_open();
  }

  //regular packets
  else {
    WispStream* stream = streams[packet->stream_id];

    if (packet->type == PacketTypes::DATA) {
      stream->on_message(packet->payload);
    }

    else if (packet->type == PacketTypes::CONTINUE) {
      ContinuePayload payload = ContinuePayload(packet->payload);
      stream->on_continue(payload.buffer_remaining);
    }

    else if (packet->type == PacketTypes::CLOSE) {
      ClosePayload payload = ClosePayload(packet->payload);
      this->cleanup_stream(packet->stream_id, payload.close_reason);
    }
  }

  delete packet;
}

void WispConnection::on_ws_close(const ix::WebSocketMessagePtr& msg) {
  this->cleanup_all();
}