#include <sys/syscall.h>

long syscall(long number, ...);

void _start()
{
  static const char msg[] = "Hello, World!\n";
  syscall(SYS_write, 1, msg, sizeof(msg) - 1);
  syscall(SYS_exit, 0);
}
