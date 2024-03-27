#ifndef _WISP_BUFFER_H_
#define _WISP_BUFFER_H_

#include <cstdlib>
#include <cstdint>
#include <cstring>

class WispBuffer {
  public:
  size_t len;
  char* content;

  WispBuffer(size_t len, char* content = NULL);
  ~WispBuffer();

  uint8_t get_uint8(size_t offset);
  void set_uint8(size_t offset, uint8_t value);

  uint16_t get_uint16(size_t offset);
  void set_uint16(size_t offset, uint16_t value);
  
  uint32_t get_uint32(size_t offset) ;
  void set_uint32(size_t offset, uint32_t value);

  WispBuffer* get_buffer(size_t offset);
  WispBuffer* copy_buffer(size_t offset);
  void set_buffer(size_t offset, WispBuffer* value);
};

#endif