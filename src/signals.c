/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magebreh <magebreh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:40:00 by magebreh          #+#    #+#             */
/*   Updated: 2025/08/19 12:42:59 by magebreh         ###   ########.fr       */
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

	s_sigint.sa_flags = SA_SIGINFO;
	s_sigquit.sa_flags = SA_SIGINFO;
	sigemptyset(&s_sigint.sa_mask);
	sigemptyset(&s_sigquit.sa_mask);
	s_sigint.sa_sigaction = parent_sigint_handler;
	s_sigquit.sa_sigaction = parent_sigquit_handler;
	sigaction(SIGINT, &s_sigint, NULL);
	sigaction(SIGQUIT, &s_sigquit, NULL);

}
//
// void	setup_signals(void)
// {
// 	signal(SIGINT, sigint_handler);
// 	signal(SIGQUIT, SIG_IGN);
// }
