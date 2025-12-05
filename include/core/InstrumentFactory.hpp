#pragma once

#include <memory>
#include <vector>

#include "core/Payoff.hpp"
#include "products/EuropeanOption.hpp"
#include "products/DigitalOption.hpp"
#include "products/AsianOption.hpp"
#include "products/BarrierOption.hpp"
#include "products/CapFloor.hpp"
#include "products/Swap.hpp"

namespace pricer::core {

class InstrumentFactory {
public:
    // ==== Equity ====

    // Option européenne (call ou put)
    static pricer::products::EuropeanOption
    makeEuropeanOption(OptionType type, double K, double T);

    // Option digitale (cash-or-nothing)
    static pricer::products::DigitalOption
    makeDigitalOption(OptionType type, double K, double T, double payout);

    // Option asiatique (arithmétique)
    static pricer::products::AsianOption
    makeAsianOption(OptionType type, double K, double T);

    // Option barrière up-and-out (simple)
    static pricer::products::BarrierOption
    makeUpAndOutOption(OptionType type, double K, double T, double barrier);

    // ==== Taux ====

    // Caplet simple
    static pricer::products::Caplet
    makeCaplet(double notional, double strike, double forward,
               double start, double end, double yearFraction);

    // Swap simple (fixed vs float, forward constant)
    static pricer::products::InterestRateSwap
    makeSwap(double notional,
             double fixedRate,
             const std::vector<double>& paymentTimes,
             const std::vector<double>& accruals,
             double forwardRate,
             bool payer);

    // Swaption européenne sur swap donné
    static pricer::products::Swaption
    makeSwaption(const pricer::products::InterestRateSwap& underlying,
                 double exerciseTime);
};

} 
