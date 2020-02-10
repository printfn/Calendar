#include <stdio.h>
#include <time.h>

#define BOOL int

typedef int Year;
typedef int Month;
typedef int Day;
typedef int DayOfWeek; // Sun = 0, Mon = 1, ..., Sat = 6

static void print_string(const char *str) {
    while (*str)
        fputc(*str++, stdout);
}

static BOOL is_leap_year(Year year) {
    return year % 400 == 0 ? 1 : year % 100 == 0 ? 0 : year % 4 == 0;
}

static int num_days_in_year(Year year) {
    return is_leap_year(year) ? 366 : 365;
}

static int num_days_in_month(Year year, Month month) {
    if (month == 2) {
        return is_leap_year(year) ? 29 : 28;
    }
    if (month == 4 || month == 6 || month == 9 || month == 11) {
        return 30;
    }
    return 31;
}

static DayOfWeek day_of_week_of_first_of_month(Year year, Month month) {
    int num_days = 0; // number of days shifted forward
    while (month > 1) {
        num_days += num_days_in_month(year, --month);
    }
    while (year < 2016) {
        num_days -= num_days_in_year(year++);
    }
    while (year > 2016) {
        num_days += num_days_in_year(--year);
    }
    while (num_days < 0) {
        num_days += 70000000;
    }
    DayOfWeek day_of_week = 5; // Friday
    return (day_of_week + num_days) % 7;
}

static void print_int(int number, int num_digits) {
    char output[] = "         0"; // length 10 + \0
    int nd = 10;
    while (nd > 0 && number != 0) {
        output[--nd] = number % 10 + '0';
        number /= 10;
    }
    print_string(&output[10 - num_digits]);
}

static void draw_month_row(Year year, Month month, int row) {
    int column_of_first = day_of_week_of_first_of_month(year, month);
    int day = 1 + 7 * row - column_of_first;
    for (int i = 0; i < 7; ++i) {
        if (day < 1 || day > num_days_in_month(year, month)) {
            print_string("  ");
        } else {
            print_int(day, 2);
        }
        if (i != 6)
            print_string(" ");
        ++day;
    }
}

/// @param month optional, pass 0 to print whitespace
static void draw_month_row_space(Month month) {
    char months[13][7] = { "      ",
        "  Jan ", "  Feb ", "  Mar ", "  Apr ", "  May ", "  Jun ",
        "  Jul ", "  Aug ", "  Sep ", "  Oct ", "  Nov ", "  Dec "
    };
    print_string(months[month]);
}

static BOOL bottom_row_empty(Year year, Month month) {
    int column_of_first = day_of_week_of_first_of_month(year, month);
    int day = 1 + 7 * 5 - column_of_first;
    return day > num_days_in_month(year, month);
}

static BOOL bottom_row_is_completely_empty(Year year, int mr) {
    return bottom_row_empty(year, mr * 3 + 1)
            && bottom_row_empty(year, mr * 3 + 2)
            && bottom_row_empty(year, mr * 3 + 3);
}

static void print_calendar(Year year) {
    print_string("      Su Mo Tu We Th Fr Sa      Su Mo   ");
    print_int(year, 4);
    print_string("   Fr Sa      Su Mo Tu We Th Fr Sa\n");
    for (int mr = 0; mr < 4; ++mr) {
        for (int row = 0; row < 6; ++row) {
            if (row == 5 && bottom_row_is_completely_empty(year, mr))
                break; // skip sixth row
            for (int mc = 0; mc < 3; ++mc) {
                Month month = mr * 3 + mc + 1;
                draw_month_row_space(row == 0 ? month : 0);
                draw_month_row(year, month, row);
            }
            print_string("\n");
        }
    }
}

static Year get_current_year(void) {
    Year year = 1970;
    time_t seconds_since_epoch = time(NULL);
    while (seconds_since_epoch >= num_days_in_year(year) * 86400) {
        seconds_since_epoch -= num_days_in_year(year++) * 86400;
    }
    return year;
}

static int parse_int(const char *str) {
    int res = 0;
    while ('0' <= *str && *str <= '9') {
        res = 10 * res + *str++ - '0';
    }
    return res;
}

int main(int argc, const char *argv[]) {
    if (argc <= 1) {
        print_calendar(get_current_year());
    } else {
        print_calendar(parse_int(argv[1]));
    }
    return 0;
}
