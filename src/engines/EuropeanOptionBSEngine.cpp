#include "engines/EuropeanOptionBSEngine.hpp"

#include "products/EuropeanOption.hpp"
#include "core/Payoff.hpp"
#include "utils/BlackFormula.hpp"

#include <cmath>
#include <stdexcept>

namespace pricer::engines {


namespace {
    double normalCdf(double x) {
        return 0.5 * std::erfc(-x / std::sqrt(2.0));
    }
}

double EuropeanOptionBSEngine::priceImpl(const pricer::core::Instrument& inst) const {
    auto const* opt = dynamic_cast<const pricer::products::EuropeanOption*>(&inst);
    if (!opt) {
        throw std::runtime_error("EuropeanOptionBSEngine: mauvais type d'instrument");
    }

    double T     = opt->maturity();
    double S0    = model_->spot();
    double sigma = model_->sigma();

    if (T <= 0.0) {
        // À maturité : payoff sur le spot
        return opt->payoff()(S0);
    }

    double df = model_->discount(T);
    double F  = model_->forward(T);

    double stdDev = sigma * std::sqrt(T);
    if (stdDev <= 0.0) {
        // Pas de volatilité : payoff déterministe sur le forward
        double payoffForward = opt->payoff()(F);
        return df * payoffForward;
    }

    auto const* pv = dynamic_cast<const pricer::core::PlainVanillaPayoff*>(&(opt->payoff()));
    if (!pv) {
        throw std::runtime_error("EuropeanOptionBSEngine: payoff non supporté (non plain vanilla)");
    }

    double K = pv->strike();
    auto type = pv->type();

    double black = pricer::utils::blackForward(F, K, stdDev, type);
    double price = df * black;

    return price;
}


} 
