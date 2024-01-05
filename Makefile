NAME = ircserv
CC=@c++
CFLAGS=-Wall -Wextra -Werror -std=c++98
RM=rm -f
SRCS= *.cpp Commands/*.cpp
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