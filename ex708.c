#include <stdio.h>
#include <stdlib.h>

#define PAGEHEIGHT 15
/* header + blank line + actual page + blank line + page number + blank line = 20 lines per page */

/* This doesn't work like a proper pager, but you can pipe the output into vim:
 *     ./ex708 [files...] | vim -
 * and then use 20jz<CR> or 20kz<CR> to pretend that you're scrolling down or up a page.
 * For more information, :h z<CR> */

void fpg(FILE *, char *);

int main(int argc, char **argv)
{
    if (argc == 1) {
        fpg(stdin, "-stdin-");
    }
    else {
        while (--argc > 1) {
            FILE *fp;
            if ((fp = fopen(*(++argv), "r")) == NULL) {
                fprintf(stderr, "error opening %s\n", *argv);
                exit(1);
            }
            fpg(fp, *argv);
            fclose(fp);
        }
    }
    if (ferror(stdout)) {
        fprintf(stderr, "error printing to stdout\n");
        exit(2);
    }
    exit(0);
}

void fpg(FILE *fp, char *fname)
{
    char *dashes = "-----------------------------------";
    printf("%s\n", fname);
    printf("%s\n", dashes);
    int linecount = 0;
    int pagecount = 1;

    char *line = NULL;
    size_t n = 0;
    ssize_t len;
    while ((len = getline(&line, &n, fp)) > 0) {
        if ((linecount != 0) && (linecount % PAGEHEIGHT == 0)) {
            printf("%s\n", fname);
            printf("%s\n", dashes);
        }
        printf("%s", line);
        linecount++;
        if (linecount % PAGEHEIGHT == 0) {
            printf("\n");
            printf("%d\n", pagecount);
            pagecount++;
            printf("\n");
        }
        free(line);
        line = NULL;
    }
    if (linecount % PAGEHEIGHT != 0) {
        while (linecount++ % PAGEHEIGHT != 0) {
            printf("\n");
        }
        printf("\n");
        printf("%d\n", pagecount);
        printf("\n");
    }
}
