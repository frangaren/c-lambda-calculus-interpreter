#include "utf8.h"

ByteTypeUtf8 utf8_byte_type(uint8_t b) {
  if      ((0x80 & b) == 0x00) return UTF8_SINGLE;       // 0xxxxxxx
  else if ((0xC0 & b) == 0x80) return UTF8_CONTINUATION; // 10xxxxxx
  else if ((0xE0 & b) == 0xC0) return UTF8_DOUBLE;       // 110xxxxx
  else if ((0xF0 & b) == 0xE0) return UTF8_TRIPLE;       // 1110xxxx
  else if ((0xF8 & b) == 0xF0) return UTF8_QUADRUPLE;    // 11110xxx
  else                       return UTF8_ERROR;
}

size_t utf8_byte_info(uint8_t b, uint8_t *info) {
  switch (utf8_byte_type(b)) {
    case UTF8_SINGLE:
      *info = b & 0x7F;
      return 7;
      break;
    case UTF8_CONTINUATION:
      *info = b & 0x3F;
      return 6;
      break;
    case UTF8_DOUBLE:
      *info = b & 0x1F;
      return 5;
      break;
    case UTF8_TRIPLE:
      *info = b & 0x0F;
      return 4;
      break;
    case UTF8_QUADRUPLE:
      *info = b & 0x07;
      return 3;
      break;
    case UTF8_ERROR:
    default:
      *info = 0x00;
      return 0;
      break;
  }
}

char_utf8 scalar_from_utf8(const uint8_t *s) {
  int size = utf8_byte_type(s[0]);
  if (size <= 0) return -1;
  char_utf8 outcome = 0x00000000;
  uint8_t temp = 0x00;
  utf8_byte_info(s[0], &temp);
  outcome |= temp;
  for (int i = 1; i < size; i++) {
    if (utf8_byte_type(s[i]) != UTF8_CONTINUATION) {
      return -1;
    } else {
      outcome <<= utf8_byte_info(s[i], &temp);
      outcome |= temp;
    }
  }
  return outcome;
}

size_t utf8_from_scalar(char_utf8 c, uint8_t *s) {
  if (c <= 0x0000007F) {
    s[0] = 0x0000007F & c;
    return 1;
  } else if (c <= 0x000007FF) {
    s[1] =  (0x0000003F & c) | 0x80;
    s[0] = ((0x000007C0 & c) >> 6) | 0xC0;
    return 2;
  } else if (c <= 0x0000FFFF) {
    s[2] =  (0x0000003F & c) | 0x80;
    s[1] = ((0x00000FC0 & c) >> 6) | 0x80;
    s[0] = ((0x0000F000 & c) >> 12) | 0xE0;
    return 3;
  } else if (c <= 0x0010FFFF) {
    s[3] =  (0x0000003F & c) | 0x80;
    s[2] = ((0x00000FC0 & c) >> 6) | 0x80;
    s[1] = ((0x0003F000 & c) >> 12) | 0x80;
    s[0] = ((0x001C0000 & c) >> 18) | 0xF0;
    return 4;
  } else {
    return 0;
  }
}
