#!/usr/bin/bash

valgrind --track-fds=yes --leak-check=full --show-leak-kinds=all --gen-suppressions=all --suppressions=gnu_suppression.supp --track-origins=yes ./minishell
