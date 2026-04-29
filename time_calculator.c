#include <sys/time.h>
#include <stdio.h>

long long get_current_time_ms(void)
{
    struct timeval tv;

    if (gettimeofday(&tv, NULL) == -1)
        return(-1);

    return (tv.tv_sec * 1000 + tv.tv_usec /1000);
}
int     main(void)
{
    long long current_time = get_current_time_ms();
    printf("Current time in milliseconds is %lld", current_time);
    return (0);
}