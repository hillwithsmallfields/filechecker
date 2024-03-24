#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#define AMOUNT 1024 * 1024

int check_file(const char *filename, char *buffer)
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
        return found_top_bit_set;
    } else {
        return 0;
    }
}

void check_directory(const char *dirname, char *buffer)
{
}

void check_file_or_directory(const char *filename, char *buffer, FILE *out_fd)
{
    struct stat statbuf;
    if (stat(filename, &statbuf) == 0) {
        if (statbuf.st_mode & S_IFREG) {
            int tbs = check_file(filename, buffer);
            int size = statbuf.st_size;
            if (size > AMOUNT) {
                size = AMOUNT;
            }
            if (tbs > 0) {
                fprintf(out_fd, "%s,%d,%d,%f\n", filename, tbs, size, (float)tbs / (float)size);
            }
        } else if (statbuf.st_mode & S_IFDIR) {
            if (filename[0] != '.') {
                check_directory(filename, buffer);
            }
        }
    } else {
        printf("could not check %s\n", filename);
    }
}

int main(int argc,
         char **argv)
{
    int i;
    char c;
    char *buffer = (char*)malloc(AMOUNT);
    FILE *out_fd = stdout;

    while ((c = getopt(argc, argv, "a:o:")) != -1) {
        switch (c)
            {
            case 'o':
                out_fd = fopen(optarg, "w");
                fprintf(out_fd, "filename,top_bit_count,size,proportion\n");
                break;
            case 'a':
                out_fd = fopen(optarg, "a");
                break;
            }
    }
    for (i = optind; i < argc; i++) {
        check_file_or_directory(argv[i], buffer, out_fd);
    }
    if (out_fd != stdout) {
        fclose(out_fd);
    }
}
