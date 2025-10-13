/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:40:00 by magebreh          #+#    #+#             */
/*   Updated: 2025/10/13 16:44:48 by magebreh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parent_sigint_handler(int sig, siginfo_t *info, void *context)
{
	g_signal_received = sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	(void)context;
	(void)info;
}

void	setup_parent_signals(void)
{
	struct sigaction	s_sigint;
	struct sigaction	s_sigquit;

	rl_done = 0;
	rl_event_hook = NULL;
	s_sigint.sa_flags = SA_SIGINFO;
	s_sigquit.sa_flags = SA_SIGINFO;
	sigemptyset(&s_sigint.sa_mask);
	sigemptyset(&s_sigquit.sa_mask);
	s_sigint.sa_sigaction = parent_sigint_handler;
	s_sigquit.sa_handler = SIG_IGN;
	sigaction(SIGINT, &s_sigint, NULL);
	sigaction(SIGQUIT, &s_sigquit, NULL);
}
