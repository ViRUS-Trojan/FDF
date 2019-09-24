/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdelsie <vdelsie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 17:33:15 by vdelsie           #+#    #+#             */
/*   Updated: 2019/09/11 17:36:39 by vdelsie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <libft.h>
#include "fdf.h"

/*
** init_img - используйте функции mlx для инициализации нашего изображения, которым мы будем
** рисуя наши карты!
**
** Начальные биты на пиксель (bpp), которые возвращает функция mlx_get_data_addr
** составляет 32 бита. Потому что мы имеем дело с цветами RGB, которые максимально
** 0xFF (255) на канал, мы делим bpp на 8 бит (nbr бит на канал)
** так что наша адресация сработает.
*/

void		init_img(t_envars *env)
{
	t_image	*img;

	img = &env->image;
	img->image = mlx_new_image(env->mlx, WIN_W, WIN_H);
	img->ptr = mlx_get_data_addr(img->image, &img->bpp, &img->line_s,
								&img->endian);
	img->bpp /= 8;
}

void		img_pixel_put(t_image *img, double x, double y, int color)
{
	if (x >= 0 && x < WIN_W && y >= 0 && y < WIN_H)
		*(int *)(img->ptr + (int)(idx(y, x, WIN_W) * img->bpp)) = color;
}

void		clear_img(t_image *img)
{
	ft_bzero(img->ptr, WIN_W * WIN_H * img->bpp);
}

/*
** Инициализируйте или повторно инициализируйте наш z-буфер.
** Наиболее отрицательным int является самый дальний в направлении оси z
** быть, если мы используем целые числа для представления нашего z.
*/

void		clear_zbuf(t_map *map)
{
	int len;

	len = (int)(WIN_H * WIN_W);
	while (--len > -1)
		map->zbuf[len] = -2147483648;
}
