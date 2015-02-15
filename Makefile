# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: iadjedj <iadjedj@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/01/21 13:29:21 by iadjedj           #+#    #+#              #
#    Updated: 2015/02/12 10:31:45 by iadjedj          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	propagande_img

SRCS	= 	utils.c mlx.c main.c glitch.c exp_wand.c

OBJS	=	$(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	gcc -Wall -Wextra -Werror -lmlx -L/usr/X11/lib -lXext -lX11 -o $(NAME) $(OBJS)

$(OBJS): %.o: %.c
	gcc -Wall -Wextra -Werror -I libft -L libft -lft -c $<

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
