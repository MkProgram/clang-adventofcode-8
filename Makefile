CC       := clang
CFLAGS   := -std=gnu11 -Wall -Wextra -Wpedantic -Wconversion -Iinclude -g
SANFLAGS := -fsanitize=address,undefined

BIN_NAME := main
BIN := build/$(BIN_NAME)

SRCS := $(wildcard src/*.c)
MAIN_SRC := src/$(BIN_NAME).c
LIB_SRCS := $(filter-out $(MAIN_SRC), $(SRCS))
LIB_OBJS := $(patsubst src/%.c,build/%.o,$(LIB_SRCS))

TEST_SRCS := $(wildcard tests/*.c)
TESTS := $(patsubst tests/%.c,build/%,$(TEST_SRCS))

all: $(BIN)

$(BIN): build/$(BIN_NAME).o $(LIB_OBJS)
	$(CC) $(CFLAGS) $(SANFLAGS) -o $@ $^

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) $(SANFLAGS) -c $< -o $@

build/%_test: tests/%_test.c $(LIB_OBJS) | build
	$(CC) $(CFLAGS) $(SANFLAGS) -o $@ $< $(LIB_OBJS)

build:
	mkdir -p build

test: $(TESTS)
	@for t in $(TESTS); do ./$$t || exit 1; done

run: $(BIN)
	/bin/sh -c "time ./$(BIN)"

clean: 
	rm -rf build/*
.PHONY: all run clean test
