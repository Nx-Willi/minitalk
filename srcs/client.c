/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wdebotte <wdebotte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 11:59:52 by wdebotte          #+#    #+#             */
/*   Updated: 2022/02/13 12:28:17 by wdebotte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

typedef struct s_info
{
	int		serverpid;
	char	*str;
}	t_info;

t_info	g_sinfo;

static void	send_signal(char c, int bytes, int serverpid)
{
	int	value;

	if (c >> bytes & 1)
		value = kill(serverpid, SIGUSR2);
	else
		value = kill(serverpid, SIGUSR1);
	if (value == -1)
	{
		ft_printf("The serverpid isn't valid!\n");
		exit(EXIT_FAILURE);
	}
}

static void	handler(int signum)
{
	static int	index = 0;
	static int	bytes = 8;
	static char	c = 0;

	if (signum == SIGUSR2)
	{
		if (g_sinfo.str[index] == '\0')
			ft_printf("The message has been received!\n");
		else
			ft_printf("An error occured with the server!\n");
		exit(EXIT_SUCCESS);
	}
	if (c == 0)
		c = g_sinfo.str[index];
	if (--bytes >= 0)
		send_signal(c, bytes, g_sinfo.serverpid);
	else
	{
		bytes = 8;
		c = 0;
		index++;
		handler(SIGUSR1);
	}
}

int	main(int args, char **argv)
{
	if (args != 3)
	{
		ft_printf("Usage: ./client [pid] [message]\n");
		return (1);
	}
	g_sinfo.serverpid = ft_atoi(argv[1]);
	if (g_sinfo.serverpid <= 0)
	{
		ft_printf("The pid seems strange!\n");
		return (1);
	}
	g_sinfo.str = argv[2];
	signal(SIGUSR1, handler);
	signal(SIGUSR2, handler);
	handler(SIGUSR1);
	while (1)
		pause();
	return (0);
}
