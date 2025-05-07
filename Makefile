NAME	= minishell
SHELL	= /bin/sh
CC	= cc
CFLAGS	= -Wall -Wextra -Werror -g
LINK	= -lreadline -lncurses
INC		= -I./
RM	 = rm -f

SRCS	= main.c parse.c tokenizer.c free_utils.c \
		  expansion.c ft_strjoin.c allocate_argv.c builtin.c \
		  redirect.c create_word.c token_utils.c \
		  expand_variables.c ft_split.c ft_itoa.c heredoc.c \
		  ft_substr.c env_management.c expand_utils.c handle_dollar.c \
		  env_utils.c pl_exec.c pl_utils.c \
		  bg_exec.c exec.c exec_cmd.c exec_simple.c interpret.c \
		  signals.c redirect_init.c redirect_primary.c redirect_primary2.c \
		  redirect_secondary.c redirect_sets.c redirect_parsers.c

OBJS	= $(SRCS:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LINK) -o $(NAME)

all: $(NAME)

clean:
		$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

debug:	all clean

re: fclean all

.PHONY: all clean fclean re
