#include "core/EngineFactory.hpp"

#include "core/Instrument.hpp"

// Produits
#include "products/EuropeanOption.hpp"
#include "products/DigitalOption.hpp"
#include "products/AsianOption.hpp"
#include "products/BarrierOption.hpp"
#include "products/CapFloor.hpp"
#include "products/Swap.hpp"

// Engines
#include "engines/EuropeanOptionBSEngine.hpp"
#include "engines/DigitalOptionBSEngine.hpp"
#include "engines/AsianOptionMCEngine.hpp"
#include "engines/BarrierOptionMCEngine.hpp"
#include "engines/CapFloorEngines.hpp"
#include "engines/SwapEngines.hpp"

#include <stdexcept>

namespace pricer::core {

std::shared_ptr<PricingEngine>
EngineFactory::createEngine(const Instrument& inst) const {
    using namespace pricer;

    // ======== Equity ========

    if (auto const* opt = dynamic_cast<const products::EuropeanOption*>(&inst)) {
        (void)opt; // non utilisé, juste pour le cast
        return std::make_shared<engines::EuropeanOptionBSEngine>(equityModel_);
    }

    if (auto const* opt = dynamic_cast<const products::DigitalOption*>(&inst)) {
        (void)opt;
        return std::make_shared<engines::DigitalOptionBSEngine>(equityModel_);
    }

    if (auto const* opt = dynamic_cast<const products::AsianOption*>(&inst)) {
        (void)opt;
        // paramètres MC par défaut
        return std::make_shared<engines::AsianOptionMCEngine>(
            equityModel_,
            10000,  // nPaths
            50,     // nSteps
            777UL   // seed
        );
    }

    if (auto const* opt = dynamic_cast<const products::BarrierOption*>(&inst)) {
        (void)opt;
        return std::make_shared<engines::BarrierOptionMCEngine>(
            equityModel_,
            10000,
            252,
            2024UL
        );
    }

    // ======== Taux ========

    if (auto const* caplet = dynamic_cast<const products::Caplet*>(&inst)) {
        (void)caplet;
        return std::make_shared<engines::CapletBlackEngine>(irModel_);
    }

    if (auto const* cap = dynamic_cast<const products::Cap*>(&inst)) {
        (void)cap;
        return std::make_shared<engines::CapBlackEngine>(irModel_);
    }

    if (auto const* floor = dynamic_cast<const products::Floor*>(&inst)) {
        (void)floor;
        return std::make_shared<engines::FloorBlackEngine>(irModel_);
    }

    if (auto const* swap = dynamic_cast<const products::InterestRateSwap*>(&inst)) {
        (void)swap;
        return std::make_shared<engines::SwapEngine>(irModel_);
    }

    if (auto const* swpt = dynamic_cast<const products::Swaption*>(&inst)) {
        (void)swpt;
        return std::make_shared<engines::SwaptionBlackEngine>(irModel_);
    }

    throw std::runtime_error("EngineFactory::createEngine: type d'instrument non supporté");
}

} 
