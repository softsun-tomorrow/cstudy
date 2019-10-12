#include <stdio.h>
main()
{
    char i;
    union date
    {
        int day;
        char month[20];
        int year;
    } a;
    scanf("%c", &i);
    if(i=='d') scanf("%d", &a.day);
    else if(i=='m') scanf("%s", &a.month);
    else if(i=='y') scanf("%d", &a.year);
    else printf("error input!\n");

    if(i=='d') printf("a.day=%d\n", a.day);
    if(i=='m') printf("a.month=%s\n", a.month);
    if(i=='y') printf("a.year=%d\n", a.year);
}
