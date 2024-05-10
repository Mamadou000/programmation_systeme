//mini_memory.c

#include <unistd.h>
//#include <sys/types.h>
#include "mini_lib.h"
#include <errno.h>
#include <stdio.h>
 

malloc_element* malloc_list = NULL;



char * mini_calloc(int size_element,int number_element)
{
 int size_alocated = size_element*number_element;
 

 if(malloc_list==NULL) 
 {
   void * p ;
   if((p = sbrk(size_alocated))==(void *)-1)
   { 
     mini_perror("sbrk");
     return NULL;
   }
  char *t = (char *)p ;//on caste le type vu qu'on va pointer sur des char
  int i;
 for(i=0;i<size_alocated;i++) t[i]='\0';
malloc_element *new_element;
 new_element = sbrk(sizeof(malloc_element));
 new_element->zone = p;
 new_element->size = size_alocated;
 new_element->statut = 1;
 new_element->next_malloc = NULL;
  malloc_list=new_element;
 return p;
 }


 else{
   malloc_element *current_element=malloc_list;
 
   while(current_element)
   {
     if(current_element->statut==0 && current_element->size>=size_alocated)
     {
     
       current_element->statut=1;
       return current_element->zone;
       
     } 
     if(current_element->next_malloc==NULL) break;  
     
     current_element=current_element->next_malloc;
    
   }
   void * p ;
   if((p = sbrk(size_alocated))==(void *)-1)
   { 
     mini_perror("sbrk");
     return NULL;
   }
  char *t = (char *)p ;//on caste le type vu qu'on va pointer sur des char
  int i;
 for(i=0;i<size_alocated;i++) t[i]='\0';
malloc_element *new_element1;
 new_element1 = sbrk(sizeof(malloc_element));
 new_element1->zone = p;
 new_element1->size = size_alocated;
 new_element1->statut = 1;
 new_element1->next_malloc = NULL;
current_element->next_malloc=new_element1;
   return p;
    
 }
 
}

void print_list()
{
  while(malloc_list->next_malloc!=NULL) 
  {
    printf(" statut %i, zone %p\n", malloc_list->statut,malloc_list->zone);
    malloc_list = malloc_list->next_malloc;
  }
  
}



void mini_free(void* ptr)
{
  malloc_element *current_el=malloc_list;
   while (current_el)
   {
     if(current_el->zone==ptr)
     {
        current_el->statut=0;
        break;
     } 
     if(current_el->next_malloc==NULL) break;

     current_el=current_el->next_malloc;
   }
 
}


void mini_exit()
{
  //vidage_buffer();
 flush_all();
 _exit(0);
}