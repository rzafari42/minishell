#include "../../incs/minishell.h"

char	*ft_addsubstr(char const *s_copy, char *ptr, t_parsing_tool *tool)
{
	int		i;
	int		len;
	char	*str;

	i = 0;
	if (ptr - s_copy == 1 && tool->quote)
		return (NULL);
	if (tool->quote)
		len = ptr - s_copy + 1 - 2;
	else
		len = ptr - s_copy + 1;
	if (!(str = malloc(sizeof(char) * (len))))
		return (NULL);
	if (tool->quote)
	{
		ptr--;
		s_copy++;
	}
	while (s_copy < ptr)
	{
		str[i] = *s_copy;
		i++;
		s_copy++;
	}
	str[i] = '\0';
	tool->quote = '\0';
	return (str);
}

int		size_arg_tool(t_parsing_tool *tool)
{
	int		count;
	int		i;
	int		n;

	i = 0;
	n = 1;
	count = 0;
	while (tool->input[i])
	{
		if (isquote(tool->input[i]))
			switcher_quote(tool, tool->input[i]);
		if (tool->input[i] != ' ' && n != 0 && !tool->open)
		{
			count++;
			n = 0;
		}
		if (tool->input[i] == ' ' && !tool->open)
			n++;
		i++;
	}
	if (tool->open)
		return (-1);
	return (count);
}

char		**ft_split_args(t_parsing_tool *tool)
{
	char	**arg;
	char	*ptr;
	char	*s_copy;
	int		n;

	n = 0;
	if (!tool->input || !(arg = malloc(sizeof(char *) * (tool->size + 1))))
	{
		ft_strerror(NULL, NULL, NULL, NULL);
		return (NULL);
	}
	arg[tool->size] = NULL;
	s_copy = tool->input;
	ptr = s_copy;
	while (*s_copy && n <= tool->size)
	{
		while (*ptr != ' ' && *ptr != '\0')
		{
			if (*ptr == '\'' || *ptr == '\"')
				switcher_quote(tool, *ptr);
			ptr++;
		}
		if (ptr - s_copy > 0 && !tool->open)
		{
			if (!(arg[n++] = ft_addsubstr(s_copy, ptr, tool)))
			{
 				ft_strerror(NULL, arg, NULL, NULL);
 				return (NULL);
 			}
			s_copy = ptr;
		}
		if (*ptr++ != '\0' && !tool->open)
			s_copy++;
	}
	return (arg);
}

char	**parsing(char *input)
{
	t_parsing_tool	tool;
	char			**arg;

	init_tool(&tool);
	if (!(tool.input = ft_clean_input(input, &tool)))
		return (NULL);
	init_tool(&tool);
	if ((tool.size = size_arg_tool(&tool)) == -1)
	{
		ft_error(SYNTAX_ERR, NULL, NULL, NULL);
		return (NULL);
	}
	init_tool(&tool);
	if(!(arg = ft_split_args(&tool)))
		return (NULL);
	free(tool.input);
	return (arg);
}
