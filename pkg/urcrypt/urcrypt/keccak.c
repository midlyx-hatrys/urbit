#include "urcrypt.h"
#include <keccak-tiny.h>

#define defkec(bits) \
  int urcrypt_keccak_##bits(const uint8_t *message, size_t length, \
                            uint8_t out[bits]) \
  { \
    return keccak_##bits(out, bits, message, length); \
  }

defkec(224)
defkec(256)
defkec(384)
defkec(512)
