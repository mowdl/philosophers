# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mel-meka <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/25 18:52:03 by mel-meka          #+#    #+#              #
#    Updated: 2024/03/20 01:18:40 by mel-meka         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
RM = rm -f

PHILO_SRCS = philo_bonus.c			\
			 parse_args.c			\
			 philo_process.c

PHILO_OBJS = $(PHILO_SRCS:.c=.o)

INCLUDES = -pthread

NAME = philo_bonus

CFLAGS = -g -Wall -Wextra -Werror $(INCLUDES)

all: $(NAME)

$(NAME): $(PHILO_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	$(RM) $(OBJS) $(PHILO_OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all
