// Copyright (C) 2019 Paul Ciarlo <paul.ciarlo@gmail.com

#define _USE_MATH_DEFINES 1
#include <math.h>
#include <stdio.h>
#include <time.h>

#include "elp2000-82b.h"
#include "sidereal_time.h"

static const double jd2000_epoch_jd = 2451545.0; // 2000-01-01T12:00:00.000Z, Unixtime 946728000
static const time_t jd2000_epoch_unixtime = 946728000;

/*
 * return moon phase in mean solar days since new moon (rough, dont say I didnt warn you)
 */
double moonphase(time_t unixt)
{
    const time_t jd2000_unixt = unixt - jd2000_epoch_unixtime;
    const double t_julian_centuries = jd2000_unixt / (86400.0 * 36525.0);
    const spherical_point moon_position = geocentric_moon_position_of_date(t_julian_centuries); // julian centuries......
    const double lunar_longitude_degrees = fmod(moon_position.longitude / 3600.0, 360.0); // arcseconds to degrees mod 360
    //const double solar_longitude = solar_longitude_of_date(jd2000_unixt);
#define THIS_WORKS 0
#if THIS_WORKS
    const double jd = jd2000_epoch_jd + (unixt - jd2000_epoch_unixtime) / 86400.0 + 0.5;
    const double jde = jd + 67.6439/86400.0;
    const double solar_longitude = get_apparent_sidereal_time(jd, jde);
#else
    const double solar_longitude = solar_longitude_of_date(jd2000_unixt);
#endif
    // normalize
    const double angle = solar_longitude > lunar_longitude_degrees ? (lunar_longitude_degrees + 360.0) - solar_longitude : lunar_longitude_degrees - solar_longitude;
    printf("unixt %ld lunar %f solar %f angle %f\t", unixt, lunar_longitude_degrees, solar_longitude, angle);

#if DIRTY_DEEDS_DONE_DIRT_CHEAP
    const double synodic_month_in_solar_days = 29.530588;
    return synodic_month_in_solar_days * angle / 360.0;
#else
    return NAN;
#endif
}

static void test_moonphase() {
    // find new moon in oct 2019
    double t;
    double lastphase = 0.0;
    double expected = 1572233880.0;
    for (t = 1569931200.0; t < 1600031200.0; t += 3600) {
        double phase = moonphase(t);
        printf("moonphase %f\n", phase);
        if (phase - lastphase < 0) {
            printf("new moon at unixtime %f\n", t);
            lastphase = phase;
            break;
        }
        lastphase = phase;
    }

    const double difference = fabs(t - expected);
    if (difference < 12*86400.0) { // well be generous, within 12 hours
        printf("within normal tolerances, captain (expected %f = 2019-10-28T03:38:00.000Z, difference = %f)\n", expected, difference);
    } else {
        //oops didnt find it but there had to be one
        printf("sorry, charlie\n");
    }
}

static void moonphase_main(int argc, char *argv[]) {
    test_moonphase();
    const double phase = moonphase(time(NULL));
    printf("Your phase now is %f\n", phase);
}

int main(int argc, char *argv[]) {
    moonphase_main(argc, argv);
    return 0;
}
