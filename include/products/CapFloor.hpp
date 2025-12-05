#pragma once

#include <vector>
#include "core/Instrument.hpp"
#include "core/Payoff.hpp"

namespace pricer::products {

class Caplet : public pricer::core::Instrument {
public:
    Caplet(double notional,
           double strike,
           double forwardRate,
           double start,
           double end,
           double yearFraction,
           pricer::core::OptionType type = pricer::core::OptionType::Call)
        : notional_(notional),
          strike_(strike),
          forwardRate_(forwardRate),
          start_(start),
          end_(end),
          yearFraction_(yearFraction),
          type_(type) {}

    double notional() const      { return notional_; }
    double strike() const        { return strike_; }
    double forwardRate() const   { return forwardRate_; }
    double start() const         { return start_; }       
    double end() const           { return end_; }        
    double yearFraction() const  { return yearFraction_; }
    pricer::core::OptionType type() const { return type_; }

private:
    double notional_;
    double strike_;
    double forwardRate_;
    double start_;
    double end_;
    double yearFraction_;
    pricer::core::OptionType type_;
};

class Floorlet : public Caplet {
public:
    Floorlet(double notional,
             double strike,
             double forwardRate,
             double start,
             double end,
             double yearFraction)
        : Caplet(notional, strike, forwardRate,
                 start, end, yearFraction,
                 pricer::core::OptionType::Put) {}
};

class Cap : public pricer::core::Instrument {
public:
    explicit Cap(std::vector<Caplet> caplets)
        : caplets_(std::move(caplets)) {}

    const std::vector<Caplet>& caplets() const { return caplets_; }

private:
    std::vector<Caplet> caplets_;
};

class Floor : public pricer::core::Instrument {
public:
    explicit Floor(std::vector<Floorlet> floorlets)
        : floorlets_(std::move(floorlets)) {}

    const std::vector<Floorlet>& floorlets() const { return floorlets_; }

private:
    std::vector<Floorlet> floorlets_;
};

} 
