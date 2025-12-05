#include "doctest/doctest.h"

#include "market/MarketData.hpp"
#include "models/BlackScholesModel.hpp"
#include "core/Payoff.hpp"
#include "products/EuropeanOption.hpp"
#include "engines/EuropeanOptionBSEngine.hpp"

using namespace pricer;

TEST_CASE("EuropeanOptionBSEngine - ATM call") {
    double S0   = 100.0;
    double r    = 0.02;
    double q    = 0.0;
    double vol  = 0.20;
    double T    = 1.0;
    double K    = 100.0;

    auto discountCurve = std::make_shared<market::YieldCurve>(r);
    auto equityCurve   = std::make_shared<market::EquityCurve>(S0, q);

    auto bsModel = std::make_shared<models::BlackScholesModel>(
        discountCurve, equityCurve, vol
    );

    auto payoffCall = std::make_unique<core::PlainVanillaPayoff>(
        core::OptionType::Call, K
    );

    products::EuropeanOption callOption(std::move(payoffCall), T);

    auto engine = std::make_shared<engines::EuropeanOptionBSEngine>(bsModel);
    callOption.setPricingEngine(engine);

    double price = callOption.NPV();

    // Valeur de référence ~8.9 (ordre de grandeur)
    CHECK(price == doctest::Approx(8.9).epsilon(1e-2));
}
