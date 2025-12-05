#pragma once

#include <memory>
#include "core/Instrument.hpp"
#include "core/Payoff.hpp"

namespace pricer::products {

enum class BarrierType {
    UpAndOut,
    DownAndOut,
    UpAndIn,
    DownAndIn
};

class BarrierOption : public pricer::core::Instrument {
public:
    BarrierOption(std::unique_ptr<pricer::core::Payoff> payoff,
                  double maturity,
                  double barrier,
                  BarrierType type)
        : payoff_(std::move(payoff)),
          maturity_(maturity),
          barrier_(barrier),
          type_(type) {}

    double maturity() const { return maturity_; }
    double barrier() const { return barrier_; }
    BarrierType barrierType() const { return type_; }
    const pricer::core::Payoff& payoff() const { return *payoff_; }

private:
    std::unique_ptr<pricer::core::Payoff> payoff_;
    double maturity_;
    double barrier_;
    BarrierType type_;
};

} 
