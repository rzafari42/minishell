/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_redir_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzafari <rzafari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/04 18:50:18 by rzafari           #+#    #+#             */
/*   Updated: 2020/10/04 18:50:33 by rzafari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

int		sort_2(t_cmd *cmd)
{
	char	**new_arg;
	char	**tmp;
	int		i;
	int		j;

	tmp = cmd->arg;
	i = 0;
	j = 2;
	if (arg_is_redir(cmd->arg[0]))
	{
		if (!(new_arg = malloc(sizeof(char *) * (arg_len(cmd->arg) + 3))))
			return (0);
		new_arg[0] = ft_strdup("echo");
		new_arg[1] = ft_strdup("-n");
		while (cmd->arg[i])
		{
			new_arg[j] = ft_strdup(cmd->arg[i]);
			i++;
			j++;
		}
		new_arg[j] = NULL;
		cmd->arg = new_arg;
		free_tab(tmp);
	}
	return (1);
}
