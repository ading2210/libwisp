#include <ixwebsocket/IXWebSocket.h>

#include "wispconnection.hpp"

WispConnection::WispConnection(std::string url) {
  this->url = url;
  this->ws.setUrl(url);
  this->ws.setOnMessageCallback([this](const ix::WebSocketMessagePtr& msg){
    this->on_ws_msg(msg);
  });
}

void WispConnection::on_ws_msg(const ix::WebSocketMessagePtr& msg) {
  printf("ws message received\n");
}