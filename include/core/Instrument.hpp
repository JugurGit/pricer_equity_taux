#pragma once

#include <memory>

namespace pricer::core {

class PricingEngine;

class Instrument {
public:
    virtual ~Instrument() = default;

    void setPricingEngine(std::shared_ptr<PricingEngine> engine);

    double NPV() const;

protected:
    Instrument() = default;

private:
    std::shared_ptr<PricingEngine> pricingEngine_;
};

} 
