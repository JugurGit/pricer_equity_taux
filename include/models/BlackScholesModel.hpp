#pragma once

#include <memory>
#include "market/MarketData.hpp"

namespace pricer::models {

class BlackScholesModel {
public:
    BlackScholesModel(std::shared_ptr<pricer::market::YieldCurve> discountCurve,
                      std::shared_ptr<pricer::market::EquityCurve> equityCurve,
                      double sigma)
        : discountCurve_(std::move(discountCurve)),
          equityCurve_(std::move(equityCurve)),
          sigma_(sigma) {}

    double spot() const;
    double sigma() const { return sigma_; }
    double discount(double T) const;
    double forward(double T) const;
    double rate() const;
    double dividendYield() const;


private:
    std::shared_ptr<pricer::market::YieldCurve> discountCurve_;
    std::shared_ptr<pricer::market::EquityCurve> equityCurve_;
    double sigma_;
};

} 