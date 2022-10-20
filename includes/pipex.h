/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icarboni <icarboni@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 11:54:13 by icarboni          #+#    #+#             */
/*   Updated: 2022/10/19 19:23:58 by icarboni         ###   ########.fr       */
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

void	pipex(int f1, int f2, char **argv, char **envp);
void	ft_child_function(int f1, char *cmd1, int pip[], char **envp);
void	ft_father_function(int f2, char *cmd2, int pip[], char **envp);
int		ft_strstr(char *str, char *to_find);

#endif