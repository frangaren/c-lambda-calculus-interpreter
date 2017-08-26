#pragma once

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

typedef int_fast64_t char_utf8;

typedef enum byte_type_utf8 {
  UTF8_SINGLE = 1,
  UTF8_DOUBLE = 2,
  UTF8_TRIPLE = 3,
  UTF8_QUADRUPLE = 4,
  UTF8_CONTINUATION = 0,
  UTF8_ERROR = -1,
} ByteTypeUtf8;

ByteTypeUtf8 utf8_byte_type(uint8_t b);
size_t utf8_byte_info(uint8_t b, uint8_t *info);

char_utf8 scalar_from_utf8(const uint8_t *s);
size_t utf8_from_scalar(char_utf8 c, uint8_t *s);
