#include "doctest/doctest.h"

#include "utils/BlackFormula.hpp"
#include <algorithm>    


using namespace pricer;

TEST_CASE("normalCdf basic properties") {
    CHECK(utils::normalCdf(0.0)   == doctest::Approx(0.5).epsilon(1e-6));
    CHECK(utils::normalCdf(10.0)  == doctest::Approx(1.0).epsilon(1e-6));
    CHECK(utils::normalCdf(-10.0) == doctest::Approx(0.0).epsilon(1e-6));
}

TEST_CASE("blackForward intrinsic when stdDev=0") {
    double F = 100.0;
    double K = 90.0;
    double stdDev = 0.0;

    double call = utils::blackForward(F, K, stdDev, core::OptionType::Call);
    double put  = utils::blackForward(F, K, stdDev, core::OptionType::Put);

    CHECK(call == doctest::Approx(std::max(F - K, 0.0)));
    CHECK(put  == doctest::Approx(std::max(K - F, 0.0)));
}
