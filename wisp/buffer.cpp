#include <cstdlib>
#include <cstdint>
#include <cstring>

#include "buffer.hpp"

WispBuffer::WispBuffer(size_t len, char* content) {
  this->len = len;
  this->content = (char*) malloc(len);
  if (content != NULL) {
    mempcpy(this->content, content, len);
  }
}
WispBuffer::~WispBuffer() {
  free(this->content);
}

uint8_t WispBuffer::get_uint8(size_t offset) {
  return *(uint8_t*)(content + offset);
}
void WispBuffer::set_uint8(size_t offset, uint8_t value) {
  *(uint8_t*)(content + offset) = value;
}

uint16_t WispBuffer::get_uint16(size_t offset) {
  return *(uint16_t*)(content + offset);
}
void WispBuffer::set_uint16(size_t offset, uint16_t value) {
  *(uint16_t*)(content + offset) = value;
}

uint32_t WispBuffer::get_uint32(size_t offset) {
  return *(uint32_t*)(content + offset);
}
void WispBuffer::set_uint32(size_t offset, uint32_t value) {
  *(uint32_t*)(content + offset) = value;
}

WispBuffer* WispBuffer::get_buffer(size_t offset) {
  return new WispBuffer(len - offset, content + offset);
}
WispBuffer* WispBuffer::copy_buffer(size_t offset) {
  WispBuffer* buffer = new WispBuffer(len - offset);
  mempcpy(buffer->content, this->content + offset, len - offset);
  return buffer;
}
void WispBuffer::set_buffer(size_t offset, WispBuffer* value) {
  mempcpy(content + offset, value->content, value->len);
}