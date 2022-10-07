#include <stdio.h>
#include <stdint.h>

struct some_struct
{
    int32_t a;
    int32_t b;
    int32_t c;
    int32_t d;
    int32_t e;
    int32_t f;
};


struct some_struct func() {
    struct some_struct s;
    s.a  = 100;
    s.b  = 100;
    s.c  = 100;
    s.d  = 100;
    s.e  = 100;
    s.f  = 100;
    return s;
}

int main() {
    struct some_struct s = func();
    printf("%d\n", s.f);
    return 0;
}



