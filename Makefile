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
test: $(BIN_DIR)/exfilter
	./$(BIN_DIR)/exfilter img_samples/canary.bmp src/encode.c
	compare output.bmp img_samples/canary.bmp output/canary_diff.bmp
	./$(BIN_DIR)/exfilter img_samples/cat.bmp src/encode.c
	compare output.bmp img_samples/cat.bmp output/cat_diff.bmp
	./$(BIN_DIR)/exfilter img_samples/dog.bmp src/encode.c
	compare output.bmp img_samples/dog.bmp output/dog_diff.bmp

.PHONY: all clean test