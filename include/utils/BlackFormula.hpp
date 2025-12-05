#pragma once

#include "core/Payoff.hpp" 

namespace pricer::utils {

// CDF de la loi normale standard
double normalCdf(double x);

// Black sur un taux/forward F, strike K, stdDev = sigma * sqrt(T)
double blackForward(double F, double K, double stdDev,
                    pricer::core::OptionType type);

// Digital cash-or-nothing sur forward F, strike K, stdDev, payoff = Q
double blackDigitalForward(double F, double K, double stdDev,
                           pricer::core::OptionType type,
                           double payout);

} 
