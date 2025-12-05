#include "engines/SwapEngines.hpp"

#include "products/Swap.hpp"
#include "core/Payoff.hpp"
#include "utils/BlackFormula.hpp"

#include <cmath>
#include <stdexcept>
#include <algorithm>

namespace pricer::engines {



double SwapEngine::priceImpl(const pricer::core::Instrument& inst) const {
    auto const* swap = dynamic_cast<const pricer::products::InterestRateSwap*>(&inst);
    if (!swap) {
        throw std::runtime_error("SwapEngine: mauvais type d'instrument");
    }

    const auto& times = swap->paymentTimes();
    const auto& accr  = swap->accruals();

    if (times.size() != accr.size()) {
        throw std::runtime_error("SwapEngine: tailles times/accruals incohérentes");
    }

    double notional = swap->notional();
    double K        = swap->fixedRate();
    double F        = swap->forwardRate();
    double sign     = swap->payer() ? 1.0 : -1.0;

    double sum = 0.0;
    for (std::size_t i = 0; i < times.size(); ++i) {
        double df = model_->discount(times[i]);
        sum += accr[i] * df * (F - K);
    }

    return sign * notional * sum;
}


double SwaptionBlackEngine::priceImpl(const pricer::core::Instrument& inst) const {
    auto const* swpt = dynamic_cast<const pricer::products::Swaption*>(&inst);
    if (!swpt) {
        throw std::runtime_error("SwaptionBlackEngine: mauvais type d'instrument");
    }

    const auto& swap = swpt->underlying();
    const auto& times = swap.paymentTimes();
    const auto& accr  = swap.accruals();

    if (times.size() != accr.size()) {
        throw std::runtime_error("SwaptionBlackEngine: tailles times/accruals incohérentes");
    }

    double notional = swap.notional();
    double K        = swap.fixedRate();
    double F        = swap.forwardRate();    
    double sigma    = model_->sigma();
    double Texp     = swpt->exerciseTime();

    if (Texp <= 0.0) {
        double swapPV = 0.0;
        double signSwap = swap.payer() ? 1.0 : -1.0;

        for (std::size_t i = 0; i < times.size(); ++i) {
            double df = model_->discount(times[i]);
            swapPV += accr[i] * df * (F - K);
        }
        swapPV *= signSwap * notional;

        return std::max(swapPV, 0.0);
    }

    double A = 0.0;
    for (std::size_t i = 0; i < times.size(); ++i) {
        double df = model_->discount(times[i]);
        A += accr[i] * df;
    }

    double stdDev = sigma * std::sqrt(Texp);

    pricer::core::OptionType type =
        swap.payer() ? pricer::core::OptionType::Call : pricer::core::OptionType::Put;

    double black = pricer::utils::blackForward(F, K, stdDev, type);

    double price = notional * A * black;
    return price;
}

} 
