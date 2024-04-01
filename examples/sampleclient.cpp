#include <chrono>
#include <cstdio>
#include <functional>
#include <thread>

#include "wisp/buffer.hpp"
#include "wisp/client.hpp"

int main() {
  bool signal = false;
  WispClient* client = new WispClient("ws://localhost:5001");
  client->on_open = std::function<void(void)>([&signal, &client]{
    printf("connection opened!\n");
    WispStream* stream = client->create_stream("alicesworld.tech", 80);
    std::string payload = "GET / HTTP/1.1\r\nHost: alicesworld.tech\r\nConnection: keepalive\r\n\r\n";
    stream->on_message = std::function<void(WispBuffer*)>([&signal, &stream](WispBuffer* buffer) {
      printf("%s\n", buffer->content);
    });
    printf("sending\n");
    stream->send(payload);
  });
  client->on_close = std::function<void(int)>([&signal, &client](int error){
    signal = true;
    printf("connection closed with code %i\n", error);
  });
  client->connect();
  

  while (!signal) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  return 0;
}