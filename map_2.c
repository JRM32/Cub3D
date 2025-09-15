/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcoga2 <marcoga2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 15:46:31 by jrollon-          #+#    #+#             */
/*   Updated: 2025/09/15 16:55:58 by marcoga2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "get_next_line.h"

/* plane_x/y is a rotation vector from dirx/y 90deg or π/2 so applying the...
...matrix rotation in 2D:
R(a) = ⎡ cos(θ)     -sin(θ)⎤ * 	⎡x⎤ -> ⎡A B⎤ * ⎡E⎤ = ⎡AE + BF⎤
		⎣ sin(θ)      cos(θ)⎦	⎣y⎦ -> ⎣C D⎦   ⎣F⎦   ⎣CE + DF⎦
so...
	x' = x * cos(θ) + (y * -sin(θ))
	y' = x * sin(θ) + y * cos(θ)
as
	cos (π/2) = 0 // sin(π/2) = 1
ends
	x' = -y
	y' = x
We multiply by fov_factor to control the length of the plane vector,
which determines the field of view (FOV) width.*/
void	init_looking_direction(t_map *map, char c)
{
	double	fov_factor;

	fov_factor = tan((FOV * PI / 180.0) / 2);
	if (c == 'N')
		map->dir_y = -1;
	else if (c == 'S')
		map->dir_y = 1;
	else if (c == 'E')
		map->dir_x = 1;
	else if (c == 'W')
		map->dir_x = -1;
	map->plane_x = -map->dir_y * fov_factor;
	map->plane_y = map->dir_x * fov_factor;
}

/*
** Libera array bidimensional (seguro con NULL)
*/
void	free_2d_array(char **arr, size_t size)
{
	size_t	i;

	i = 0;
	if (!arr)
		return ;
	while (i < size)
	{
		free(arr[i]);
		arr[i] = NULL;
		i++;
	}
	free(arr);
}

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

int	blank_line(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (!ft_isspace(s[i]))
			return (0);
		i++;
	}
	return (1);
}

int	contains_invalid_char(char *str, char *valid)
{
	int	i;
	int	j;
	int	found;

	i = 0;
	if (!str || blank_line(str))
		return (1);
	while (str[i] != '\0')
	{
		found = 0;
		j = 0;
		while (valid[j] != '\0')
		{
			if (str[i] == valid[j])
			{
				found = 1;
				break ;
			}
			j++;
		}
		if (!found)
			return (1);
		i++;
	}
	return (0);
}

void	save_color_in(char *s, int *buf, int *count)
{
	int	r;
	int	g;
	int	b;
	int	i;

	r = 0;
	g = 0;
	b = 0;
	i = 0;
	while (ft_isspace(s[i]))
		i++;
	r = ft_atoi(&s[i]);
	while (s[i] && s[i] != ',')
		i++;
	while (ft_isspace(s[i]) || s[i] == ',')
		i++;
	g = ft_atoi(&s[i]);
	while (s[i] && s[i] != ',')
		i++;
	while (ft_isspace(s[i]) || s[i] == ',')
		i++;
	b = ft_atoi(&s[i]);
	*buf = (r << 16) | (g << 8) | b;
	*count += i - 1;
}

void	save_texture_in(char *s, char **buf, int *count)
{
	int	i;
	int	j;

	i = 0;
	if (!s || !buf)
		return ;
	while (ft_isspace(s[i]))
		i++;
	j = ft_strlen(s + i) - 1;
	while (j >= 0 && ft_isspace(s[i + j]))
		j--;
	if (*buf != NULL)
		free(*buf);
	*buf = ft_strndup(s + i, j + 1);
	*count += i + j + 1;
}

/*
** Pone count espacios en un string nuevo y lo devuelve
*/
char	*make_padding(size_t count)
{
	char	*padding;
	size_t	i;

	padding = NULL;
	i = 0;
	padding = malloc(count + 1);
	if (!padding)
		return (NULL);
	while (i < count)
	{
		padding[i] = ' ';
		i++;
	}
	padding[i] = '\0';
	return (padding);
}

/*
** Hacw join de la línea y el padding y libera
*/
char	*join_and_replace(char *line, char *padding)
{
	char	*result;
	size_t	len;
	size_t	pad_len;

	result = NULL;
	len = ft_strlen(line);
	pad_len = ft_strlen(padding);
	result = malloc(len + pad_len + 1);
	if (!result)
	{
		free(line);
		free(padding);
		return (NULL);
	}
	ft_strcpy(result, line);
	ft_strcat(result, padding);
	free(line);
	free(padding);
	return (result);
}

/*
** Hace cuadrado todo el m apa
*/
static void	remove_newline_and_truncate(char **row)
{
	size_t len = ft_strlen(*row);
	if (len > 0 && (*row)[len - 1] == '\n')
		(*row)[len - 1] = '\0';
}


static void	pad_row_with_zeros(char **row, size_t columns)
{
	size_t	len;
	size_t	l;
	char	*zero_buf;
	char	*tmp;

	len = ft_strlen(*row);
	if (len < columns)
	{
		l = columns - len;
		zero_buf = malloc(l + 1);
		if (!zero_buf)
			exit(EXIT_FAILURE);
		for (size_t i = 0; i < l; i++)
			zero_buf[i] = '0';
		zero_buf[l] = '\0';
		tmp = *row;
		*row = ft_strjoin(tmp, zero_buf);
		free(tmp);
		free(zero_buf);
		if (!*row)
			exit(EXIT_FAILURE);
	}
}

static void	add_newline_end(char **row)
{
	size_t	newlen;
	char	*newline_buf;

	newlen = ft_strlen(*row);
	newline_buf = malloc(newlen + 2);
	if (!newline_buf)
		exit(EXIT_FAILURE);
	ft_strcpy(newline_buf, *row);
	newline_buf[newlen] = '\n';
	newline_buf[newlen + 1] = '\0';
	free(*row);
	*row = newline_buf;
}

void	squarify_map(size_t size, t_map *map)
{
	size_t	j;

	j = 0;
	while (j < size)
	{
		remove_newline_and_truncate(&map->map[j]);
		pad_row_with_zeros(&map->map[j], map->columns);
		add_newline_end(&map->map[j]);
		j++;
	}
}

/////////////////////////////////////77
/////////////////////////////////////77
/////////////////////////////////////77
/////////////////////////////////////77
/////////////////////////////////////77
/////////////////////////////////////77
/////////////////////////////////////77
void	init_textures(t_map *map)
{
	map->NO_tex = NULL;
	map->NO_tex = NULL;
	map->NO_tex = NULL;
	map->NO_tex = NULL;
	map->floor_color = -1;
	map->sky_color = -1;
}

/*
** Carga líneas en el mapa
*/
void	load_lines(int fd, char *line, t_map *map)
{
	size_t	i;

	i = 0;
	init_textures(map);
	line = jump_to_map(fd, line, map);
	map->columns = 0;
	while (line)
	{
		map->map[i] = line;
		if (map->columns < ft_strlen(map->map[i]))
			map->columns = ft_strlen(map->map[i]);
		i++;
		line = get_next_line(fd);
		if (!line && i < map->lines)
		{
			free_2d_array(map->map, i);
			map->map = NULL;
			return ;
		}
	}
	map->map[i] = NULL;
}

int	how_many_lines(char *map_dir)
{
	char	*line;
	int		cols;
	int		fd;

	cols = 0;
	fd = open(map_dir, O_RDONLY);
	if (fd == -1)
		return (write(1, "Error loading map\n", 18));
	line = get_next_line(fd);
	while (line != NULL)
	{
		cols++;
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (cols);
}

void	load_map(t_map *map, char *map_dir)
{
	char	*line;
	int		fd;

	fd = open(map_dir, O_RDONLY);
	if (fd == -1)
	{
		printf("Error\nError loading map\n");
		return ;
	}
	line = get_next_line(fd);
	if (!line)
	{
		close(fd);
		return ;
	}
	map->map = (char **)ft_calloc(how_many_lines(map_dir) + 1, sizeof(char *));
	if (!map->map)
	{
		free(line);
		close(fd);
		return ;
	}
	load_lines(fd, line, map);
	close(fd);
}
