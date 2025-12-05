#pragma once

namespace pricer::market {

class YieldCurve {
public:
    explicit YieldCurve(double flatRate)
        : r_(flatRate) {}

    double rate() const { return r_; }

    double discount(double T) const; 

private:
    double r_;
};

class EquityCurve {
public:
    EquityCurve(double spot, double dividendYield)
        : spot_(spot), q_(dividendYield) {}

    double spot() const { return spot_; }
    double dividendYield() const { return q_; }

private:
    double spot_;
    double q_;
};

} 
