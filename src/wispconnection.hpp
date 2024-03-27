#ifndef _WISP_CONNECTION_H_
#define _WISP_CONNECTION_H_

#include <queue>
#include <ixwebsocket/IXWebSocket.h>
#include "wispbuffer.hpp"

class WispConnection;
class WispStream {
  public:
  uint32_t buffer_remaining;
  uint8_t type;
  uint32_t stream_id;
  std::function<void(WispBuffer*)> on_message;
  std::function<void*(int)> on_close;
  std::queue<WispBuffer*> send_queue;
  WispConnection* connection;

  WispStream(WispConnection* connection, uint8_t type, uint32_t stream_id);
  void close(uint8_t reason = 0x01);
  void send(WispBuffer* buffer);
  void on_continue(uint32_t buffer_remaining);

  private:
  void send_now(WispBuffer* buffer);
};

class WispConnection {
  public:
  bool open = false;
  ix::WebSocket ws;
  std::string url;
  std::map<uint32_t, WispStream*> streams;
  uint32_t buffer_size;
  WispConnection(std::string url);
  WispStream create_stream(std::string hostname, uint16_t port, uint8_t type = 0x01);
  std::function<void*(void)> on_open;
  std::function<void*(int)> on_close;
  void close();
  void close_stream(uint32_t stream_id, uint8_t close_reason = 0x01);
  void connect();
  void ws_send(WispBuffer* buffer);

  private:
  void cleanup_all();
  void cleanup_stream(uint32_t stream_id, uint8_t close_reason = 0x01);
  bool stream_exists(uint32_t stream_id);
  void on_ws_event(const ix::WebSocketMessagePtr& msg);
  void on_ws_msg(const ix::WebSocketMessagePtr& msg);
  void on_ws_close(const ix::WebSocketMessagePtr& msg);
};

#endif