/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 04:14:45 by vfries            #+#    #+#             */
/*   Updated: 2023/02/10 16:56:00 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "lexer.h"
# include <stdbool.h>

void	init_interactive_signal_handling(void);
char	*get_command_from_fd(int fd);
int	set_end_of_file(char **command);
int					run_interactive_parsing(char **command,
						t_list **parsed_tokens);

int					parse_syntax(t_list *tokens);

int					check_parentheses(t_list *tokens);
// quote
int					remove_quotes(t_list *tokens);
int					is_valid_quote(char *command);

//	simplify_tokens
//	{
//		add_command
//		{
//			add_args.c
int					add_args(t_token *token, t_list **args);
//			add_command.c
t_token				*get_new_command(t_list **tokens);
//			get_files.c
t_list				*get_files(t_list **files);
//		}
//		get_subshells.c
t_list				*get_subshells(t_list *tokens);
//		simplify_tokens.c
int					simplify_tokens(t_list **tokens);
//	}

//	syntax
//	{
//		is_syntax_ok.c
bool				is_syntax_ok(t_list *tokens);
//		syntax_tools.c
bool				syntax_is_operator(t_list *token_lst);
bool				syntax_is_empty(t_list *token_lst);
enum e_operators	syntax_get_operator(t_list *token_lst);
//void				print_syntax_error(t_list *token);
//	}

#endif
