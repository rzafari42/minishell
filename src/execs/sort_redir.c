/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celestin <celestin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/25 15:43:53 by celestin          #+#    #+#             */
/*   Updated: 2020/10/02 02:13:55 by celestin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

int		find_first_redir(t_cmd *cmd)
{
	int		i;

	i = 0;
	while (cmd->arg[i])
	{
		if (arg_is_redir(cmd->arg[i]))
			return (1);
		i++;
	}
	return (0);
}

int		arg_is_redir(char *str)
{
	if ((ft_strcmp(str, ">") == 0) ||
			(ft_strcmp(str, "<") == 0) ||
			(ft_strcmp(str, ">>") == 0))
		return (1);
	else
		return (0);
}

int		sort_cmd(t_cmd *cmd, t_sort_redir *sort)
{
	char	**new_arg;
	char	**tmp;
	int		i;

	i = -1;
	if (!(new_arg = malloc(sizeof(char *) * (sort->len_arg + 1))))
		return (0);
	while (++i < sort->i_first_redir)
		new_arg[i] = ft_strdup(cmd->arg[i]);
	new_arg[i] = ft_strdup(cmd->arg[sort->i]);
	while (cmd->arg[++i])
	{
		if (i <= sort->i)
			new_arg[i] = ft_strdup(cmd->arg[i - 1]);
		else
			new_arg[i] = ft_strdup(cmd->arg[i]);
	}
	new_arg[i] = NULL;
	tmp = cmd->arg;
	cmd->arg = new_arg;
	free_tab(tmp);
	sort->i_first_redir++;
	return (1);
}

void	sort_redir_next(t_sort_redir *sort)
{
	sort->prev_redir = 0;
	sort->prev_file = 1;
}

int		sort_redir(t_cmd *cmd)
{
	t_sort_redir	sort;

	sort.len_arg = arg_len(cmd->arg);
	sort.i = 0;
	sort.prev_redir = 0;
	sort.prev_file = 0;
	if (!(sort.i_first_redir = find_first_redir(cmd)))
		return (0);
	while (cmd->arg[sort.i])
	{
		if (sort.prev_file && !arg_is_redir(cmd->arg[sort.i]))
		{
			if (!sort_cmd(cmd, &sort))
				return (0);
		}
		if (arg_is_redir(cmd->arg[sort.i]))
		{
			sort.prev_redir = 1;
			sort.prev_file = 0;
		}
		else if (sort.prev_redir && !arg_is_redir(cmd->arg[sort.i]))
			sort_redir_next(&sort);
		sort.i++;
	}
	return (1);
}
