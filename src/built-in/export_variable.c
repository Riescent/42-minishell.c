
#include <stdlib.h>
#include "libft.h"
#include "expansions.h"

static int		add_value(char *key, char *value, t_hashmap env_variables);
static int		addition_export(char *key, char *value,
					t_hashmap env_variables);
static bool		is_valid_key(char *key);
static int		change_quote_value(char *key, t_hashmap env_variables);

int	export_variable(char *variable, t_hashmap env_variables)
{
	char	*key;
	char	*value;
	char	*equal;
	int		return_code;

	equal = ft_strchr(variable, '=');
	if (equal == variable || equal == NULL)
		return (is_valid_key(variable));
	*equal = '\0';
	key = variable;
	if (!is_valid_key(key))
	{
		*equal = '=';
		return (0);
	}
	value = ft_strdup(equal + 1);
	if (value == NULL)
		return (-1);
	return_code = add_value(key, value, env_variables);
	if (return_code < 0)
		return (-1);
	if (change_quote_value(key, env_variables) < 0)
		return (-1);
	return (1);
}

static int	add_value(char *key, char *value, t_hashmap env_variables)
{
	if (key[ft_strlen(key) - 1] == '+')
		return (addition_export(key, value, env_variables));
	else if (ft_hm_add_elem(env_variables, key, value, free) < 0)
	{
		free(value);
		return (-1);
	}
	return (1);
}

static	bool	is_valid_key(char *key)
{
	size_t	index;

	index = 0;
	if (!ft_isalpha(key[index]) && key[index] != '_')
		return (false);
	while (key[index] != '\0')
	{
		if (key[index] == '+' && key[index + 1] == '\0')
			return (true);
		if (!ft_isalnum(key[index]) && key[index] != '_')
			return (false);
		index++;
	}
	return (true);
}

static int	addition_export(char *key, char *value, t_hashmap env_variables)
{
	char	*previous_value;
	char	*tmp;

	key[ft_strlen(key) - 1] = '\0';
	previous_value = ft_hm_get_content(env_variables, key);
	if (previous_value != NULL)
	{
		tmp = ft_strjoin(previous_value, value);
		free(value);
		value = tmp;
	}
	if (value == NULL)
		return (-1);
	if (ft_hm_add_elem(env_variables, key, value, free) < 0)
	{
		free(value);
		return (-1);
	}
	return (1);
}

static int	change_quote_value(char *key, t_hashmap env_variables)
{
	char	*value;
	char	*new_value;
	char	*tmp;

	if (key[ft_strlen(key) - 1] == '+')
		key[ft_strlen(key) - 1] = '\0';
	value = ft_hm_get_content(env_variables, key);
	ft_printf("%s\n", value);
	if (value == NULL)
		return (1);
	new_value = ft_strdup(value);
	if (new_value == NULL)
		return (-1);
	tmp = ft_strjoin_three("\"", new_value, "\"");
	free(new_value);
	new_value = tmp;
	if (new_value == NULL)
		return (-1);
	ft_printf("%s\n", new_value);
	if (ft_hm_add_elem(env_variables, key, new_value, free) < 0)
		return (free(new_value), -1);
	return (1);
}