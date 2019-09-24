# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vdelsie <vdelsie@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/08/31 20:43:52 by vdelsie           #+#    #+#              #
#    Updated: 2019/09/11 18:41:03 by vdelsie          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

MLX_BASEDIR = ./minilibx/

#определить ОС, чтобы определить версию mlx для использования ...
TARGETOS := $(shell uname -s)

ifeq ($(TARGETOS), Darwin)
	OSXRELEASE := $(shell uname -r | sed 's/\..*//')
	ifeq ($(OSXRELEASE), 17)
		OSXVER = highsierra
		MLX = $(addprefix $(MLX_BASEDIR), minilibx_macos_sierra)
	endif
	ifeq ($(OSXRELEASE), 16)
		OSXVER = sierra
		MLX = $(addprefix $(MLX_BASEDIR), $(join minilibx_macos_, $(OSXVER)))
	endif
	ifeq ($(OSXRELEASE), 15)
		OSXVER = elcapitan
		MLX = $(addprefix $(MLX_BASEDIR), $(join minilibx_macos_, $(OSXVER)))
	endif
	ifeq ($(OSXRELEASE), 14)
		OSXVER = yosemite
	endif
	ifeq ($(OSXRELEASE), 13)
		OSXVER = maverick
	endif
	ifeq ($(OSXRELEASE), 12)
		OSXVER = mountainlion
	endif
	ifeq ($(OSXRELEASE), 11)
		OSXVER = lion
	endif
endif

 NAME = fdf
 SRCS_DIR = ./
 INCLUDES = ./includes/
 RM = /bin/rm -f

 FILES = prep_xform_matrices fdf_render read_utils matrix_utils image main\
	mouse_hooks color key_hooks occlusion
 CFILES = $(patsubst %, $(SRCS_DIR)%.c, $(FILES))
 OFILES = $(patsubst %, %.o, $(FILES))
 CFLAGS = -Wall -Wextra -Werror

 #mlx library
 MLX_LIB = $(addprefix $(MLX), mlx.a)
 MLX_INC = -I $(MLX)
 MLX_LINK = -L $(MLX) -l mlx -framework OpenGL -framework AppKit

 #libft
 LFT = ./libft/
 LFT_LIB = $(addprefix $(LFT), ft.a)
 LFT_INC = -I $(LFT)includes/
 LFT_LINK = -L $(LFT) -l ft

 .PHONY: all clean fclean re

 all: $(MLX_LIB) $(LFT_LIB) $(NAME)
 
 $(OFILES):
	gcc $(CFLAGS) -c -I $(INCLUDES) $(MLX_INC) $(LFT_INC) $(CFILES)
 
 $(LFT_LIB):
	make -C $(LFT) 

 $(MLX_LIB):
	make -C $(MLX) 

 $(NAME): $(OFILES)
	gcc $(CFLAGS) $(OFILES) $(LFT_LINK) -o $(NAME) $(MLX_LINK)

 clean:
	make -C $(MLX) clean
	make -C $(LFT) clean
	$(RM) $(OFILES)

 fclean: clean
	make -C $(LFT) fclean
	$(RM) $(NAME) $(OFILES)

 re: fclean all
 
# RESULT_NAME = fdf
# FLAGS = -Wall -Wextra -Werror
# INCLUDES = -I includes/
# LIBS = -L libft/ -lft -lmlx -lm
# FRAMEWORKS = -framework AppKit -framework OpenGL
# all:
# 	gcc $(FLAGS) -o $(RESULT_NAME) *.c $(LIBS) $(INCLUDES) $(FRAMEWORKS)
# fclean:
# 	/bin/rm fdf