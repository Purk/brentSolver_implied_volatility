//  Brent's method adopted from Numerical Recipes in C
//  http://www.nr.com/

#ifndef OPTLIB_H
#define OPTLIB_H
#include <stdio.h>
#include <math.h>
#include <vector>
#include "stats.h"

class optlib
{
public:
    //optlib();

    double black_scholes_call_value(double s, double k, double r, double t, double vol);
    double black_scholes_call_implied_volatility_brent(double s, double k, double r, double t, double call_option_price, double x1, double x2, double tol);
    double black_scholes_call_iv_obj_function(double s, double k, double r, double t, double vol, double call_option_price);

    double black_scholes_put_value(double s, double k, double r, double t, double vol);
    double black_scholes_put_implied_volatility_brent(double s, double k, double r, double t, double call_option_price, double x1, double x2, double tol);
    double black_scholes_put_iv_obj_function(double s, double k, double r, double t, double vol, double call_option_price);
};

#endif // OPTLIB_H
