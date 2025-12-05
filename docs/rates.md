# Produits de taux (Rates)

Cette page décrit les produits de taux implémentés dans la librairie :

- Caplet ;
- Cap (portefeuille de caplets) ;
- Interest Rate Swap ;
- Swaption européenne.

Les exemples utilisent un environnement de taux simplifié :

- Courbe de taux plate : `r = 2%` ;
- Volatilité Black pour les taux : `sigma_IR = 25%` ;
- Modèle : `BlackIRModel` + `YieldCurve`.

---

## 1. Caplet (modèle de Black)

### Produit

Un caplet est une option call sur un taux forward `L_T`.

Payoff (à la date de paiement) :

`Payoff = N * delta * max(L_T - K, 0)`

Paramètres de l’exemple (`run_caplet_example()`):

- Notional `N = 1 000 000` ;
- Strike `K = 3%` ;
- Taux forward `F = 2.8%` ;
- Période `[0.5, 1.0]` an ;
- `yearFraction = 0.5`.

### Modèle de pricing

- Modèle : Black sur le taux forward ;
- Moteur : `CapletBlackEngine` ;
- Utilise `BlackIRModel` pour :
  - l’actualisation (courbe de taux) ;
  - la volatilité Black.

### Lancer l’exemple

```bash
./pricing_example caplet

L’exemple construit un `Caplet`, lui assigne un `CapletBlackEngine` et affiche la NPV.

---

## 2. Cap (portefeuille de caplets)

### Produit

Un cap est un portefeuille de caplets sur plusieurs périodes successives.

Dans l’exemple (`run_cap_example()`):

- Notional `N = 1 000 000` ;
- Strike `K = 3%` ;
- Taux forward constant `F = 2.8%` ;
- Périodes approximées :
  - `starts = {0.5, 1.5, 2.5, 3.5}`
  - `ends   = {1.5, 2.5, 3.5, 4.5}`
  - `accrual = {1.0, 1.0, 1.0, 1.0}`

La classe `Cap` contient un `std::vector<Caplet>` qui représente ce portefeuille.

### Modèle de pricing

- Moteur : `CapBlackEngine` ;
- Prix = somme des valeurs des caplets, chacun étant pricé avec la formule de Black.

### Lancer l’exemple

```bash
./pricing_example cap

La fonction `run_cap_example()` :

1. Construit un `std::vector<Caplet>` à partir des dates de début, de fin et des accruals ;
2. Construit un `Cap` à partir de ce vecteur ;
3. Attache un `CapBlackEngine` au produit ;
4. Affiche la NPV du cap.

---

## 3. Interest Rate Swap (payer swap)

### Produit

Swap de taux simple :

- Jambe fixe : taux fixe `K` ;
- Jambe flottante : taux forward `F` (supposé constant dans l’exemple) ;
- Périodes annuelles sur 5 ans.

Paramètres de l’exemple (`run_swap_example()`):

- Notional `N = 1 000 000` ;
- Taux fixe `K = 3%` ;
- Taux forward `F = 2.8%` ;
- Temps de paiement : `times = {1, 2, 3, 4, 5}` ;
- `accrual = {1, 1, 1, 1, 1}` ;
- `payer = true` : payeur fixe / receveur flottant.

Intuition :

- Si le forward `F` est inférieur à `K`, le swap payer fixe a une valeur négative (on paie trop cher) ;
- Si `F` est supérieur à `K`, la valeur est positive.

### Modèle de pricing

- Moteur : `SwapEngine` ;
- Prix = somme des flux actualisés de la jambe nette (flottant - fixe, avec le bon signe selon `payer`).

### Lancer l’exemple

```bash
./pricing_example swap

La fonction `run_swap_example()` :

1. Crée un `InterestRateSwap` avec ses flux (dates, accrual, taux) ;
2. Attache un `SwapEngine` ;
3. Affiche la NPV du swap.

---

## 4. Swaption européenne (payer swaption)

### Produit

Swaption européenne = option sur un swap.

Dans l’exemple (`run_swaption_example()`):

- Sous-jacent : le swap précédent (payer swap) ;
- Date d’exercice : `T_exp = 1 an` ;
- Exercice européen : on décide à `T_exp` si on entre ou non dans le swap.

Intuition :

- C’est une option call sur le taux de swap (pour une payer swaption) ;
- La valeur dépend du niveau attendu du taux de swap à la date d’exercice et de sa volatilité.

### Modèle de pricing

- Modèle : Black sur le taux de swap ;
- Moteur : `SwaptionBlackEngine` ;
- Utilise :
  - l’annuité du swap (somme actualisée des accruals) comme “notionnel effectif” ;
  - la volatilité Black `sigma_IR` fournie par `BlackIRModel`.

### Lancer l’exemple

```bash
./pricing_example swaption

La fonction `run_swaption_example()` :

1. Construit un `InterestRateSwap` (payer) ;
2. Construit une `Swaption` associée à ce swap, avec une date d’exercice ;
3. Attache un `SwaptionBlackEngine`
4. Affiche la NPV de la swaption.