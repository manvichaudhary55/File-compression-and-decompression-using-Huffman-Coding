CC = g++
CFLAGS = -std=c++11 -Wall -Iinclude
SRC_DIR = src
OBJ_DIR = output
BIN_DIR = output

# Find all source files in the src directory
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
# Generate object file names from source file names
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# Default target
all: directories huffman_compression

# Create output directory if it doesn't exist
directories:
	mkdir -p $(OBJ_DIR)

# Link all object files to create the executable
huffman_compression: $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/huffman_compression $^

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Test compression and decompression
test: all
	$(BIN_DIR)/huffman_compression -c test_files/sample.txt test_files/sample_compressed.huff
	$(BIN_DIR)/huffman_compression -d test_files/sample_compressed.huff test_files/sample_decompressed.txt
	diff test_files/sample.txt test_files/sample_decompressed.txt && echo "Test passed: Files are identical!"

# Clean up generated files
clean:
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/huffman_compression test_files/sample_compressed.huff test_files/sample_decompressed.txt

.PHONY: all directories clean test