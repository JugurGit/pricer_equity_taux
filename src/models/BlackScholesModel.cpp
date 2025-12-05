#include "models/BlackScholesModel.hpp"
#include <cmath>

namespace pricer::models {

double BlackScholesModel::spot() const {
    return equityCurve_->spot();
}

double BlackScholesModel::discount(double T) const {
    return discountCurve_->discount(T);
}

double BlackScholesModel::forward(double T) const {
    double S0 = equityCurve_->spot();
    double r  = discountCurve_->rate();
    double q  = equityCurve_->dividendYield();
    return S0 * std::exp((r - q) * T);
}

double BlackScholesModel::rate() const {
    return discountCurve_->rate();
}

double BlackScholesModel::dividendYield() const {
    return equityCurve_->dividendYield();
}


} 
