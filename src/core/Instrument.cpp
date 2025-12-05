#include "core/Instrument.hpp"
#include "core/PricingEngine.hpp"

namespace pricer::core {

void Instrument::setPricingEngine(std::shared_ptr<PricingEngine> engine) {
    pricingEngine_ = std::move(engine);
}

double Instrument::NPV() const {
    if (!pricingEngine_) {
        return 0.0;
    }
    return pricingEngine_->calculate(*this);
}

} 
