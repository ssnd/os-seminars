#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static const int buff_size = 4096;

int main(int argc, char* argv[])
{
    char buffer[buff_size];
    char execstr[buff_size];

    fgets(buffer, sizeof(buffer), stdin);
    
    size_t len = strlen(buffer);

    if (len == 0) {
        return 0;
    };

    if ('\n' == buffer[len - 1])
        buffer[len - 1] = '\0';

    snprintf(execstr, sizeof(execstr), "a = %s; print(a)", buffer);
    // sprintf() is not safe
    execlp("python3", "python3", "-c", execstr, NULL);

    perror("pyexec");
    exit(1);
}