NAME			=	minishell


SRC_PATH		=	src/

SRC				=\
	built-in/cd.c						\
	built-in/echo.c						\
	built-in/env.c						\
	built-in/exit.c						\
	built-in/export.c					\
	built-in/export_print.c				\
	built-in/export_variables.c			\
	built-in/export_variable.c			\
	built-in/pwd.c						\
	built-in/unset.c					\
\
\
	env_variables/get_env_variables.c	\
	env_variables/get_envp.c			\
	env_variables/print_env_variables.c	\
\
\
	execution/exec_here_docs_tools.c	\
	execution/execute_command.c			\
	execution/execute_command_no_pipe.c	\
	execution/execute_piped_command.c	\
	execution/execute_pipes.c			\
	execution/execution.c				\
	execution/io_save.c					\
	execution/open_and_dup_files.c		\
	execution/run_builtin.c				\
\
\
	expansions/paths/absolute_path.c	\
	expansions/paths/add_path.c			\
	expansions/paths/builtin.c			\
	expansions/paths/command.c			\
	expansions/wildcards/add.c			\
	expansions/wildcards/duplicate.c	\
	expansions/wildcards/list.c			\
	expansions/wildcards/match.c		\
	expansions/wildcards/utils.c		\
	expansions/alias.c					\
	expansions/files.c					\
	expansions/here_docs.c				\
	expansions/parameters.c				\
	expansions/parameters_utils.c		\
	expansions/quotes.c					\
	expansions/token.c					\
	expansions/words.c					\
\
\
	error/error.c	\
\
\
	lexer/identifier.c \
	lexer/operator.c	\
	lexer/token.c		\
\
\
	parser/quotes.c	\
\
	parser/simplify_tokens/add_command/add_args.c		\
	parser/simplify_tokens/add_command/add_command.c	\
	parser/simplify_tokens/add_command/get_files.c		\
\
	parser/simplify_tokens/get_subshells.c			\
	parser/simplify_tokens/simplify_tokens.c		\
\
	parser/syntax.c		\
\
	parser/parse_tokens.c	\
\
\
	interactive/here_docs.c			\
	interactive/here_docs_syntax.c	\
	interactive/input.c				\
	interactive/parsing.c			\
	interactive/shell.c				\
	interactive/utils.c				\
\
\
	signals/general_signal_handling.c		\
	signals/interactive_signal_handling.c	\
	signals/pipes_signal_handling.c			\
\
\
	execute_single_line_command.c	\
	exit_code.c						\
	get_cursor_x_pos.c				\
	main.c							\
	run_minishellrc.c				\
	terminal.c						\
	free_minishell.c


DIR_INCS =\
	include/			\
	$(LIBFT_INCLUDES)

OS	= $(shell uname -s)

ifeq ($(OS), Linux)
	LIBS = \
		-l readline	\
		$(LIBFT_L)
	INCLUDES =\
	$(addprefix -I , $(DIR_INCS))
endif
ifeq ($(OS), Darwin)
	LIBS = \
		$(LDFLAGS) -l readline	\
		$(LIBFT_L)
	INCLUDES =\
		$(addprefix -I , $(DIR_INCS))	\
		$(CPPFLAGS)
endif


DIR_BUILD		=	.build/
OBJS			=	$(patsubst %.c, $(DIR_BUILD)%.o, $(SRC))
DEPS			=	$(patsubst %.c, $(DIR_BUILD)%.d, $(SRC))
DEPS_FLAGS		=	-MMD -MP
CFLAGS			=	-Wall -Wextra -Werror  -g3 #-fsanitize=address
RM				=	rm -rf
AR				=	ar rcs

LIBFT_PATH		=	lib/libft/
LIBFT_INCLUDES	=	$(LIBFT_PATH)include
LIBFT_L			=	-L $(LIBFT_PATH) -l ft
LIBFT_L_DEBUG	=	-L $(LIBFT_PATH) -l ft_debug
LIBFT_A			=	$(LIBFT_PATH)libft.a
LIBFT_A_DEBUG	=	$(LIBFT_PATH)libft_debug.a
MAKE_LIBFT		=	$(MAKE) -C $(LIBFT_PATH)

DEPENDENCIES	=	$(LIBFT_A)

.PHONY:		all
all:
			$(MAKE_LIBFT)
			$(MAKE) $(NAME)

$(NAME):	$(OBJS)
			$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) $(OBJS) -o $(NAME)

.PHONY:	bonus
bonus:	all

.PHONY:	clean
clean:
		$(MAKE_LIBFT) clean
		$(RM) $(DIR_BUILD)

.PHONY:	fclean
fclean:	clean
		$(MAKE_LIBFT) fclean
		$(RM) $(NAME)

.PHONY:	re
re:		fclean
		$(MAKE) all

.PHONY:	leaks
leaks:	all
		echo "{" > valgrind_ignore_leaks.txt
		echo "    leak readline" >> valgrind_ignore_leaks.txt
		echo "    Memcheck:Leak" >> valgrind_ignore_leaks.txt
		echo "    ..." >> valgrind_ignore_leaks.txt
		echo "    fun:readline" >> valgrind_ignore_leaks.txt
		echo "}" >> valgrind_ignore_leaks.txt
		echo "{" >> valgrind_ignore_leaks.txt
		echo "    leak add_history" >> valgrind_ignore_leaks.txt
		echo "    Memcheck:Leak" >> valgrind_ignore_leaks.txt
		echo "    ..." >> valgrind_ignore_leaks.txt
		echo "    fun:add_history" >> valgrind_ignore_leaks.txt
		echo "}" >> valgrind_ignore_leaks.txt
		valgrind --suppressions=valgrind_ignore_leaks.txt --leak-check=full\
			--show-leak-kinds=all --track-origins=yes --verbose\
			--show-mismatched-frees=yes --read-var-info=yes ./${NAME}

-include $(DEPS)

$(DIR_BUILD)%.o : $(SRC_PATH)%.c $(LIBFT_A)
		@mkdir -p $(shell dirname $@)
		$(CC) $(CFLAGS) $(DEPS_FLAGS) $(INCLUDES) -c $< -o $@
