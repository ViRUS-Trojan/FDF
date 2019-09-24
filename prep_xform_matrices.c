/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep_xform_matrices.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdelsie <vdelsie@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/08 21:01:57 by vdelsie           #+#    #+#             */
/*   Updated: 2019/09/08 21:14:37 by vdelsie          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <libft.h>
#include "fdf.h"

/*
** Матрица перевода принимает следующую форму:
** 1 0 0 vx
** 0 1 0 vy
** 0 0 1 vz
** 0 0 0 1
**
** vx, vy, vz представляют переводы по осям x, y и z соответственно
*/

t_trmat	trans_mat(double x_shift, double y_shift, double z_shift)
{
	t_trmat	trans_mat;

	trans_mat.rows = 4;
	trans_mat.cols = 4;
	ft_bzero(trans_mat.m, sizeof(trans_mat.m));
	trans_mat.m[idx(0, 0, 4)] = 1;
	trans_mat.m[idx(1, 1, 4)] = 1;
	trans_mat.m[idx(2, 2, 4)] = 1;
	trans_mat.m[idx(0, 3, 4)] = x_shift;
	trans_mat.m[idx(1, 3, 4)] = y_shift;
	trans_mat.m[idx(2, 3, 4)] = z_shift;
	trans_mat.m[idx(3, 3, 4)] = 1;
	return (trans_mat);
}

/*
** Вращение вокруг оси X:
** 1 0 0 0
** 0 cos -sin 0
** 0 sin cos 0
** 0 0 0 1
**
** Примечание: для защиты от вращения переключите sin и -sin
** Примечание 2: тета измеряется в радианах
*/

t_trmat	x_rot_mat(double theta, int transpose)
{
	t_trmat trans_mat;

	trans_mat.rows = 4;
	trans_mat.cols = 4;
	ft_bzero(trans_mat.m, sizeof(trans_mat.m));
	trans_mat.m[idx(0, 0, 4)] = 1;
	trans_mat.m[idx(1, 1, 4)] = cos(theta);
	trans_mat.m[idx(1, 2, 4)] = (transpose) ? -sin(theta) : sin(theta);
	trans_mat.m[idx(2, 1, 4)] = (transpose) ? sin(theta) : -sin(theta);
	trans_mat.m[idx(2, 2, 4)] = cos(theta);
	trans_mat.m[idx(3, 3, 4)] = 1;
	return (trans_mat);
}

/*
** Вращение вокруг оси Y:
** cos	 0  	sin		0
**  0	 1  	 0		0
** -sin  0  	cos		0
**  0	 0  	 0		1
**
** Примечание: для защиты от вращения переключите sin и -sin
** Примечание 2: тета измеряется в радианах
*/

t_trmat	y_rot_mat(double theta, int transpose)
{
	t_trmat	trans_mat;

	trans_mat.rows = 4;
	trans_mat.cols = 4;
	ft_bzero(trans_mat.m, sizeof(trans_mat.m));
	trans_mat.m[idx(0, 0, 4)] = cos(theta);
	trans_mat.m[idx(0, 2, 4)] = (transpose) ? sin(theta) : -sin(theta);
	trans_mat.m[idx(1, 1, 4)] = 1;
	trans_mat.m[idx(2, 0, 4)] = (transpose) ? -sin(theta) : sin(theta);
	trans_mat.m[idx(2, 2, 4)] = cos(theta);
	trans_mat.m[idx(3, 3, 4)] = 1;
	return (trans_mat);
}

/*
** Вращение вокруг оси Z:
** cos	 -sin	 0	 0
** sin	  cos	 0	 0
**  0	   0 	 1	 0
**  0	   0	 0	 1
**
** Примечание: для защиты от вращения переключите sin и -sin
** Примечание 2: тета измеряется в радианах
*/

t_trmat	z_rot_mat(double theta, int transpose)
{
	t_trmat	trans_mat;

	trans_mat.rows = 4;
	trans_mat.cols = 4;
	ft_bzero(trans_mat.m, sizeof(trans_mat.m));
	trans_mat.m[idx(0, 0, 4)] = cos(theta);
	trans_mat.m[idx(0, 1, 4)] = (transpose) ? -sin(theta) : sin(theta);
	trans_mat.m[idx(1, 0, 4)] = (transpose) ? sin(theta) : -sin(theta);
	trans_mat.m[idx(1, 1, 4)] = cos(theta);
	trans_mat.m[idx(2, 2, 4)] = 1;
	trans_mat.m[idx(3, 3, 4)] = 1;
	return (trans_mat);
}

/*
** Масштабная матрица:
** Sx 0 0 0
** 0 Sy 0 0
** 0  0 0 Sz
** 0  0 0 1
**
** Sx, Sy, Sz представляют масштабирование, которое будет выполнено для x, y и z
*/

t_trmat	scale_mat(double x_scale, double y_scale, double z_scale)
{
	t_trmat trans_mat;

	trans_mat.rows = 4;
	trans_mat.cols = 4;
	ft_bzero(trans_mat.m, sizeof(trans_mat.m));
	trans_mat.m[idx(0, 0, 4)] = x_scale;
	trans_mat.m[idx(1, 1, 4)] = y_scale;
	trans_mat.m[idx(2, 2, 4)] = z_scale;
	trans_mat.m[idx(3, 3, 4)] = 1;
	return (trans_mat);
}
