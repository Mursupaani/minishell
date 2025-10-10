/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_signals.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anpollan <anpollan@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 18:20:30 by anpollan          #+#    #+#             */
/*   Updated: 2025/10/10 18:29:49 by anpollan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_child_signals(void)
{
	struct sigaction	s_sigint;
	struct sigaction	s_sigquit;

	s_sigint.sa_flags = 0;
	s_sigquit.sa_flags = 0;
	sigemptyset(&s_sigint.sa_mask);
	sigemptyset(&s_sigquit.sa_mask);
	s_sigint.sa_handler = SIG_DFL;
	s_sigquit.sa_handler = SIG_DFL;
	sigaction(SIGINT, &s_sigint, NULL);
	sigaction(SIGQUIT, &s_sigquit, NULL);
}
