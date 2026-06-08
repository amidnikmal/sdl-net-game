CC ?= gcc
CFLAGS = -Wall -Wextra -g
BUILD_DIR = build
APP = app
SRC_DIR = src
OBJ = $(BUILD_DIR)/main.o $(BUILD_DIR)/add.o

all: $(APP)

$(APP): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(APP)
