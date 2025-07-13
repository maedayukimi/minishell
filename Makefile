NAME	= minishell
SHELL	= /bin/sh
CC	= cc
CFLAGS	= -Wall -Wextra -Werror -g
LINK	= -lreadline -lncurses -L/opt/homebrew/opt/readline/lib
INC = -I./ -I/opt/homebrew/opt/readline/include
RM	 = rm -f

SRCS	= main.c parse.c tokenizer.c free_utils.c \
		  expansion.c ft_strjoin.c allocate_argv.c builtin.c \
		  redirect.c create_word.c token_utils.c \
		  expand_variables.c ft_split.c ft_itoa.c heredoc.c \
		  ft_substr.c setenv.c expand_utils.c handle_dollar.c \
		  env_utils.c pl_exec.c pl_utils.c \
		  bg_exec.c exec.c exec_cmd.c exec_simple.c interpret.c \
		  signals.c redirect_init.c redirect_primary.c redirect_primary2.c \
		  redirect_secondary.c redirect_sets.c redirect_parsers.c \
		  node_utils.c parse_utils.c parse_utils2.c parse_group_command.c \
		  parse_simple_command.c get_complete_input.c remove_quote_word.c \
		  search_path.c ft_sprintf.c ft_dprintf.c expansion_utils.c \
		  ft_strlen.c env_management.c handle_pl.c token_utils2.c \
		  builtin2.c builtin3.c builtin4.c builtin5.c builtin6.c \
		  ft_strcmp.c exec_sep.c redirect_open.c redirect_do.c redirect_reset.c \
		  ft_strdup.c ft_strlcpy.c ft_strcpy.c ft_strncmp.c \
		  ft_calloc.c ft_bzero.c ft_memset.c ft_strchr.c \
		  ft_isalnum.c ft_memcpy.c ft_isspace.c ft_atoi.c ft_strndup.c
OBJS	= $(SRCS:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LINK) -o $(NAME)

all: $(LIBFT_A) $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

debug:	all clean

re: fclean all

.PHONY: all clean fclean re
