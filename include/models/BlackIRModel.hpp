#pragma once

#include <memory>
#include "market/MarketData.hpp"

namespace pricer::models {

class BlackIRModel {
public:
    explicit BlackIRModel(std::shared_ptr<pricer::market::YieldCurve> discountCurve,
                          double sigma)
        : discountCurve_(std::move(discountCurve)),
          sigma_(sigma) {}

    double discount(double T) const {
        return discountCurve_->discount(T);
    }

    double rate() const {
        return discountCurve_->rate();
    }

    double sigma() const { return sigma_; }

private:
    std::shared_ptr<pricer::market::YieldCurve> discountCurve_;
    double sigma_;
};

} 
