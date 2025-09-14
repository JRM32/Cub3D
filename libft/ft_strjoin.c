/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrollon- <jrollon-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 13:17:41 by jrollon-          #+#    #+#             */
/*   Updated: 2025/01/19 09:56:21 by jrollon-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*Joins both s1 first and s2 in a new string with memory allocation*/
/*if Mem alloc is not success then return NULL*/
char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	s1_len;
	size_t	s2_len;
	char	*dest;
	size_t	i;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	dest = malloc(s1_len + s2_len + 1);
	if (!dest)
		return (NULL);
	for (i = 0; i < s1_len; i++)
		dest[i] = s1[i];
	for (i = 0; i < s2_len; i++)
		dest[s1_len + i] = s2[i];
	dest[s1_len + s2_len] = '\0';
	return (dest);
}
