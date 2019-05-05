#include "optlib.h"
#include <stdio.h>
#include <math.h>
#include <vector>
#include "stats.h"

//  Brent's method adopted from Numerical Recipes in C
//  http://www.nr.com/

//optlib::optlib()
//{
//}

using namespace std;

double SIGN(double a, double b)
{
    if (b>=0.0) {
        return fabsf(a);
    } else {
        return -fabsf(a);
    }
}

// call option value
double optlib::black_scholes_call_value(double s, double k, double r, double t, double vol)
{
    stats statsObj;
    double time_sqrt = std::sqrt(t);
    double d1 = (std::log(s/k)+r*t)/(vol*time_sqrt)+0.5*vol*time_sqrt;
    double d2 = d1-(vol*time_sqrt);
    return s*statsObj.N(d1) - k*std::exp(-r*t)*statsObj.N(d2);;
}

// objective function for implied volatility solver
double optlib::black_scholes_call_iv_obj_function(double s, double k, double r, double t, double vol,
                                                  double call_option_price)
{
    return call_option_price-black_scholes_call_value(s,k,r,t,vol);
}

// brent solver for implied volatility
double optlib::black_scholes_call_implied_volatility_brent(double s, double k, double r, double t,
                                                           double call_option_price, double x1, double x2, double tol)
{
    int ITMAX=100; // Maximum allowed number of iterations.
    double EPS=3.0e-8; // Machine floating-point precision.

    int iter;
    double a=x1,b=x2,c=x2,d=0.0,e=0.0,min1,min2;
    double fa=black_scholes_call_iv_obj_function(s,k,r,t,a,call_option_price);
    double fb=black_scholes_call_iv_obj_function(s,k,r,t,b,call_option_price);

    double fc,p,q,r_,s_,tol1,xm;
    if ((fa > 0.0 && fb > 0.0) || (fa < 0.0 && fb < 0.0))
        return -400.0;
    fc=fb;
    for (iter=1;iter<=ITMAX;iter++) {
        if ((fb > 0.0 && fc > 0.0) || (fb < 0.0 && fc < 0.0)) {
            c=a; // Rename a, b, c and adjust bounding interval d
            fc=fa;
            e=d=b-a;
        }
        if (fabsf(fc) < fabsf(fb)) {
            a=b;
            b=c;
            c=a;
            fa=fb;
            fb=fc;
            fc=fa;
        }
        tol1=2.0*EPS*fabsf(b)+0.5*tol; // Convergence check.
        xm=0.5*(c-b);
        if (fabsf(xm) <= tol1 || fb == 0.0) return b;
        if (fabsf(e) >= tol1 && fabsf(fa) > fabsf(fb)) {
            s_=fb/fa; // Attempt inverse quadratic interpolation.
            if (a == c) {
                p=2.0*xm*s_;
                q=1.0-s_;
            } else {
                q=fa/fc;
                r_=fb/fc;
                p=s_*(2.0*xm*q*(q-r_)-(b-a)*(r_-1.0));
                q=(q-1.0)*(r_-1.0)*(s_-1.0);
            }
            if (p > 0.0) q = -q; // Check whether in bounds.
            p=fabsf(p);
            min1=3.0*xm*q-fabsf(tol1*q);
            min2=fabsf(e*q);
            if (2.0*p < (min1 < min2 ? min1 : min2)) {
                e=d; // Accept interpolation.
                d=p/q;
            } else {
                d=xm; // Interpolation failed, use bisection.
                e=d;
            }
        } else { // Bounds decreasing too slowly, use bisection.
            d=xm;
            e=d;
        }
        a=b; // Move last best guess to a.
        fa=fb;
        if (fabsf(d) > tol1) {// Evaluate new trial root.
            b += d;
        } else {
            b += SIGN(tol1,xm);
        }
        fb=black_scholes_call_iv_obj_function(s,k,r,t,b,call_option_price);
    }
    return -200.0; // Never get here.
}

// put option value
double optlib::black_scholes_put_value(double s, double k, double r, double t, double vol)
{
    stats statsObj;
    double time_sqrt = sqrt(t);
    double d1 = (log(s/k)+r*t)/(vol*time_sqrt) + 0.5*vol*time_sqrt;
    double d2 = d1-(vol*time_sqrt);
    return k*exp(-r*t)*statsObj.N(-d2) - s*statsObj.N(-d1);
}

// objective function for implied volatility solver
double optlib::black_scholes_put_iv_obj_function(double s, double k, double r, double t, double vol,
                                                 double call_option_price)
{
    return call_option_price-black_scholes_put_value(s,k,r,t,vol);
}

// brent solver for implied volatility
double optlib::black_scholes_put_implied_volatility_brent(double s, double k, double r, double t,
                                                          double call_option_price, double x1, double x2, double tol)
{

    int ITMAX=100; // Maximum allowed number of iterations.
    double EPS=3.0e-8; // Machine floating-point precision.

    int iter;
    double a=x1,b=x2,c=x2,d=0.0,e=0.0,min1,min2;
    double fa=black_scholes_put_iv_obj_function(s,k,r,t,a,call_option_price);
    double fb=black_scholes_put_iv_obj_function(s,k,r,t,b,call_option_price);

    double fc,p,q,r_,s_,tol1,xm;
    if ((fa > 0.0 && fb > 0.0) || (fa < 0.0 && fb < 0.0))
        return -400.0;
    fc=fb;
    for (iter=1;iter<=ITMAX;iter++) {
        if ((fb > 0.0 && fc > 0.0) || (fb < 0.0 && fc < 0.0)) {
            c=a; // Rename a, b, c and adjust bounding interval d
            fc=fa;
            e=d=b-a;
        }
        if (fabsf(fc) < fabsf(fb)) {
            a=b;
            b=c;
            c=a;
            fa=fb;
            fb=fc;
            fc=fa;
        }
        tol1=2.0*EPS*fabsf(b)+0.5*tol; // Convergence check.
        xm=0.5*(c-b);
        if (fabsf(xm) <= tol1 || fb == 0.0) return b;
        if (fabsf(e) >= tol1 && fabsf(fa) > fabsf(fb)) {
            s_=fb/fa; // Attempt inverse quadratic interpolation.
            if (a == c) {
                p=2.0*xm*s_;
                q=1.0-s_;
            } else {
                q=fa/fc;
                r_=fb/fc;
                p=s_*(2.0*xm*q*(q-r_)-(b-a)*(r_-1.0));
                q=(q-1.0)*(r_-1.0)*(s_-1.0);
            }
            if (p > 0.0) q = -q; // Check whether in bounds.
            p=fabsf(p);
            min1=3.0*xm*q-fabsf(tol1*q);
            min2=fabsf(e*q);
            if (2.0*p < (min1 < min2 ? min1 : min2)) {
                e=d; // Accept interpolation.
                d=p/q;
            } else {
                d=xm; // Interpolation failed, use bisection.
                e=d;
            }
        } else { // Bounds decreasing too slowly, use bisection.
            d=xm;
            e=d;
        }
        a=b; // Move last best guess to a.
        fa=fb;
        if (fabsf(d) > tol1) {// Evaluate new trial root.
            b += d;
        } else {
            b += SIGN(tol1,xm);
        }
        fb=black_scholes_put_iv_obj_function(s,k,r,t,b,call_option_price);
    }
    return -200.0; // Never get here.
}
