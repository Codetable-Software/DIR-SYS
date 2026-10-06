CC ?= cc
CFLAGS ?= -std=c11 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -Wpedantic -O2
CPPFLAGS ?= -Iinclude/dirsys
LDFLAGS ?=

BUILD := build
BIN := $(BUILD)/dirsys
LIB_OBJS := $(BUILD)/config.o $(BUILD)/parser.o $(BUILD)/filesystem.o $(BUILD)/generator.o
TEST_BINS := $(BUILD)/test_config $(BUILD)/test_parser $(BUILD)/test_generator $(BUILD)/test_mapping $(BUILD)/test_structure $(BUILD)/test_user_storage

.PHONY: all build test clean

all: build

build: $(BIN)

$(BUILD)/%.o: src/%.c | $(BUILD)/.gitkeep
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BIN): $(BUILD)/main.o $(LIB_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(BUILD)/test_config: tests/parser/test_config.c $(BUILD)/config.o | $(BUILD)/.gitkeep
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@

$(BUILD)/test_parser: tests/parser/test_parser.c $(BUILD)/parser.o $(BUILD)/config.o | $(BUILD)/.gitkeep
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@

$(BUILD)/test_generator: tests/generator/test_generator.c $(BUILD)/generator.o $(BUILD)/filesystem.o | $(BUILD)/.gitkeep
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@

$(BUILD)/test_mapping: tests/generator/test_mapping.c $(BUILD)/generator.o $(BUILD)/filesystem.o | $(BUILD)/.gitkeep
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@

$(BUILD)/test_structure: tests/filesystem/test_structure.c $(BUILD)/generator.o $(BUILD)/filesystem.o | $(BUILD)/.gitkeep
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@

$(BUILD)/test_user_storage: tests/filesystem/test_user_storage.c $(BUILD)/generator.o $(BUILD)/filesystem.o | $(BUILD)/.gitkeep
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@

test: $(TEST_BINS) $(BIN)
	@./$(BUILD)/test_config
	@./$(BUILD)/test_parser
	@./$(BUILD)/test_generator
	@./$(BUILD)/test_mapping
	@./$(BUILD)/test_structure
	@./$(BUILD)/test_user_storage
	@./$(BIN) validate
	@rm -rf output/*
	@./$(BIN) generate
	@test -d output/home/usr/Photo
	@test ! -e output/os.conf
	@test ! -e output/kernel.conf
	@test ! -e output/dirsys.conf
	@echo "DIR-SYS: all tests passed"

clean:
	rm -f $(BUILD)/*.o $(BIN) $(TEST_BINS)
	rm -rf output/*
