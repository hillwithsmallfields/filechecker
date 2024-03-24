#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#define AMOUNT 1024 * 1024

void check_file(const char *filename, char *buffer)
{
    int fd = open(filename, O_RDONLY);
    int found_top_bit_set = 0;

    if (fd >= 0) {
        int amount = read(fd, buffer, AMOUNT);
        if (amount > 0) {
            int i;
            for (i = amount;
                 i >= 0;
                 i--) {
                if (buffer[i] & 0x80) {
                    found_top_bit_set++;
                }
            }
        }
        close(fd);
        if (found_top_bit_set > 0) {
            printf("%s has %d top bits set\n", filename, found_top_bit_set);
        }
    } else {
        fprintf(stderr, "Could not open %s\n", filename);
    }
}

void check_directory(const char *dirname, char *buffer)
{
}

int main(int argc,
         char **argv)
{
    int i;
    char *buffer = (char*)malloc(AMOUNT);

    for (i = 1; i < argc; i++) {
        struct stat statbuf;
        const char *filename = argv[i];
        if (stat(filename, &statbuf) == 0) {
            if (statbuf.st_mode & S_IFREG) {
                check_file(filename, buffer);
            } else if (statbuf.st_mode & S_IFDIR) {
                if (filename[0] != '.') {
                    check_directory(filename, buffer);
                }
            }
        } else {
            printf("could not check %s\n", filename);
        }
    }
}
