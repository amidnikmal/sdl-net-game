CC ?= gcc
CFLAGS = -Wall -Wextra -g
BUILD_DIR = build
APP = app
SRC_DIR = src
OBJ = $(BUILD_DIR)/main.o $(BUILD_DIR)/add.o

all: $(APP)

$(APP): $(OBJ)
	# TODO: собери итоговый бинарник из всех объектных файлов.
	# Подсказка по смыслу: здесь нужен $(CC), $(CFLAGS), $^ и -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	# TODO: собери один объектный файл из одного исходника.
	# Подсказка по смыслу: здесь нужен $(CC), $(CFLAGS), -c, $< и -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(APP)
