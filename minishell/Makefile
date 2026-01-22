NAME := minishell
CC := cc
CFLAGS := -Wall -Wextra -Werror
INCLUDES := -Iincludes

SRCS := src/main.c src/loop.c ./src/lexer/lexer_split.c \
		./libft/ft_toupper.c ./libft/ft_tolower.c ./libft/ft_substr.c ./libft/ft_strtrim.c \
		./libft/ft_strrchr.c ./libft/ft_strnstr.c ./libft/ft_strncmp.c ./libft/ft_strmapi.c \
		./libft/ft_strlen.c ./libft/ft_strlcpy.c ./libft/ft_strlcat.c ./libft/ft_strjoin.c\
		./libft/ft_striteri.c ./libft/ft_strdup.c ./libft/ft_strchr.c ./libft/ft_split.c \
		./libft/ft_putstr_fd.c ./libft/ft_putnbr_fd.c ./libft/ft_putendl_fd.c ./libft/ft_putchar_fd.c \
	  ./libft/ft_memset.c ./libft/ft_memcpy.c ./libft/ft_memcmp.c ./libft/ft_memchr.c \
	  ./libft/ft_memmove.c ./libft/ft_itoa.c ./libft/ft_isprint.c ./libft/ft_isdigit.c \
	  ./libft/ft_isascii.c ./libft/ft_isalpha.c ./libft/ft_isalnum.c \
	  ./libft/ft_calloc.c ./libft/ft_bzero.c ./libft/ft_atoi.c ./libft/ft_strcmp.c \
	  ./src/env/env_clear.c ./src/env/env_get.c ./src/env/env_init.c ./src/env/env_print.c \
	  ./src/env/env_set.c ./src/env/env_to_envp_helper.c ./src/env/env_to_envp.c \
	  ./src/env/env_unset.c ./src/parser/parser.c ./src/parser/parser_print.c \
	  ./src/executor/exec_external.c ./src/executor/exec_one.c ./src/executor/path_resolve.c \
	  ./src/exec/apply_redir.c

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
