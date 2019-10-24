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
 * return moon phase in range [0.0, 1.0)
 */
double moonphase(time_t unixt, int use_sidereal_time)
{
    const time_t jd2000_unixt = unixt - jd2000_epoch_unixtime;
    const double t_julian_centuries = jd2000_unixt / (86400.0 * 36525.0);
    const spherical_point moon_position = geocentric_moon_position_of_date(t_julian_centuries); // julian centuries......
    const double lunar_longitude_degrees = fmod(moon_position.longitude / 3600.0, 360.0); // arcseconds to degrees mod 360
    //const double solar_longitude = solar_longitude_of_date(jd2000_unixt);

    const double jdtime = jd2000_epoch_jd + (unixt - jd2000_epoch_unixtime) / 86400.0 - 0.5;
    const double jd = round(jdtime);
    const double jdhours = jdtime - jd;
    const double jde = jd + 67.6439/86400.0;
    double solar_longitude0 = get_apparent_sidereal_time(jd, jde);
    const double solar_longitude_interp = jdhours < 0 ? get_apparent_sidereal_time(jd - 1.0, jde - 1.0) : get_apparent_sidereal_time(jd + 1.0, jde + 1.0);
    // interpolate to the hour
    const double solar_longitude_diff = fabs(solar_longitude_interp - solar_longitude0);
    solar_longitude0 += jdhours * solar_longitude_diff;

    const double solar_longitude1 = solar_longitude_of_date(jd2000_unixt);

    const double solar_longitude = use_sidereal_time ? solar_longitude0 : solar_longitude1;

    // normalize
    const double angle = solar_longitude > lunar_longitude_degrees ? (lunar_longitude_degrees + 360.0) - solar_longitude : lunar_longitude_degrees - solar_longitude;
    printf("unixt %ld lunar %f solar %f solar0 %f solar1 %f angle %f\t", unixt, lunar_longitude_degrees, solar_longitude, solar_longitude0, solar_longitude1, angle);

    return angle / 360.0;
}

static void test_moonphase() {
    // find new moon in oct 2019
    //time_t t = 1569931200;
    //double expected = 1572233880;
    // find next new moon
    time_t t = time(NULL);
    double lastphase = moonphase(t, 1);
    // can't be more than 32 days away so it must be a bug, don't run forever
    for (time_t giveup = t+32*86400 ; t < giveup; t += 600) {
        double phase = moonphase(t, 1);
        printf("moonphase %f\n", phase);
        if (fabs(phase - lastphase) > 0.1) {
            printf("new moon at unixtime %ld = %s\n", t, asctime(gmtime(&t)));
            lastphase = phase;
            break;
        }
        lastphase = phase;
    }
/*
    const double difference = fabs(t - expected);
    if (difference < 12*86400) { // well be generous, within 12 hours
        printf("within normal tolerances, captain (expected %f = 2019-10-28T03:38:00.000Z, difference = %f)\n", expected, difference);
    } else {
        //oops didnt find it but there had to be one
        printf("sorry, charlie\n");
    }*/
}

static void moonphase_main(int argc, char *argv[]) {
    test_moonphase();
    const double phase = moonphase(time(NULL), 1);
    const double mean_synodic_month_in_solar_days = 29.530588;
    // really is just solar days since last new moon but good enough estimate
    const double phase_in_mean_solar_days = phase * mean_synodic_month_in_solar_days;
    printf("Your phase now is %f (~%f in mean solar days)\n", phase, phase_in_mean_solar_days);
}

int main(int argc, char *argv[]) {
    moonphase_main(argc, argv);
    return 0;
}
