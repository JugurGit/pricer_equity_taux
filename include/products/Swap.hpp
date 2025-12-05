#pragma once

#include <vector>
#include "core/Instrument.hpp"

namespace pricer::products {

class InterestRateSwap : public pricer::core::Instrument {
public:
    InterestRateSwap(double notional,
                     double fixedRate,
                     std::vector<double> paymentTimes,
                     std::vector<double> accruals,
                     double forwardRate,
                     bool payer)
        : notional_(notional),
          fixedRate_(fixedRate),
          paymentTimes_(std::move(paymentTimes)),
          accruals_(std::move(accruals)),
          forwardRate_(forwardRate),
          payer_(payer) {}

    double notional() const { return notional_; }
    double fixedRate() const { return fixedRate_; }
    const std::vector<double>& paymentTimes() const { return paymentTimes_; }
    const std::vector<double>& accruals() const { return accruals_; }
    double forwardRate() const { return forwardRate_; }

    // true  = payer swap (pay fixed, receive float)
    // false = receiver swap (receive fixed, pay float)
    bool payer() const { return payer_; }

private:
    double notional_;
    double fixedRate_;
    std::vector<double> paymentTimes_;
    std::vector<double> accruals_;
    double forwardRate_;
    bool payer_;
};

class Swaption : public pricer::core::Instrument {
public:
    Swaption(InterestRateSwap underlying, double exerciseTime)
        : underlying_(std::move(underlying)),
          exerciseTime_(exerciseTime) {}

    const InterestRateSwap& underlying() const { return underlying_; }
    double exerciseTime() const { return exerciseTime_; }

private:
    InterestRateSwap underlying_;
    double exerciseTime_;
};

} 
