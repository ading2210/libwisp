#include <ixwebsocket/IXWebSocket.h>

class WispConnection {
  public:
  ix::WebSocket ws;
  std::string url;

  WispConnection(std::string url);

  void on_ws_msg(const ix::WebSocketMessagePtr& msg);
};