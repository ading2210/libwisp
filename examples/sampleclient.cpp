#include <chrono>
#include <thread>

#include "wisp/client.hpp"

int main() {
  bool signal = false;
  WispClient* client = new WispClient("ws://localhost:5001");
  WispStream* stream = client->create_stream("ading.dev", 443);

  while (!signal) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  return 0;
}