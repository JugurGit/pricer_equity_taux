#pragma once

namespace pricer::core {

enum class OptionType { Call, Put };

class Payoff {
public:
    virtual ~Payoff() = default;
    virtual double operator()(double spot) const = 0;
};

class PlainVanillaPayoff : public Payoff {
public:
    PlainVanillaPayoff(OptionType type, double strike)
        : type_(type), strike_(strike) {}

    double operator()(double spot) const override;

    OptionType type() const { return type_; }
    double strike() const { return strike_; }

private:
    OptionType type_;
    double strike_;
};

class DigitalPayoff : public Payoff {
public:
    DigitalPayoff(OptionType type, double strike, double payout)
        : type_(type), strike_(strike), payout_(payout) {}

    double operator()(double spot) const override;

    OptionType type() const { return type_; }
    double strike() const { return strike_; }
    double payout() const { return payout_; }

private:
    OptionType type_;
    double strike_;
    double payout_;
};


} 
