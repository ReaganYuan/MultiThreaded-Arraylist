#include "arraylist.h"
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * @visibility HIDDEN FROM USER
 * @return     true on success, false on failure
 */

extern pthread_key_t key_removal;
int read_cnt = 0;
void* store_index;

static bool resize_al(arraylist_t* self){
    bool ret = false;
    int old_capacity = 0;
    int length = self->length;
    int capacity = self->capacity;

    if(length == capacity){
        old_capacity = self->capacity;
        self->capacity = self->capacity * 2;

        void* ptr = calloc(self->capacity,self->item_size);
        memcpy(ptr,self->base,self->item_size*old_capacity);
        self->base = ptr;
        return true;
    }
    else if(self->length   == (self->capacity/2) - 1 && (self->capacity/2) >= INIT_SZ){
        //half
        //never shrink below init_sz);
        old_capacity = self->capacity;
        self->capacity = self->capacity / 2;
        void* ptr = calloc(1,sizeof(char)*self->capacity*self->item_size);
        memcpy(ptr,self->base,self->item_size*self->capacity);
        self->base = ptr;

        return true;
    }
    return ret;
}


arraylist_t *new_al(size_t item_size){

    arraylist_t *i = calloc(1,sizeof(arraylist_t));

    if(item_size == 0){
        return NULL;
    }

    i->capacity = INIT_SZ;
    i->length = 0;
    i->item_size = item_size;
    i->base = calloc(INIT_SZ,item_size);
    i->isforeaching = calloc(256,sizeof(char));
    i->isforeaching[0] = false;

    int pshared = 0;
    int value = 1;

    sem_init(&(i->s_write), pshared, value);
    sem_init(&(i->s_read),pshared,value);
    sem_init(&(i->s_insert),pshared,value);
    sem_init(&(i->s_remove),pshared,value);


    /* error checking */
    if(i == NULL || i->base == NULL){
        errno = ENOMEM;
        return NULL;
    }

    return (void*)i;
}

size_t insert_al(arraylist_t *self, void* data){

    if(sem_wait(&(self->s_write)) == -1){
        return UINT_MAX;
    }
    if(data == NULL || self->capacity == 0){
        // errno
        sem_post(&(self->s_write));
        return UINT_MAX;
    }


    if(self->capacity == self->length){
        if(resize_al(self) == false){
            sem_post(&(self->s_write));
            return UINT_MAX;
        }
    }


    int offset = self->length * self->item_size;
    memcpy((char*)self->base + offset,data,self->item_size);
    self->length = self->length + 1;
    sem_post(&(self->s_write));

    return self->length - 1;
}

size_t get_data_al(arraylist_t *self, void *data){

    if(sem_wait(&(self->s_read)) == -1){
        return UINT_MAX;
    }
    read_cnt++;
    if(read_cnt == 1){
        sem_wait(&(self->s_write));
    }
    sem_post(&(self->s_read));


    if(data == NULL || self->capacity == 0){
        errno = EINVAL;
        sem_wait(&(self->s_read));
        read_cnt--;
        if(read_cnt == 0){
            sem_post(&(self->s_write));
        }
        sem_post(&(self->s_read));

        return 0;
    }


    if(self->length == 0){
        sem_wait(&(self->s_read));
        read_cnt--;
        if(read_cnt == 0){
            sem_post(&(self->s_write));
        }
        sem_post(&(self->s_read));

        return UINT_MAX;
    }

    void* al_iterator = self->base;

    for(int i = 0 ; i < self->length*self->item_size; i = i + self->item_size){
        if(memcmp(al_iterator,data,self->item_size) == 0 ){
            sem_wait(&(self->s_read));
            read_cnt--;
            if(read_cnt == 0){
                sem_post(&(self->s_write));
            }
            sem_post(&(self->s_read));

            return i/self->item_size;
        }
        al_iterator = (char*)al_iterator + self->item_size;
    }
        sem_wait(&(self->s_read));
        read_cnt--;
        if(read_cnt == 0){
            sem_post(&(self->s_write));
        }
        sem_post(&(self->s_read));
    return UINT_MAX;
}
void *get_index_al(arraylist_t *self, size_t index){
    void *ret = calloc(1,self->item_size);
 
    if(sem_wait(&(self->s_read))==-1){
        return NULL;
    }
    read_cnt++;
    if(read_cnt == 1){
        sem_wait(&(self->s_write));
    }
    sem_post(&(self->s_read));

    if(self->base == NULL  || self == NULL || self->length == 0 || self->capacity == 0){
        // errno = ??
        sem_wait(&(self->s_read));
        read_cnt--;
        if(read_cnt == 0){
            sem_post(&(self->s_write));
        }
        sem_post(&(self->s_read));

        return NULL;
    }
    if(index >= self->length){
        memcpy(ret,(char*)self->base + (self->length-1)*self->item_size ,self->item_size);

        sem_wait(&(self->s_read));
        read_cnt--;
        if(read_cnt == 0){
            sem_post(&(self->s_write));
        }
        sem_post(&(self->s_read));

        return ret;
    }
    memcpy(ret,(char*)self->base + (index  *self->item_size),self->item_size);

    sem_wait(&(self->s_read));
    read_cnt--;
    if(read_cnt == 0){
        sem_post(&(self->s_write));
    }
    sem_post(&(self->s_read));
    return ret;
}

bool remove_data_al(arraylist_t *self, void *data){

    if(sem_wait(&(self->s_write))==-1){
        return false;
    }
    if(self->length == 0 || self == NULL || self->base == NULL || self->capacity == 0){
        return false;
    }


    // sem_wait(&(self->s_write));
    // self->isforeaching = pthread_getspecific(key_removal);
    if(self->x == true){
        sem_post(&(self->s_write));
        return false;
    }

    if(data == NULL){
        memcpy((char*)self->base,(char*)self->base + self->item_size, self->item_size*self->length);
        self->length--;
        if(self->length == (self->capacity/2) - 1 && self->capacity/2 >= INIT_SZ){
            resize_al(self);
        }
        sem_post(&(self->s_write));
        return true;
    }

    int max_size = (self->length) * self->item_size;
    for(int i = 0 ; i < (self->length)*self->item_size ; i += self->item_size){
        if(memcmp((char*)self->base + i , data , self->item_size) == 0){
            memcpy((char*)self->base + i , (char*)self->base + i + self->item_size, max_size - i - self->item_size);
            self->length--;
            if(self->length == (self->capacity/2) - 1 && self->capacity/2 >= INIT_SZ){
                resize_al(self);
            }
            sem_post(&(self->s_write));
            return true;
        }
    }
    sem_post(&(self->s_write));
    return false;
}
void *remove_index_al(arraylist_t *self, size_t index){


    if(sem_wait(&(self->s_write))==-1){
        return NULL;
    }
    void *ret = calloc(1,self->item_size);
    // self->isforeaching = pthread_getspecific(key_removal);
    if( self->length == 0 || self == NULL || self->base == NULL || self->capacity == 0){
        sem_post(&(self->s_write));
        return NULL;
    }

    if(self->x == true){
        sem_post(&(self->s_write));
        return NULL;
    }

    int max_size = self->length;
    int original_length = self->length;

    if(index >= original_length){
        //  remove last item in the arraylist
        memcpy(ret,(char*)self->base + self->capacity*self->item_size, self->item_size);
        self->length = self->length - 1;
        if(self->length  == (self->capacity/2) - 1){
            resize_al(self);
        }
        sem_post(&(self->s_write));
        return ret;
    }

    memcpy(ret,(char*)self->base + (index*self->item_size),self->item_size);
    for(int i = 0 ; i < max_size - index - 1 ; i++){
        memmove((char*)self->base + (index + i)  * self->item_size,
            (char*)self->base + (index + i + 1) * self->item_size,
            self->item_size);
    }
    self->length = self->length - 1;
    if(self->length  == (self->capacity/2) - 1){
        resize_al(self);
    }

    sem_post(&(self->s_write));
    return ret;
}

void delete_al(arraylist_t *self, void (*free_item_func)(void*)){
    sem_wait(&(self->s_write));

    //already deleted
    if(self->capacity == 0 ){
        return;
    }

    // check if free_item_func is NULL
    if(free_item_func != NULL){
        for(int i = 0 ;i < self->length ; i++){
            free_item_func((char*)self->base + (i * self->item_size));
        }
    }
    // set arraylist struct values
    self->capacity = 0;
    self->length = 0;
    self->item_size = 0;
    sem_post(&(self->s_write));
    return;
}
