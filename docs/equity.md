# Produits actions (Equity)

Cette page décrit les produits actions implémentés dans la librairie et explique comment lancer les exemples correspondants via l’exécutable `pricing_example`.

Les exemples utilisent un environnement par défaut :

- Sous-jacent : `S0 = 100`
- Taux sans risque : `r = 2%`
- Dividende continu : `q = 0%`
- Volatilité : `σ = 20%`
- Modèle : **Black–Scholes** sous la mesure risque neutre.

---

## 1. Option européenne (call / put)

### Produit

Option européenne de type call (dans l’exemple) :

- Payoff à maturité T : `(S_T - K)+`.

- Paramètres de l’exemple :

  - `K = 100`
  - `T = 1 an`

### Modèle de pricing

- Modèle : Black–Scholes avec volatilité constante ;
- Moteur de pricing : `EuropeanOptionBSEngine` (formule fermée) ;
- Utilise la fonction `blackForward` encapsulée dans `utils::BlackFormula`.

### Lancer l’exemple

Depuis `build/Debug` :

```bash
./pricing_example european

Côté C++, c’est la fonction `run_european_example()` qui :

1. Crée un environnement equity (`YieldCurve`, `EquityCurve`, `BlackScholesModel`) ;
2. Construit un `EuropeanOption` (call) avec un `PlainVanillaPayoff` ;
3. Attache un `EuropeanOptionBSEngine` au produit ;
4. Affiche la NPV de l’option.

---

## 2. Option digitale (cash-or-nothing)

### Produit

Option digitale de type cash-or-nothing :

- Payoff call digital : `Q * 1_{S_T > K}`,  
  où `Q` est le montant versé si l’option est dans la monnaie.

Paramètres de l’exemple :

- `K = 100`
- `T = 1 an`
- `payout = 10`

### Modèle de pricing

- Modèle : Black–Scholes ;
- Moteur de pricing : `DigitalOptionBSEngine` ;
- Utilise une formule fermée pour les digitales dans le modèle de Black–Scholes.

### Lancer l’exemple

```bash
./pricing_example digital

---

L’exemple construit un `DigitalOption` avec un `DigitalPayoff` et appelle `NPV()` via `DigitalOptionBSEngine`.


## 3. Option asiatique arithmétique (Monte Carlo)

### Produit

Call asiatique arithmétique :

- Payoff : `(S_bar - K)+`
- Moyenne arithmétique : `S_bar = (1/n) * sum_i S_{t_i}`

Paramètres de l’exemple :

- `K = 100`
- `T = 1 an`
- Nombre de pas : `nSteps = 50`
- Nombre de chemins : `nPaths = 10000`

### Modèle de pricing

- Modèle : Black–Scholes (diffusion lognormale) ;
- Moteur : `AsianOptionMCEngine` ;
- Technique : Monte Carlo :

  1. Simulation de trajectoires sous GBM ;
  2. Calcul de la moyenne arithmétique des spots ;
  3. Calcul du payoff `(S_bar - K)+` ;
  4. Actualisation via la courbe de taux.

### Lancer l’exemple

```bash
./pricing_example asian

---

La fonction `run_asian_example()` illustre un pricer Monte Carlo simple sur un produit exotique.


## 4. Option barrière (Up-and-Out)

### Produit

Option barrière de type call up-and-out :

- Si le sous-jacent franchit la barrière `B` pendant la vie du produit → knock-out (payoff nul) ;
- Sinon, payoff vanille `(S_T - K)+`.

Paramètres de l’exemple :

- `K = 100`
- `T = 1 an`
- `barrier = 120` (up-and-out)

### Modèle de pricing

- Modèle : Black–Scholes ;
- Moteur : `BarrierOptionMCEngine` ;
- Technique : Monte Carlo avec détection de franchissement de barrière le long des trajectoires.

### Lancer l’exemple

```bash
./pricing_example barrier

La fonction `run_barrier_example()` montre :
- la construction d’une `BarrierOption` avec un `PlainVanillaPayoff` ;
- le pricing Monte Carlo via `BarrierOptionMCEngine