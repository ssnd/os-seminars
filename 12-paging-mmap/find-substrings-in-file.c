#define _GNU_SOURCE

#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "wrong arg count\n");
    return 1;
  }

  char* filename = argv[1];
  char* pattern = argv[2];

  int f = open(filename, O_RDONLY);

  struct stat st;
  if (-1 == fstat(f, &st)) {
    fprintf(stderr, "fstat error\n");
    goto err_exit;
  }

  if (f == -1 || st.st_size == 0) {
    fprintf(stderr, "open error\n");
    goto err_exit;
  }

  void* ftext = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, f, 0);
  if (ftext == MAP_FAILED) {
    fprintf(stderr, "mmap error\n");
    goto err_exit;
  }

  void* pointer = ftext;
  int delta = 0;
  while ((pointer = memmem(pointer, st.st_size - delta, pattern, strlen(pattern))) != NULL) {
    delta = pointer - ftext;
    printf("%d ", delta);
    ++pointer;
  }

  printf("\n");

  munmap(ftext, st.st_size);
  close(f);

  return 0;

  err_exit:
    close(f);
    return 1;
}