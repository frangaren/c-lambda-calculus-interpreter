#include <assert.h>
#include <stdint.h>
#include <string.h>

#include "utf8.h"

int main(int argc, char **argv) {
  uint8_t buffer[4] = "";
  // One byte character
  assert(scalar_from_utf8((uint8_t *)"a") == 0x000061);
  assert(utf8_from_scalar(0x000061, buffer) == 1);
  assert(memcmp(buffer, "a", 1) == 0);
  // Two bytes character
  assert(scalar_from_utf8((uint8_t *)"λ") == 0x0003BB);
  assert(utf8_from_scalar(0x0003BB, buffer) == 2);
  assert(memcmp(buffer, "λ", 2) == 0);
  // Three bytes character
  assert(scalar_from_utf8((uint8_t *)"※") == 0x00203B);
  assert(utf8_from_scalar(0x00203B, buffer) == 3);
  assert(memcmp(buffer, "※", 3) == 0);
  // Four bytes character
  assert(scalar_from_utf8((uint8_t *)"𐇲") == 0x0101F2);
  assert(utf8_from_scalar(0x0101F2, buffer) == 4);
  assert(memcmp(buffer, "𐇲", 4) == 0);
  return 0;
}
