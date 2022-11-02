/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icarboni <icarboni@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 20:12:59 by icarboni          #+#    #+#             */
/*   Updated: 2022/11/02 18:23:15 by icarboni         ###   ########.fr       */
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


char	*ft_get_path(char **paths, char *cmd)
{
	int 	i;
	int 	j;
	char	*command;
	char	*command2;

	i = -1;
	j = -1;
	while (paths[++i])
	{
		command = ft_strjoin(paths[i], "/");
		command2 = ft_strjoin(command, cmd);
		free(command);
		if (access(command2, 0) == 0)
			return (command2);
		free(command2);
	}
	return (NULL);
}

void	ft_execute_father(char *cmd, char **envp)
{
	char 	**args;
	char 	**paths;
	char	*command;

	args = ft_split(cmd, ' ');
	paths = ft_env_parse(envp);
	command = ft_get_path(paths, cmd);
	printf("%s", command);
	if (command == NULL)
		exit(1);
	execve(command, args, envp);
}

void	ft_execute_child(char *cmd, char **envp)
{
	char **args;
	char **paths;
	char	*command;

	args = ft_split(cmd, ' ');
	paths = ft_env_parse(envp);
	command = ft_get_path(paths, cmd);
	printf("%s", command);
	if (command == NULL)
		exit(1);
	execve(command, args, envp);
}

void	ft_child_function(int f1, char **argv, int pip[], char **envp)
{
	int		d;

	d = dup2(f1, STDIN_FILENO);
	if (d < 0)
		exit(1);
	d = dup2(pip[1], STDOUT_FILENO);
	if (d < 0)
		exit(1);
	close(pip[0]);
	ft_execute_child(argv[2], envp);
	close(f1);
}

void	ft_father_function(int f2, char **argv, int pip[], char **envp)
{
	int		d;
	int 	status;
	int 	i;

	i = -1;
	waitpid(-1, &status, 0);
	d = dup2(f2, STDOUT_FILENO);
	if (d < 0)
		exit(1);
	d = dup2(pip[0], STDIN_FILENO);
	if (d < 0)
		exit(1);
	close(pip[1]);
	ft_execute_father(argv[3], envp);
	close(f2);
}

void	pipex(int f1, int f2, char **argv, char **envp)
{
	int		pip[2];
	int	father;

	pipe(pip);
	father = fork();
	if (father < 0)
		exit(1);
	if (father == 0) 
		ft_child_function(f1, argv, pip, envp);
	else
		ft_father_function(f2, argv, pip, envp);
	exit(0);
}

int main(int argc, char **argv, char **envp)
{
	int		f1;
	int		f2;

	if (argc < 5)
	{
		printf("Wrong number of arguments.");
		exit(1);
	}
	else
	{
		f1 = open(argv[1], O_RDONLY);
		if (f1 < 0)
			return (ft_putstr2(strerror(errno), argv[4]));
		f2 = open(argv[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (f2 < 0)
			return (ft_putstr2(strerror(errno), argv[4]));
		else
			pipex(f1, f2, argv, envp);
	}
	return (0);
}
