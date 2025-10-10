/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:40:00 by magebreh          #+#    #+#             */
/*   Updated: 2025/10/10 19:09:13 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// FIXME: 
// - rl_event_hook
// - rl_catch_signal
// - rl_done
//
// Jos heredoc saa sigint, niin rl_done = 1
// tama rl_even_hookiin, joka asetetaan handlerin sisalla

int	check_if_sigint_received(void)
{
	if (g_signal_received == SIGINT)
	{
		rl_done = 1;
		return (1);
	}
	return (0);
}

void	heredoc_sigint_handler(int sig, siginfo_t *info, void *context)
{
	g_signal_received = sig;
	(void)info;
	(void)context;
}

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

void	parent_sigquit_handler(int sig, siginfo_t *info, void *context)
{
	g_signal_received = sig;
	(void)context;
	(void)info;
}

void	setup_parent_signals(void)
{
	struct sigaction	s_sigint;
	struct sigaction	s_sigquit;

	//NOTE: If rl_done == 1 then readline doesn't wait for input. This way we can exit heredocs.
	//FIXME: Make a 'setup_child_signals' function that resets child signal handling so they can exit with ctr-c and ctrl-'\'
	rl_done = 0;
	s_sigint.sa_flags = SA_SIGINFO;
	s_sigquit.sa_flags = SA_SIGINFO;
	sigemptyset(&s_sigint.sa_mask);
	sigemptyset(&s_sigquit.sa_mask);
	s_sigint.sa_sigaction = parent_sigint_handler;
	s_sigquit.sa_sigaction = parent_sigquit_handler;
	sigaction(SIGINT, &s_sigint, NULL);
	sigaction(SIGQUIT, &s_sigquit, NULL);
}
