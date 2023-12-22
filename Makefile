NAME=server
CC=@c++
CFLAGS=-Wall -Wextra -Werror
RM=rm -f
SRCS= *.cpp
OBJS=$(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

re: fclean all

clean:

fclean: clean
	$(RM) $(NAME)

run: all
	./$(NAME) 1313