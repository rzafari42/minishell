#ifndef MINISHELL_H
# define MINISHELL_H

# define MAX_INPUT_SIZE 131072
# define NB_BUILINS 7
# define TRUE  1
# define FALSE 0

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "error.h"
#include "libft.h"
#include "env.h"
#include "builtin.h"
#include "ft_printf.h"
#include "parsing.h"

typedef int			t_ret;
t_ret   g_ret;
typedef int			(*builtfunc_addr)(char **s);

typedef struct      s_parse
{
    char      *builtnb[NB_BUILINS];
}                   t_parse;

typedef int			t_ret;
t_ret				g_ret;

void	ft_builtinstab(t_parse *parse);
int		ft_checkbuiltins(char **s, t_parse *parse);
void    print_prompt_prefix(void);
void	prompt(void);
int 	launch(char *input, t_parse *parse);
void	free_tab(char **tab);
void	assign_and_free(char **newstr, char **oldstr);
void	ft_strncpy(char *dest, char *src, int len);
void	fd_dup(int i);
int     arg_len(char **arg);
int		ft_exec(char **arg_list);
int     redirection(char **arg, t_parse *parse);
char	**semicolon(char **arg, int i, int len_new_arg_list);
char    *find_path_env(char **env, char *arg);

char	**tablst(t_list *lst);
#endif
