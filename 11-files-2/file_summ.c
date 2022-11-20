#include <stdio.h>
#include <stdint.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>

int64_t regular_file_size(const char* filename) {
  struct stat st;
  int ret = lstat(filename, &st);
  if (ret == -1)
    return 0;

  if (S_ISREG(st.st_mode)) {
    return st.st_size;
  }

  return 0;
}

int main(int argc, char* argv[]) {
  int ch;
  char file[PATH_MAX];
  size_t i = 0;
  int64_t sum = 0;

  while ((ch = getchar()) != EOF) {
    if (ch == '\n') {
      file[i] = '\0';
      i = 0;
      sum += regular_file_size(file);
      continue;
    }
    file[i] = (char) ch;
    ++i;
  }
  printf("%lld\n", sum);
  return 0;
}