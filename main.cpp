//  Brent's method adopted from Numerical Recipes in C
//  http://www.nr.com/
//  10000 options priced ~38ms.

#include <QCoreApplication>
#include <QDebug>
#include "optlib.h"
#include <boost/math/tools/roots.hpp>

#include <QElapsedTimer>
#define CONCAT_(x,y) x##y
#define CONCAT(x,y) CONCAT_(x,y)

#define CHECKTIME(x)  \
    QElapsedTimer CONCAT(sb_, __LINE__); \
    CONCAT(sb_, __LINE__).start(); \
    x \
    qDebug() << __FUNCTION__ << ":" << __LINE__ << " Elapsed time: " <<  CONCAT(sb_, __LINE__).elapsed() << " ms.";


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    double strike = 235.0;
    double r = .020;
    double maturity = (18.0/360.0);
    double stockPx = 234.09;
    double adjS = stockPx * std::exp(-r * maturity);
    double oPx = 3.72;
    double x1 = 0.10;
    double x2 = 0.25;
    double tol = 0.001;

    optlib obj;
    double test;
    CHECKTIME(
        for(int i = 0; i < 10000; ++i) {
            test = obj.black_scholes_call_implied_volatility_brent(adjS,strike,r,
                                                                      maturity,oPx,
                                                                      x1,x2,tol);
        }
    )

    qDebug()<< "brent test:"<<test;

    return a.exec();
}
