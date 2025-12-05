#pragma once

#include <memory>
#include "core/PricingEngine.hpp"
#include "models/BlackScholesModel.hpp"

namespace pricer::engines {

class AsianOptionMCEngine : public pricer::core::PricingEngine {
public:
    AsianOptionMCEngine(std::shared_ptr<pricer::models::BlackScholesModel> model,
                        std::size_t nPaths,
                        std::size_t nSteps,
                        unsigned long seed = 42UL)
        : model_(std::move(model)),
          nPaths_(nPaths),
          nSteps_(nSteps),
          seed_(seed) {}

protected:
    double priceImpl(const pricer::core::Instrument& inst) const override;

private:
    std::shared_ptr<pricer::models::BlackScholesModel> model_;
    std::size_t nPaths_;
    std::size_t nSteps_;
    unsigned long seed_;
};

} 
