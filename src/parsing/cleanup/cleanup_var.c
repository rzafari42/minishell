/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celestin <celestin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 19:59:05 by celestin          #+#    #+#             */
/*   Updated: 2020/09/20 20:05:56 by celestin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../incs/minishell.h"

int		insert_env_var(t_parsing_tool *tool, int i)
{
	char	*env_name;
	char	*var;

	if (!(env_name = get_var_name(tool, i)))
		return (0);
	if (!(var = parsing_variable(tool, env_name)))
		return (0);
	if (tool->empty_var)
	{
		remove_var(tool, i, env_name);
		tool->empty_var = 0;
	}
	else
		replace_var(tool, i, var, env_name);
	free(env_name);
	free(var);
	return (1);
}

int		conditions_expand_env(t_parsing_tool *tool, int i)
{
	if (is_dollar(tool->input[i])
		&& !(tool->open && tool->quote == '\'')
		&& !test_lone_dollar(tool->input, i)
		&& (tool->input[i + 1] != '?'))
		return (1);
	return (0);
}

int		expand_env(t_parsing_tool *tool)
{
	int i;

	i = 0;
	while (tool->input[i])
	{
		bs_checker(tool, i);
		if (is_quote(tool->input[i]))
			switcher_quote(tool, i);
		else if (conditions_expand_env(tool, i))
		{
			if (!tool->pre_bs)
			{
				if (!insert_env_var(tool, i))
					return (0);
				i--;
			}
		}
		cancel_pre_bs(tool, i);
		i++;
	}
	return (1);
}

int		check_var(char **arg_list)
{
	int				i;
	t_parsing_tool	tool;

	i = 0;
	while (arg_list[i])
	{
		if (includes_var(arg_list[i]))
		{
			init_tool(&tool);
			tool.input = ft_strdup(arg_list[i]);
			if (!(expand_env(&tool)))
				return (0);
			assign_and_free(&tool.input, &arg_list[i]);
		}
		i++;
	}
	return (1);
}