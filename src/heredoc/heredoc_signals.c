/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signals.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 13:05:05 by anpollan          #+#    #+#             */
/*   Updated: 2025/10/11 13:07:13 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_if_sigint_received(void);
static void	heredoc_sigint_handler(int sig, siginfo_t *info, void *context);

void	setup_heredoc_signals(void)
{
	struct sigaction	s_sigint;

	rl_done = 0;
	rl_event_hook = check_if_sigint_received;
	sigemptyset(&s_sigint.sa_mask);
	s_sigint.sa_flags = SA_SIGINFO;
	s_sigint.sa_sigaction = heredoc_sigint_handler;
	sigaction(SIGINT, &s_sigint, NULL);
}

static int	check_if_sigint_received(void)
{
	if (g_signal_received == SIGINT)
	{
		rl_done = 1;
		return (1);
	}
	return (0);
}

static void	heredoc_sigint_handler(int sig, siginfo_t *info, void *context)
{
	g_signal_received = sig;
	(void)info;
	(void)context;
}
