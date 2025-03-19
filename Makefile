VERSION := 2.0.1

CC := gcc
CD := gdb
CT := valgrind
C_STANDARD := c11
C_COMMON_FLAGS := -std=$(C_STANDARD) -pedantic -W -Wall -Wextra -Werror
C_RELEASE_FLAGS := $(C_COMMON_FLAGS) -O3 $(C_EXTRA_FLAGS)
C_DEBUG_FLAGS := $(C_COMMON_FLAGS) -g -g$(CD) $(C_EXTRA_FLAGS)
C_EXTRA_FLAGS :=

OS_BUILD_DIR := ./build
PROJECT_NAME := lab02

.PHONY: all clean build debug run test

all: build

validate_mode:
	@if [ "$(mode)" != "debug" ] && [ "$(mode)" != "release" ]; then \
	  echo "\"mode\" must be: debug or release, not \"$(mode)\""; exit 1; \
	fi

build: validate_mode | $(OS_BUILD_DIR)
	$(MAKE) _build_$(mode)

$(OS_BUILD_DIR):
	mkdir -p $(OS_BUILD_DIR)

_build_debug:
	$(CC) $(C_DEBUG_FLAGS) parent.c -o $(OS_BUILD_DIR)/parent
	$(CC) $(C_DEBUG_FLAGS) child.c -o $(OS_BUILD_DIR)/child

_build_release:
	$(CC) $(C_RELEASE_FLAGS) parent.c -o $(OS_BUILD_DIR)/parent
	$(CC) $(C_RELEASE_FLAGS) child.c -o $(OS_BUILD_DIR)/child

run: build
	$(OS_BUILD_DIR)/parent $(args)

debug: build
	$(CD) $(OS_BUILD_DIR)/debug

clean:
	rm -rf $(OS_BUILD_DIR)

test: build
	$(CT) --leak-check=full --show-leak-kinds=all --track-origins=yes $(OS_BUILD_DIR)/parent $(args)
	$(CT) --leak-check=full --show-leak-kinds=all --track-origins=yes $(OS_BUILD_DIR)/child $(args)
