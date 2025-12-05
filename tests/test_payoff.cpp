#include "doctest/doctest.h"

#include "core/Payoff.hpp"

using namespace pricer::core;

TEST_CASE("PlainVanillaPayoff Call") {
    PlainVanillaPayoff payoff(OptionType::Call, 100.0);

    CHECK(payoff(120.0) == doctest::Approx(20.0));
    CHECK(payoff(80.0)  == doctest::Approx(0.0));
}

TEST_CASE("PlainVanillaPayoff Put") {
    PlainVanillaPayoff payoff(OptionType::Put, 100.0);

    CHECK(payoff(80.0)  == doctest::Approx(20.0));
    CHECK(payoff(120.0) == doctest::Approx(0.0));
}

TEST_CASE("DigitalPayoff Call") {
    DigitalPayoff payoff(OptionType::Call, 100.0, 10.0);

    CHECK(payoff(101.0) == doctest::Approx(10.0));
    CHECK(payoff(99.0)  == doctest::Approx(0.0));
}

TEST_CASE("DigitalPayoff Put") {
    DigitalPayoff payoff(OptionType::Put, 100.0, 10.0);

    CHECK(payoff(99.0)  == doctest::Approx(10.0));
    CHECK(payoff(101.0) == doctest::Approx(0.0));
}
