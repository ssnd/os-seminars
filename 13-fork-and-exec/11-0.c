#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
  int processes = 1;
  pid_t pid;
  do {
    pid = fork();
    if (pid != -1) ++processes;
  } while (pid == 0);

  if (pid > 0) {
    int status;
    waitpid(pid, &status, 0);

  } else if (pid == -1) {
    printf("%d\n", processes);
  }

  return 0;
}