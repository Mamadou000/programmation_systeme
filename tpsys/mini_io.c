#include <unistd.h>
#include <sys/types.h>

#include <fcntl.h>
#include "mini_lib.h"
#include <errno.h>
#include <stdio.h>



int IOBUFFER_SIZE=2048;
MYFILE * listfiles=NULL;


MYFILE * mini_fopen(char *file, char mode)
{
    if(file==NULL) return NULL;
    
    MYFILE* fo;
    
    if((fo=(MYFILE *)mini_calloc(sizeof(MYFILE),1))==NULL) mini_perror("erreur alloc");

    if(mode=='r') fo->fd=open(file,O_RDONLY);
    else if(mode=='w') fo->fd=open(file,O_WRONLY |O_CREAT| O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    else if(mode=='b') fo->fd=open(file,O_RDWR);
    else if(mode=='a') fo->fd=open(file,O_APPEND);
    else mini_printf("erreur mode ou fichier iniexistant\n");
    
    if(fo->fd==-1) return NULL;
    fo->ind_read=-1;
    fo->ind_write=-1;
    fo->buffer_read=NULL;
    fo->buffer_write=NULL;
    fo->nextfile=NULL;

    if (listfiles==NULL) listfiles=fo;
    else
    {
        MYFILE* currentfile = listfiles;
        while(currentfile->nextfile!=NULL) currentfile=(MYFILE*)(currentfile->nextfile);
        currentfile->nextfile=(struct MYFILE*)fo;
    }
    return fo;
}

int mini_fread(void* buffer,int size_element,int number_element,MYFILE *file)
{
    if (file->ind_read==-1)
    {
        file->buffer_read=mini_calloc(1,IOBUFFER_SIZE);
        file->ind_read=0;
    }
     
    int i =0;
    while(i<=size_element*number_element) 
    {
        if(file->ind_read==IOBUFFER_SIZE)	file->ind_read=0;
		
		if(file->ind_read==0){
			if(read(file->fd,file->buffer_read,IOBUFFER_SIZE)==-1)
            {
                mini_perror("read()");
                return -1;
            }
                                
		}
		((char*)buffer)[i]=((char*)file->buffer_read)[file->ind_read];
		file->ind_read++;
		i++;
    }
    return i;
}  

int mini_fwrite(void* buffer,int size_element,int number_element,MYFILE* file)
{

    if(file->ind_write==-1)
    {
        file->buffer_write=mini_calloc(1,IOBUFFER_SIZE);
        file->ind_write=0;
    }
  
   
    
    int i=0;
    while(i<=size_element*number_element)
    {
        if(file->ind_write==IOBUFFER_SIZE) 
        {
          if(write(file->fd,file->buffer_write,IOBUFFER_SIZE)==-1){
            mini_perror("write()");
            return -1;
            }
           file->ind_write=0  ;                  
		}
            
        
        ((char*)file->buffer_write)[file->ind_write]=((char*)buffer)[i];
            file->ind_write++;
             i++;
    }

    return i; //caractère fin de chaine  compris

}


int mini_fflush(MYFILE* file)
{
        lseek(file->fd,-1,SEEK_CUR);
    if(write(file->fd,file->buffer_write,file->ind_write)==-1) return -1;
    int nbecrit= file->ind_write;
    file->ind_write=0;
    return nbecrit;//caractère fin de chaine  compris
}

void flush_all() {
    
    MYFILE* current = listfiles;

    
    while (current != NULL) {
        
        if (current->buffer_write != NULL && current->ind_write > 0) {
            
            mini_fflush(current);
        }
       if(current->nextfile==NULL) break;

        current =(MYFILE*) current->nextfile;
    }
}

int mini_fclose(MYFILE *file)
{
    if (file == NULL) return -1;
    mini_fflush(file);
     
     //suppresion de l'element
    MYFILE* current= listfiles;
    MYFILE* previous=NULL;
      //si element en tête de liste
      if (current == file)
      {
        listfiles=(MYFILE*)current->nextfile;
        current->nextfile = NULL;
        current = NULL;
        return close(file->fd);
      }
      // si element ailleurs dans la liste
      
    while(current)
    {
        if(current==file)
        {
            previous->nextfile= current->nextfile ;//saut
            current->nextfile=NULL;
            current=NULL;
            return close(file->fd);
        }
        previous=current;
        current=(MYFILE*)current->nextfile;
    }
    //fichier non trouvé
    return -1;
}

int mini_fgetc(MYFILE* file)
{
    char *c = mini_calloc(1,1);
    
    if(mini_fread(c,1,1,file)==-1) return -1;
    else return (int)(*c);
}

int mini_fputc(MYFILE* file,char c)
{
   return mini_fwrite(&c,1,1,file);
}
