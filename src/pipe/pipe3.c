/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celestin <celestin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/21 21:34:01 by celestin          #+#    #+#             */
/*   Updated: 2020/10/05 15:32:24 by celestin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

int		pipe_arg_cleanup(t_pipe_cmd *pipe_cmd)
{
	int		i;
	t_cmd	new_cmd;
	char	**tmp;

	tmp = pipe_cmd->cmd[pipe_cmd->i];
	i = 0;
	if (!(new_cmd.arg = malloc(sizeof(char *) *
		(arg_len(pipe_cmd->cmd[pipe_cmd->i]) + 1))))
		return (ft_strerror(NULL, pipe_cmd->cmd[pipe_cmd->i], NULL, NULL));
	while (pipe_cmd->cmd[pipe_cmd->i][i])
	{
		new_cmd.arg[i] = ft_strdup(pipe_cmd->cmd[pipe_cmd->i][i]);
		i++;
	}
	new_cmd.arg[i] = NULL;
	if (!arg_cleanup(&new_cmd))
		return (ft_strerror(NULL, pipe_cmd->cmd[pipe_cmd->i], NULL, NULL));
	pipe_cmd->cmd[pipe_cmd->i] = new_cmd.arg;
	free_tab(tmp);
	return (1);
}

int		pipe_fork(t_pipe_cmd *pipe_cmd)
{
	if (pipe(pipe_cmd->pfd) == -1)
	{
		ft_strerror(NULL, NULL, NULL, NULL);
		return (-1);
	}
	if ((pipe_cmd->proc = fork()) == -1)
	{
		free_tab(pipe_cmd->tab_env);
		free_tab_3d(pipe_cmd->cmd);
		return (-1);
	}
	return (0);
}

void	toggle_redir(char **s, int i, t_pipe_cmd *pipe_cmd)
{
	if ((ft_strcmp(s[i], ">") == 0) ||
		(ft_strcmp(s[i], ">>") == 0) ||
		(ft_strcmp(s[i], "<") == 0))
	{
		pipe_cmd->check_redir = 1;
	}
}

void	redir_pipe(char **s, t_pipe_cmd *pipe_cmd, t_cmd *cmd)
{
	int	i;
	int	ret_red;

	i = 0;
	ret_red = 0;
	pipe_cmd->check_redir = 0;
	count_redir_pipe(s, cmd);
	while (s[i])
	{
		toggle_redir(s, i, pipe_cmd);
		if (ft_strcmp(s[i], ">") == 0)
			ret_red = r_anglebracket(s, cmd, s[i + 1], pipe_cmd);
		else if (ft_strcmp(s[i], ">>") == 0)
			ret_red = r_dbanglebracket(s, cmd, s[i + 1], pipe_cmd);
		else if (ft_strcmp(s[i], "<") == 0)
			ret_red = l_anglebracket(s, cmd, s[i + 1], pipe_cmd);
		if (ret_red == -1)
			free_and_exit(pipe_cmd, cmd, 1);
		i++;
	}
}
