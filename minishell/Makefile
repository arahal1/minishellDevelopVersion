NAME := minishell
CC := cc
CFLAGS := -Wall -Wextra -Werror
INCLUDES := -Iincludes

SRCS := src/main.c src/loop.c
OBJS := $(SRCS:.c=.o)

# On many Linux setups you also need -lncurses (or -ltinfo).
READLINE_LIBS := -lreadline -lhistory -lncurses

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(READLINE_LIBS) -o $(NAME)

%.o: %.c includes/minishell.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
