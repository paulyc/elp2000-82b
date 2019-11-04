//
//  sidereal_time.h
//  tetrabiblos
//
//  Created by Paul Ciarlo on 10/10/19.
//  Copyright © 2019 paulyc. All rights reserved.
//

#ifndef SIDEREAL_TIME_H
#define SIDEREAL_TIME_H

#include "precession.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Calculate the mean sidereal time at the meridian of Greenwich (GMST) of a given date.
 * returns mean sidereal time (degrees).
 * Formula 11.1, 11.4 pg 83
 * MAKE SURE argument JD is UT, not TT!
 * GZfix for V0.14: Replace by expression (43) given in:
 * N. Capitaine, P.T.Wallace, J. Chapront: Expressions for IAU 2000 precession quantities.
 * A&A412, 567-586 (2003)
 * DOI: 10.1051/0004-6361:20031539
 */
double get_mean_sidereal_time (double JD, double JDE);

/* Calculate the apparent sidereal time at the meridian of Greenwich of a given date.
 * returns apparent sidereal time (degrees).
 * Formula 11.1, 11.4 pg 83
 * GZ modified for V0.14 to use nutation IAU-2000B
 */
double get_apparent_sidereal_time (double JD, double JDE);

#ifdef __cplusplus
}
#endif

#endif /* SIDEREAL_TIME_H */
