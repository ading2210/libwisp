#include "packet.hpp"

WispPacket::WispPacket(uint8_t type, uint32_t stream_id, WispBuffer* payload) {
  this->type = type;
  this->stream_id = stream_id;
  this->payload = payload;
}
WispPacket::WispPacket(WispBuffer* buffer) {
  this->type = buffer->get_uint8(0);
  this->stream_id = buffer->get_uint32(1);
  this->payload = buffer->copy_buffer(5);
}
WispBuffer* WispPacket::pack() {
  WispBuffer* buffer = new WispBuffer(this->header_size + this->payload->len);
  buffer->set_uint8(0, this->type);
  buffer->set_uint32(1, this->stream_id);
  buffer->set_buffer(5, this->payload);
  return buffer;
}
WispPacket::~WispPacket() {
  delete this->payload;
}

ConnectPayload::ConnectPayload(uint8_t stream_type, uint16_t dest_port, WispBuffer* hostname) {
  this->stream_type = stream_type;
  this->dest_port = dest_port;
  this->hostname = hostname;
}
ConnectPayload::ConnectPayload(WispBuffer* buffer) {
  this->stream_type = buffer->get_uint8(0);
  this->dest_port = buffer->get_uint16(1);
  this->hostname = buffer->copy_buffer(3);
}
WispBuffer* ConnectPayload::pack() {
  WispBuffer* buffer = new WispBuffer(this->header_size + this->hostname->len);
  buffer->set_uint8(0, this->stream_type);
  buffer->set_uint16(1, this->dest_port);
  buffer->set_buffer(3, this->hostname);
  return buffer;
}
ConnectPayload::~ConnectPayload() {
  delete this->hostname;
}

ContinuePayload::ContinuePayload(uint32_t buffer_remaining) {
  this->buffer_remaining = buffer_remaining;
}
ContinuePayload::ContinuePayload(WispBuffer* buffer) {
  this->buffer_remaining = buffer->get_uint32(0);
}
WispBuffer* ContinuePayload::pack() {
  WispBuffer* buffer = new WispBuffer(header_size);
  buffer->set_uint32(0, this->buffer_remaining);
  return buffer;
}

ClosePayload::ClosePayload(uint8_t close_reason) {
  this->close_reason = close_reason;
}
ClosePayload::ClosePayload(WispBuffer* buffer) {
  this->close_reason = buffer->get_uint8(0);
}
WispBuffer* ClosePayload::pack() {
  WispBuffer* buffer = new WispBuffer(this->header_size);
  buffer->set_uint8(0, this->close_reason);
  return buffer;
}
