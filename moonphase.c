// Copyright (C) 2019 Paul Ciarlo <paul.ciarlo@gmail.com

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <time.h>

#include "elp2000-82b.h"

const double jd2000_epoch_jd = 2451545.0; // 2000-01-01T12:00:00.000Z, Unixtime 946728000
const time_t jd2000_epoch_unixtime = 946728000;

/*
 * return moon phase in mean solar days since new moon (rough, dont say I didnt warn you)
 */
double moonphase(double dts)
{
    dts -= jd2000_epoch_unixtime;
    const spherical_point moon_position = geocentric_moon_position_of_date(dts/(86400.0*36525.0)); // julian centuries......
    const double lunar_longitude_radians = moon_position.longitude * M_PI / 648000.0; // arcseconds to radians
    double lunar_longitude_degrees = lunar_longitude_radians * 180.0 / M_PI; // radians to degrees
    while (lunar_longitude_degrees > 360.0) {
        lunar_longitude_degrees -= 360.0;
    }
    double solar_longitude = solar_longitude_of_date(dts);

    // normalize
    double angle = solar_longitude > lunar_longitude_degrees ? (lunar_longitude_degrees + 360.0) - solar_longitude : lunar_longitude_degrees - solar_longitude;
    printf("lunar %f solar %f angle %f\t", lunar_longitude_degrees, solar_longitude, angle);

#if DIRTY_DEEDS_DONE_DIRT_CHEAP
    const double synodic_month_in_solar_days = 29.530588;
    const double phase = synodic_month_in_solar_days * angle / 360.0;
    if (phase > 0.0) {
        return phase;
    } else {
        return synodic_month_in_solar_days + phase;
    }
    return phase;
#else
#error *starts crying*
    return NaN;
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
        if (lastphase - phase > 20.0) {
            printf("new moon at unixtime %f\n", t);

            lastphase = phase;
            break;
        }
        lastphase = phase;
    }

    //oops didnt find it but there had to be one
    if (abs(t - expected) < 12*86400.0) { // well be generous, within 12 hours
        printf("within normal tolerances, captain\n");
    } else {
        printf("sorry, charlie\n");

    }
}

static void moonphase_main(int argc, char *argv[]) {
    //const double dt = t - epoch;
    test_moonphase();
    const double phase = moonphase((double)time(NULL));
    printf("Your phase now is %f\n", phase);
}

int main(int argc, char *argv[]) {
    moonphase_main(argc, argv);
    return 0;
}
