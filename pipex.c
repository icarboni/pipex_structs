/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icarboni <icarboni@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 20:12:59 by icarboni          #+#    #+#             */
/*   Updated: 2022/10/20 19:12:39 by icarboni         ###   ########.fr       */
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
	//printf("%s\n", PATH_from_envp);
	mypaths = ft_split(path_from_envp, ':');
	return (mypaths);
}

void	ft_execute(char *cmd, char **envp)
{
	int i;
	int j;
	char *command;
	char **args;
	char **paths;

	i = -1;
	j = -1;
	args = ft_split("wc infile.txt", ' ');
	//args = ft_split(cmd, ' ');
	//while (args[++j] != 0)
	//		printf("%s\n", args[j]);
	paths = ft_env_parse(envp);
	printf("\n");
	printf("%s\n", cmd);
	while (paths[++i])
	{
		command = ft_strjoin(paths[i], "/");
		command = ft_strjoin(command, args[0]);
		printf("%s\n", paths[i]);
		printf("%s\n\n", command);
		execve("/usr/bin/wc", args, NULL);
		perror("Error");
		free(command);
	}
	//return (EXIT_FAILURE);
}

void	ft_child_function(int f1, char *cmd1, int pip[], char **envp)
{
	int		d;

	d = dup2(f1, STDIN_FILENO);
	if (d < 0)
		return (perror("Dup2: "));
	d = dup2(pip[1], STDOUT_FILENO);
	if (d < 0)
		return (perror("Dup2: "));
	ft_execute(cmd1, envp);	
	close(pip[0]);
	close(f1);
	exit(EXIT_FAILURE);
}

void	ft_father_function(int f2, char *cmd2, int pip[], char **envp)
{
	int		d;
	int 	status;
	int 	i;
	
	i = -1;
	waitpid(-1, &status, 0);
	d = dup2(f2, STDOUT_FILENO);
	if (d < 0)
		return (perror("Dup2: "));
	d = dup2(pip[0], STDIN_FILENO);
	if (d < 0)
		return (perror("Dup2: "));
	ft_execute(cmd2, envp);

	close(pip[1]);
	close(f2);
}

void	pipex(int f1, int f2, char **argv, char **envp)
{
	int		pip[2];
	pid_t	father;

	pipe(pip);
	father = fork();
	if (father < 0)
		return (perror("Fork: "));
	if (father == 0) 
		ft_child_function(f1, argv[2], pip, envp);
	else
		ft_father_function(f2, argv[3], pip, envp);
}

int main(int argc, char **argv, char **envp)
{
	int		f1;
	int		f2;

	if (argc != 5)
	{
		printf("Wrong number of arguments.");
		return (0);
	}
	else
	{
		f1 = open(argv[1], O_RDONLY);
		f2 = open(argv[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (f1 < 0 || f2 < 0)
		{
			printf("Error opening files.");
			return (-1);
		}
		else
			pipex(f1, f2, argv, envp);
	}
}
