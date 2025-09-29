/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcoga2 <marcoga2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 12:15:33 by jrollon-          #+#    #+#             */
/*   Updated: 2025/09/29 17:00:07 by marcoga2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

//norminette cub3d.h floor_ceiling.c  get_next_line.h libft map_3.c  map_bonus.c raycast_2.c rotation.c textures door_bonus.c frame_bonus.c    get_next_line_utils.c  main.c map_4.c  map.c minimap_bonus.c  raycast.c sprite_bonus.c  window.c enemy_bonus2.c  frame.c jump.c  map_5.c  movement.c raycast_door_2.c  sprite.c enemy_bonus.c   get_next_line.c  keys.c map_2.c   map_6.c  menu_keys.c  pixels.c raycast_door.c sprite_draw.c
void	error_window(t_game *game, int option)
{
	if (option == 0)
		perror("Error creating window");
	else
		perror("Error creating canvas window");
	clean_up_memory(game, 0);
	exit (1);
}

int	close_win(t_game *game)
{
	clean_up_memory(game, 0);
	exit (0);
}

void	draw_window(t_game *game)
{
	game->win->width = WIN_W;
	game->win->height = WIN_H;
	if (!game->win->win)
	{
		game->win->win = mlx_new_window(game->win->mlx, WIN_W, WIN_H, "Cub3D");
		if (!game->win->win)
			error_window(game, 0);
	}
	game->win->canvas.img = mlx_new_image(game->win->mlx, WIN_W, WIN_H);
	if (!game->win->canvas.img)
		error_window(game, 1);
	game->win->canvas.addr = mlx_get_data_addr(game->win->canvas.img,
			&game->win->canvas.bits_x_pixel,
			&game->win->canvas.line_length,
			&game->win->canvas.endian);
	if (!game->win->canvas.addr)
		error_window(game, 1);
	game->win->ray.fov_factor = ((FOV * PI / 180.0) / 2);
	if (game->player.mouse_control)
		game->win->ray.rotation_speed = (2 * PI * ROTATION_SPEED / 360);
	else
		game->win->ray.rotation_speed = (4 * PI * ROTATION_SPEED / 360);
}
