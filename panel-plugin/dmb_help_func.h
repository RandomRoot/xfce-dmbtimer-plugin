
#include <time.h>
#include "dmb_shared.h"


static const int monthDays[12] = {
  31, 28, 31, 30, 31, 30,
  31, 31, 30, 31, 30, 31
};

static int
countLeapYears(Date d)
{
    int years = d.y;
    if (d.m <= 2)
        years--;

    return years / 4 - years / 100 + years / 400;
}

static long int
getDifference(Date dt1, Date dt2)
{
    long int n1 = dt1.y * 365 + dt1.d;

    for (int i = 0; i < dt1.m - 1; i++) {
        n1 += monthDays[i];
    }

    n1 += countLeapYears(dt1);

    long int n2 = dt2.y * 365 + dt2.d;
    for (int i = 0; i < dt2.m - 1; i++) {
        n2 += monthDays[i];
    }
    n2 += countLeapYears(dt2);

    return n2 - n1;
}

static int
getLeftWeeks(Date dmb_date)
{
    struct tm timeinfo = {
      .tm_year = dmb_date.y - 1900,
      .tm_mon = dmb_date.m - 1,
      .tm_mday = dmb_date.d
    };

    time_t dt = timegm(&timeinfo);
    time_t ct = time(NULL);

    struct tm tm = *localtime(&ct);
    timeinfo = *localtime(&dt);

    int current_week = tm.tm_yday / 7 + 1;
    int target_week = timeinfo.tm_yday / 7 + 1;

    return 52 * (timeinfo.tm_year - tm.tm_year) + (target_week - current_week);
}

static Date
get_current_date()
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    Date current_date = {
      .y = tm.tm_year + 1900,
      .m = tm.tm_mon + 1,
      .d = tm.tm_mday
    };

    return current_date;
}


static char *
get_day_word_with_ending(gint days_left)
{
    // дней - 0, 5-19
    // день - 1
    // дня  - 2, 3, 4
    char *result;
    gint last_number = days_left % 100;
    if (!(last_number > 10 && last_number < 15)) {
        last_number = days_left % 10;
    }
    switch (last_number) {
    case 1:result = "день";
        break;
    case 2:
    case 3:
    case 4:result = "дня";
        break;
    default:result = "дней";
    }
    return result;
}