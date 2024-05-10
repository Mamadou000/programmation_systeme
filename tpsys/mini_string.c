#include <unistd.h>
// #include <sys/types.h>
#include "mini_lib.h"
#include <errno.h>
#include <stdio.h>

char *buffer;
int ind = -1;

void mini_printf(char *s)
{

   if ((buffer = mini_calloc(BUF_SIZE, 1)) == NULL)
      perror("erreur allocation");
   int i = 0;
   while (i < BUF_SIZE)
   {
      if (ind == BUF_SIZE || s[i] == '\0')
      {
         if (write(STDOUT_FILENO, buffer, ind + 1) == -1)
            perror("write");
         if (s[i] == '\n')
         {
            if (write(STDOUT_FILENO, "\n", 1) == -1)
               perror("write");
         }
         ind = -1;
         break;
      }
      ind++;
      buffer[ind] = s[i];
      i++;
   }
}

void vidage_buffer()
{
   if (buffer != NULL)
   {
      if (write(STDOUT_FILENO, buffer, BUF_SIZE + 1) == -1)
         mini_printf("write()");
      for (int x = 0; x < BUF_SIZE; x++)
         buffer[x] = '\0';
   }
}

int mini_scanf(char *buffer1, int size_buffer)
{

   if (buffer1 == NULL || size_buffer < 0)
      return -1;
   int i = 0;
   char caractere;
   while (i < size_buffer)
   {
      if (read(STDOUT_FILENO, &caractere, 1) == -1)
         mini_printf("read");

      buffer1[i] = caractere;
      i++;
      if (caractere == '\n')
         break;
   }
   buffer1[i] = '\0';
   return i;
}

int mini_strlen(char *s)
{
   int i = 0;
   while (s[i] != '\0')
      i++;
   return i;
}

int mini_strcpy(char *s, char *d)
{
   int i;
   for (i = 0; s[i] != '\0'; i++)
      d[i] = s[i];
   return i;
}

int mini_strcmp(char *s1, char *s2)
{
   int i = 0;
   while (s1[i] == s2[i] && s1[i] != '\0')
      i++;
   if (i == mini_strlen(s1))
      return 0;
   else
      return -1;
}

char *mini_itoa(int num, char *str)
{
   int i = 0;
   int sign = (num < 0) ? -1 : 1;

  
   if (num == 0)
   {
      str[i++] = '0';
      str[i] = '\0';
      return str;
   }

    
   if (sign == -1)
   {
      str[i++] = '-';
      num = -num;
   }

    
   while (num > 0)
   {
      int digit = num % 10;
      str[i++] = '0' + digit;
      num /= 10;
   }

    
   int taille = i;
   for (int j = 0; j < taille / 2; j++)
   {
      char temp = str[j];
      str[j] = str[taille - 1 - j];
      str[taille - 1 - j] = temp;
   }

   str[i] = '\0';

   return str;
}

void mini_perror(char *message)
{
   int err = errno;

   int compteur = 0;
   while (err != 0)
   {
      err = err / 10;
      compteur++;
   }
   err = errno;
   char str[compteur]; 
   mini_itoa(err, str);

   write(2, message, mini_strlen(message));
   write(2, ", code : ", mini_strlen(", code : "));
   write(2, mini_itoa(err, str), mini_strlen(mini_itoa(err, str)));

 
   write(2, "\n", 1);
   mini_exit();
}
