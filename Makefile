#############################################
###		 			CONFIG		          ###
#############################################

NAME			= minishell

CC				= cc
RM				= rm -rf
CFLAGS			= -Wall -Werror -Wextra -g $(INCLUDES)

MKDIR			= mkdir -p

SOURCE_FILES	= main.c check_for_builtins.c exact_match.c exit_handler.c \
				  interactive.c ft_strjoin_char.c handle_meta_char.c \
				  lexer.c lexer_handler_functions.c lexer_utils.c tokenizer.c \
				  parser.c cmd_list_utils.c cmd_make_utils.c cmd_free_utils.c \
				  cmd_execute.c cmd_path_utils.c signal_handler.c \
				  env_list_utils.c env_make_utils.c env_export_utils.c \
				  remove_quotes.c parameter_expansion.c builtin_cd.c \
				  builtin_export.c builtin_functions.c builtin_exit.c \
				  lexer_handle_quotes.c parameter_expansion_utils.c \
				  lexer_handle_make_var.c parameter_expansion_expand_string.c \
				  valid_var_checks.c shell_init.c here_doc.c \
				  cmd_special_checks.c ft_wait.c cmd_file_utils.c

SOURCES_DIR		= src
SOURCES			= $(addprefix $(SOURCES_DIR)/, $(SOURCE_FILES))
OBJ_DIR			= obj
OBJ				= $(addprefix $(OBJ_DIR)/, $(notdir $(SOURCES:.c=.o)))

INCLUDES		= -I ./includes

TOTAL_FILES := $(words $(SOURCE_FILES))

COMPILE_COUNT = 0

#############################################
###					COLORS			      ###
#############################################

RED				= \033[0;31m
GREEN			= \033[0;32m
YELLOW			= \033[0;33m
BLUE			= \033[0;34m
MAGENTA			= \033[0;35m
CYAN			= \033[0;36m
RESET_COLOR		= \033[0m
MOVEUP			= \033[F
RESET			= \e[2K\r

#############################################
###		 			RULES	      	      ###
#############################################

all: $(NAME)

$(NAME): libft/libft.a $(OBJ)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) libft/libft.a -lreadline
	@echo "$(RESET)$(GREEN)Compiled $(NAME)$(RESET_COLOR)"

$(OBJ_DIR)/%.o: $(SOURCES_DIR)/%.c
	@$(MKDIR) $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@$(eval COMPILE_COUNT=$(shell echo $$(($(COMPILE_COUNT)+1))))
	@echo -n "$(RESET)[$(YELLOW)$$(($(COMPILE_COUNT)*100/$(TOTAL_FILES)))%] Compiling minishell"

libft/libft.a:
	@make -sC libft

clean:
	@make fclean -sC libft
	@$(RM) $(OBJ_DIR)
	@echo "$(GREEN)Removed the object files from minishell$(RESET_COLOR)"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(GREEN)Removed $(NAME) and the object files$(RESET_COLOR)"

re:
	@make -s fclean
	@make -s

.PHONY:	all clean fclean re
