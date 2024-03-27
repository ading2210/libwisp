#include <cstdlib>
#include <cstdint>
#include <cstring>

#include "wispbuffer.hpp"

class WispPacket {
  public:
  static const size_t header_size = sizeof(uint8_t) + sizeof(uint32_t);
  uint8_t type;
  uint32_t stream_id;
  WispBuffer* payload;

  WispPacket(uint8_t type, uint32_t stream_id, WispBuffer* payload);
  WispPacket(WispBuffer* buffer);
  WispBuffer* pack();
  void cleanup();
};

class WispPayload {
  public:
  virtual WispBuffer* pack();
};

class ConnectPayload: public WispPayload {
  public:
  static const size_t header_size = sizeof(uint8_t) + sizeof(uint16_t);
  uint8_t stream_type;
  uint16_t dest_port;
  WispBuffer* hostname;

  ConnectPayload(uint8_t stream_type, uint16_t dest_port, WispBuffer* hostname);
  ConnectPayload(WispBuffer* buffer);
  WispBuffer* pack();
};

class ContinuePayload: public WispPayload {
  public:
  static const size_t header_size = sizeof(uint32_t);
  uint32_t buffer_remaining;

  ContinuePayload(uint32_t header_size);
  ContinuePayload(WispBuffer* buffer);
  WispBuffer* pack();
};

class ClosePayload: public WispPayload {
  public:
  static const size_t header_size = sizeof(uint8_t);
  uint8_t close_reason;

  ClosePayload(uint8_t close_reason);
  ClosePayload(WispBuffer* buffer);
  WispBuffer* pack();
};