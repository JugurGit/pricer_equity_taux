#pragma once

#include <memory>
#include "core/Instrument.hpp"
#include "core/Payoff.hpp"

namespace pricer::products {

class DigitalOption : public pricer::core::Instrument {
public:
    DigitalOption(std::unique_ptr<pricer::core::Payoff> payoff,
                  double maturity)
        : payoff_(std::move(payoff)),
          maturity_(maturity) {}

    double maturity() const { return maturity_; }
    const pricer::core::Payoff& payoff() const { return *payoff_; }

private:
    std::unique_ptr<pricer::core::Payoff> payoff_;
    double maturity_;
};

} 
