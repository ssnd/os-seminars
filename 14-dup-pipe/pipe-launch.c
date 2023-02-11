#include <stdio.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/wait.h>
#include <inttypes.h>
#include <zconf.h>
#include <stdlib.h>

static const int READ = 0;
static const int WRITE = 1;
static const int BUF_SIZE = 4096;

int main(int argc, char* argv[]) {
  if (argc != 3) {
    return 1;
  }

  char* cmd = argv[1];
  char* input_file = argv[2];

  int in = open(input_file, O_RDONLY);
  if (-1 == in) {
    perror("open");
    return 1;
  }

  if (dup2(in, STDIN_FILENO) == -1) {
    perror("dup2");
    return 1;
  }
  close(in);

  int pipe_fds[2];
  if (pipe(pipe_fds) == -1) {
    perror("pipe");
    return 1;
  }

  pid_t pid = fork();
  if (-1 == pid) {
    perror("fork");
    return 1;
  }

  if (pid == 0) {
    close(pipe_fds[READ]);
    if (dup2(pipe_fds[WRITE], STDOUT_FILENO) == -1) {
      perror("dup2");
      return 1;
    };

    close(pipe_fds[WRITE]);
    execlp(cmd, cmd, NULL);
    perror("execlp");
    exit(1);
  } else {
    close(pipe_fds[WRITE]);

    int total_count = 0;
    int c;
    char buff[BUF_SIZE];

    while ((c = read(pipe_fds[READ], buff, sizeof(buff))) > 0) {
      total_count += c;
    }

    waitpid(pid, NULL,0);
    close(pipe_fds[READ]);
    printf("%d\n", total_count);
  }
  return 0;
}
