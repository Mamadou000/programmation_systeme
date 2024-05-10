#ifndef __mini_lib_h__
#define __mini_lib_h__

#define BUF_SIZE 2048
#include <stdio.h>
typedef struct malloc_element
{
    void *zone;
    int size;
    int statut;
    struct malloc_element *next_malloc;
} malloc_element;
typedef struct
{
    int fd;
    int ind_read;
    int ind_write;
    void *buffer_read;
    void *buffer_write;
    struct MYFILE *nextfile;
} MYFILE;

// déclaration des fonctions

char *mini_calloc(int size_element, int number_element);
void mini_free(void *ptr);
void mini_exit();
void mini_printf(char *s);
void vidage_buffer();
void print_list();
int mini_scanf(char *buffer, int size_buffer);
int mini_strlen(char *s);
int mini_strcpy(char *s, char *d);
int mini_strcmp(char *s1, char *s2);
char *mini_itoa(int num, char *str);
void mini_perror(char *message);
MYFILE *mini_fopen(char *file, char mode);
int mini_fread(void *buffer, int size_element, int number_element, MYFILE *file);
int mini_fwrite(void *buffer, int size_element, int number_element, MYFILE *file);
int mini_fflush(MYFILE *file);
void flush_all();
int mini_fclose(MYFILE *file);
int mini_fgetc(MYFILE *file);
int mini_fputc(MYFILE *file, char c);

void mini_touch(char *file);
int sizefile(char *f);
void mini_cp(char *src, char *dest);
void mini_echo(char *chaine);
void mini_cat(char *file);
int nb_ligne(char *nfile);
void mini_head(char *file, int n);
void mini_tail(char *file, int n);
void mini_clean(char *file);
void mini_grep(char *file, char *mot);
void mini_wc(char *file);

void get_cmd();     // obtient la commande de l'utilisateur
void convert_cmd(); // convetis la commande au bon format by execvp()
void c_shell();
void execute_command();
#endif