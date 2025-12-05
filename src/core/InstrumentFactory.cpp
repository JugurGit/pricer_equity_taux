#include "core/InstrumentFactory.hpp"

namespace pricer::core {

// ==== Equity ====

pricer::products::EuropeanOption
InstrumentFactory::makeEuropeanOption(OptionType type, double K, double T) {
    auto payoff = std::make_unique<PlainVanillaPayoff>(type, K);
    return pricer::products::EuropeanOption(std::move(payoff), T);
}

pricer::products::DigitalOption
InstrumentFactory::makeDigitalOption(OptionType type, double K, double T, double payout) {
    auto payoff = std::make_unique<DigitalPayoff>(type, K, payout);
    return pricer::products::DigitalOption(std::move(payoff), T);
}

pricer::products::AsianOption
InstrumentFactory::makeAsianOption(OptionType type, double K, double T) {
    auto payoff = std::make_unique<PlainVanillaPayoff>(type, K);
    return pricer::products::AsianOption(std::move(payoff), T);
}

pricer::products::BarrierOption
InstrumentFactory::makeUpAndOutOption(OptionType type, double K, double T, double barrier) {
    auto payoff = std::make_unique<PlainVanillaPayoff>(type, K);
    return pricer::products::BarrierOption(
        std::move(payoff),
        T,
        barrier,
        pricer::products::BarrierType::UpAndOut
    );
}

// ==== Taux ====

pricer::products::Caplet
InstrumentFactory::makeCaplet(double notional, double strike, double forward,
                              double start, double end, double yearFraction) {
    return pricer::products::Caplet(
        notional,
        strike,
        forward,
        start,
        end,
        yearFraction,
        OptionType::Call
    );
}

pricer::products::InterestRateSwap
InstrumentFactory::makeSwap(double notional,
                            double fixedRate,
                            const std::vector<double>& paymentTimes,
                            const std::vector<double>& accruals,
                            double forwardRate,
                            bool payer) {
    return pricer::products::InterestRateSwap(
        notional,
        fixedRate,
        paymentTimes,
        accruals,
        forwardRate,
        payer
    );
}

pricer::products::Swaption
InstrumentFactory::makeSwaption(const pricer::products::InterestRateSwap& underlying,
                                double exerciseTime) {
    return pricer::products::Swaption(underlying, exerciseTime);
}

} 
