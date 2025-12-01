# Compiler and tools
CC       = gcc
DEBUGGER = gdb

# Flags
CFLAGS   = -std=c99 -O0 -pthread -Isrc -Icollectors
LFLAGS   = -ldl -lm -lpthread

# Directories
TARGET    = watchtower
SRC_DIR   = src
CLT_DIR   = collectors
BIN_DIR   = bin
OBJ_DIR   = $(BIN_DIR)/.obj

# Collect all .c files (src + one-level subfolders + collectors subfolders)
SRC = $(wildcard $(SRC_DIR)/*.c \
               $(SRC_DIR)/*/*.c \
               $(CLT_DIR)/*/*.c)

# Convert each .c file into corresponding .o inside .obj/
OBJ = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC))

# Default target
build: $(BIN_DIR) $(BIN_DIR)/$(TARGET)

$(BIN_DIR):
	mkdir -p $@

# Universal build rule (keeps directory structure)
$(OBJ_DIR)/%.o : %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Link final binary
$(BIN_DIR)/$(TARGET): $(OBJ)
	$(CC) $(OBJ) $(LFLAGS) -o $@

# --- EXTRA TARGETS ---

run: build
	./$(BIN_DIR)/$(TARGET)

debug: build
	$(DEBUGGER) $(BIN_DIR)/$(TARGET)

clean:
	rm -f $(OBJ)

remove: clean
	rm -f $(BIN_DIR)/$(TARGET)

.PHONY: build run debug clean remove
