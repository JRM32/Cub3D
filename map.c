/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcoga2 <marcoga2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 11:02:09 by jrollon-          #+#    #+#             */
/*   Updated: 2025/09/24 14:22:26 by marcoga2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "get_next_line.h"

void	check_internal_lines(char *line, t_map *map, size_t columns, size_t ln)
{
	size_t	i;

	i = 0;
	while (line[i])
	{
		if (i == 0 && line[0] == '1' && ft_strlen(line) == columns)
			map->num_walls++;
		if (i == (columns - 1) && line[columns - 1] == '1'
			&& ft_strlen(line) == columns)
			map->num_walls++;
		if (line[i] == 'N' || line[i] == 'S' || line[i] == 'E'
			|| line[i] == 'W')
		{
			map->num_p++;
			map->p_x = i + 0.5;
			map->p_y = (ln - 1) + 0.5;
			init_looking_direction(map, line[i]);
		}
		i++;
	}
}

void	check_line(char *line, char *next_line, t_map *map, size_t columns)
{
	size_t	i;

	i = 0;
	map->lines++;
	while (line[i])
	{
		if (!ft_strchr(VALID_MAP_CHARS, line[i]))
		{
			if (line[i] != ' ')
				map->no_valid_char = 1;
		}
		i++;
	}
	if (map->lines != 1 && next_line != NULL)
		check_internal_lines(line, map, columns, map->lines);
}

int	free_and_get_line(int *is_first_char, char **line, int fd)
{
	free(*line);
	*line = get_next_line(fd);
	*is_first_char = 1;
	return (0);
}

char	*jump_to_map(int fd, char *line, t_map *map)
{
	int	i;
	int	is_first_char;

	i = -1;
	is_first_char = 1;
	while (line != NULL && contains_invalid_char(line, VALID_MAP_CHARS))
	{
		if (line[++i] == '\0')
			i = free_and_get_line(&is_first_char, &line, fd);
		if (line[i] == 'N' && line[i + 1] == 'O' && is_first_char)
			save_texture_in(&(line[i + 2]), &(map->NO_tex), &i);
		else if (line[i] == 'S' && line[i + 1] == 'O' && is_first_char)
			save_texture_in(&(line[i + 2]), &(map->SO_tex), &i);
		else if (line[i] == 'W' && line[i + 1] == 'E' && is_first_char)
			save_texture_in(&(line[i + 2]), &(map->WE_tex), &i);
		else if (line[i] == 'E' && line[i + 1] == 'A' && is_first_char)
			save_texture_in(&(line[i + 2]), &(map->EA_tex), &i);
		else if (line[i] == 'F' && is_first_char)
			save_color_in(&(line[i + 1]), &(map->floor_color), &i);
		else if (line[i] == 'C' && is_first_char)
			save_color_in(&(line[i + 1]), &(map->sky_color), &i);
		else if (!ft_isspace(line[i]))
			is_first_char = 0;
	}
	return (line);
}

void	check_map(t_map *map)
{
	size_t	i;
	size_t	len;

	i = 0;
	map->lines = 0;
	map->columns = 0;
	while (map->map[i])
	{
		len = ft_strlen(map->map[i]);
		if (map->columns < len)
			map->columns = len;
		check_line(map->map[i], map->map[i + 1], map, map->columns);
		i++;
	}
}

t_map	*process_map(char *map_dir)
{
	t_map	*map;

	map = (t_map *)ft_calloc(1, sizeof(t_map));
	if (!map)
		return (NULL);
	load_map(map, map_dir);
	check_map(map);
	if (check_map_errors(map) || map->lines == 0)
		return (free(map), NULL);
	if (!map->map)
		return (free(map), NULL);
	if (!floodfill(map->p_y, map->p_x, map->map))
	{
		printf("Error\nMap not fully surrounded by walls\n");
		return (free(map), NULL);
	}
	free_map(map, 0);
	load_map(map, map_dir);
	if (!map->map)
		return (free(map), NULL);
	zerify_map(map);
	squarify_map(map->lines, map);
	return (map);
}
