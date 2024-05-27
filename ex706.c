#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    if (argc != 3) {
        fprintf(stderr, "usage: %s file1 file2\n", argv[0]);
    }
    else {
        FILE *fp;
        if ((fp = fopen(argv[1], "r")) == NULL) {
            fprintf(stderr, "error opening file: %s\n", argv[1]);
            exit(1);
        }
        FILE *fq;
        if ((fq = fopen(argv[2], "r")) == NULL) {
            fprintf(stderr, "error opening file: %s\n", argv[2]);
            exit(1);
        }

        char *sp = NULL;
        char *sq = NULL;
        size_t n = 0;
        ssize_t valp;
        ssize_t valq;
        size_t lineno = 1;

        /* loop over lines */
        for (;;) {
            valp = getline(&sp, &n, fp);
            valq = getline(&sq, &n, fq);
            
            if ((valp == -1) && (valq == -1)) break; /* EOF or error on both */
            else if (valp == -1) {
                printf("%s reached end of file\n", argv[1]);
                printf("%s, line %lu\n>>> %s\n", argv[2], lineno, sq);
                free(sp); free(sq); fclose(fp); fclose(fq);
                exit(0);
            }
            else if (valq == -1) {
                printf("%s, line %lu\n>>> %s\n", argv[1], lineno, sp);
                printf("%s reached end of file\n", argv[2]);
                free(sp); free(sq); fclose(fp); fclose(fq);
                exit(0);
            }
            else {
                if (strcmp(sp, sq) != 0) {
                    printf("%s, line %lu\n>>> %s\n", argv[1], lineno, sp);
                    printf("%s, line %lu\n>>> %s\n", argv[2], lineno, sq);
                    free(sp); free(sq); fclose(fp); fclose(fq);
                    exit(0);
                }
            }
            free(sp); free(sq);
            sp = NULL;
            sq = NULL;
            lineno++;
        }

        /* check for errors */
        if (ferror(fp)) {
            fprintf(stderr, "error reading %s\n", argv[1]);
            fclose(fp);
            fclose(fq);
            exit(2);
        }
        if (ferror(fq)) {
            fprintf(stderr, "error reading %s\n", argv[2]);
            fclose(fp);
            fclose(fq);
            exit(2);
        }
        if (ferror(stdout)) {
            fprintf(stderr, "error writing to stdout\n");
            fclose(fp);
            fclose(fq);
            exit(3);
        }
        
        /* if we reached here, means both files are the same */
        fprintf(stderr, "files %s and %s are identical\n", argv[1], argv[2]);
        fclose(fp);
        fclose(fq);
        exit(0);
    }

}
