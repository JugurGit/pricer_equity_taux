#include "engines/DigitalOptionBSEngine.hpp"

#include "products/DigitalOption.hpp"
#include "core/Payoff.hpp"
#include "utils/BlackFormula.hpp"

#include <cmath>
#include <stdexcept>

namespace pricer::engines {


double DigitalOptionBSEngine::priceImpl(const pricer::core::Instrument& inst) const {
    auto const* opt = dynamic_cast<const pricer::products::DigitalOption*>(&inst);
    if (!opt) {
        throw std::runtime_error("DigitalOptionBSEngine: mauvais type d'instrument");
    }

    double T     = opt->maturity();
    double S0    = model_->spot();
    double sigma = model_->sigma();

    if (T <= 0.0) {
        return opt->payoff()(S0);
    }

    double df = model_->discount(T);
    double F  = model_->forward(T);

    double stdDev = sigma * std::sqrt(T);
    if (stdDev <= 0.0) {
        return df * opt->payoff()(F);
    }

    auto const* dp = dynamic_cast<const pricer::core::DigitalPayoff*>(&(opt->payoff()));
    if (!dp) {
        throw std::runtime_error("DigitalOptionBSEngine: payoff non DigitalPayoff");
    }

    double K   = dp->strike();
    double Q   = dp->payout();
    auto type  = dp->type();

    double undiscounted = pricer::utils::blackDigitalForward(F, K, stdDev, type, Q);
    double price = df * undiscounted;
    return price;
}


} 
