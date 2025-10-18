/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:40:00 by magebreh          #+#    #+#             */
/*   Updated: 2025/10/15 15:23:30 by magebreh         ###   ########.fr       */
/*                                                                            */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_parent_sigint(void)
{
	if (g_signal_received == SIGINT)
		rl_done = 1;
	return (0);
}

void	parent_sigint_handler(int sig, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	g_signal_received = sig;
}

void	execution_sigint_handler(int sig, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	g_signal_received = sig;
	write(STDOUT_FILENO, "\n", 1);
}

void	setup_parent_signals(void)
{
	struct sigaction	s_sigint;
	struct sigaction	s_sigquit;

	rl_done = 0;
	rl_event_hook = check_parent_sigint;
	s_sigint.sa_flags = SA_SIGINFO;
	s_sigquit.sa_flags = SA_SIGINFO;
	sigemptyset(&s_sigint.sa_mask);
	sigemptyset(&s_sigquit.sa_mask);
	s_sigint.sa_sigaction = parent_sigint_handler;
	s_sigquit.sa_handler = SIG_IGN;
	sigaction(SIGINT, &s_sigint, NULL);
	sigaction(SIGQUIT, &s_sigquit, NULL);
}

void	setup_execution_signals(void)
{
	struct sigaction	s_sigint;
	struct sigaction	s_sigquit;

	s_sigint.sa_flags = SA_SIGINFO;
	s_sigquit.sa_flags = SA_SIGINFO;
	sigemptyset(&s_sigint.sa_mask);
	sigemptyset(&s_sigquit.sa_mask);
	s_sigint.sa_sigaction = execution_sigint_handler;
	s_sigquit.sa_handler = SIG_IGN;
	sigaction(SIGINT, &s_sigint, NULL);
	sigaction(SIGQUIT, &s_sigquit, NULL);
}
