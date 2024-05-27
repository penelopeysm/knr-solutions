#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>

/* coding sscanf instead of scanf because it's better anyway */
/* also, trying POSIX getline instead of my_getline which we used previously */

int minsscanf(char *, const char *, ...);

int main(void)
{
    char *line = NULL;
    size_t len = 0; /* size_t is an unsigned int */
    ssize_t nread; /* ssize_t ranges from -1 to INT_MAX - an unsigned int which can handle exit codes */

    printf("The format string is: 'zz%%d %%s %%d %%d:%%d:%%d'.\n\n");

    int day;
    char month[30];
    int year;
    int hour;
    int minute;
    int second;

    int sscanf_return;
    int minsscanf_return;

    while ((nread = getline(&line, &len, stdin) > 0)) {
        day = year = hour = minute = second = 0;
        month[0] = '\0';

        sscanf_return = sscanf(line, "zz%d %s %d %d:%d:%d", &day, month, &year, &hour, &minute, &second);
        printf("sscanf returned   : %d\n", sscanf_return);
        printf("day = %d\t\tmonth = |%s|\t\tyear = %d\n", day, month, year);
        printf("hour = %d\t\tminute = %d\t\tsecond = %d\n", hour, minute, second);
        printf("\n");

        day = year = hour = minute = second = 0;
        month[0] = '\0';

        minsscanf_return = minsscanf(line, "zz%d %s %d %d:%d:%d", &day, month, &year, &hour, &minute, &second);
        printf("minsscanf returned: %d\n", minsscanf_return);
        printf("day = %d\t\tmonth = |%s|\t\tyear = %d\n", day, month, year);
        printf("hour = %d\t\tminute = %d\t\tsecond = %d\n", hour, minute, second);
        printf("\n");
    }

    return 0;
}

int minsscanf(char *s, const char *fmt, ...)
{
    /* only %s and %d implemented */
    va_list ap;
    va_start(ap, fmt);

    char *sval;
    int *ival;
    const char *f;
    int args_processed = 0;
    int i = 0;

    /* loop over the format string */
    for (f = fmt; *f; f++) {
        /* printf("processing %c\n", *f); */
        /* skip whitespace */
        if (isspace(*f)) continue;

        /* check other characters to make sure they match */
        if (*f != '%') {
            if (*f == *s) {
                s++;
                continue;
            }
            else {
                return args_processed;
            }
        }
        else {
            char c = *++f;
            switch (c) {
                case 'd':
                    /* printf("processing d\n"); */
                    ival = va_arg(ap, int *);
                    char nums[30];
                    char *pnums = nums;
                    i = 0;

                    while (isspace(*s)) s++; /* skip leading whitespace */
                    if ((*s == '-') || (*s == '+')) {
                        *pnums = *s;
                        pnums++;
                        s++;
                        i++;
                    }
                    while ((i < 30) && (isdigit(*s))) {
                        *pnums = *s;
                        pnums++;
                        s++;
                        i++;
                    }
                    *pnums = '\0';
                    if (strlen(nums) == 0) return args_processed;
                    *ival = atoi(nums);
                    args_processed++;
                    break;
                case 's':
                    /* printf("processing s\n"); */
                    sval = va_arg(ap, char *);
                    size_t lim = sizeof(sval);
                    
                    i = 1;
                    while (isspace(*s)) s++; /* skip leading whitespace */
                    while ((i < 30) && (*s) && !(isspace(*s))) {
                        *sval = *s;
                        sval++;
                        s++;
                        i++;
                    }
                    *sval = '\0';
                    /* printf("i = %d\n", i); */
                    if (i == 1) return args_processed; /* no string was found */
                    args_processed++;
                    break;
                default:
                    /* check that the string matches */
                    if ((*s == '%') && (*++s == c)) s++;
                    else return args_processed;
                    break;
            }
        }
    }
    return args_processed;
}
