/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgeorgea <fgeorgea@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 17:11:34 by fgeorgea          #+#    #+#             */
/*   Updated: 2023/05/31 01:16:13 by fgeorgea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	ft_add_slash(t_global *g)
{
	int		i;
	char	*new_str;

	i = 0;
	while (g->paths[i])
	{
		new_str = ft_strjoin(g->paths[i], "/");
		if (!new_str)
			ft_error(g, "Failed to add '/' to cmd path\n");
		free(g->paths[i]);
		g->paths[i] = new_str;
		i++;
	}
}

static void	ft_init_paths(char **env, t_global *g)
{
	int	i;

	i = 0;
	while (env[i] && (ft_strncmp(env[i], "PATH=", 5) != 0))
		i++;
	if (ft_strncmp(env[i], "PATH=", 5) != 0)
		ft_error(g, "Did not find any PATH line in the env\n");
	g->paths = ft_split(&env[i][5], ':');
	if (!g->paths)
		ft_error(g, "Failed to create paths array with split\n");
	g->nbr_paths = ft_tablen(g->paths, g);
}

static void	ft_init_cmds(char **argv, t_global *g)
{
	int		i;
	int		cmd_offset;
	char	**tmp;

	i = 0;
	cmd_offset = 2;
	tmp = NULL;
	if (g->is_heredoc)
		cmd_offset = 3;
	while (i < g->nbr_cmds)
	{
		tmp = ft_split(argv[i + cmd_offset], ' ');
		if (!tmp)
			ft_error(g, "Failed to create commands array with split\n");
		ft_lstadd_back_pipex(&g->lst, ft_lstnew_pipex(tmp, g));
		i++;
	}
}

void	ft_init_struct(int argc, char **argv, char **env, t_global *g)
{
	g->is_heredoc = 0;
	ft_check_here_doc(argc, argv, g);
	ft_init_struct_vars(argc, argv, g);
	ft_init_paths(env, g);
	ft_add_slash(g);
	ft_init_cmds(argv, g);
	g->pipefd = ft_createpipe_array(g);
	g->pids = ft_createfork_array(g);
}
