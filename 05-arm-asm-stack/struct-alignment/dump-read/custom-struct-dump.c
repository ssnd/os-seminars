#include <stdio.h>
#include "fheader.h"


int dump(void * inf, uint32_t size, char* fname) {
    const object_count = 1;
    FILE* file = fopen(fname, "w");

    if (NULL==file) return -1;

    fwrite(inf, size, object_count, file);

    fclose(file);
    return 0;
}

const uint32_t magic_header = 0xDEADBEEF;
const uint32_t struct_size = 1000;
const uint32_t flags = 0xFFFFFFFF;

int main() {
    struct fheader a;
    a.magic = magic_header;
    a.size = struct_size;
    a.flags = flags;

    struct fheader2 a2;
    a2.magic = magic_header;
    a2.size = struct_size;
    a2.flags = flags;

    dump(&a, sizeof(struct fheader), "out_packed.dump");
    dump(&a2,sizeof(struct fheader2), "out_unpacked.dump");
    return 0;
}