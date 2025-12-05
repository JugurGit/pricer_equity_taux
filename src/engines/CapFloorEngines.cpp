#include "engines/CapFloorEngines.hpp"

#include "products/CapFloor.hpp"
#include "core/Payoff.hpp"
#include "utils/BlackFormula.hpp"

#include <cmath>
#include <stdexcept>

namespace pricer::engines {



double CapletBlackEngine::priceImpl(const pricer::core::Instrument& inst) const {
    auto const* caplet = dynamic_cast<const pricer::products::Caplet*>(&inst);
    if (!caplet) {
        throw std::runtime_error("CapletBlackEngine: mauvais type d'instrument");
    }

    double Toption = caplet->start();   
    double Tend    = caplet->end();     
    double F       = caplet->forwardRate();
    double K       = caplet->strike();
    double sigma   = model_->sigma();
    double df      = model_->discount(Tend);

    double stdDev = sigma * std::sqrt(Toption);
    double black  = pricer::utils::blackForward(F, K, stdDev, caplet->type());

    double price = caplet->notional() * caplet->yearFraction() * df * black;

    return price;
}


double CapBlackEngine::priceImpl(const pricer::core::Instrument& inst) const {
    auto const* cap = dynamic_cast<const pricer::products::Cap*>(&inst);
    if (!cap) {
        throw std::runtime_error("CapBlackEngine: mauvais type d'instrument");
    }

    double total = 0.0;

    for (const auto& c : cap->caplets()) {
        double Toption = c.start();
        double Tend    = c.end();
        double F       = c.forwardRate();
        double K       = c.strike();
        double sigma   = model_->sigma();
        double df      = model_->discount(Tend);

        double stdDev = sigma * std::sqrt(Toption);
        double black  = pricer::utils::blackForward(F, K, stdDev, c.type());

        total += c.notional() * c.yearFraction() * df * black;

    }

    return total;
}


double FloorBlackEngine::priceImpl(const pricer::core::Instrument& inst) const {
    auto const* floor = dynamic_cast<const pricer::products::Floor*>(&inst);
    if (!floor) {
        throw std::runtime_error("FloorBlackEngine: mauvais type d'instrument");
    }

    double total = 0.0;

    for (const auto& f : floor->floorlets()) {
        double Toption = f.start();
        double Tend    = f.end();
        double F       = f.forwardRate();
        double K       = f.strike();
        double sigma   = model_->sigma();
        double df      = model_->discount(Tend);

        double stdDev = sigma * std::sqrt(Toption);
        double black  = pricer::utils::blackForward(F, K, stdDev, f.type());

        total += f.notional() * f.yearFraction() * df * black;

    }

    return total;
}

} 
