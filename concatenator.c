/*
** concatenator.c for myirc in /home/flipper/rendu/PSU_2016_myirc/server/src/
**
** Made by Matthis Le Texier
** Login   <matthis.le-texier@epitech.eu@epitech.eu>
**
** Started on  Sun Jun  4 16:29:16 2017 Matthis Le Texier
** Last update Tue Jun  6 15:58:43 2017 gaby
*/

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "server.h"

#define GET_NEXT_LINE_ACTUAL_BUFFLEN (BUF_SIZE - 1)

char *concatenator(char *dest, char *src)
{
  if (!dest)
    return (strdup(src));
  if ((dest = realloc(dest, strlen(dest) + strlen(src) + 1)) == NULL)
    return (NULL);
  strcat(dest, src);
  return (dest);
}

static char *search_the_line(char **buff, unsigned int *currentpos)
{
  char  *tmp;
  int   i;

  i = 0;
  tmp = *buff + *currentpos;
  while (tmp[i] && tmp[i] != '\n')
    ++i;
  if ((tmp = malloc(sizeof(char) * (i + 1))) == NULL)
    return (NULL);
  memcpy(tmp, *buff + *currentpos, i);
  tmp[i] = '\0';
  if (tmp[i - 1] == '\r')
    tmp[i - 1] = '\0';
  if (!(*buff)[*currentpos + i])
      return (free(*buff), *buff = NULL, *currentpos = 0, tmp);
  *currentpos += i + 1;
  return (tmp);
}

char *get_next_line(int fd)
{
  static char *buff = NULL;
  static unsigned int currentpos = 0;
  char read_buffer[BUF_SIZE];

  memset(read_buffer, 0, BUF_SIZE);
  if (buff && strlen(buff) != 0)
    return (search_the_line(&buff, &currentpos));
  if (read(fd, read_buffer, GET_NEXT_LINE_ACTUAL_BUFFLEN) < 0)
    return (perror("read"), NULL);
  buff = concatenator(buff, read_buffer);
  if (!buff)
    return (NULL);
  return (search_the_line(&buff, &currentpos));
}

int main()
{
    char *tmp = NULL;

    int fd = open("./txt", O_RDONLY);
    if (fd <= 0)
    {
        perror("open");
        return (-1);
    }

    while ((tmp = get_next_line(fd)) != NULL)
    {
        //printf("%s\n", tmp);
        free(tmp);
    }
    close(fd);
}
