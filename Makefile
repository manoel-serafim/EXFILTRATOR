# Makefile for EXFILTRATOR

CC = gcc -g
CFLAGS = -I include
LDFLAGS = -lm
BUILD_DIR = build
BIN_DIR = bin
SRC_DIR = src

# List of source files
SRCS = $(SRC_DIR)/encode.c $(SRC_DIR)/decode.c $(SRC_DIR)/complexity.c $(SRC_DIR)/types.c
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Main target
all: $(BIN_DIR)/exfilter

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

# Link object files to create the executable
$(BIN_DIR)/exfilter: main.c $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

# Phony target for cleaning
clean:
	rm -f $(OBJS) $(BIN_DIR)/exfilter

# Phony target for testing
test_canary: $(BIN_DIR)/exfilter
	./$(BIN_DIR)/exfilter --mode hide img_samples/canary.bmp utils/files/bigfile output/canary_bin.bmp
	./$(BIN_DIR)/exfilter --mode show output/canary_bin.bmp output/canary.bin
	compare output/canary_bin.bmp img_samples/canary.bmp output/canary_diff.bmp

test_cat: $(BIN_DIR)/exfilter
	./$(BIN_DIR)/exfilter --mode hide img_samples/cat.bmp utils/files/smallfile output/cat_bin.bmp
	compare output/cat_bin.bmp img_samples/cat.bmp output/cat_diff.bmp
	
test_dog: $(BIN_DIR)/exfilter	
	./$(BIN_DIR)/exfilter --mode hide img_samples/dog.bmp bin/exfilter output/dog_bin.bmp
	compare output/dog_bin.bmp img_samples/dog.bmp output/dog_diff.bmp

.PHONY: all clean test_canary test_cat test_dog