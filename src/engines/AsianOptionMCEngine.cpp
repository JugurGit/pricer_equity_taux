#include "engines/AsianOptionMCEngine.hpp"

#include "products/AsianOption.hpp"

#include <random>
#include <cmath>
#include <stdexcept>

namespace pricer::engines {

double AsianOptionMCEngine::priceImpl(const pricer::core::Instrument& inst) const {
    auto const* opt = dynamic_cast<const pricer::products::AsianOption*>(&inst);
    if (!opt) {
        throw std::runtime_error("AsianOptionMCEngine: mauvais type d'instrument");
    }

    if (nPaths_ == 0 || nSteps_ == 0) {
        throw std::runtime_error("AsianOptionMCEngine: nPaths ou nSteps nul");
    }

    double T      = opt->maturity();
    double S0     = model_->spot();
    double sigma  = model_->sigma();
    double r      = model_->rate();
    double q      = model_->dividendYield();

    double dt     = T / static_cast<double>(nSteps_);
    double drift  = (r - q - 0.5 * sigma * sigma) * dt;
    double volDt  = sigma * std::sqrt(dt);

    std::mt19937_64 gen(seed_);
    std::normal_distribution<> norm(0.0, 1.0);

    double sumPayoff = 0.0;

    for (std::size_t p = 0; p < nPaths_; ++p) {
        double S = S0;
        double sumS = 0.0;

        for (std::size_t i = 0; i < nSteps_; ++i) {
            double z = norm(gen);
            S *= std::exp(drift + volDt * z);
            sumS += S;
        }

        double avgS = sumS / static_cast<double>(nSteps_);
        double payoff = opt->payoff()(avgS);
        sumPayoff += payoff;
    }

    double meanPayoff = sumPayoff / static_cast<double>(nPaths_);
    double df = model_->discount(T);
    return df * meanPayoff;
}

} 
