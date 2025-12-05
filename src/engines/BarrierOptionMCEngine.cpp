#include "engines/BarrierOptionMCEngine.hpp"

#include "products/BarrierOption.hpp"

#include <random>
#include <cmath>
#include <stdexcept>

namespace pricer::engines {

double BarrierOptionMCEngine::priceImpl(const pricer::core::Instrument& inst) const {
    auto const* opt = dynamic_cast<const pricer::products::BarrierOption*>(&inst);
    if (!opt) {
        throw std::runtime_error("BarrierOptionMCEngine: mauvais type d'instrument");
    }

    if (nPaths_ == 0 || nSteps_ == 0) {
        throw std::runtime_error("BarrierOptionMCEngine: nPaths ou nSteps nul");
    }

    double T      = opt->maturity();
    double S0     = model_->spot();
    double sigma  = model_->sigma();
    double r      = model_->rate();
    double q      = model_->dividendYield();

    double dt     = T / static_cast<double>(nSteps_);
    double drift  = (r - q - 0.5 * sigma * sigma) * dt;
    double volDt  = sigma * std::sqrt(dt);

    double B      = opt->barrier();
    auto   bType  = opt->barrierType();

    std::mt19937_64 gen(seed_);
    std::normal_distribution<> norm(0.0, 1.0);

    double sumPayoff = 0.0;

    for (std::size_t p = 0; p < nPaths_; ++p) {
        double S = S0;
        bool hit = false;

        for (std::size_t i = 0; i < nSteps_; ++i) {
            double z = norm(gen);
            S *= std::exp(drift + volDt * z);

            switch (bType) {
                case pricer::products::BarrierType::UpAndOut:
                case pricer::products::BarrierType::UpAndIn:
                    if (S >= B) hit = true;
                    break;
                case pricer::products::BarrierType::DownAndOut:
                case pricer::products::BarrierType::DownAndIn:
                    if (S <= B) hit = true;
                    break;
            }
        }

        double payoff = 0.0;

        using pricer::products::BarrierType;
        switch (bType) {
            case BarrierType::UpAndOut:
            case BarrierType::DownAndOut:
                payoff = hit ? 0.0 : opt->payoff()(S);
                break;
            case BarrierType::UpAndIn:
            case BarrierType::DownAndIn:
                payoff = hit ? opt->payoff()(S) : 0.0;
                break;
        }

        sumPayoff += payoff;
    }

    double meanPayoff = sumPayoff / static_cast<double>(nPaths_);
    double df = model_->discount(T);
    return df * meanPayoff;
}

} 
