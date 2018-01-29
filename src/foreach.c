#include "debug.h"
#include "arraylist.h"
#include "foreach.h"
#include <pthread.h>
#include <string.h>


static pthread_key_t key;
static pthread_once_t key_once = PTHREAD_ONCE_INIT;
static pthread_key_t key_removal;
static int* counter;
int rd_ct;


static void make_key(){
    (void) pthread_key_create(&key, NULL);
    (void) pthread_key_create(&key_removal,NULL);
}


void *foreach_init(arraylist_t *items){

    if(sem_wait(&(items->s_insert)) == -1){
        return NULL;
    }
    void *ret = NULL;
    if(items != NULL && items->base != NULL && items->length != 0 && items->capacity != 0){
        items->isforeaching = calloc(1,sizeof(char));
        items->isforeaching[0] = true;
        if(items->isforeaching[0] == true){
            items->x = true;
        }
        ret = calloc(1,items->item_size);
        counter = calloc(1,sizeof(void*));
        counter[0] = 0;
        (void) pthread_once(&key_once, make_key);
        (void) pthread_setspecific(key, counter);
        (void) pthread_setspecific(key_removal,items->isforeaching);
        ret = get_index_al(items,0);
    }
    sem_post(&(items->s_insert));
    return ret;
}

void *foreach_next(arraylist_t *items, void* data){

    if(sem_wait(&(items->s_insert)) == -1){
        return NULL;
    };

    counter = pthread_getspecific(key);
    items->isforeaching = pthread_getspecific(key_removal);
    void *ret = NULL;
    if(items->length == 0 || items->capacity == 0 || items->base == NULL ||
    counter[0] == items->length){
        sem_post(&(items->s_insert));
        return NULL;
    }
    if(data != NULL){
        memcpy((char*)items->base + (items->item_size*counter[0]),data,items->item_size);
    }
    if(counter[0] != items->length-1){
        ret = calloc(1,items->item_size);
        counter[0]++;
        ret = get_index_al(items,counter[0]);
    }
    else{
        items->isforeaching[0] = false;
        // items->x = false;
    }

    sem_post(&(items->s_insert));
    return ret;
}

size_t foreach_index(){
    if((counter = pthread_getspecific(key)) == NULL){
        return UINT_MAX;
    }
    return counter[0];
}

bool foreach_break_f(){
    return true;
}


int apply(arraylist_t *items, int (*application)(void*)){
    int ret = 0;
    foreach(void*,value,items){
        if(application(value) != 0){
            value = NULL;
        }

    }
    return ret;
}




