/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icarboni <icarboni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 20:12:59 by icarboni          #+#    #+#             */
/*   Updated: 2022/11/09 12:50:49 by icarboni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/pipex.h"
#include "./libft/libft.h"

/* For the child process, we want infile to be our stdin (we want it as input), and end[1] to be our stdout
(we want to write to end[1] the output of cmd1).
In the parent process, we want end[0] to be our stdin (end[0] reads from end[1] the output of cmd1), 
and outfile to be our stdout (we want to write to it the output of cmd2).*/

char	**ft_env_parse(char **envp)
{
	char	*path_from_envp;
	char	**mypaths;
	int		i;
	i = 0;
	while (envp[i] != 0)
	{
		if (ft_strstr(envp[i], "PATH") != '\0')
			path_from_envp = envp[i];
		i++;
	}
	path_from_envp = ft_substr(path_from_envp, 5, ft_strlen(path_from_envp));
	//printf("%s\n", path_from_envp);
	mypaths = ft_split(path_from_envp, ':');
	return (mypaths);
}


char	*ft_get_path(t_pipex pipex)
{
	int 	i;
	char	*command;
	char	*command2;

	i = -1;
	while (pipex.paths[++i])
	{
		command = ft_strjoin(pipex.paths[i], "/");
		command2 = ft_strjoin(command, pipex.cmd);
		free(command);
		//("%s\n", command2);
		if (access(command2, 0) == 0)
			return (command2);
		free(command2);
	}
	exit(1);
}
/* 
void	ft_execute(char *cmd, char **envp)
{
	char 	**args;
	char 	**paths;
	char	*command;
	int		d;
	char	**args_clean;

	args = ft_split(cmd, ' ');
	//args = ft_clean_args(args);
	paths = ft_env_parse(envp);
	command = ft_get_path(paths, args[0]);
	//printf("%s", command);
	args_clean = ft_clean_args(args);
	if (command == NULL)
	{
		command = ft_get_path(paths, args_clean[0]);
		if (command == NULL)
			exit(EXIT_FAILURE);
	}
	d = execve(command, args, envp);
	if (d == -1)
	{
		d = execve(command, args_clean, envp);
		if (d == -1)
		{
			ft_putstr_error(strerror(errno), "execve");
			exit(1);

		}
	}
}*/

void	ft_execute(t_pipex pipex, char *full_cmd, char **envp)
{
	int		d;

	pipex.args = ft_split(full_cmd, ' ');
	pipex.cmd = pipex.args[0];
	pipex.paths = ft_env_parse(envp);
	pipex.cmd_path = ft_get_path(pipex);
	if (pipex.cmd_path == NULL)
		exit(EXIT_FAILURE);
	d = execve(pipex.cmd_path, pipex.args, envp);
	if (d == -1)
	{
		d = execve(pipex.cmd_path, ft_clean_args(pipex.args), envp);
		if (d == -1)
		{
			ft_putstr_error(strerror(errno), "execve");
			exit(1);
		}
	}
} 

void	ft_child1(t_pipex pipex, char **argv, char **envp)
{
	int		d;

	d = dup2(pipex.infile, STDIN_FILENO);
	if (d < 0)
		exit(1);
	d = dup2(pipex.pip[1], STDOUT_FILENO);
	if (d < 0)
		exit(1);
	close(pipex.pip[0]);
	ft_execute(pipex, argv[2], envp);
	close(pipex.infile);
}

void	ft_child2(t_pipex pipex, char **argv, char **envp)
{
	int		d;
	int 	status;

	waitpid(-1, &status, 0);
	d = dup2(pipex.outfile, STDOUT_FILENO);
	if (d < 0)
		exit(1);
	d = dup2(pipex.pip[0], STDIN_FILENO);
	if (d < 0)
		exit(1);
	close(pipex.pip[1]);
	ft_execute(pipex, argv[3], envp);
	close(pipex.outfile);
}

void	ft_pipex(t_pipex pipex, char **argv, char **envp)
{
	pipe(pipex.pip);
	pipex.pid1 = fork();
	if (pipex.pid1 < 0)
		exit(1);
	if (pipex.pid1 == 0) 
		ft_child1(pipex, argv, envp);
	pipex.pid2 = fork();
	if (pipex.pid2 < 0)
		exit(1);
	if (pipex.pid2 == 0) 
		ft_child2(pipex, argv, envp);
	//waitpid(pipex.pid1, NULL, 0);
	//waitpid(pipex.pid2, NULL, 0);
	//exit(0);
}

int main(int argc, char **argv, char **envp)
{
	t_pipex pipex;

	if (argc < 5)
	{
		printf("Wrong number of arguments.");
		exit(1);
	}
	else
	{
		pipex.infile = open(argv[1], O_RDONLY);
		if (pipex.infile < 0)
		{
			ft_putstr_error(strerror(errno), argv[1]);
			exit(1);
		}
		pipex.outfile = open(argv[argc - 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (pipex.outfile < 0)
		{
			ft_putstr_error(strerror(errno), argv[argc - 1]);
			exit(1);
		}
		else
			ft_pipex(pipex, argv, envp);
	}
	return (0);
}
