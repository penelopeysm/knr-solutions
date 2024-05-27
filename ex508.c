#include <stdio.h>

int day_of_year (int year, int month, int day) ;
void month_day (int year, int day, int *mo, int *da) ;

int main()
{
    int d = day_of_year(1995, 6, 6);
    printf("%d\n", d);
    d = day_of_year(2019, 12, 26);
    printf("%d\n", d);
    d = day_of_year(2020, 12, 26);
    printf("%d\n", d);
    d = day_of_year(2003, 39, 31);
    printf("%d\n", d);

    int mo, da;
    month_day(1995, 300, &mo, &da);
    printf("%d, %d\n", mo, da);
    month_day(1996, 300, &mo, &da);
    printf("%d, %d\n", mo, da);
    month_day(1996, 400, &mo, &da);
    printf("%d, %d\n", mo, da);
    return 0;
}

static char daytab[2][13] = {
    {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

int day_of_year(int year, int month, int day)
{
    int i, leap;
    leap = (year%4 == 0 && year%100 != 0) || year%400 == 0;
    // error-checking
    if ((year < 1) || (month > 12) || (month < 1) || (day < 1) || (day > daytab[leap][month]))
        return -1;
    for (i = 1; i < month; i++) {
        day += daytab[leap][i];
    }
    return day;
}

void month_day(int year, int yearday, int *pmonth, int *pday)
{
    int i, leap;
    leap = (year%4 == 0 && year%100 != 0) || year%400 == 0;
    // error-checking
    if ((year < 1) || (yearday < 1) || (yearday > 365 + leap)) {
        *pmonth = -1;
        *pday = -1;
    }
    else {
        for (i = 1; yearday > daytab[leap][i]; i++) {
            yearday -= daytab[leap][i];
        }
        *pmonth = i;
        *pday = yearday;
    }
}

