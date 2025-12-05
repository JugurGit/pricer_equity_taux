#pragma once

namespace pricer::core {

class Instrument; 

class PricingEngine {
public:
    virtual ~PricingEngine() = default;

    double calculate(const Instrument& inst) const {
        return priceImpl(inst);
    }

protected:
    PricingEngine() = default;

    virtual double priceImpl(const Instrument& inst) const = 0;
};

} 
