# Nome do executável
TARGET = program

# Compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall -Wextra -std=c11

# Arquivos de origem
SRC = main.c helpers.c

# Arquivos objeto
OBJ = $(SRC:.c=.o)

# Regra padrão (default)
all: $(TARGET)

# Regra para construir o executável
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Regra para construir os arquivos objeto
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza dos arquivos objeto e do executável
clean:
	rm -f $(OBJ) $(TARGET)

# Regra para garantir que o Makefile seja refeito se helpers.h mudar
$(OBJ): helpers.h

# Phony targets (não geram arquivos)
.PHONY: all clean
