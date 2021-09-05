//Milton Eduardo Barroso Ramírez.
//A01634505.

#include <stdio.h>
#include <stdlib.h>

/* month_day function's prototype*/
void month_day(int year, int yearday, int *pmonth, int *pday);

void month_day(int year, int yearday, int *pmonth, int *pday){

    int leap = year%4 == 0 && year%100 !=0 || year%400 == 0;
    
    if(yearday<1 || (leap && yearday>366) || (!leap && yearday>365) || year<0){//As long as year is a positive int, I don't believe the year should be a problem
        char *msg = "The year days are invalid";
        if(year<0) msg="The year is invalid";
        printf("%s\n",msg);

        *pmonth= -1;
        *pday= -1;
        return;
    }
    
    static char *monthName[] = {
        "Illegal month",//Never really going to use this value, but i'm using it as a placeholder for easier month manipulation
        "Jan", "Feb", "Mar",
        "Apr", "May", "Jun",
        "Jul", "Aug", "Sep",
        "Oct", "Nov", "Dec"
    };

    static char daysInMonth[] = {0,31,0,31,30,31,30,31,31,30,31,30,31};

    if(leap) daysInMonth[2] = 29;
    else daysInMonth[2] = 28;

    int i,day = yearday;

    for (i = 1; day>0; i++) {
        if (day<=daysInMonth[i]) break;
        day-= daysInMonth[i];
    }

    *pmonth = i;
    *pday = day;


    printf("%s %d, %d\n",monthName[i],day,year);


}

int main(int argc, char *argv[]) {

    if(argc < 3){
        printf("Please use correct arguments ./month_day <year> <yearday>\n");
        return -1;
    }

    int month = 0, day = 0;

    month_day(atoi(argv[1]), atoi(argv[2]), &month, &day);

    return 0;
}
