# Librairie de pricing C++17 – Vue d’ensemble

Cette librairie implémente le pricing de produits actions et de taux en C++17, avec :

- Options vanilles européennes (call / put) sous Black–Scholes ;
- Options digitales, asiatiques, barrières (actions) ;
- Produits de taux (caplet, cap, swap, swaption) sous modèle de Black sur les taux.

Le code est structuré autour de quelques abstractions clés :

- `Instrument` : représente un produit financier (option, caplet, swap, etc.) ;
- `PricingEngine` : moteur de valorisation (formule fermée Black–Scholes, Black IR, Monte Carlo…) ;
- `Payoff` : fonction de payoff (call, put, digital…) ;
- `Model` : modèle de marché (`BlackScholesModel`, `BlackIRModel`) ;
- `MarketData` : courbes de taux et actions (`YieldCurve`, `EquityCurve`).

Deux exécutables principaux :

- `pricing_example` : montre comment pricer chaque produit individuellement ;
- `pricing_factory_example` : montre l’utilisation des factories (`InstrumentFactory`, `EngineFactory`).

---

## Compilation rapide

Depuis la racine du projet :

```bash
mkdir build
cd build
cmake ..
cmake --build . --config Debug

## Lancer les exemples

Depuis le dossier où se trouve l’exécutable (par ex. build/Debug) :

./pricing_example european     
./pricing_example digital       
./pricing_example asian        
./pricing_example barrier       

./pricing_example caplet       
./pricing_example cap           
./pricing_example swap         
./pricing_example swaption  

./pricing_example all           

## Documentation détaillée

- [Produits actions (equity)](equity.md)
- [Produits de taux (rates)](rates.md)

Chaque page décrit :
- le produit financier (payoff, intuition économique) ;
- le modèle de valorisation utilisé ;
- la commande exacte pour lancer l’exemple correspondant ;
- un bref rappel du moteur de pricing utilisé.

