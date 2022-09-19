#include <stdlib.h>
#include <stdio.h>

/* source: https://www.cs.utexas.edu/~shmat/courses/cs380s_fall09/blexim.txt */

int *dummy_malloc(int size)  {
    printf("dummy_malloc has allocated %d bytes\n", size);
    return NULL;
}

void bad_function(int *array, int len){
    int *myarray, i;

    myarray = dummy_malloc(len * sizeof(int));
    printf("array length is %d\n", len);
    // another seemingly innocent piece of code
    // for(i = 0; i < len; i++){
    //     myarray[i] = array[i];
    // }
}

int main() {
    int b = 0x40000002;
    int a[20];
    bad_function(a, b);
    return 0;
}