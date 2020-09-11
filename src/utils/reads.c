/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reads.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzafari <rzafari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/19 14:07:50 by rzafari           #+#    #+#             */
/*   Updated: 2020/09/11 12:07:29 by rzafari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

static void		print_gret(char *str)
{
	printf("%s		--	g_ret = %d\n", str, g_ret);
	int hey = 1;
}

void		printtab(char **tab)
{
	int i;

	i = 0;
	printf("len tab: %d\n", arg_len(tab));
	while (tab[i])
	{
		printf("str-%d: %s\n", i, tab[i]);
		i++;
	}
}

char			**tablst(t_list *lst)
{
	char	**ret;
	t_list	*tmp;
	int		i;

	ret = NULL;
	tmp = NULL;
	if (!(ret = malloc((ft_lstsize(lst) + 1) * sizeof(*ret))))
		return (NULL);
	i = 0;
	tmp = lst;
	while (tmp)
	{
		if (!(ret[i] = ft_strdup(tmp->content)))
			return (NULL);
		tmp = tmp->next;
		i++;
	}
	ret[i] = NULL;
	return (ret);
}

void			fd_dup(int i)
{
	static int input;
	static int output;

	if (i == 0)
	{
		if ((input = dup(0)) == -1)
		{
			ft_strerror(NULL, NULL, NULL, NULL);
			exit(errno);
		}
		if ((output = dup(1)) == -1)
		{
			ft_strerror(NULL, NULL, NULL, NULL);
			exit(errno);
		}
	}
	else if (i == 1)
	{
		if (dup2(input, 0) == -1) 
		{
			ft_strerror(NULL, NULL, NULL, NULL);
			exit(errno);
		}
		if (dup2(output, 1) == -1)
		{
			ft_strerror(NULL, NULL, NULL, NULL);
			exit(errno);
		}
	}
}

int				launch_exec(char **arg, t_cmd *cmd)
{
	int	ret_red;
	int	ret_exec;

	fd_dup(0);
	g_ret = 0;
	ret_red = redirection(cmd);
	if (!ret_red)
	{
		if (!ft_checkbuiltins(cmd->arg, cmd))
		{
			ret_exec = ft_exec(cmd->arg);
			if (ret_exec == -1)
			{
				return (ft_strerror(NULL, NULL, "fork", NULL));
			}
			else if (ret_exec == -2)
			{
				ft_error(CMD_NOT_FOUND, NULL, NULL, cmd->arg[0]);
				return (-2);
			}
		}
		fd_dup(1);
		return (1);
	}
	if (ret_red == -1)
	{
		fd_dup(1);
		return (-1);
	}
	fd_dup(1);
	return (0);
}

void			check_signal(int status)
{
	if (WTERMSIG(status) == 3)
		ft_putstr("Quit\n");
	if (WIFEXITED(status))
		g_ret = WEXITSTATUS(status);
	if (WIFSIGNALED(status))
		g_ret = 128 + WTERMSIG(status);
}

int				ft_exec(char **arg_list)
{
	pid_t	proc;
	int		status;
	char	*s;
	char	**tab_env;

	if (!(tab_env = tablst(g_env)))
		return (-1);
	if ((proc = fork()) == -1)
	{
		free_tab(tab_env);
		return (-1);
	}
	if (proc == 0)
	{
		s = find_path_env(tab_env, arg_list[0]);
		if ((execve(s, arg_list, tab_env)) == -1)
		{
			free(s);
			free_tab(tab_env);
			return (-2);
		}
	}
	else if (proc > 0)
	{
		if (wait(&status) == -1)
		{
			free_tab(tab_env);
			ft_strerror(NULL, NULL, "wait", NULL);
			return (-1);
		}
		check_signal(status);
		if (g_ret == 127)
			return (127);
	}
	free_tab(tab_env);
	return (0);
}

int				launch(char *input, t_cmd *cmd)
{
	char	**arg;
	int		i;
	int		len_new_arg_list;
	int		ret_exec;

	if (!(arg = parsing(input)))
		return (0);
	i = 0;
	while (arg[i] != NULL)
	{
		len_new_arg_list = 0;
		while (i < arg_len(arg) && ft_strcmp(arg[i], ";") != 0)
		{
			len_new_arg_list++;
			i++;
		}
		if ((cmd->arg = semicolon(arg, i, len_new_arg_list)) == NULL)
		{
			free_tab(cmd->arg);
			return (ft_strerror(NULL, arg, NULL, NULL));
		}
		if (!check_g_ret_var(cmd->arg))
		{
			free_tab(cmd->arg);
			return (ft_strerror(NULL, arg, NULL, NULL));
		}
		if (!cleanup_quotes(cmd->arg))
		{
			free_tab(cmd->arg);
			return (ft_strerror(NULL, arg, NULL, NULL));
		}
		if (ft_count_pipe(cmd->arg) > 0)
		{
			ret_exec = ft_pipe_2(cmd->arg, cmd);
			if (ret_exec == -1)
				g_ret = 127;
		}
		else
			ret_exec = launch_exec(arg, cmd);
		if (ret_exec == -2)
			exit(127);
		if (arg[i] == NULL)
		{
			free_tab(cmd->arg);
			break ;
		}
		free_tab(cmd->arg);
		i++;
	}
	free_tab(arg);
	return (0);
}

void			prompt(void)
{
	char	buffer[MAX_INPUT_SIZE];
	int		ret;
	t_cmd	cmd;

	ft_builtinstab(&cmd);
	while (1)
	{
		ret = 0;
		if (to_print == 0)
			print_prompt_prefix();
		if (to_print == 1)
			to_print = 0;
		ret = read(STDIN_FILENO, buffer, MAX_INPUT_SIZE);
		if (ret == -1)
			exit(errno);
		if (ret)
			buffer[ret - 1] = '\0';
		else
			ft_strlcpy(buffer, "exit", 5);
		if (launch(buffer, &cmd) == -1)
			return ;
	}
}
