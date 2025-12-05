#include "doctest/doctest.h"

#include "market/MarketData.hpp"
#include "models/BlackIRModel.hpp"
#include "products/CapFloor.hpp"
#include "engines/CapFloorEngines.hpp"
#include "products/Swap.hpp"
#include "engines/SwapEngines.hpp"

using namespace pricer;

TEST_CASE("CapletBlackEngine - simple caplet") {
    double r        = 0.02;
    double sigmaIR  = 0.25;
    double notional = 1'000'000.0;
    double strike   = 0.03;
    double forward  = 0.028;

    // période [0.5, 1.0], yearFraction = 0.5
    double start        = 0.5;
    double end          = 1.0;
    double yearFraction = 0.5;

    auto discountCurveIR = std::make_shared<market::YieldCurve>(r);
    auto modelIR         = std::make_shared<models::BlackIRModel>(
        discountCurveIR, sigmaIR
    );

    products::Caplet caplet(
        notional,
        strike,
        forward,
        start,
        end,
        yearFraction,
        core::OptionType::Call
    );

    auto engine = std::make_shared<engines::CapletBlackEngine>(modelIR);
    caplet.setPricingEngine(engine);

    double price = caplet.NPV();

    // Valeur de référence ~585.9953
    CHECK(price == doctest::Approx(585.9953).epsilon(1e-3));
}

TEST_CASE("SwapEngine - 5Y payer swap") {
    double r        = 0.02;
    double notional = 1'000'000.0;
    double fixedK   = 0.03;
    double forward  = 0.028;
    bool payer      = true; // pay fixed, receive float

    auto discountCurveIR = std::make_shared<market::YieldCurve>(r);
    auto modelIR         = std::make_shared<models::BlackIRModel>(
        discountCurveIR, 0.25  // sigma pas utilisé par SwapEngine
    );

    std::vector<double> times   = {1.0, 2.0, 3.0, 4.0, 5.0};
    std::vector<double> accrual = {1.0, 1.0, 1.0, 1.0, 1.0};

    products::InterestRateSwap swap(
        notional,
        fixedK,
        times,
        accrual,
        forward,
        payer
    );

    auto swapEngine = std::make_shared<engines::SwapEngine>(modelIR);
    swap.setPricingEngine(swapEngine);

    double pv = swap.NPV();

    // Valeur de référence ~ -9421.41
    CHECK(pv == doctest::Approx(-9421.41).epsilon(1e-2));
}

TEST_CASE("SwaptionBlackEngine - payer swaption") {
    double r        = 0.02;
    double sigmaIR  = 0.25;
    double notional = 1'000'000.0;
    double fixedK   = 0.03;
    double forward  = 0.028;
    bool payer      = true;

    auto discountCurveIR = std::make_shared<market::YieldCurve>(r);
    auto modelIR         = std::make_shared<models::BlackIRModel>(
        discountCurveIR, sigmaIR
    );

    std::vector<double> times   = {1.0, 2.0, 3.0, 4.0, 5.0};
    std::vector<double> accrual = {1.0, 1.0, 1.0, 1.0, 1.0};

    products::InterestRateSwap swap(
        notional,
        fixedK,
        times,
        accrual,
        forward,
        payer
    );

    double exerciseTime = 1.0;
    products::Swaption swaption(swap, exerciseTime);

    auto engine = std::make_shared<engines::SwaptionBlackEngine>(modelIR);
    swaption.setPricingEngine(engine);

    double price = swaption.NPV();

    // Valeur de référence ~ 9390.13
    CHECK(price == doctest::Approx(9390.13).epsilon(1e-2));
}
