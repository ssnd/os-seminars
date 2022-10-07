#include <elf.h>
#include <stdio.h>


// see http://www.skyfree.org/linux/references/ELF_Format.pdf
int main() {
        Elf64_Ehdr header;
        FILE * f = fopen("a.out", "rb");
        if (!f) {
                return -1;
        }

        fread(&header, sizeof(header), 1, f);

        printf("e_type=%u\n", header.e_type);
        printf("e_entry=0x%x\n", header.e_entry);
        return 0;
}