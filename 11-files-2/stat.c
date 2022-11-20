#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>


void display_stat(const char * fname) {
    struct stat a;

    stat(fname, &a);
    printf("fname=%s\n", fname);

    printf("device_id=%d\n",a.st_dev);
    printf("inode=%llu\n",a.st_ino);
    printf("is_dir=%d\n",S_ISDIR(a.st_mode));
    printf("is_link=%d\n",S_ISLNK(a.st_mode));
    printf("hard links=%d\n", a.st_nlink);
}

int main() {
    display_stat("some_file.txt");
    printf("===\n");
    display_stat("some_file1.txt");
    printf("===\n");
    display_stat("folder");

}