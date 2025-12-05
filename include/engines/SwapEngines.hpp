#pragma once

#include <memory>
#include "core/PricingEngine.hpp"
#include "models/BlackIRModel.hpp"

namespace pricer::engines {

class SwapEngine : public pricer::core::PricingEngine {
public:
    explicit SwapEngine(std::shared_ptr<pricer::models::BlackIRModel> model)
        : model_(std::move(model)) {}

protected:
    double priceImpl(const pricer::core::Instrument& inst) const override;

private:
    std::shared_ptr<pricer::models::BlackIRModel> model_;
};

class SwaptionBlackEngine : public pricer::core::PricingEngine {
public:
    explicit SwaptionBlackEngine(std::shared_ptr<pricer::models::BlackIRModel> model)
        : model_(std::move(model)) {}

protected:
    double priceImpl(const pricer::core::Instrument& inst) const override;

private:
    std::shared_ptr<pricer::models::BlackIRModel> model_;
};

} 
