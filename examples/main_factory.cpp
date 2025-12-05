#include <iostream>
#include <memory>
#include <vector>

#include "core/InstrumentFactory.hpp"
#include "core/EngineFactory.hpp"
#include "market/MarketData.hpp"
#include "models/BlackScholesModel.hpp"
#include "models/BlackIRModel.hpp"

int main() {
    using namespace pricer;

    // ===== Environnement equity =====
    auto eqDiscount = std::make_shared<market::YieldCurve>(0.02);
    auto eqCurve    = std::make_shared<market::EquityCurve>(100.0, 0.0);
    auto bsModel    = std::make_shared<models::BlackScholesModel>(
        eqDiscount, eqCurve, 0.20
    );

    // ===== Environnement taux =====
    auto irCurve = std::make_shared<market::YieldCurve>(0.02);
    auto irModel = std::make_shared<models::BlackIRModel>(irCurve, 0.25);

    // ===== EngineFactory =====
    core::EngineFactory engineFactory(bsModel, irModel);

    auto call = core::InstrumentFactory::makeEuropeanOption(
        core::OptionType::Call, 100.0, 1.0
    );
    auto callEngine = engineFactory.createEngine(call);
    call.setPricingEngine(callEngine);
    std::cout << "European Call NPV = " << call.NPV() << "\n";

    auto caplet = core::InstrumentFactory::makeCaplet(
        1'000'000.0, 0.03, 0.028,
        0.5, 1.0, 0.5
    );
    auto capletEngine = engineFactory.createEngine(caplet);
    caplet.setPricingEngine(capletEngine);
    std::cout << "Caplet NPV        = " << caplet.NPV() << "\n";

    std::vector<double> times   = {1.0, 2.0, 3.0, 4.0, 5.0};
    std::vector<double> accrual = {1.0, 1.0, 1.0, 1.0, 1.0};

    auto swap = core::InstrumentFactory::makeSwap(
        1'000'000.0,
        0.03,       
        times,
        accrual,
        0.028,      
        true        // payer
    );

    auto swapEngine = engineFactory.createEngine(swap);
    swap.setPricingEngine(swapEngine);
    std::cout << "Swap NPV          = " << swap.NPV() << "\n";

    auto swaption = core::InstrumentFactory::makeSwaption(swap, 1.0);
    auto swaptionEngine = engineFactory.createEngine(swaption);
    swaption.setPricingEngine(swaptionEngine);
    std::cout << "Swaption NPV      = " << swaption.NPV() << "\n";

    return 0;
}
