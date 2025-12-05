#include "core/Payoff.hpp"

#include <algorithm>

namespace pricer::core {

double PlainVanillaPayoff::operator()(double spot) const {
    if (type_ == OptionType::Call) {
        return std::max(spot - strike_, 0.0);
    } else {
        return std::max(strike_ - spot, 0.0);
    }
}

double DigitalPayoff::operator()(double spot) const {
    bool inTheMoney = false;
    if (type_ == OptionType::Call) {
        inTheMoney = (spot > strike_);
    } else {
        inTheMoney = (spot < strike_);
    }
    return inTheMoney ? payout_ : 0.0;
}


}
