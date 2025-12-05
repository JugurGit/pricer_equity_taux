#pragma once

#include <memory>
#include "core/PricingEngine.hpp"
#include "models/BlackScholesModel.hpp"

namespace pricer::engines {

class DigitalOptionBSEngine : public pricer::core::PricingEngine {
public:
    explicit DigitalOptionBSEngine(
        std::shared_ptr<pricer::models::BlackScholesModel> model)
        : model_(std::move(model)) {}

protected:
    double priceImpl(const pricer::core::Instrument& inst) const override;

private:
    std::shared_ptr<pricer::models::BlackScholesModel> model_;
};

} 
