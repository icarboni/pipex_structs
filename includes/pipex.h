/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icarboni <icarboni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 11:54:13 by icarboni          #+#    #+#             */
/*   Updated: 2022/11/09 12:43:24 by icarboni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
#define PIPEX_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>


typedef struct s_pipex
{
	pid_t	pid1;
	pid_t	pid2;
	int		pip[2];
	int		infile;
	int		outfile;
	char	*cmd_path;
	char	**paths;
	char	**args;
	char	*cmd;
}t_pipex;

void	ft_pipex(t_pipex pipex, char **argv, char **envp);
void	ft_child1(t_pipex pipex, char **argv, char **envp);
void	ft_child2(t_pipex pipex, char **argv, char **envp);
int		ft_strstr(char *str, char *to_find);
void	ft_putstr_error(char *s, char *t);
char    **ft_clean_args(char **args);
void	ft_execute(t_pipex pipex, char *cmd, char **envp);

#endif