#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>


int main(int argc, char *argv[]) {
  int f = open("bigfile.json", O_RDONLY);
  struct stat st;
  fstat(f, &st);
  if (f == -1 || st.st_size == 0) {
    close(f);
    exit(1);
  }

  void* ftext = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, f, 0);
  if (ftext == MAP_FAILED) {
    fprintf(stderr, "mmap error\n");
    close(f);
    exit(1);
  }

  char* pointer = (char *)ftext;

  for (size_t i =0 ;i < 2; ++i) {
    printf("%c", *(pointer++));
  }

  munmap(ftext, st.st_size);
  close(f);

  return 0;
}
