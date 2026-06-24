CC = clang
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
AR = ar rcs

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

LIB_NAME = libhcb_server.a
EXEC_NAME = hcb_server

# Bütün C dosyalarını bul
ALL_SRCS = $(shell find $(SRC_DIR) -name "*.c")

# main.c'yi statik kütüphaneye girmemesi için filtrele
LIB_SRCS = $(filter-out $(SRC_DIR)/main.c, $(ALL_SRCS))
LIB_OBJS = $(LIB_SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Sadece main için
MAIN_SRC = $(SRC_DIR)/main.c
MAIN_OBJ = $(MAIN_SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# all kuralı artık hem kütüphaneyi hem de uygulamayı üretiyor
all: $(BIN_DIR)/$(LIB_NAME) $(BIN_DIR)/$(EXEC_NAME)

# 1. Aşama: Statik kütüphane çıktısı (main.c hariç)
$(BIN_DIR)/$(LIB_NAME): $(LIB_OBJS)
	@mkdir -p $(BIN_DIR)
	$(AR) $@ $^

# 2. Aşama: main.o dosyasını al ve kendi ürettiğimiz libhcb_server.a ile bağla
$(BIN_DIR)/$(EXEC_NAME): $(MAIN_OBJ) $(BIN_DIR)/$(LIB_NAME)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $< -L$(BIN_DIR) -lhcb_server -o $@

# Derleme kuralı (Hem kütüphane dosyaları hem de main.c için ortak)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(BIN_DIR)

re: fclean all

.PHONY: all clean fclean re
