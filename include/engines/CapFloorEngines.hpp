#pragma once

#include <memory>
#include "core/PricingEngine.hpp"
#include "models/BlackIRModel.hpp"

namespace pricer::engines {

class CapletBlackEngine : public pricer::core::PricingEngine {
public:
    explicit CapletBlackEngine(std::shared_ptr<pricer::models::BlackIRModel> model)
        : model_(std::move(model)) {}

protected:
    double priceImpl(const pricer::core::Instrument& inst) const override;

private:
    std::shared_ptr<pricer::models::BlackIRModel> model_;
};

class CapBlackEngine : public pricer::core::PricingEngine {
public:
    explicit CapBlackEngine(std::shared_ptr<pricer::models::BlackIRModel> model)
        : model_(std::move(model)) {}

protected:
    double priceImpl(const pricer::core::Instrument& inst) const override;

private:
    std::shared_ptr<pricer::models::BlackIRModel> model_;
};

class FloorBlackEngine : public pricer::core::PricingEngine {
public:
    explicit FloorBlackEngine(std::shared_ptr<pricer::models::BlackIRModel> model)
        : model_(std::move(model)) {}

protected:
    double priceImpl(const pricer::core::Instrument& inst) const override;

private:
    std::shared_ptr<pricer::models::BlackIRModel> model_;
};

} 
