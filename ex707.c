#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void fgrep(FILE *, char *, int, char *);

int main(int argc, char **argv)
{
    int printname; /* controls printing of file name */
    if (argc == 1) {
        fprintf(stderr, "usage: %s <string> [file1 file2 ...]\n", argv[0]);
    }
    else if (argc == 2) {
        printname = 0;
        char *needle = *(++argv);
        fgrep(stdin, needle, printname, NULL);
    }
    else {
        char *needle = *(++argv);
        printname = (argc > 3) ? 1 : 0;

        while (--argc > 1) {
            FILE *fp;
            if ((fp = fopen(*(++argv), "r")) == NULL) {
                fprintf(stderr, "error opening %s\n", *argv);
                exit(1);
            }
            fgrep(fp, needle, printname, *argv);
            fclose(fp);
        }
    }
    if (ferror(stdout)) {
        fprintf(stderr, "error printing to stdout\n");
        exit(2);
    }
    exit(0);
}

void fgrep(FILE *fp, char *needle, int printname, char *fname)
{
    char *haystack = NULL;
    size_t n = 0;
    ssize_t haystacklen;

    size_t needlelen = strlen(needle);
    while ((haystacklen = getline(&haystack, &n, fp)) > 0) {
        if (haystacklen >= needlelen) { /* skip lines that are too short */
            if (strstr(haystack, needle) != NULL) { /* match found */
                if (printname == 1) printf("%s:%s", fname, haystack);
                else printf("%s", haystack);
            }
        }
        free(haystack);
        haystack = NULL;
    }
}
