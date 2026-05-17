CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -Werror

UI_BACKEND ?= terminal

ifeq ($(UI_BACKEND),terminal)
UI_SRC := ui.c
else ifeq ($(UI_BACKEND),graphics)
UI_SRC := ui_graphics.c
else
$(error Unsupported UI_BACKEND '$(UI_BACKEND)'. Use 'terminal' or 'graphics')
endif

APP_BIN := job
TEST_BIN := test_game

APP_SRCS := main.c game.c $(UI_SRC)
TEST_SRCS := test_game.c game.c

.PHONY: all test run clean

all: $(APP_BIN)

$(APP_BIN): $(APP_SRCS) game.h ui.h
	$(CC) $(CFLAGS) $(APP_SRCS) -o $(APP_BIN)

$(TEST_BIN): $(TEST_SRCS) game.h
	$(CC) $(CFLAGS) $(TEST_SRCS) -o $(TEST_BIN)

test: $(TEST_BIN)
	./$(TEST_BIN)

run: $(APP_BIN)
	./$(APP_BIN)

clean:
	rm -f $(APP_BIN) $(TEST_BIN)
