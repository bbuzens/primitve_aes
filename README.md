# primitve_aes

Implémentation pédagogique de la primitive AES-128 en C, **calculée à la main** à partir de la norme [FIPS-197](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.197-upd1.pdf).

## Principes

- **Aucune bibliothèque externe** : uniquement la bibliothèque standard C (`<stdint.h>`, `<string.h>`, `<stdio.h>`).
  L'arithmétique de AES se fait dans le corps fini GF(2⁸) : une bibliothèque matricielle classique (sur les réels ou les entiers) donnerait des résultats faux.
- **Lisibilité avant optimisation** : le code doit être facile à lire et à confronter à la norme. Pas de tables précalculées ni d'astuces de performance à ce stade.
- **Opérations atomiques** : chaque opération a son propre fichier source et son propre fichier de tests unitaires.
- **Tests d'abord** : pour chaque opération, on écrit le test, on vérifie qu'il échoue, puis on implémente l'opération.

## Organisation

```
config/
  aes_config.h            # constantes AES, polynôme m(x), convention de l'état
src/
  entities/
    state.c/.h            # BytesToState / StateToBytes : conversion bloc <-> état
  operations/
    add_round_key.c/.h    # AddRoundKey
  math/
    xtime.c/.h            # multiplication par x ({02}) dans GF(2^8)
    gf_multiply.c/.h      # multiplication quelconque dans GF(2^8)
tests/
  test_utils.c/.h         # check_true, check_byte, check_bytes, check_state, test_report
  fips197_vectors.h       # vecteurs de test de la FIPS-197, annexe B
  entities/test_state.c
  operations/test_add_round_key.c
  math/test_xtime.c
  math/test_gf_multiply.c
Makefile
```

- `config/` : la configuration, séparée des sources. Elle contient les dimensions de l'état et accueillera les paramètres `Nk` / `Nr` qui distinguent AES-128, 192 et 256.
- `src/entities/` : les structures de données manipulées et leurs conversions.
- `src/operations/` : les transformations AES.
- `src/math/` : l'arithmétique du corps GF(2⁸), sur laquelle s'appuient les transformations.
- `tests/` reprend la même arborescence que `src/` ; les outils de test partagés restent à sa racine.

Les en-têtes s'incluent par leur nom seul (`#include "state.h"`) : le Makefile passe `-Iconfig -Isrc/entities -Isrc/operations -Isrc/math -Itests` au compilateur.

## Convention de l'état

L'état AES est une matrice `uint8_t state[4][4]`, indexée `state[ligne][colonne]`, comme `s[r][c]` dans la FIPS-197 (§3.4).
Un bloc de 16 octets remplit l'état **colonne par colonne** (`in[r + 4c] -> state[r][c]`) :

```
in[0]  in[4]  in[8]  in[12]
in[1]  in[5]  in[9]  in[13]
in[2]  in[6]  in[10] in[14]
in[3]  in[7]  in[11] in[15]
```

Les clés de tour utilisent la même disposition que l'état.
Les vecteurs de test de [tests/fips197_vectors.h](tests/fips197_vectors.h) sont écrits sous forme de grille, comme dans les figures de la norme, pour pouvoir les comparer à l'œil.

## Compiler et tester

Prérequis : un compilateur C23 (GCC ≥ 14 ou Clang ≥ 18) et `make`.
Le C23 permet de passer un tableau `uint8_t [4][4]` à un paramètre `const uint8_t [4][4]` sans avertissement.

En C23, `bool`, `true` et `false` sont des mots-clés : `<stdbool.h>` n'est plus nécessaire.
Le code l'inclut quand même, pour rester familier aux habitués du C99/C17, où ces noms sont des macros définies par cet en-tête.

L'IDE (extension C/C++ de VS Code) ne lit pas le Makefile : [.vscode/c_cpp_properties.json](.vscode/c_cpp_properties.json) lui indique la même norme (`c23`) et les mêmes chemins d'inclusion que le compilateur.
À tenir à jour si un dossier est ajouté à `SRC_DIRS` hors de `src/`.

```bash
make test    # compile chaque build/test_<op> et les exécute tous
make clean   # supprime le dossier build/
```

En cas d'échec, `check_state` affiche les grilles attendue et obtenue côte à côte :

```
[FAIL] AddRoundKey : clair XOR clé (FIPS-197 B)
       attendu        obtenu
       19 a0 9a e9    32 88 31 e0
       ...
```

## Ajouter une opération

1. Créer `tests/<dossier>/test_<op>.c` (avec un `main` qui se termine par `return test_report();`) et ajouter les vecteurs attendus dans `tests/fips197_vectors.h` si besoin.
2. Créer `<op>.h` et un `<op>.c` vide dans `src/<dossier>/`, puis vérifier avec `make test` que le test échoue.
3. Implémenter l'opération jusqu'à ce que `make test` passe.

Le Makefile découvre seul les fichiers `tests/<dossier>/test_*.c` : aucune liste à tenir à jour. Seul un **nouveau dossier** doit être ajouté à `SRC_DIRS`.

Toutes les sources sont liées à chaque exécutable de test : une unité peut en utiliser une autre (`gf_multiply` utilise `xtime`) sans déclarer de dépendance. Chaque test ne teste pourtant qu'une seule unité.

Les noms de tests doivent rester uniques dans tout `tests/` : `tests/<dossier>/test_<nom>.c` produit `build/test_<nom>`.

## Avancement

| Étape | Opération | Vecteur de référence | État |
|---|---|---|---|
| 1 | `BytesToState` / `StateToBytes` | FIPS-197 §3.4, annexe B | ✅ |
| 2 | `AddRoundKey` | FIPS-197 annexe B (début du tour 1) | ✅ |
| 3 | `xtime` : multiplication par x dans GF(2⁸) | FIPS-197 §4.2.1 : `{57}·{02} = {ae}`… | ✅ |
| 4 | `gf_multiply` : multiplication dans GF(2⁸) | FIPS-197 §4.2 : `{57}·{83} = {c1}`, `{57}·{13} = {fe}` | ✅ |
| 5 | `SubBytes` (S-box calculée : inverse dans GF(2⁸) + transformation affine) | FIPS-197 §5.1.1, annexe B | ⏳ |
| 6 | `ShiftRows` | FIPS-197 §5.1.2, annexe B | ⏳ |
| 7 | `MixColumns` | FIPS-197 §5.1.3, annexe B | ⏳ |
| 8 | `KeyExpansion` | FIPS-197 annexe A.1 | ⏳ |
| 9 | `Cipher` (assemblage) | FIPS-197 annexe B : `39 25 84 1d 02 dc 09 fb dc 11 85 97 19 6a 0b 32` | ⏳ |

## Limites connues

> [!WARNING]
> **Implémentation pédagogique, non durcie : ne pas l'utiliser pour protéger de vraies données.**

### Exécution non constante dans le temps (canal auxiliaire temporel)

Certaines fonctions prennent des branches qui dépendent des valeurs manipulées :

| Fonction | Branche dépendante des données |
|---|---|
| [`xtime`](src/math/xtime.c) | `if (overflows)` : la réduction par `0x1b` n'a lieu que si le bit x⁷ est à 1. |
| [`gf_multiply`](src/math/gf_multiply.c) | `if (bit_is_set)` : l'ajout de `a · xⁱ` n'a lieu que si le bit `i` de `b` est à 1. |

En AES, ces valeurs dépendent de la clé. Le temps d'exécution peut donc varier selon la clé et laisser fuir de l'information exploitable par une attaque temporelle.

Ce choix est volontaire : les branches rendent le code fidèle à la description de la FIPS-197 et facile à suivre.
Pour la phase de durcissement, chaque branche se remplace par un masque, sans changer le résultat. Par exemple :

```c
/* xtime : masque = 0xff si le bit x^7 est à 1, 0x00 sinon */
shifted ^= AES_REDUCTION_POLYNOMIAL & -(uint8_t)(byte >> 7);

/* gf_multiply : masque = 0xff si le bit est à 1, 0x00 sinon */
product ^= a_times_x_power & -(uint8_t)bit_is_set;
```

Les tests unitaires existants permettront de vérifier que la version durcie donne les mêmes résultats.
Il faudra aussi vérifier le code machine produit : un compilateur peut réintroduire une branche.

Les opérations à venir seront ajoutées à ce tableau si elles introduisent de nouvelles dépendances aux données (par exemple une S-box sous forme de table, dont les accès mémoire dépendent de l'octet lu).

## Vecteurs utilisés (FIPS-197, annexe B)

| | Valeur |
|---|---|
| Clair | `32 43 f6 a8 88 5a 30 8d 31 31 98 a2 e0 37 07 34` |
| Clé | `2b 7e 15 16 28 ae d2 a6 ab f7 15 88 09 cf 4f 3c` |
| Chiffré attendu | `39 25 84 1d 02 dc 09 fb dc 11 85 97 19 6a 0b 32` |

Vérification indépendante d'un bloc avec OpenSSL (oracle de test uniquement, non utilisé par le code) :

```bash
echo -n "3243f6a8885a308d313198a2e0370734" | xxd -r -p | \
  openssl enc -aes-128-ecb -nopad -K 2b7e151628aed2a6abf7158809cf4f3c | xxd -p
```
