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
    char (*pdaytab)[13] = daytab;
    /* C variables follow "declaration mimics use", i.e. int *ip; means that "*ip is an int". */
    /* Likewise, char (*pdaytab)[13] means that "*pdaytab is an array of 13 chars". */
    /* Initialising it as daytab makes pdaytab point to daytab[0], since "daytab" decays to &daytab[0]. */
    pdaytab += (year%4 == 0 && year%100 != 0) || year%400 == 0;
    /* If it's a leap year, pdaytab now points to daytab[1], because pdaytab+1 = &daytab[1]. */
    /* How does the programme know how much memory to skip over? It's because of the pointer declaration:
     * since we defined pdaytab as a pointer to an array of 13 chars, pdaytab++ moves it forward by the
     * exact amount of memory that corresponds to 13 chars. */
    if ((year < 1) || (month > 12) || (month < 1) || (day < 1) || (day > (*pdaytab)[month]))
        return -1;
    /* just some rudimentary error-checking.
     * Note how *pdaytab is daytab[i], so (*pdaytab)[month] means daytab[i][month]. */
    char *p = *pdaytab + 1;
    /* pdaytab is a pointer to the array daytab[i] (i = 0 or 1).  
     * So, *pdaytab is equal to daytab[i] - but that decays to &daytab[i][0] when used in an expression.
     * Likewise, *pdaytab + 1 is simply &daytab[i][1].
     * To get at the value of daytab[i][1], we create p, a pointer to a char, and set p = *pdaytab + 1.
     * Then, *p gives us the value of daytab[i][1].
     * Note that this definition doesn't mean *p = *pdaytab + 1; it means "p is a pointer to a char, and p = *pdaytab + 1". */
    while (p < *pdaytab + month) {
        day += *p++; /* p++ accesses the successive elements of daytab[i]. */
    }
    return day;
}

void month_day(int year, int yearday, int *pmonth, int *pday)
{
    char (*pdaytab)[13] = daytab;
    pdaytab += (year%4 == 0 && year%100 != 0) || year%400 == 0;
    // error-checking
    if ((year < 1) || (yearday < 1) || (yearday > 365 + (pdaytab - daytab))) {
        *pmonth = -1;
        *pday = -1;
    }
    else {
        char *p = *pdaytab + 1;
        int m = 1;
        while (yearday > *p) {
            yearday -= *p++;
            m++;
        }
        *pmonth = m;
        *pday = yearday;
    }
}

