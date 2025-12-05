#include "utils/BlackFormula.hpp"

#include <cmath>
#include <algorithm>

namespace pricer::utils {

double normalCdf(double x) {
    return 0.5 * std::erfc(-x / std::sqrt(2.0));
}

double blackForward(double F, double K, double stdDev,
                    pricer::core::OptionType type)
{
    if (stdDev <= 0.0) {
        double intrinsic = 0.0;
        if (type == pricer::core::OptionType::Call) {
            intrinsic = std::max(F - K, 0.0);
        } else {
            intrinsic = std::max(K - F, 0.0);
        }
        return intrinsic;
    }

    double lnFK = std::log(F / K);
    double d1 = (lnFK + 0.5 * stdDev * stdDev) / stdDev;
    double d2 = d1 - stdDev;

    double Nd1  = normalCdf(d1);
    double Nd2  = normalCdf(d2);
    double Nmd1 = normalCdf(-d1);
    double Nmd2 = normalCdf(-d2);

    if (type == pricer::core::OptionType::Call) {
        return F * Nd1 - K * Nd2;
    } else {
        return K * Nmd2 - F * Nmd1;
    }
}

double blackDigitalForward(double F, double K, double stdDev,
                           pricer::core::OptionType type,
                           double payout)
{
    if (stdDev <= 0.0) {
        bool inTheMoney = false;
        if (type == pricer::core::OptionType::Call) {
            inTheMoney = (F > K);
        } else {
            inTheMoney = (F < K);
        }
        return inTheMoney ? payout : 0.0;
    }

    double lnFK = std::log(F / K);
    double d1 = (lnFK + 0.5 * stdDev * stdDev) / stdDev;
    double d2 = d1 - stdDev;

    if (type == pricer::core::OptionType::Call) {
        return payout * normalCdf(d2);
    } else {
        return payout * normalCdf(-d2);
    }
}

} // namespace pricer::utils
