#include <stdio.h>

int f()
{
    int i;
    int j = 0;
    for (i = 1; i > 0; i += i) {
        ++j;
    }
    return j;
}


int main() {
	f();
}