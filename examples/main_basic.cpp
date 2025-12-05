#include <iostream>
#include <memory>
#include <vector>
#include <string>

#include "market/MarketData.hpp"
#include "models/BlackScholesModel.hpp"
#include "models/BlackIRModel.hpp"

#include "core/Payoff.hpp"
#include "core/InstrumentFactory.hpp"

#include "products/EuropeanOption.hpp"
#include "products/DigitalOption.hpp"
#include "products/AsianOption.hpp"
#include "products/BarrierOption.hpp"
#include "products/CapFloor.hpp"
#include "products/Swap.hpp"

#include "engines/EuropeanOptionBSEngine.hpp"
#include "engines/DigitalOptionBSEngine.hpp"
#include "engines/AsianOptionMCEngine.hpp"
#include "engines/BarrierOptionMCEngine.hpp"
#include "engines/CapFloorEngines.hpp"
#include "engines/SwapEngines.hpp"

// ==========================
// Helpers "environnement"
// ==========================

struct EquityEnv {
    std::shared_ptr<pricer::market::YieldCurve> yc;
    std::shared_ptr<pricer::market::EquityCurve> ec;
    std::shared_ptr<pricer::models::BlackScholesModel> model;
};

struct RatesEnv {
    std::shared_ptr<pricer::market::YieldCurve> yc;
    std::shared_ptr<pricer::models::BlackIRModel> model;
};

EquityEnv makeDefaultEquityEnv() {
    using namespace pricer;

    double S0   = 100.0;
    double r    = 0.02;
    double q    = 0.0;
    double vol  = 0.20;

    auto yc = std::make_shared<market::YieldCurve>(r);
    auto ec = std::make_shared<market::EquityCurve>(S0, q);
    auto bs = std::make_shared<models::BlackScholesModel>(yc, ec, vol);

    return EquityEnv{yc, ec, bs};
}

RatesEnv makeDefaultRatesEnv() {
    using namespace pricer;

    double r       = 0.02;
    double sigmaIR = 0.25;

    auto yc   = std::make_shared<market::YieldCurve>(r);
    auto bir  = std::make_shared<models::BlackIRModel>(yc, sigmaIR);

    return RatesEnv{yc, bir};
}

// ==========================
// Fonctions d'exemple
// ==========================

void run_european_example() {
    using namespace pricer;

    std::cout << "=== European Option (Black-Scholes) ===\n";

    auto env = makeDefaultEquityEnv();

    double K = 100.0;
    double T = 1.0;

    auto payoff = std::make_unique<core::PlainVanillaPayoff>(
        core::OptionType::Call, K
    );

    products::EuropeanOption call(std::move(payoff), T);

    auto engine = std::make_shared<engines::EuropeanOptionBSEngine>(env.model);
    call.setPricingEngine(engine);

    std::cout << "NPV(Call) = " << call.NPV() << "\n\n";
}

void run_digital_example() {
    using namespace pricer;

    std::cout << "=== Digital Option (Black-Scholes) ===\n";

    auto env = makeDefaultEquityEnv();

    double K       = 100.0;
    double T       = 1.0;
    double payout  = 10.0;

    auto payoff = std::make_unique<core::DigitalPayoff>(
        core::OptionType::Call, K, payout
    );

    products::DigitalOption digital(std::move(payoff), T);

    auto engine = std::make_shared<engines::DigitalOptionBSEngine>(env.model);
    digital.setPricingEngine(engine);

    std::cout << "NPV(Digital Call) = " << digital.NPV() << "\n\n";
}

void run_asian_example() {
    using namespace pricer;

    std::cout << "=== Asian Option (Monte Carlo) ===\n";

    auto env = makeDefaultEquityEnv();

    double K = 100.0;
    double T = 1.0;

    auto payoff = std::make_unique<core::PlainVanillaPayoff>(
        core::OptionType::Call, K
    );
    products::AsianOption asian(std::move(payoff), T);

    std::size_t nPaths = 10000;
    std::size_t nSteps = 50;
    std::uint64_t seed = 1234UL;

    auto engine = std::make_shared<engines::AsianOptionMCEngine>(
        env.model, nPaths, nSteps, seed
    );
    asian.setPricingEngine(engine);

    std::cout << "NPV(Asian Call) = " << asian.NPV() << "\n\n";
}

void run_barrier_example() {
    using namespace pricer;

    std::cout << "=== Barrier Option (Monte Carlo) ===\n";

    auto env = makeDefaultEquityEnv();

    double K       = 100.0;
    double T       = 1.0;
    double barrier = 120.0;

    auto payoff = std::make_unique<core::PlainVanillaPayoff>(
        core::OptionType::Call, K
    );
    products::BarrierOption opt(
        std::move(payoff),
        T,
        barrier,
        products::BarrierType::UpAndOut
    );

    std::size_t nPaths = 10000;
    std::size_t nSteps = 252;
    std::uint64_t seed = 5678UL;

    auto engine = std::make_shared<engines::BarrierOptionMCEngine>(
        env.model, nPaths, nSteps, seed
    );
    opt.setPricingEngine(engine);

    std::cout << "NPV(Up-and-out Call) = " << opt.NPV() << "\n\n";
}

void run_caplet_example() {
    using namespace pricer;

    std::cout << "=== Caplet (Black IR) ===\n";

    auto env = makeDefaultRatesEnv();

    double notional     = 1'000'000.0;
    double strike       = 0.03;
    double forward      = 0.028;
    double start        = 0.5;
    double end          = 1.0;
    double yearFraction = 0.5;

    products::Caplet caplet(
        notional,
        strike,
        forward,
        start,
        end,
        yearFraction,
        core::OptionType::Call
    );

    auto engine = std::make_shared<engines::CapletBlackEngine>(env.model);
    caplet.setPricingEngine(engine);

    std::cout << "NPV(Caplet) = " << caplet.NPV() << "\n\n";
}

void run_cap_example() {
    using namespace pricer;

    std::cout << "=== Cap (Black IR) ===\n";

    auto env = makeDefaultRatesEnv();

    double notional = 1'000'000.0;
    double strike   = 0.03;
    double forward  = 0.028;   // taux forward supposé constant

    // Dates de début/fin et accruals des périodes du cap
    std::vector<double> starts  = {0.5, 1.5, 2.5, 3.5};
    std::vector<double> ends    = {1.5, 2.5, 3.5, 4.5};
    std::vector<double> accrual = {1.0, 1.0, 1.0, 1.0};

    if (starts.size() != ends.size() || starts.size() != accrual.size()) {
        throw std::runtime_error("run_cap_example: tailles de vecteurs incohérentes");
    }

    // 1) Construire le portefeuille de caplets, comme le veut TON design
    std::vector<products::Caplet> caplets;
    caplets.reserve(starts.size());

    for (std::size_t i = 0; i < starts.size(); ++i) {
        caplets.emplace_back(
            notional,
            strike,
            forward,
            starts[i],
            ends[i],
            accrual[i]  // yearFraction
            // type = Call par défaut, pas besoin de le passer
        );
    }

    // 2) Construire le Cap à partir du vector<Caplet>
    products::Cap cap(std::move(caplets));

    // 3) Assigner le moteur de pricing de type Black IR
    auto engine = std::make_shared<engines::CapBlackEngine>(env.model);
    cap.setPricingEngine(engine);

    // 4) Calculer et afficher la NPV
    std::cout << "NPV(Cap) = " << cap.NPV() << "\n\n";
}


void run_swap_example() {
    using namespace pricer;

    std::cout << "=== Interest Rate Swap ===\n";

    auto env = makeDefaultRatesEnv();

    double notional = 1'000'000.0;
    double fixedK   = 0.03;
    double forward  = 0.028;
    bool payer      = true; // pay fixed, receive float

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

    auto engine = std::make_shared<engines::SwapEngine>(env.model);
    swap.setPricingEngine(engine);

    std::cout << "NPV(Swap) = " << swap.NPV() << "\n\n";
}

void run_swaption_example() {
    using namespace pricer;

    std::cout << "=== Swaption (Black IR) ===\n";

    auto env = makeDefaultRatesEnv();

    double notional = 1'000'000.0;
    double fixedK   = 0.03;
    double forward  = 0.028;
    bool payer      = true;

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

    auto engine = std::make_shared<engines::SwaptionBlackEngine>(env.model);
    swaption.setPricingEngine(engine);

    std::cout << "NPV(Swaption payer) = " << swaption.NPV() << "\n\n";
}

// ==========================
// main avec dispatch
// ==========================

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: pricing_example <product>\n";
        std::cout << "Products:\n";
        std::cout << "  european\n";
        std::cout << "  digital\n";
        std::cout << "  asian\n";
        std::cout << "  barrier\n";
        std::cout << "  caplet\n";
        std::cout << "  cap\n";
        std::cout << "  swap\n";
        std::cout << "  swaption\n";
        std::cout << "  all\n";
        return 0;
    }

    std::string prod = argv[1];

    if (prod == "european")      run_european_example();
    else if (prod == "digital")  run_digital_example();
    else if (prod == "asian")    run_asian_example();
    else if (prod == "barrier")  run_barrier_example();
    else if (prod == "caplet")   run_caplet_example();
    else if (prod == "cap")      run_cap_example();
    else if (prod == "swap")     run_swap_example();
    else if (prod == "swaption") run_swaption_example();
    else if (prod == "all") {
        run_european_example();
        run_digital_example();
        run_asian_example();
        run_barrier_example();
        run_caplet_example();
        run_cap_example();
        run_swap_example();
        run_swaption_example();
    } else {
        std::cerr << "Unknown product: " << prod << "\n";
        return 1;
    }

    return 0;
}
