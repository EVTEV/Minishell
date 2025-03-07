# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: acaes <acaes@student.s19.be>               +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/03 17:26:12 by acaes             #+#    #+#              #
#    Updated: 2025/01/07 18:41:20 by acaes            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
MAKEFLAGS = --no-print-directory

INC = ./inc
SRC = ./src
OBJ = ./obj

LIBFT = $(wildcard $(SRC)/libft/*.c)
PRINTF = $(wildcard $(SRC)/printf/*.c)
GNL = $(wildcard $(SRC)/gnl/*.c)
SOURCE = $(LIBFT) $(PRINTF) $(GNL)

OBJET = $(SOURCE:$(SRC)/%.c=$(OBJ)/%.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -I $(INC)
RM = @rm -rf
AR = ar -rcs

all: $(NAME)

$(NAME): $(OBJET)
	@$(AR) $(NAME) $(OBJET)

$(OBJ)/%.o: $(SRC)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
