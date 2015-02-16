# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: iadjedj <iadjedj@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/01/21 13:29:21 by iadjedj           #+#    #+#              #
#    Updated: 2015/02/16 17:56:25 by iadjedj          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	propagande_img

SRCS	= 	utils.c mlx.c main.c glitch.c exp_wand.c

OBJS	=	$(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	gcc -O2 -Wall -Wextra -Werror -pthread -lmlx -L/usr/X11/lib -lXext -lX11 -o $(NAME) $(OBJS)

$(OBJS): %.o: %.c
	gcc -O2 -Wall -Wextra -Werror -I libft -pthread -L libft -lft -c $<

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
