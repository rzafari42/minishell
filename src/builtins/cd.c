#include "../../incs/minishell.h"

int     update_pwd(void)
{
    char *dir;
    char *old_dir;

    dir = NULL;
    old_dir = NULL;
    if (!(old_dir = get_var(g_env, "PWD=")))
		ft_error();
	if (!(dir = getcwd(dir, 0)))
		ft_error();
    set_var(g_env, "PWD=", dir);
	set_var(g_export, "PWD=", dir);
	set_var(g_env, "OLDPWD=", old_dir);
	set_var(g_export, "OLDPWD=", old_dir);
    free(dir);
    free(old_dir);
    return (0);
}

int 	builtin_cd(char **arg)
{
	char *new_dir;

	if (arg_len(arg) == 1)
	{
        ft_putstr("hey\n");
		if (!(new_dir = get_var(g_env, "HOME=")))
            ft_error();
        //printf("this is the home path: %s", new_dir);
        if (chdir(new_dir) == -1)
            ft_error();
        free(new_dir);
	}
    else
    {
        ft_putstr("ho\n");
        if (!ft_strcmp(arg[1], ""))
			return (0);
        new_dir = arg[1];
        if (chdir(new_dir) == -1)
            ft_error();
    }
    return(update_pwd());
}