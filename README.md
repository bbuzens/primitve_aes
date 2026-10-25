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
include/
  aes.h                   # interface publique : tailles, KeyExpansion, Cipher
config/
  aes_config.h            # paramètres Nb/Nk/Nr, constantes AES, matrice MixColumns, convention de l'état
src/
  entities/
    state.c/.h            # BytesToState / StateToBytes : conversion bloc <-> état
  operations/
    add_round_key.c/.h    # AddRoundKey
    sbox.c/.h             # SBox : S(b) = affine(b^-1), pour un octet
    sub_bytes.c/.h        # SubBytes : SBox appliquée à tout l'état
    shift_rows.c/.h       # ShiftRows : rotation de la ligne r de r cases
    mix_columns.c/.h      # MixColumns : produit matrice x colonne dans GF(2^8)
  math/
    xtime.c/.h            # multiplication par x ({02}) dans GF(2^8)
    gf_multiply.c/.h      # multiplication quelconque dans GF(2^8)
    gf_inverse.c/.h       # inverse multiplicatif dans GF(2^8)
    affine_transform.c/.h # transformation affine de la S-box
  key_schedule/
    rot_word.c/.h         # RotWord : [a0,a1,a2,a3] -> [a1,a2,a3,a0]
    sub_word.c/.h         # SubWord : SBox sur les 4 octets d'un mot
    rcon.c/.h             # Rcon : constante de tour [x^(i-1), 00, 00, 00]
    key_expansion.c/.h    # KeyExpansion : clé -> 44 mots w[0..43]
    round_key.c/.h        # GetRoundKey : mots w[4r..4r+3] -> clé du tour r (état 4x4)
  cipher/
    cipher.c/.h           # Cipher : assemblage des tours, chiffrement d'un bloc
tests/
  test_utils.c/.h         # check_true, check_byte, check_bytes, check_state, test_report
  fips197_vectors.h       # vecteurs de test de la FIPS-197 (annexe B, S-box de la figure 7)
  entities/test_state.c
  operations/test_add_round_key.c
  operations/test_sbox.c
  operations/test_sub_bytes.c
  operations/test_shift_rows.c
  operations/test_mix_columns.c
  math/test_xtime.c
  math/test_gf_multiply.c
  math/test_gf_inverse.c
  math/test_affine_transform.c
  key_schedule/test_rot_word.c
  key_schedule/test_sub_word.c
  key_schedule/test_rcon.c
  key_schedule/test_key_expansion.c
  key_schedule/test_round_key.c
  cipher/test_cipher.c
  public_api/test_public_api.c  # programme utilisateur : aes.h + libaes.a uniquement
Makefile
```

- `include/` : l'interface publique, seul en-tête nécessaire à un programme qui utilise la bibliothèque.
- `config/` : la configuration, séparée des sources. Elle contient les dimensions de l'état, les constantes de la norme et les paramètres `Nb` / `Nk` / `Nr` (fixés pour AES-128). Elle inclut `aes.h`, qui définit les tailles publiques une seule fois ; des `static_assert` vérifient à la compilation qu'elles concordent avec `Nb` / `Nk` / `Nr`.
- `src/entities/` : les structures de données manipulées et leurs conversions.
- `src/operations/` : les transformations AES.
- `src/math/` : l'arithmétique du corps GF(2⁸), sur laquelle s'appuient les transformations.
- `src/cipher/` : l'assemblage final, qui enchaîne toutes les unités.
- `src/key_schedule/` : l'expansion de la clé et l'extraction des clés de tour. Un mot y est un tableau de 4 octets (`uint8_t word[4]`), pour rester octet par octet comme le reste du code.
- `tests/` reprend la même arborescence que `src/` ; les outils de test partagés restent à sa racine.

Les en-têtes s'incluent par leur nom seul (`#include "state.h"`) : le Makefile passe `-Iinclude -Iconfig -Isrc/entities -Isrc/operations -Isrc/math -Isrc/key_schedule -Isrc/cipher -Itests` au compilateur.

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
Pour la même raison, [config/aes_config.h](config/aes_config.h) inclut `<assert.h>`, qui définit `static_assert` avant C23.

### Compatibilité avec les normes antérieures

Le C23 reste la norme de référence du projet, mais le code compile aussi avec les normes précédentes :

| Norme | Résultat |
|---|---|
| C23 (`-std=c23`) | ✅ compile sans avertissement |
| C17 / C11 (`-std=c17`, `-std=c11`) | ✅ compile et tous les tests passent ; avertissements avec `-Wpedantic` (22 emplacements, dans `cipher.c` et les tests) |
| C99 et antérieures | ❌ `static_assert` n'existe pas |

Pour vérifier avec une autre norme :

```bash
make clean && make test CFLAGS="-std=c17 -Wall -Wextra -Wpedantic -O2 -g"
```

Les avertissements C11/C17 (`invalid use of pointers to arrays with different qualifiers`) viennent des paramètres `const uint8_t [4][4]` : avant C23, la norme ne permet pas formellement d'y passer un `uint8_t [4][4]` non `const`.
Le code généré et le comportement sont identiques. Les supprimer demanderait de retirer ces `const` ou d'ajouter des conversions à chaque appel, au détriment de la lisibilité : ils sont donc conservés.

L'IDE (extension C/C++ de VS Code) ne lit pas le Makefile : [.vscode/c_cpp_properties.json](.vscode/c_cpp_properties.json) lui indique la même norme (`c23`) et les mêmes chemins d'inclusion que le compilateur.
À tenir à jour si un dossier est ajouté à `SRC_DIRS` hors de `src/`.

```bash
make test    # compile chaque build/test_<op> et le test de l'API publique, et les exécute tous
make lib     # produit la bibliothèque statique build/libaes.a
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
| 5 | `gf_inverse` : inverse dans GF(2⁸), calculé comme a²⁵⁴ | FIPS-197 §4.2 : `{53}⁻¹ = {ca}` | ✅ |
| 6 | `affine_transform` : transformation affine | FIPS-197 §5.1.1, équation 5.1 | ✅ |
| 7 | `SBox` : S(b) = affine(b⁻¹) | FIPS-197 §5.1.1 : `S({53}) = {ed}`, table complète de la figure 7 | ✅ |
| 8 | `SubBytes` | FIPS-197 annexe B (tour 1 après SubBytes) | ✅ |
| 9 | `ShiftRows` | FIPS-197 §5.1.2, annexe B (tour 1 après ShiftRows) | ✅ |
| 10 | `MixColumns` | FIPS-197 §5.1.3, annexe B (tour 1 après MixColumns) | ✅ |
| 11 | `RotWord`, `SubWord`, `Rcon` | FIPS-197 §5.2, annexe A.1 (i = 4 et i = 8) | ✅ |
| 12 | `KeyExpansion` | FIPS-197 annexe A.1 (44 mots) | ✅ |
| 13 | `GetRoundKey` | FIPS-197 annexe B (clés des tours 0, 1 et 10) | ✅ |
| 14 | `Cipher` (assemblage) | FIPS-197 annexes B et C.1, clé et clair nuls, 200 vecteurs aléatoires contre OpenSSL | ✅ |

## Limites connues

> [!WARNING]
> **Implémentation pédagogique, non durcie : ne pas l'utiliser pour protéger de vraies données.**

### Exécution non constante dans le temps (canal auxiliaire temporel)

Certaines fonctions prennent des branches qui dépendent des valeurs manipulées :

| Fonction | Branche dépendante des données |
|---|---|
| [`xtime`](src/math/xtime.c) | `if (overflows)` : la réduction par `0x1b` n'a lieu que si le bit x⁷ est à 1. |
| [`gf_multiply`](src/math/gf_multiply.c) | `if (bit_is_set)` : l'ajout de `a · xⁱ` n'a lieu que si le bit `i` de `b` est à 1. |
| [`gf_inverse`](src/math/gf_inverse.c), [`SBox`](src/operations/sbox.c), [`SubBytes`](src/operations/sub_bytes.c), [`MixColumns`](src/operations/mix_columns.c), [`SubWord`](src/key_schedule/sub_word.c), [`KeyExpansion`](src/key_schedule/key_expansion.c) | Aucune branche propre dépendant des données (le test `i % Nk == 0` de `KeyExpansion` ne dépend que de la position) (boucles de longueur fixe), mais héritent de celles de `gf_multiply` et `xtime`. |

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

La S-box est **calculée** et non lue dans une table : il n'y a donc pas d'accès mémoire dont l'adresse dépend de l'octet substitué, source classique d'attaques par cache sur AES.
Une fois `gf_multiply` et `xtime` durcis, toute la chaîne `SubBytes` sera sans branche dépendante des données.

Les opérations à venir seront ajoutées à ce tableau si elles introduisent de nouvelles dépendances aux données.

### Performance

Lisibilité d'abord : `gf_inverse` calcule a²⁵⁴ par 253 multiplications successives, recalculées pour chaque octet de chaque tour.
C'est très lent comparé à une table précalculée ou à une exponentiation rapide, mais sans conséquence pour des tests unitaires.

### AES-128 uniquement

`Nk` et `Nr` sont fixés pour AES-128 dans [config/aes_config.h](config/aes_config.h).
Changer ces valeurs ne suffit pas pour AES-256 : il faut aussi adapter les tailles de [include/aes.h](include/aes.h) (les `static_assert` le signalent), et `KeyExpansion` devrait appliquer `SubWord` quand `Nk > 6` et `i mod Nk = 4` (FIPS-197 §5.2), étape non implémentée et non testée.

## Utilisation comme bibliothèque

Bibliothèque à usage personnel et pédagogique : `make lib` produit `build/libaes.a`, et un programme n'a besoin que de [include/aes.h](include/aes.h).

```c
/* mon_programme.c */
#include "aes.h"

uint8_t w[AES_EXPANDED_KEY_WORDS][AES_WORD_SIZE];
uint8_t ciphertext[AES_BLOCK_SIZE];

KeyExpansion(key, w);              /* une fois par clé */
Cipher(plaintext, ciphertext, w);  /* un bloc de 16 octets */
```

```bash
make lib
cc -std=c23 -I<projet>/include mon_programme.c -L<projet>/build -laes -o mon_programme
```

- `-I<projet>/include` : le compilateur trouve `aes.h` (les déclarations).
- `-L<projet>/build -laes` : l'éditeur de liens prend dans `libaes.a` le code de `Cipher`, `KeyExpansion` et de tout ce qu'elles appellent. L'exécutable produit est autonome.

[tests/public_api/test_public_api.c](tests/public_api/test_public_api.c) est un programme de ce type : il est compilé exactement ainsi par `make test`, ce qui garantit que `aes.h` se suffit à lui-même.

**Noms FIPS conservés** : `aes.h` expose `KeyExpansion` et `Cipher` sous leurs noms de la norme, sans préfixe.
Les fonctions internes (`SubBytes`, `xtime`, `Rcon`…) n'y sont pas déclarées, mais leurs symboles sont présents dans `libaes.a` : un programme utilisateur ne doit pas définir de fonctions portant les mêmes noms, sous peine de conflit à l'édition de liens.

`aes.h` et les en-têtes internes [cipher.h](src/cipher/cipher.h) / [key_expansion.h](src/key_schedule/key_expansion.h) déclarent les mêmes fonctions : comme les seconds incluent le premier, le compilateur vérifie que les deux déclarations sont identiques.

`Cipher` chiffre **un seul bloc** : c'est la primitive AES, pas un mode de chiffrement (ECB, CBC, GCM…).
Le déchiffrement (`InvCipher`, FIPS-197 §5.3) n'est pas implémenté.

## Vecteurs utilisés

| | Clé | Clair | Chiffré attendu |
|---|---|---|---|
| FIPS-197 annexe B | `2b7e1516 28aed2a6 abf71588 09cf4f3c` | `3243f6a8 885a308d 313198a2 e0370734` | `3925841d 02dc09fb dc118597 196a0b32` |
| FIPS-197 annexe C.1 | `00010203 04050607 08090a0b 0c0d0e0f` | `00112233 44556677 8899aabb ccddeeff` | `69c4e0d8 6a7b0430 d8cdb780 70b4c55a` |
| Clé et clair nuls | `00000000 …` | `00000000 …` | `66e94bd4 ef8a2c3b 884cfa59 ca342b2e` |

L'annexe B sert de fil rouge : chaque transformation est testée sur les états intermédiaires de son premier tour.

Vérification indépendante d'un bloc avec OpenSSL (oracle de test uniquement, non utilisé par le code) :

```bash
echo -n "3243f6a8885a308d313198a2e0370734" | xxd -r -p | \
  openssl enc -aes-128-ecb -nopad -K 2b7e151628aed2a6abf7158809cf4f3c | xxd -p
```

L'implémentation a aussi été comparée à OpenSSL sur 200 couples clé/clair aléatoires, sans aucun écart.
