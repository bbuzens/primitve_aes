CC      = cc
CFLAGS  = -std=c23 -Wall -Wextra -Wpedantic -O2 -g

BUILD   = build

# Interface publique (aes.h) et configuration (constantes AES) séparées des sources.
INCLUDE_DIR = include
CONFIG_DIR  = config
SRC_DIRS    = src/entities src/operations src/math src/key_schedule src/cipher
TEST_DIR    = tests
TEST_DIRS   = $(patsubst src/%, $(TEST_DIR)/%, $(SRC_DIRS))

CPPFLAGS = $(addprefix -I, $(INCLUDE_DIR) $(CONFIG_DIR) $(SRC_DIRS) $(TEST_DIR))

# Toutes les sources sont liées à chaque test : une unité peut donc en utiliser
# une autre (gf_multiply utilise xtime) sans déclarer la dépendance.
# Chaque test ne teste pourtant qu'une seule unité.
SRC        = $(wildcard $(addsuffix /*.c, $(SRC_DIRS)))
TEST_UTILS = $(TEST_DIR)/test_utils.c
HEADERS    = $(wildcard $(INCLUDE_DIR)/*.h $(CONFIG_DIR)/*.h $(addsuffix /*.h, $(SRC_DIRS)) $(TEST_DIR)/*.h)

# Les tests sont découverts automatiquement : tests/<dossier>/test_<nom>.c -> build/test_<nom>.
TEST_SRC = $(wildcard $(addsuffix /test_*.c, $(TEST_DIRS)))
TESTS    = $(addprefix $(BUILD)/, $(basename $(notdir $(TEST_SRC))))

# Bibliothèque statique : un fichier objet par source, regroupés dans une archive.
OBJ = $(patsubst src/%.c, $(BUILD)/obj/%.o, $(SRC))
LIB = $(BUILD)/libaes.a

# Test de l'interface publique : compilé comme un programme utilisateur,
# avec -Iinclude seulement, et lié à la bibliothèque.
PUBLIC_API_TEST = $(BUILD)/test_public_api

vpath test_%.c $(TEST_DIRS)

test: $(TESTS) $(PUBLIC_API_TEST)
	@status=0; for t in $(TESTS) $(PUBLIC_API_TEST); do ./$$t || status=1; done; exit $$status

lib: $(LIB)

$(BUILD)/test_%: test_%.c $(SRC) $(TEST_UTILS) $(HEADERS) | $(BUILD)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $(filter %.c, $^)

$(PUBLIC_API_TEST): $(TEST_DIR)/public_api/test_public_api.c $(INCLUDE_DIR)/aes.h $(LIB)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -o $@ $< -L$(BUILD) -laes

$(LIB): $(OBJ)
	ar rcs $@ $^

$(BUILD)/obj/%.o: src/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

$(BUILD):
	mkdir -p $@

clean:
	rm -rf $(BUILD)

.PHONY: test lib clean
