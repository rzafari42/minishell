/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzafari <rzafari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/19 13:42:59 by rzafari           #+#    #+#             */
/*   Updated: 2020/08/31 16:38:56 by rzafari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"

t_ret	g_ret = 0;

int		is_forking(int val)
{
	static int is_forking = 0;

	if (val != 2)
		is_forking = val;
	return (is_forking);
}

void	signal_handler(int n)
{
	to_print = 0;
	if (n == SIGINT)
	{
		if (to_print == 0)
		{
			ft_putchar('\n');
			print_prompt_prefix();
			to_print = 1;
		}
		g_ret = 130;
	}
	else if (n == SIGQUIT)
		g_ret = 131;
}

int		main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;

	g_ret = 0;
	to_print = 0;
	if ((signal(SIGINT, signal_handler) == SIG_ERR))
	{
		ft_strerror(NULL, NULL, "signal", NULL);
		exit(1);
	}
	if (signal(SIGQUIT, signal_handler) == SIG_ERR)
	{
		ft_putstr("\033[1C");
		ft_putstr("\b\b \b\b \b\b");
		ft_putstr("\033[1C");
		ft_strerror(NULL, NULL, "signal", NULL);
		exit(1);
	}
	g_env = init_env(&g_env, envp);
	g_export = init_env(&g_export, envp);
	prompt();
	ft_lstclear(&g_env, free);
	ft_lstclear(&g_export, free);
	return (0);
}
