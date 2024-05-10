#include <unistd.h>
// #include <sys/types.h>
#include "mini_lib.h"
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>

void mini_touch(char *file)
{
    // nom vide
    if (mini_strlen(file) == 0)
        return;
    else
    {
        MYFILE *f = mini_fopen(file, 'w');
        if (f == NULL)
            mini_perror("mini_fopen()");
        else
            mini_fclose(f);
    }
}

int sizefile(char *f)
{
    struct stat st;
    if (stat(f, &st) == 0)
        return st.st_size;
    else
        return -1;
}

void mini_cp(char *src, char *dest)
{
    if (src == NULL || dest == NULL)
        return;

    else
    {

        MYFILE *source = mini_fopen(src, 'r');       // read
        MYFILE *destination = mini_fopen(dest, 'w'); // write
        if (source == NULL || destination == NULL)
            mini_perror("mini_fopen()");
        char *buffer[BUF_SIZE];
        int i = 0;
        int taille_source = sizefile(src);
        i = mini_fread(buffer, 1, taille_source - 1, source);  
        mini_fwrite(buffer, 1, i - 1, destination);

        mini_fclose(source);
        mini_fclose(destination);
    }
}

void mini_echo(char *chaine)
{

    if (chaine == NULL)
        return;
    else
    {
        mini_printf(chaine);
    }
}

void mini_cat(char *file)
{
    if (file == NULL)
        return;
    else
    {
        MYFILE *f = mini_fopen(file, 'r');
        if (f == NULL)
            mini_perror("mini_fopen()");
        int taille_file = sizefile(file);
        char bufferu[BUF_SIZE];
        int j = 0;
        j = mini_fread(bufferu, 1, sizeof(bufferu), f);
        mini_printf(bufferu);

        mini_fclose(f);
    }
}

void mini_head(char *file, int n)
{
    if (file == NULL || n < 0)
        return;
    else
    {
        MYFILE *fu = mini_fopen(file, 'r');
        if (fu == NULL)
            mini_perror("mini_fopen()");
        char buff[BUF_SIZE];
        int k = 0;

        k = mini_fread(buff, sizeof(char), sizefile(file) - 1, fu);

        // verifions le nombre de lignes dans le fichier
        int cmp = 0;
        for (int i = 0; i < sizefile(file); i++)
        {
            if (buff[i] == '\n')
                cmp++;
        }

        char newbuff[BUF_SIZE];
        int no = 0;
        // si le nb ligne demandé est superieur au nb lignes du fichier
        if (n > cmp)
            mini_printf(buff);
        else
        {

            for (int m = 0; m < sizefile(file); m++)
            {
                newbuff[m] = buff[m];
                if (buff[m] == '\n')
                    no++;
                if (no == n)
                    break;
            }
            mini_printf(newbuff);
        }
    }
}

void mini_tail(char *nfile, int N)
{
    char buffer[BUF_SIZE];

    if (nfile == NULL || mini_strlen(nfile) == 0)
    {
        mini_perror("Nom de fichier incorrect. \n");
        return;
    }

    MYFILE *file = mini_fopen(nfile, 'r');
    if (file == NULL)
    {
        
            mini_perror("mini_fopen() ");
        return;
    }

    int nb = mini_fread(buffer, 1, sizeof(buffer), file);

    // Si le nombre de ligne est <= 0, dans ce cas on affiche rien
    if (N <= 0)
    {
        mini_printf("\n");
    }

    else if (N >= nb_ligne(nfile))
    {
        mini_printf(buffer);
        mini_printf("\n");
    }

    else
    {
        int cmpt = 0;
        int last_newline_index = -1;

        for (int i = nb - 1; i >= 0; i--)
        {
            if (buffer[i] == '\n')
            {
                cmpt++;
                if (cmpt == N)
                {
                    last_newline_index = i;
                    break;
                }
            }
        }

        if (last_newline_index != -1)
        {
            mini_printf(&buffer[last_newline_index + 1]);
        }
        else
        {

            mini_printf(buffer);
        }
        mini_printf("\n");
    }

    mini_fclose(file);
}

void mini_clean(char *file)
{
    if (file == NULL)
        return;
    else
    {
        MYFILE *f = mini_fopen(file, 'w');
        if (f == NULL)
            mini_perror("mini_fopen()");
        mini_fclose(f);
    }
}

void mini_grep(char *file, char *mot)
{
    if (file == NULL || mot == NULL)
        return;

    MYFILE *fe = mini_fopen(file, 'r');
    if (fe == NULL)
        mini_perror("mini_fopen()");

    char boof[BUF_SIZE];

    int k = mini_fread(boof, sizeof(char), sizefile(file), fe);

    char tmp[BUF_SIZE];
    int j = 0, no = 0, noo = 0, x = 0, i, l;
    for (i = 0; i < sizefile(file); i++)
    {
        if (boof[i] == '\n')
            no++;
        if (boof[i] == mot[j])
            j++;
        else
            j = 0;

        if (j == mini_strlen(mot))
        {

            for (l = 0; l < sizefile(file); l++)
            {
                if (boof[l] == '\n')
                    noo++;
                if (noo == no && boof[l] != '\n')
                {
                    tmp[x++] = boof[l];
                    // if (boof[l]=='\n') break; //prochaine ligne
                }
            }
            tmp[x++] = '\n';
            noo = 0;
            j = 0;
            l = 0;
        }
    }

    mini_printf(tmp);
}

void mini_wc(char *file)
{
    if (file == NULL)
        return;

    MYFILE *fu = mini_fopen(file, 'r');
    if (fu == NULL)
    {
        mini_perror("mini_fopen()");
        return;
    }

    char boofer[BUF_SIZE];
    int k = mini_fread(boofer, sizeof(char), sizefile(file), fu);

    int i = 0;
    int cpt = 0;
    int inWord = 0; // Flag pour indiquer si nous sommes à l'intérieur d'un mot

    while (boofer[i] != '\0')
    {
        // Si le caractère courant est un espace, mettez à jour le drapeau inWord
        if (boofer[i] == ' ' || boofer[i] == '\t' || boofer[i] == '\n' || boofer[i] == '\r')
        {
            inWord = 0;
        }
        else
        {
            // Si le caractère courant n'est pas un espace et inWord est à 0, commencez un nouveau mot
            if (!inWord)
            {
                cpt++;
                inWord = 1; // Mettez à jour le drapeau inWord
            }
        }
        i++;
    }

    mini_fclose(fu);

    printf("%d\n", cpt);
}

int nb_ligne(char *nfile)
{
    char buffer[BUF_SIZE];

    // Vérifie si le nom de fichier est valide
    if (nfile == NULL || mini_strlen(nfile) == 0)
    {
        mini_perror("Nom de fichier incorrect. \n");
        return -1;
    }

    // Ouvre le fichier en mode lecture
    MYFILE *file = mini_fopen(nfile, 'r');
    if (file == NULL)
    {
        // En cas d'erreur d'ouverture, affiche un message d'erreur et quitte la fonction
        mini_perror("mini_fopen() ");
        return -1;
    }

    // Lit les données du fichier pour les mettre dans le buffer
    int nb = mini_fread(buffer, 1, sizeof(buffer), file);
    int nb_ligne = 1;
    // Parcourt le buffer pour compter le nombre de lignes
    for (int i = 0; i < nb; i++)
    {
        if (buffer[i] == '\n')
        {
            nb_ligne++;
        }
    }

    // Ferme le fichier
    mini_fclose(file);

    return nb_ligne;
}