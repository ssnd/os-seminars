#include <stdint.h>

struct fheader {
    uint32_t magic; // 0xDEADBEEF
    uint16_t size;  // uint integer
    uint32_t flags; // some binary flags
}  __attribute__((__packed__));


struct fheader2 {
    uint32_t magic; // 0xDEADBEEF
    uint16_t size;  // uint integer
    uint32_t flags; // some binary flags
} ;