CC      = cc
CFLAGS  = -std=c23 -Wall -Wextra -Wpedantic -O2 -g

BUILD   = build

# Configuration (constantes AES) séparée des sources.
CONFIG_DIR = config
SRC_DIRS   = src/entities src/operations src/math src/key_schedule
TEST_DIR   = tests
TEST_DIRS  = $(patsubst src/%, $(TEST_DIR)/%, $(SRC_DIRS))

CPPFLAGS = $(addprefix -I, $(CONFIG_DIR) $(SRC_DIRS) $(TEST_DIR))

# Toutes les sources sont liées à chaque test : une unité peut donc en utiliser
# une autre (gf_multiply utilise xtime) sans déclarer la dépendance.
# Chaque test ne teste pourtant qu'une seule unité.
SRC        = $(wildcard $(addsuffix /*.c, $(SRC_DIRS)))
TEST_UTILS = $(TEST_DIR)/test_utils.c
HEADERS    = $(wildcard $(CONFIG_DIR)/*.h $(addsuffix /*.h, $(SRC_DIRS)) $(TEST_DIR)/*.h)

# Les tests sont découverts automatiquement : tests/<dossier>/test_<nom>.c -> build/test_<nom>.
TEST_SRC = $(wildcard $(addsuffix /test_*.c, $(TEST_DIRS)))
TESTS    = $(addprefix $(BUILD)/, $(basename $(notdir $(TEST_SRC))))

vpath test_%.c $(TEST_DIRS)

test: $(TESTS)
	@status=0; for t in $(TESTS); do ./$$t || status=1; done; exit $$status

$(BUILD)/test_%: test_%.c $(SRC) $(TEST_UTILS) $(HEADERS) | $(BUILD)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $(filter %.c, $^)

$(BUILD):
	mkdir -p $@

clean:
	rm -rf $(BUILD)

.PHONY: test clean
