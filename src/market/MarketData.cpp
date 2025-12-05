#include "market/MarketData.hpp"
#include <cmath>

namespace pricer::market {

double YieldCurve::discount(double T) const {
    return std::exp(-r_ * T);
}

} 
