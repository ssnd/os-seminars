#include <stdio.h>
#include "fheader.h"


int load(void * inf, uint32_t size, char* fname) {
    const object_count = 1;
    FILE* file = fopen(fname, "r");

    if (NULL==file) return -1;

    fread(inf, size, object_count, file);

    fclose(file);
    return 0;
}

int main() {
    struct fheader a;
    struct fheader2 a2;

    load(&a, sizeof(struct fheader), "out_packed.dump");
    load(&a2,sizeof(struct fheader2), "out_unpacked.dump");

    printf("magic=0x%x, size=%u, flags=0x%x\n", a.magic, a.size, a.flags);
    printf("magic=0x%x, size=%u, flags=0x%x\n", a2.magic, a2.size, a2.flags);
    return 0;
}