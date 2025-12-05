#pragma once

#include <memory>

#include "core/PricingEngine.hpp"
#include "models/BlackScholesModel.hpp"
#include "models/BlackIRModel.hpp"

namespace pricer::core {

class Instrument; 

class EngineFactory {
public:
    EngineFactory(std::shared_ptr<pricer::models::BlackScholesModel> equityModel,
                  std::shared_ptr<pricer::models::BlackIRModel> irModel)
        : equityModel_(std::move(equityModel)),
          irModel_(std::move(irModel)) {}

    std::shared_ptr<PricingEngine> createEngine(const Instrument& inst) const;

private:
    std::shared_ptr<pricer::models::BlackScholesModel> equityModel_;
    std::shared_ptr<pricer::models::BlackIRModel>      irModel_;
};

} 
