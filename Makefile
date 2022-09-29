# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: adinari <adinari@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/09/27 21:17:31 by adinari           #+#    #+#              #
#    Updated: 2022/09/27 21:19:39 by adinari          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex

FILES = main.c

OBJECTS = $(FILES:.c=.o)

CC = gcc

FLAGS = -Wall -Wextra -Werror

all: $(NAME)
	
%.o:%.c
	$(CC) $(FLAGS) -c $< -o $@
$(NAME): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(NAME)

clean:
	rm -rf $(OBJECTS)
	

fclean: clean
	rm -rf $(NAME) *.o

re: fclean all