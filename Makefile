GCC = gcc
NAME = mini-compiler
INCLUDE = include
CFLAGS = -I $(INCLUDE)
SRC_FILES = main.c $(wildcard src/*.c)
OBJ_FILES = bin/main.o $(patsubst src/%.c, bin/%.o, $(wildcard src/*.c))

all: $(NAME)

$(NAME): $(OBJ_FILES)
	$(GCC) $(CFLAGS) $^ -o $@

bin/main.o: main.c | bin
	$(GCC) $(CFLAGS) -c $< -o $@

bin/%.o: src/%.c | bin
	$(GCC) $(CFLAGS) -c $< -o $@

run: $(NAME)
	./$(NAME)

bin:
	mkdir -p bin

clean:
	rm -f $(NAME)

fclean: clean
	rm -f OBJ_FILES
	rm -rf bin

re: fclean all

.PHONY: all bin clean fclean re
