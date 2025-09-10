/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_draw.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrollon- <jrollon-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 13:56:38 by jrollon-          #+#    #+#             */
/*   Updated: 2025/09/10 18:42:43 by jrollon-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_enemy_on_canvas(t_game *game, t_sprite sprite, int px, int py)
{
	int		i;
	int		j;
	int		color;
	char	*src;
	
	j = 0;
	while (j < game->enemy.sprite_size)
	{
		i = 0;
		while (i < game->enemy.sprite_size)
		{
			src = sprite.img[0].addr + (j * sprite.img[0].line_length
					+ i * (sprite.img[0].bits_x_pixel / 8));
			color = *(unsigned int *)src;
			if (color != 0x00F8FF00 && px + i >= 0 && px + i < WIN_W && py + j >= 0 && py + j < WIN_H)
				put_pixel(&game->win->canvas, px + i, py + j, color);
			i++;
		}
		j++;
	}
}

void	draw_minisprite_on_canvas(t_game *game, t_sprite sprite, int x, int y)
{
	size_t	i;
	size_t	j;
	int		color;
	char	*src;

	j = 0;
	while (j < MINISPRITE)
	{
		i = 0;
		while (i < MINISPRITE)
		{
			src = sprite.img[0].addr + (j * sprite.img[0].line_length
					+ i * (sprite.img[0].bits_x_pixel / 8));
			color = *(unsigned int *)src;
			put_pixel(&game->win->canvas, x + i, y + j, color);
			i++;
		}
		j++;
	}
}
