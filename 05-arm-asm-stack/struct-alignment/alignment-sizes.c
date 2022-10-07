#include <stdio.h>
#include <stdint.h>


struct mystruct_packed {
    uint16_t a; 
    int32_t b;
    uint16_t c;
}  __attribute__((__packed__));


struct mystruct_unpacked {
    uint16_t a;
    int32_t b;
    uint16_t c;
};

struct mystruct_unpacked2 {
    int32_t b;
    uint16_t a;
    uint16_t c;
};

int main() {
    printf("sizeof(mystruct_packed)=%lu\n", sizeof(struct mystruct_packed));
    printf("sizeof(mystruct_unpacked)=%lu\n", sizeof(struct mystruct_unpacked));
    printf("sizeof(mystruct_unpacked2)=%lu\n", sizeof(struct mystruct_unpacked2));
}