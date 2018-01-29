#include <criterion/criterion.h>
#include <criterion/logging.h>
#include <stdio.h>
#include <stdbool.h>
#include "arraylist.h"
#include <pthread.h>
#include <unistd.h>
#include "foreach.h"


#ifndef NUM_THREADS
#define NUM_THREADS 5000
#endif
#ifndef TEMP1

#define TEMP1 1000
#endif
#ifndef TEMP2
#define TEMP2 50
#endif


/******************************************
 *                  ITEMS                 *
 ******************************************/
arraylist_t *global_list;
arraylist_t *global_list2;
arraylist_t *global_list3;
arraylist_t *global_list4;
arraylist_t *global_list5;
arraylist_t *global_list6;
arraylist_t *global_list7;
arraylist_t *global_list8;
arraylist_t *foreach_list;
arraylist_t *foreach_list2;

extern int* count;

int x[500];
sem_t s;
typedef struct {
    char* name;
    int32_t id;
    double gpa;
}student_t;

typedef struct{
    int i;
    float f;
    long double ld;
    char c1:4;
    char c2:4;
    short s;
    void *some_data;
}test_item_t;


test_item_t *gt[500];

test_item_t *gtt;
arraylist_t *some_item;
/******************************************
 *              HELPER FUNCS              *
 ******************************************/
void test_item_t_free_func(void *argptr){

    test_item_t* ptr = (test_item_t*) argptr;
;
    if(ptr->some_data != NULL){
        free(ptr->some_data);
    }
    else
        cr_log_warn("%s\n", "Pointer was NULL");
}


void setup(void) {
    cr_log_warn("Setting up test");
    some_item = new_al(sizeof(test_item_t));
}

void teardown(void) {
    cr_log_error("Tearing down");

    delete_al(some_item, test_item_t_free_func);
}


void* test_insert(void* threadid)
{

    test_item_t *test = calloc(1,sizeof(test_item_t));
    test_item_t *test2 = calloc(1,sizeof(test_item_t));

    insert_al(global_list,test);
    insert_al(global_list2,test2);

    return NULL;
}

void* test_insert2(void* threadid)
{

    test_item_t *test = calloc(1,sizeof(test_item_t));
    test_item_t *test2 = calloc(1,sizeof(test_item_t));

    insert_al(global_list5,test);
    insert_al(global_list6,test2);

    return NULL;
}


void* test_insert_delete(void* threadid)
{
    test_item_t *test = calloc(1,sizeof(test_item_t));

    insert_al(global_list5,test);
    insert_al(global_list6,test);

    remove_index_al(global_list5,0);
    remove_index_al(global_list6,0);

    return NULL;
}

void* test_delete(void* threadid)
{

    remove_data_al(global_list3,gtt);
    remove_data_al(global_list4,gtt);

    return NULL;
}

void* delete_one_from_list(void *index)
{

    // sem_wait(&s);
    remove_index_al(global_list7, *(int*)index);
    // sem_post(&s);
    return NULL;
}

void* add_one_to_list()
{
    // sem_wait(&s);
    student_t *test = calloc(1, sizeof(student_t) + 1);
    insert_al(global_list7, test);
    // free(test);
    // sem_post(&s);
    return NULL;
}



// /******************************************
//  *                  TESTS                 *
//  ******************************************/


// arraylist_t *arrays[250];

// void* create_al(void * k )
// {
//     arrays[*(int*)k] = new_al(sizeof(test_item_t));
//     return NULL;
// }

// Test(al_suite, test_new_al, .timeout = 2)
// {
//     pthread_t threads[250];
//     int rc;
//     int i;

//     for(i = 0  ; i < 250; i++)
//     {
//         int* y = malloc(sizeof(int));
//         *y = i;
//         rc = pthread_create(&threads[i],NULL,create_al,(void*)y);
//         if (rc)
//         {
//             printf("ERROR; return code from pthread_create() is %d\n", rc);
//             exit(-1);
//        }
//     }

//     for(int i = 0 ; i < 250 ; i++)
//     {
//         pthread_join(threads[i],NULL);
//     }

//     for(int i = 0 ; i < 250 ; i++)
//     {
//         cr_assert(arrays[i]->length == 0);
//         cr_assert(arrays[i]->capacity == INIT_SZ );
//     }
//     for(int i = 0 ; i < 250 ; i++)
//     {
//         for(int j =  0 ; j < 250 ; j++)
//         {
//             if(i != j )
//             {
//                 cr_assert(arrays[i]->base != arrays[j]->base);
//             }
//         }
//     }

// }

// Test(al_suite, delete, .init = setup , .fini = teardown, .timeout = 2)
// {
//     arraylist_t *some_item = new_al(sizeof(test_item_t));
//     test_item_t *t = calloc(1,sizeof(test_item_t));
//     t->some_data = calloc(256,sizeof(char));
//     insert_al(some_item,t);
//     // delete_al(some_list,test_item_t_free_func);
// }

// Test(al_suite, 0_creation, .timeout=2)
// {


//     arraylist_t *locallist = new_al(sizeof(test_item_t));

//     test_item_t *k = calloc(sizeof(test_item_t),sizeof(char));
//     k->i = 10;
//     k->f = 100;

//     test_item_t *z = calloc(sizeof(test_item_t),sizeof(char));
//     z->i = 69;

//     test_item_t *j = calloc(1000,sizeof(char));

//     int x = insert_al(locallist, k);
//     void* voidptr  = (char*)locallist->base + (x*sizeof(test_item_t));
//     j = voidptr;

//     cr_assert(j->i == 10 && j->f == 100);

//     x = insert_al(locallist, z);
//     voidptr = (char*)locallist->base + (x*sizeof(test_item_t));
//     j = voidptr;
//     cr_assert(j->i == 69);

//     cr_assert_not_null(locallist, "List returned was NULL");

// }

// Test(al_suite, test1 , .timeout=2)
// {

//     arraylist_t *locallist = new_al(sizeof(test_item_t));
//     int arr[100];

//     for(int i = 0 ; i < 100 ; i++)
//     {
//         test_item_t *test = calloc(sizeof(test_item_t),sizeof(char));
//         test->i = i+100;
//         test->f = i + 1;

//         arr[i] = insert_al(locallist,test);
//     }

//     void* vp = calloc(1000,sizeof(char));
//     test_item_t *t = calloc(1000,sizeof(char));
//     for(int i = 0 ; i < 100 ; i++)
//     {
//         vp = (char*)locallist->base + arr[i]*sizeof(test_item_t);
//         // printf("%d\n",arr[i]);
//         t = vp;
//         cr_assert(t->i == i + 100);
//         cr_assert(t->f == i + 1);
//     }


//     arraylist_t *locallist2 = new_al(sizeof(student_t));
//     int arr2[10];

//     for(int i = 0 ; i < 10 ; i++)
//     {
//         student_t *student = calloc(sizeof(student_t),sizeof(char));
//         student->name = "ali";
//         student->id = 6969 + i;
//         student->gpa = 2.6 + i ;
//         arr2[i] = insert_al(locallist2,student);
//     }

//     void* vp2 = calloc(1000,sizeof(char));
//     student_t *student2 = calloc(sizeof(student_t),sizeof(char));

//     for(int i = 0 ; i < 10 ; i++)
//     {
//         vp2 = (char*)locallist2->base + arr2[i]*sizeof(student_t);
//         student2 = vp2;
//         cr_assert(strcmp(student2->name,"ali") == 0 );
//         cr_assert(student2->id == 6969 + i );
//         cr_assert(student2->gpa == 2.6 + i);
//     }

// }

// Test(al_suite, test2 , .timeout=2)
// {

//     arraylist_t *locallist = new_al(sizeof(student_t));

//     student_t *some = calloc(sizeof(student_t),sizeof(char));
//     some->name = "ali";
//     some->id = 6969;
//     some->gpa  = 2.5;
//     for(int i = 0 ; i < 10 ; i++)
//     {
//         insert_al(locallist,some);
//     }


//     student_t *t = calloc(sizeof(student_t),sizeof(char));
//     t->name = "ali";
//     t->id = 6969;
//     t->gpa = 2.6;

//     insert_al(locallist,t);

//     cr_assert(get_data_al(locallist,t)==10);
//     cr_assert(get_data_al(locallist,some)==0);

// }
// Test(al_suite, test3, .timeout=2)
// {

//     arraylist_t *locallist = new_al(sizeof(student_t));

//     student_t *s = calloc(sizeof(student_t),sizeof(char));
//     s->name = "reagan";
//     s->id = 109696144;
//     s->gpa = 4.0;

//     student_t *s2 = calloc(sizeof(student_t),sizeof(char));
//     s2->name = "brian";
//     s2->id = 12345;
//     s2->gpa = 1.0;

//     insert_al(locallist,s);
//     insert_al(locallist,s2);


//     remove_data_al(locallist,s);

//     void* vp = calloc(256,sizeof(char));
//     student_t* getdata = calloc(sizeof(student_t),sizeof(char));
//     vp = locallist->base;
//     getdata = vp;

//     cr_assert(strcmp(getdata->name,"brian") == 0 );
//     cr_assert(getdata->id == 12345);
//     cr_assert(getdata->gpa == 1.0);

//     insert_al(locallist,s);
//     int x = get_data_al(locallist,s);
//     vp = (char*)locallist->base + locallist->item_size*x;
//     getdata = vp;

//     cr_assert(strcmp(getdata->name,"reagan") == 0 );
//     cr_assert(getdata->gpa == 4.0 );
//     cr_assert(getdata->id = 109696144);
// }

// Test(al_suite, test4 , .timeout =2)
// {

//    arraylist_t *locallist1 = new_al(sizeof(student_t));
//    arraylist_t *locallist2 = new_al(sizeof(student_t));
//    arraylist_t *locallist3 = new_al(sizeof(student_t));

//    cr_assert(locallist1 != locallist2 && locallist2 != locallist3 && locallist1 != locallist3);

//    student_t *t = calloc(1,sizeof(student_t));
//    t->gpa = 2.5;
//    insert_al(locallist1,t);
//    t->gpa = 3.0;
//    insert_al(locallist2,t);
//    t->gpa = 3.5;
//    insert_al(locallist3,t);

//    t = locallist1->base;
//    cr_assert(t->gpa == 2.5);
//    t = locallist2->base;
//    cr_assert(t->gpa == 3.0);
//    t = locallist3->base;
//    cr_assert(t->gpa == 3.5);

// }

Test(al_suite, test5 , .timeout = 2)
{
    arraylist_t *locallist1 = new_al(sizeof(student_t));
    arraylist_t *locallist2 = new_al(sizeof(student_t));

    student_t *student = calloc(1,sizeof(student_t));
    student->gpa = 4.0;

    for(int i = 0 ; i < 1000 ; i++)
    {
        cr_assert(i == insert_al(locallist1,student));
        cr_assert(locallist1->length == i + 1);
        cr_assert(i == insert_al(locallist2,student));
        cr_assert(locallist2->length == i + 1);
    }

    for(int i = 0 ; i < 1000 ; i++)
    {
        cr_assert(remove_data_al(locallist1,student) == true);
        cr_assert(remove_data_al(locallist2,student) == true);
    }

    // printf("locallist1 capacity:%lu\n",locallist1->capacity);
    // printf("locallist1 length:%lu\n",locallist1->length);

    cr_assert(locallist1->capacity == 4);
    cr_assert(locallist1->length == 0);
    cr_assert(locallist2->capacity == 4);
    cr_assert(locallist2->length == 0);

}
Test(al_suite, test6 , .timeout =2)
{
    arraylist_t *list = new_al(sizeof(student_t));

    student_t *s1 = calloc(1,sizeof(student_t));
    student_t *s2 = calloc(1,sizeof(student_t));
    student_t *s3 = calloc(1,sizeof(student_t));
    student_t *s4 = calloc(1,sizeof(student_t));
    student_t *s5 = calloc(1,sizeof(student_t));

    s1->gpa = 1;
    s2->gpa = 2;
    s3->gpa = 3;
    s4->gpa = 4;
    s5->gpa = 5;

    cr_assert(0 == insert_al(list,s1));
    cr_assert(1 == insert_al(list,s2));
    cr_assert(2 == insert_al(list,s3));
    cr_assert(3 == insert_al(list,s4));
    cr_assert(4 == insert_al(list,s5));

    void* p = remove_index_al(list,1);
    student_t *test = calloc(1,sizeof(student_t));
    test = p;

    cr_assert(test->gpa == 2);

    cr_assert(UINT_MAX == get_data_al(list,s2));
    cr_assert(list->length == 4);

    p = remove_index_al(list,1);

    test = p;
    cr_assert(test->gpa == 3);
    cr_assert(UINT_MAX == get_data_al(list,s2));

    // printf("length: %lu\n",list->length);
    // printf("capacity: %lu\n",list->capacity);

    cr_assert(list->capacity == 4);
    cr_assert(list->length == 3);

    p = get_index_al(list,0);
    test = p;
    cr_assert(test->gpa == 1);

    p = get_index_al(list,2);
    test = p ;

    p = remove_index_al(list,1);
    test =p;
    cr_assert(test->gpa == 4);

    p = remove_index_al(list,1);
    test = p;
    cr_assert(test->gpa == 5);

    cr_assert(list->length == 1);
    cr_assert(list->capacity == 4);

    p = remove_index_al(list, 0);
    test = p ;
    cr_assert(test->gpa == 1);

    cr_assert(list->length == 0);

    cr_assert(0 == insert_al(list,s1));
    cr_assert(1 == insert_al(list,s2));
    cr_assert(2 == insert_al(list,s3));
    cr_assert(3 == insert_al(list,s4));
    cr_assert(4 == insert_al(list,s5));

    cr_assert(0 == get_data_al(list,s1));


}
Test(al_suite, test7, .timeout = 2)
{
    arraylist_t *locallist = new_al(sizeof(student_t));
    student_t *s1 = calloc(1,sizeof(student_t));
    student_t *s2 = calloc(1,sizeof(student_t));
    student_t *s3 = calloc(1,sizeof(student_t));
    student_t *s4 = calloc(1,sizeof(student_t));
    student_t *s5 = calloc(1,sizeof(student_t));
    s1->gpa = 0; s2->gpa = 1;s3->gpa = 2;s4->gpa = 3;s5->gpa = 4;
    insert_al(locallist,s1);
    insert_al(locallist,s2);
    insert_al(locallist,s3);
    insert_al(locallist,s4);
    insert_al(locallist,s5);

    remove_data_al(locallist,s1);
    remove_data_al(locallist,s2);

    cr_assert(locallist->capacity == 4);

}

Test(al_suite, single_thread_everything , .timeout = 2)
{

    arraylist_t *list = new_al(sizeof(test_item_t));

    char* str = "hello world";
    test_item_t *t1 = calloc(1,sizeof(test_item_t));
    t1->i = 1;
    t1->f = 2.0;
    t1->ld = 3000;
    t1->c1 = 3;
    t1->c2 = 5;
    t1->s = 6;
    t1->some_data = str;

    int offset1 = insert_al(list,t1);

    test_item_t *test = calloc(1,sizeof(test_item_t));
    void* p = (char*)list->base + (offset1 * list->item_size);
    test = p;

    cr_assert(test->i == 1);
    cr_assert(test->f == 2.0);
    cr_assert(test->ld == 3000);
    cr_assert(test->c1 == 3);
    cr_assert(test->c2 == 5);
    cr_assert(test->s == 6);
    cr_assert(strcmp(test->some_data,"hello world") == 0);

    test_item_t *t2 = calloc(1,sizeof(test_item_t));
    t2->i = 15;
    t2->f = 5;
    t2->ld = 300;
    t2->c1 = 1;
    t2->c2 = 2;
    t2->s = 11;
    t2->some_data = "cse 320";

    int offset2 = insert_al(list,t2);
    p = (char*)list->base + (offset2 * list->item_size);
    test = p;

    cr_assert(test->i == 15);
    cr_assert(test->f == 5);
    cr_assert(test->ld == 300);
    cr_assert(test->c1 == 1);
    cr_assert(test->c2 == 2);
    cr_assert(test->s == 11);
    cr_assert(strcmp(test->some_data,"cse 320") == 0);

    cr_assert(get_data_al(list,t1) == 0);
    cr_assert(get_data_al(list,t2) == 1);


    cr_assert(remove_data_al(list,t1) == true);
    cr_assert(get_data_al(list,t2) == 0);


    p = get_index_al(list,0);
    t1 = p;


    cr_assert(t1->i == 15);
    cr_assert(t1->f == 5);
    cr_assert(t1->ld == 300);
    cr_assert(t1->c1 == 1);
    cr_assert(t1->c2 == 2);
    cr_assert(t1->s == 11);

    cr_assert(list->length == 1);
    cr_assert(list->capacity == 4);

    // memcpy((void*)t1,vp,list->item_size);
    // t1 = vp;

    void* vp = get_index_al(list,0);
    test_item_t *t3 = calloc(1,sizeof(test_item_t));

    memcpy(t3,vp,list->item_size);
    // t3 = vp;

    cr_assert(t3->i == 15);
    cr_assert(t3->f == 5);
    cr_assert((int)t3->ld == 300);
    cr_assert(t3->c1 == 1);
    cr_assert(t3->c2 == 2);
    cr_assert(t3->s == 11);

    // cr_assert(0 == memcmp(vp,p,list->item_size));
}

Test(al_suite, insert_and_delete, .timeout = 2)
{

    arraylist_t *locallist = new_al(sizeof(student_t));
    student_t *s1 = calloc(1,sizeof(student_t));
    student_t *s2 = calloc(1,sizeof(student_t));
    student_t *s3 = calloc(1,sizeof(student_t));
    student_t *s4 = calloc(1,sizeof(student_t));
    student_t *s5 = calloc(1,sizeof(student_t));
    s1->gpa = 0; s2->gpa = 1;s3->gpa = 2;s4->gpa = 3;s5->gpa = 4;
    insert_al(locallist,s1);
    insert_al(locallist,s2);
    insert_al(locallist,s3);
    insert_al(locallist,s4);
    insert_al(locallist,s5);

    cr_assert(locallist->capacity == 8);
    cr_assert(locallist->length == 5);

    cr_assert(remove_data_al(locallist,s1) == true);
    cr_assert(locallist->capacity == 8);
    cr_assert(get_data_al(locallist,s2) == 0);


    cr_assert(remove_data_al(locallist,s2) == true);
    cr_assert(get_data_al(locallist,s3) == 0);


    // cr_assert(locallist->capacity == 4);

    cr_assert(remove_data_al(locallist,s3) == true);
    cr_assert(get_data_al(locallist,s4) == 0);

    cr_assert(remove_data_al(locallist,s4) == true);
    cr_assert(get_data_al(locallist,s5) == 0);
    cr_assert(remove_data_al(locallist,s5) == true);

    cr_assert(locallist->length == 0 );

}

Test(al_suite, insert_and_delete_2 , .timeout = 2)
{

    arraylist_t *locallist = new_al(sizeof(student_t));
    student_t *s1 = calloc(1,sizeof(student_t));
    student_t *s2 = calloc(1,sizeof(student_t));
    student_t *s3 = calloc(1,sizeof(student_t));
    student_t *s4 = calloc(1,sizeof(student_t));
    student_t *s5 = calloc(1,sizeof(student_t));
    s1->gpa = 0; s2->gpa = 1;s3->gpa = 2;s4->gpa = 3;s5->gpa = 4;
    insert_al(locallist,s1);
    insert_al(locallist,s2);
    insert_al(locallist,s3);
    insert_al(locallist,s4);
    insert_al(locallist,s5);

    student_t *tester = calloc(1,sizeof(student_t));
    void* vp = calloc(1,sizeof(void*));

    cr_assert(locallist->capacity == 8);
    cr_assert(locallist->length == 5);

    cr_assert(remove_data_al(locallist,s4) == true);
    cr_assert(locallist->capacity == 8);
    cr_assert(get_data_al(locallist,s5) == 3);

    vp = get_index_al(locallist,1000);
    tester = vp;
    cr_assert(tester->gpa == 4);

    vp = get_index_al(locallist,2);
    tester = vp;
    cr_assert(tester->gpa == 2);

    cr_assert(remove_data_al(locallist,s1) == true);
    cr_assert(get_data_al(locallist,s1) == UINT_MAX);

    cr_assert(remove_data_al(locallist,s2) == true);
    cr_assert(get_data_al(locallist,s3) == 0);

    cr_assert(remove_data_al(locallist,s3) == true);
    cr_assert(get_data_al(locallist,s4) == UINT_MAX);

    cr_assert(remove_data_al(locallist,s4) == false);


    cr_assert(get_data_al(locallist,s5) == 0);
    cr_assert(remove_data_al(locallist,s5) == true);

    cr_assert(locallist->length == 0 );
    cr_assert(locallist->capacity == 4);

}


Test(al_suite, multithread_test_1, .timeout = 10)
{


    pthread_t threads[3000];
    int rc;
    long i;

    global_list= new_al(sizeof(test_item_t));
    global_list2 = new_al(sizeof(test_item_t));

    for(i = 0  ; i < 3000; i++)
    {
        rc = pthread_create(&threads[i],NULL,test_insert,(void*)i);
        if (rc)
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
       }
    }

    for(int i = 0 ; i < 3000 ; i++)
    {
        pthread_join(threads[i],NULL);
    }

    cr_assert(global_list->length == 3000);
    cr_assert(global_list2->length == 3000);

}

Test(al_suite, multithread_test_2, .timeout=10)
{

    pthread_t threads[3000];
    int rc;
    long i;

    global_list3 = new_al(sizeof(test_item_t));
    global_list4 = new_al(sizeof(test_item_t));

    gtt = calloc(1,sizeof(test_item_t));
    gtt->i = 10;
    for(int i = 0 ; i < 3000 ; i++)
    {
        insert_al(global_list3,gtt);
        insert_al(global_list4,gtt);
    }

    for(i = 0  ; i < 3000; i++)
    {
        rc = pthread_create(&threads[i],NULL,test_delete,NULL);
        if (rc)
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
       }
    }

    for(int i = 0 ; i < 3000 ; i++)
    {
        pthread_join(threads[i],NULL);
    }

    // printf("%lu\n",global_list3->length);
    cr_assert(global_list3->length == 0);
    cr_assert(global_list4->length == 0);

    cr_assert(global_list3->capacity == 4);
    cr_assert(global_list4->capacity == 4);
}


Test(al_suite, simple_test, .timeout = 2)
{
    arraylist_t *t = new_al(sizeof(student_t));
    student_t *test = calloc(1,sizeof(student_t));
    test->id = 1;
    insert_al(t,test);

}

Test(al_suite, using_deleted_arraylist, .timeout = 2)
{
    arraylist_t *local = new_al(sizeof(student_t));
    student_t *s = calloc(1,sizeof(student_t));
    insert_al(local,s);
}

Test(al_suite, test_get_data, .timeout = 2)
{
    arraylist_t *l1 = new_al(sizeof(student_t));
    student_t* s_arr[1000];
    for(int i = 0 ; i < 1000 ; i++)
    {
        student_t *s = calloc(1,sizeof(student_t));
        s_arr[i] = s;
        s_arr[i]->gpa = rand();
        insert_al(l1,s_arr[i]);
    }

    for(int i = 0 ; i < 1000 ; i++)
    {
       int x =  get_data_al(l1,s_arr[i]);
       cr_assert(s_arr[x]->gpa == s_arr[i]->gpa);
    }

}

Test(al_suite, 2_insertion, .timeout=2){
    cr_assert(true, "I win");
}

Test(al_suite, 4_insert, .timeout=2 ){
    arraylist_t *locallist = new_al(sizeof(test_item_t));
    test_item_t* t1 = (test_item_t*)malloc(sizeof(test_item_t));
    t1->i = 1;
    t1->f = 2.0f;
    t1->ld = 3.14;
    t1->c1 = t1->c2 = 1;
    t1->s = 2;
    t1->some_data = locallist->base;
    insert_al(locallist, t1);
    test_item_t* t2 = (test_item_t*)malloc(sizeof(test_item_t));
    t2->i = 3;
    t2->f = 4.2f;
    t2->ld = 2.71828;
    t2->c1 = t2->c2 = 2;
    t2->s = 1;
    t2->some_data = ((char*)(locallist->base)) + locallist->length * locallist->item_size;
    insert_al(locallist, t2);
    test_item_t* t3 = (test_item_t*)malloc(sizeof(test_item_t));
    t3->i = 5;
    t3->f = 7.8f;
    t3->ld = 1.4142;
    t3->c1 = t3->c2 = 3;
    t3->s = 4;
    t3->some_data = ((char*)(locallist->base)) + locallist->length * locallist->item_size;
    insert_al(locallist, t3);
    test_item_t* t4 = (test_item_t*)malloc(sizeof(test_item_t));
    t4->i = 9;
    t4->f = 3.04f;
    t4->ld = 1.732;
    t4->c1 = t4->c2 = 4;
    t4->s = 3;
    t4->some_data = ((char*)(locallist->base)) + locallist->length * locallist->item_size;
    insert_al(locallist, t4);
    test_item_t* t5 = (test_item_t*)malloc(sizeof(test_item_t));
    t5->i = 32;
    t5->f = 84.1f;
    t5->ld = 931.02;
    t5->c1 = t5->c2 = 5;
    t5->s = 6;
    t5->some_data = ((char*)(locallist->base)) + locallist->length * locallist->item_size;



    insert_al(locallist, t5);


    test_item_t* s1 = (test_item_t*)get_index_al(locallist, 0);
    cr_assert(t1->i == s1->i, "i value did not set properly, 0");
    cr_assert(t1->f == s1->f, "f value did not set properly, 0");
    cr_assert(t1->ld == s1->ld, "ld value did not set properly, 0");
    cr_assert(t1->c1 == s1->c1, "c1 value did not set properly, 0");
    cr_assert(t1->c2 == s1->c2, "c2 value did not set properly, 0");
    cr_assert(t1->s == s1->s, "s value did not set properly, 0");
    cr_assert(t1->some_data == s1->some_data, "some_data value did not set properly, 0");

    test_item_t* s2 = (test_item_t*)get_index_al(locallist, 1);
    cr_assert(t2->i == s2->i, "i value did not set properly, 1");
    cr_assert(t2->f == s2->f, "f value did not set properly, 1");
    cr_assert(t2->ld == s2->ld, "ld value did not set properly, 1");
    cr_assert(t2->c1 == s2->c1, "c1 value did not set properly, 1");
    cr_assert(t2->c2 == s2->c2, "c2 value did not set properly, 1");
    cr_assert(t2->s == s2->s, "s value did not set properly, 1");
    cr_assert(t2->some_data == s2->some_data, "some_data value did not set properly, 1");

    test_item_t* s3 = (test_item_t*)get_index_al(locallist, 2);
    cr_assert(t3->i == s3->i, "i value did not set properly, 2");
    cr_assert(t3->f == s3->f, "f value did not set properly, 2");
    cr_assert(t3->ld == s3->ld, "ld value did not set properly, 2");
    cr_assert(t3->c1 == s3->c1, "c1 value did not set properly, 2");
    cr_assert(t3->c2 == s3->c2, "c2 value did not set properly, 2");
    cr_assert(t3->s == s3->s, "s value did not set properly, 2");
    cr_assert(t3->some_data == s3->some_data, "some_data value did not set properly, 2");

    test_item_t* s4 = (test_item_t*)get_index_al(locallist, 3);
    cr_assert(t4->i == s4->i, "i value did not set properly, 3");
    cr_assert(t4->f == s4->f, "f value did not set properly, 3");
    cr_assert(t4->ld == s4->ld, "ld value did not set properly, 3");
    cr_assert(t4->c1 == s4->c1, "c1 value did not set properly, 3");
    cr_assert(t4->c2 == s4->c2, "c2 value did not set properly, 3");
    cr_assert(t4->s == s4->s, "s value did not set properly, 3");
    cr_assert(t4->some_data == s4->some_data, "some_data value did not set properly, 3");


    test_item_t* s5 = (test_item_t*)get_index_al(locallist, 5);

    cr_assert(t5->i == s5->i, "i value did not set properly, 4");
    cr_assert(t5->f == s5->f, "f value did not set properly, 4");
    cr_assert(t5->ld == s5->ld, "ld value did not set properly, 4");
    cr_assert(t5->c1 == s5->c1, "c1 value did not set properly, 4");
    cr_assert(t5->c2 == s5->c2, "c2 value did not set properly, 4");
    cr_assert(t5->s == s5->s, "s value did not set properly, 4");
    cr_assert(t5->some_data == s5->some_data, "some_data value did not set properly, 4");
    cr_assert(locallist->capacity == 8, "capacity did not double");
    cr_assert(locallist->length == 5, "length is not correct");

    test_item_t* s6 = (test_item_t*)get_index_al(locallist, 6);
    cr_assert(t5->i == s6->i, "i value did not set properly, 5");
    cr_assert(t5->f == s6->f, "f value did not set properly, 5");
    cr_assert(t5->ld == s6->ld, "ld value did not set properly, 5");
    cr_assert(t5->c1 == s6->c1, "c1 value did not set properly, 5");
    cr_assert(t5->c2 == s6->c2, "c2 value did not set properly, 5");
    cr_assert(t5->s == s6->s, "s value did not set properly, 5");
    cr_assert(t5->some_data == s6->some_data, "some_data value did not set properly, 5");
}


Test(al_suite, 8_insert, .timeout=2){
    arraylist_t *locallist = new_al(sizeof(test_item_t));
    test_item_t* t1 = (test_item_t*)malloc(sizeof(test_item_t));
    t1->i = 1;
    t1->f = 2.0f;
    t1->ld = 3.14;
    t1->c1 = t1->c2 = 1;
    t1->s = 2;
    t1->some_data = locallist->base;
    insert_al(locallist, t1);
    test_item_t* t2 = (test_item_t*)malloc(sizeof(test_item_t));
    t2->i = 3;
    t2->f = 4.2f;
    t2->ld = 2.71828;
    t2->c1 = t2->c2 = 2;
    t2->s = 1;
    t2->some_data = ((char*)(locallist->base)) + locallist->length * locallist->item_size;
    insert_al(locallist, t2);
    test_item_t* t3 = (test_item_t*)malloc(sizeof(test_item_t));
    t3->i = 5;
    t3->f = 7.8f;
    t3->ld = 1.4142;
    t3->c1 = t3->c2 = 3;
    t3->s = 4;
    t3->some_data = ((char*)(locallist->base)) + locallist->length * locallist->item_size;
    insert_al(locallist, t3);
    test_item_t* t4 = (test_item_t*)malloc(sizeof(test_item_t));
    t4->i = 9;
    t4->f = 3.04f;
    t4->ld = 1.732;
    t4->c1 = t4->c2 = 4;
    t4->s = 3;
    t4->some_data = ((char*)(locallist->base)) + locallist->length * locallist->item_size;
    insert_al(locallist, t4);
    test_item_t* t5 = (test_item_t*)malloc(sizeof(test_item_t));
    t5->i = 32;
    t5->f = 84.1f;
    t5->ld = 931.02;
    t5->c1 = t5->c2 = 5;
    t5->s = 6;
    t5->some_data = ((char*)(locallist->base)) + locallist->length * locallist->item_size;
    insert_al(locallist, t5);
    size_t s1 = get_data_al(locallist, t1);
    cr_assert(s1 == 0, "get index not correct. s1");
    size_t s2 = get_data_al(locallist, t2);
    cr_assert(s2 == 1, "get index not correct. s2");
    size_t s3 = get_data_al(locallist, t3);
    cr_assert(s3 == 2, "get index not correct. s3");
    size_t s4 = get_data_al(locallist, t4);
    cr_assert(s4 == 3, "get index not correct. s4");
    size_t s5 = get_data_al(locallist, t5);
    cr_assert(s5 == 4, "get index not correct. s5");
    size_t s6 = get_data_al(locallist, NULL);
    cr_assert(s6 == 0, "get index not correct. s6");
    test_item_t* t6 = (test_item_t*)malloc(sizeof(test_item_t));
    t5->i = 18;
    t5->f = 70.1f;
    t5->ld = 91.02;
    t5->c1 = t5->c2 = 6;
    t5->s = 5;
    t5->some_data = ((char*)(locallist->base)) + locallist->length * locallist->item_size;
    size_t s7 = get_data_al(locallist, t6);
    cr_assert(s7 == UINT_MAX, "get index not correct. s7");
}

Test(al_suite, 1_remove, .timeout=2)
{

    arraylist_t *list = new_al(sizeof(student_t));

    student_t *s1 = calloc(1,sizeof(student_t));
    student_t *s2 = calloc(1,sizeof(student_t));
    student_t *s3 = calloc(1,sizeof(student_t));
    student_t *s4 = calloc(1,sizeof(student_t));
    student_t *s5 = calloc(1,sizeof(student_t));
    student_t *s6 = calloc(1,sizeof(student_t));
    student_t *s7 = calloc(1,sizeof(student_t));

    s1->id = 1;
    s2->id = 2;
    s3->id = 3;
    s4->id = 4;
    s5->id = 5;
    s6->id = 6;

    insert_al(list,s1);
    insert_al(list,s2);
    insert_al(list,s3);
    insert_al(list,s4);
    insert_al(list,s5);
    insert_al(list,s6);

    remove_data_al(list,s1);

    void* p = get_index_al(list,0);
    cr_assert(((student_t*)p)->id == 2);

    cr_assert(remove_data_al(list,s7) == false);

    cr_assert(list->length == 5);
}



test_item_t* gt2[500];
arraylist_t* global_list7;
int x2[500] = {0};
void* test_insert_get(void *index)
{
    int y = *((int*)index);
    free(index);
    gt2[y]->i = y;
    insert_al(global_list7,gt2[y]);
    x2[y] = get_data_al(global_list7,gt2[y]);
    // printf("y = %d and x[y] = %d\n", y, x2[y]);
    // fflush(stdout);
    return NULL;
}

Test(al_suite, multithread_test_4, .timeout = 4)
{
    global_list7 = new_al(sizeof(test_item_t));

    pthread_t threads[500];
    int rc;
    int i;

    for(i = 0  ; i < 500; i++)
    {
        gt2[i] = malloc(sizeof(test_item_t));
        int* y = malloc(sizeof(int));
        *y = i;
        rc = pthread_create(&threads[i],NULL,test_insert_get,(void*)y);
        if (rc)
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
       }
    }

    for(int i = 0 ; i < 500 ; i++)
    {
        pthread_join(threads[i],NULL);
    }

    bool test_bool[500] = {0};
    for(int i = 0 ; i < 500 ; i++)
    {
        // printf("%d\n", x2[i]);
        cr_assert(!test_bool[x2[i]], "i = %d, x[i] = %d", i, x2[i]);
        test_bool[x2[i]] = true;
    }
    for(int i = 0 ; i < 500 ; i++)
    {
        // printf("%d\n", x[i]);
        cr_assert(test_bool[gt2[i]->i], "i = %d, gt[i]->i = %d", i, gt2[i]->i);
        test_bool[gt2[i]->i] = false;
    }

}


/*                                   FOREACH UNIT TEST                               */




void* iterate_foreach(void* index)
{

    foreach(student_t, value,foreach_list)
    {

    }
    return NULL;

}


Test(al_suite, singlethread_for_each, .timeout = 2)
{
    arraylist_t *t1 = new_al(sizeof(student_t));

    student_t *t = calloc(1,sizeof(student_t));

    for(int i = 0 ; i < 5 ; i++)
    {
        insert_al(t1,t);
    }

    int i = 0 ;
    foreach(student_t,value,t1)
    {
        if(i == 0)
        {
            value->gpa = 1.2;
            value->id = 15;
            value->name = "john";
        }
        else if(i == 1)
        {
            value->gpa = 3.4;
            value->id = 70;
            value->name = "apple";
        }
        else if(i == 2)
        {
            value->gpa = 0;
            value->id = 5000;
            value->name = "chun";
        }
        else if(i == 3)
        {
            value->gpa = 4.1;
            value->id = 132;
            value->name = "dud";
        }
        else if(i == 4)
        {
            value->gpa = 2.3;
            value->id = 13;
            value->name = "mike";
        }
        i++;
    }

    void* p = calloc(1,t1->item_size);
    student_t *s = calloc(1,sizeof(student_t));
    for(int i = 0 ; i < t1->length ; i++)
    {
        p = get_index_al(t1,i);
        s = p;
        if(i == 0)
        {
            cr_assert(s->gpa == 1.2);
            cr_assert(s->id == 15);
            cr_assert(strcmp(s->name,"john") == 0);
        }
        else if(i == 1)
        {
            cr_assert(s->gpa == 3.4);
            cr_assert(s->id == 70);
            cr_assert(strcmp(s->name,"apple") == 0);
        }
        else if(i == 2)
        {
            cr_assert(s->gpa == 0);
            cr_assert(s->id == 5000);
            cr_assert(strcmp(s->name,"chun") == 0);
        }
        else if(i == 3)
        {
            cr_assert(s->gpa == 4.1);
            cr_assert(s->id == 132);
            cr_assert(strcmp(s->name,"dud") == 0);
        }
        else if(i == 4)
        {
            cr_assert(s->gpa == 2.3);
            cr_assert(s->id == 13);
            cr_assert(strcmp(s->name,"mike") == 0);
        }
    }

}

Test(al_suite, singlethread_for_each_2, .timeout = 2)
{
    arraylist_t *t = new_al(sizeof(test_item_t));
    // test_item_t *test = calloc(1,sizeof(test_item_t));

    foreach(test_item_t,value,t)
    {
        cr_assert(t == NULL);
    }

}


Test(al_suite, multi_thread_for_each_1, .timeout = 10 )
{
    foreach_list = new_al(sizeof(student_t));
    foreach_list2 = new_al(sizeof(student_t));
    for(int i = 0 ; i < 3 ; i++)
    {
        student_t *t = calloc(1,sizeof(student_t));
        t->id = i;
        insert_al(foreach_list,t);
    }

    pthread_t threads[3];
    int rc;
    int i;

    for(i = 0  ; i < 3; i++)
    {
        int* y = malloc(sizeof(int));
        *y = i;
        rc = pthread_create(&threads[i],NULL,iterate_foreach,(void*)y);
        if (rc)
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    for(int i = 0 ; i < 3 ; i++)
    {
        pthread_join(threads[i],NULL);
    }

    int x = 0;
    foreach(student_t, value, foreach_list2)
    {

        printf("value: %d vs x %d\n",value->id,x%10);
        cr_assert(get_data_al(foreach_list2,value) == x%10);
        x++;

    }

}


arraylist_t *g1;
arraylist_t *g2;
void* foreach_help(void* index)
{
    foreach(student_t,value,g1)
    {

    }
    return NULL;
}


Test(al_suite, multi_thread_for_each_2, .timeout = 10)
{
    g1 = new_al(sizeof(student_t));
    g2 = new_al(sizeof(student_t));

    for(int i = 0 ; i < 5 ; i++)
    {
        student_t *s = calloc(1,sizeof(student_t));
        s->id = i;
        insert_al(g1,s);
        insert_al(g2,s);
    }


    pthread_t threads[3];
    int rc;
    int i;

    for(i = 0  ; i < 3; i++)
    {
        int* y = malloc(sizeof(int));
        *y = i;
        rc = pthread_create(&threads[i],NULL,foreach_help,(void*)y);
        if (rc)
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    for(int i = 0 ; i < 3 ; i++)
    {
        pthread_join(threads[i],NULL);
    }

}


void* multi_thread_for_each_3_helper(void* threadid)
{
    foreach(student_t, s1, global_list8)
    {
        s1->id = 200;
    }
    foreach(student_t, s2, global_list8)
    {
        s2->id = 300;
    }

    return NULL;
}


arraylist_t *global_list8;
Test(al_suite, multi_thread_for_each_3, .timeout = 10)
{

    for(int i = 0 ; i < 100 ; i++){
    global_list8 = new_al(sizeof(student_t));

    for(int i = 0 ; i < 5 ; i++)
    {
        student_t *student = calloc(1,sizeof(student_t));
        student->id = i;
        insert_al(global_list8,student);
    }

    pthread_t threads[10];
    int rc;
    int i;

    for(i = 0  ; i < 10 ; i++)
    {
        int* y = malloc(sizeof(int));
        *y = i;
        rc = pthread_create(&threads[i],NULL,multi_thread_for_each_3_helper,(void*)y);
        if (rc)
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    for(int i = 0 ; i < 10 ; i++)
    {
        pthread_join(threads[i],NULL);
    }

    void* p = calloc(1,global_list8->item_size);
    for(int i = 0 ; i < global_list8->length; i++)
    {
        student_t *test = calloc(1,sizeof(student_t));
        p = get_index_al(global_list8,i);
        test = p;
        cr_assert(test->id == 300);

    }
}

}

arraylist_t *global_list9;
void* multi_thread_for_each_5_helper(void* ptr)
{
    foreach(student_t,value,global_list9)
    {
        value->id = 1000;
    }
    return NULL;
}

// Test(al_suite, multithread_test_5, .timeout = 5)
// {
//     global_list9 = new_al(sizeof(student_t));
//     student_t *s = calloc(1,sizeof(student_t));
//     insert_al(global_list9,s);

//    pthread_t threads[1000];
//     int i;

//     for(i = 0  ; i < 1000 ; i++)
//     {
//         int* y = malloc(sizeof(int));
//         *y = i;
//         pthread_create(&threads[i],NULL,multi_thread_for_each_5_helper,(void*)y);
//     }

//     for(int i = 0 ; i < 1000 ; i++)
//     {
//         pthread_join(threads[i],NULL);
//     }
// }



Test(al_suite, test_for_each_block_single_thread, .timeout = 2)
{

    arraylist_t *list = new_al(sizeof(student_t));

    student_t *student = calloc(1,sizeof(student_t));
    for(int i = 0 ; i < 100 ; i++)
    {
        student = calloc(1,sizeof(student_t));
        student->id = i;
        insert_al(list,student);
    }

    foreach(student_t, value, list)
    {
        remove_index_al(list,0);
        remove_data_al(list,student);
    }

    void* p = calloc(1,list->item_size);
    student_t *result = calloc(1,sizeof(student_t));
    for(int i = 0 ; i < 100 ;i++)
    {
        p = get_index_al(list,i);
        result = p;
        cr_assert(list->length == 100);
        cr_assert(result->id == i);
    }

}



arraylist_t *g1;
arraylist_t *g2;

void* for_each_test_deletion(void* threadid)
{
    foreach(student_t,value,g1)
    {
        remove_data_al(g1,value);
        remove_index_al(g1,0);

    }
    foreach(student_t,value,g1)
    {
        remove_data_al(g1,value);
        remove_index_al(g1,0);

    }
    foreach(student_t,value,g1)
    {
        remove_data_al(g1,value);
        remove_index_al(g1,0);

    }


    return NULL;
}

Test(al_suite, test_for_each_remove_multithreading, .timeout = 2)
{
    g1 = new_al(sizeof(student_t));
    g2 = new_al(sizeof(student_t));

    student_t *student = calloc(1,sizeof(student_t));

    insert_al(g1,student);
    pthread_t threads[1000];

    int i;
    for(i = 0  ; i < 1000; i++)
    {
     pthread_create(&threads[i],NULL, for_each_test_deletion, NULL);
    }

for(int i = 0 ; i < 1000; i++)
{
    pthread_join(threads[i],NULL);
}
    printf("LENGTH: %lu\n",g1->length);
    cr_assert(g1->length == 1);

}

int helper(void* data){ ((student_t*)data)->id++; return 0; }

Test(al_suite, apply_single_thread, .timeout = 2)
{
    arraylist_t *l1 = new_al(sizeof(student_t));
    arraylist_t *l2 = new_al(sizeof(student_t));


    for(int i = 0 ; i < 100 ; i++)
    {
        student_t *test = calloc(1,sizeof(student_t));
        test->id = i;
        insert_al(l1,test);
        insert_al(l2,test);
    }

    apply(l1,helper);
    apply(l2,helper);

    void* ptr = calloc(1,l1->item_size);
    student_t *j = calloc(1,sizeof(student_t));
    for(int i = 0 ; i < 100 ; i++)
    {
        ptr = get_index_al(l1,i);
        j = ptr;
        cr_assert(j->id == i+1);
    }
}

Test(al_suite, insert_then_delete, .timeout = 2)
{
    arraylist_t *l1 = new_al(sizeof(student_t));
    // arraylist_t *l2 = new_al(sizeof(student_t));

    student_t *s1 = calloc(1,sizeof(student_t));
    student_t *s2 = calloc(1,sizeof(student_t));
    student_t *s3 = calloc(1,sizeof(student_t));
    student_t *s4 = calloc(1,sizeof(student_t));
    student_t *s5 = calloc(1,sizeof(student_t));

    s1->id = 1;
    s1->gpa = 3.5;
    s1->name = "j";

    s2->id = 2;
    s2->gpa = 2.5;
    s2->name = "l";

    s3->id = 5;
    s3->gpa = 6.5;
    s3->name = "z";

    s4->id = 9;
    s4->gpa = 11.5;
    s4->name = "a";

    s5->id = 6;
    s5->gpa = 2.4;
    s5->name = "o";

    insert_al(l1,s1);
    insert_al(l1,s2);
    insert_al(l1,s3);
    insert_al(l1,s4);
    insert_al(l1,s5);

    remove_index_al(l1,2);

    cr_assert(l1->length == 4);
    cr_assert(l1->capacity == 8);

    student_t *s = calloc(1,sizeof(student_t));
    s = get_index_al(l1,0);

    cr_assert(s->id == 1);
    cr_assert(s->gpa == 3.5);
    cr_assert(strcmp(s->name,"j") == 0);

    s = get_index_al(l1,1);

    cr_assert(s->id == 2);
    cr_assert(s->gpa = 2.5);
    cr_assert(strcmp(s->name,"l") == 0);

    s = get_index_al(l1,2);

    cr_assert(s->id == 9);
    cr_assert(s->gpa == 11.5);
    cr_assert(strcmp(s->name,"a") == 0);

    s = get_index_al(l1,3);
    cr_assert(s->id == 6);
    cr_assert(s->gpa == 2.4);
    cr_assert(strcmp(s->name,"o") == 0);

    remove_data_al(l1,s4);

    s = get_index_al(l1,0);

    cr_assert(s->id == 1);
    cr_assert(s->gpa == 3.5);
    cr_assert(strcmp(s->name,"j") == 0);

    s = get_index_al(l1,1);

    cr_assert(s->id == 2);
    cr_assert(s->gpa = 2.5);
    cr_assert(strcmp(s->name,"l") == 0);

    s = get_index_al(l1,2);
    cr_assert(s->id == 6);
    cr_assert(s->gpa == 2.4);
    cr_assert(strcmp(s->name,"o") == 0);



}
Test(al_suite, test_remove_index_single, .timeout = 2)
{
    arraylist_t *l1 = new_al(sizeof(student_t));
    student_t *s1 = calloc(1,sizeof(student_t));
    student_t *s2 = calloc(1,sizeof(student_t));
    student_t *s3 = calloc(1,sizeof(student_t));
    student_t *s4 = calloc(1,sizeof(student_t));

    s1->id = 1;
    s2->id = 2;
    s3->id = 3;
    s4->id = 4;
    insert_al(l1,s1);
    insert_al(l1,s2);
    insert_al(l1,s3);
    insert_al(l1,s4);

    remove_index_al(l1,4);

    void* p = calloc(1,l1->item_size);
    student_t *test = calloc(1,sizeof(student_t));
    for(int i = 0 ; i < l1->length ;i++)
    {
        p = get_index_al(l1,i);
        test = p;
        cr_assert(test->id == i+1);
    }
    cr_assert(l1->length == 3);

}

Test(al_suite, test_remove_insert_single, .timeout = 2)
{
    arraylist_t *l1 = new_al(sizeof(test_item_t));

    test_item_t *t = calloc(1,sizeof(test_item_t));
    t->i = 10;

    insert_al(l1,t);
    insert_al(l1,t);

    void* p = get_index_al(l1,0);
    void* p2 = get_index_al(l1,1);

    test_item_t *tester = calloc(1,sizeof(test_item_t));
    test_item_t *tester2 = calloc(1,sizeof(test_item_t));
    tester = p;
    tester2 = p2;
    cr_assert(l1->length == 2);
    cr_assert(tester->i == tester2->i);
    cr_assert(memcmp(tester2,tester,l1->item_size) == 0);


}



arraylist_t *somelist;
void* help(void* k)
{
    test_item_t *t = calloc(1,sizeof(test_item_t));
    insert_al(somelist,t);
    insert_al(somelist,t);
    remove_index_al(somelist,0);
    remove_data_al(somelist,t);
    return NULL;
}
Test(al_suite, test_insert_then_delete, .timeout = 2)
{
    somelist = new_al(sizeof(student_t));

    pthread_t threads[100];
    int i;
    for(i = 0  ; i < 100; i++)
    {
     int* y = malloc(sizeof(int));
     *y = i;
     pthread_create(&threads[i],NULL,help, NULL);
    }

    for(int i = 0 ; i < 100; i++)
    {
        pthread_join(threads[i],NULL);
    }

     cr_assert(somelist->length == 0);
     student_t *stu = calloc(1,sizeof(student_t));
     insert_al(somelist,stu);
     cr_assert(somelist->length == 1);
     remove_index_al(somelist,0);
     cr_assert(somelist->length == 0);

     insert_al(somelist,stu);
     remove_data_al(somelist,stu);
     cr_assert(somelist->length == 0);
}


Test(al_suite, for_each_insert, .timeout = 2)
{
    arraylist_t *a = new_al(sizeof(student_t));
    arraylist_t *b = new_al(sizeof(student_t));

    student_t *stu = calloc(1,sizeof(student_t));
    for(int i = 0 ; i < 5 ; i++)
    {
        insert_al(a,stu);
    }

    for(int i = 0 ; i < 100 ; i++)
    {
        foreach(void*,value,a)
        {
            insert_al(b,stu);
        }
    }
    cr_assert(b->length == 500);
}


arraylist_t *somelist2;
arraylist_t *somelist3;
static int y = 0;
void* help2(void* id)
{
    student_t *s = calloc(1,sizeof(student_t));
    foreach(void*,value,somelist2)
    {
        y++;
        insert_al(somelist3,s);
    }
    return NULL;
}

Test(al_suite, test_insert, .timeout = 2)
{
    somelist2 = new_al(sizeof(student_t));
    somelist3 = new_al(sizeof(student_t));
    student_t *stu = calloc(1,sizeof(student_t));

    for(int i = 0 ; i < 5 ; i++)
    {
        insert_al(somelist2,stu);
    }

    pthread_t threads[500];
    int i;
    for(i = 0  ; i < 500; i++)
    {
        int* y = malloc(sizeof(int));
        *y = i;
        pthread_create(&threads[i],NULL,help2, NULL);
    }

    for(int i = 0 ; i < 500; i++)
    {
        pthread_join(threads[i],NULL);
    }
    cr_assert(somelist3->length == 2500);

}


// Test(al_suite, test_delete_al, .timeout = 2)
// {
//     arraylist_t *somelist = new_al(sizeof(test_item_t));
//     test_item_t *stu = calloc(1,sizeof(test_item_t));

//     for(int i = 0 ; i < 5 ; i++)
//     {
//         stu->some_data = "hello";
//         insert_al(somelist,stu);
//     }

//     delete_al(somelist,test_item_t_free_func);
// }


arraylist_t *global_list25;

student_t* students[10];
void* thread_itrator(void* list){

    //volatile int i = 0;
    foreach(student_t,value,list)
    {
        usleep(1);
    }

    return NULL;
}

Test(al_suite, two_threads_iterator_one_array, .timeout = 10)
{

    global_list25 = new_al(sizeof(test_item_t));

    for(int i = 0; i < 10; i++){
        student_t *student = malloc(sizeof(student_t));
        student -> id = i;
        students[i] = student;
        insert_al(global_list25, student);
        free(student);
    }

    pthread_t threads[10];
    int rc;
    int i;
    volatile int j = 0;
    for(i = 0  ; i < 5; i++)
    {
        rc = pthread_create(&threads[j],NULL,thread_itrator,global_list25);
        if (rc)
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
        j++;
        rc = pthread_create(&threads[j],NULL,thread_itrator,global_list25);
        j++;
        if (rc)
        {
            printf("ERROR; return code from pthread_delete() is %d\n", rc);
            exit(-1);
        }
    }
    for(int i = 0 ; i < 10; i++)
    {
        pthread_join(threads[i],NULL);
    }
}


//
arraylist_t *ll;
void* help3(void* k)
{
    delete_al(ll,NULL);
    return NULL;
}

// Test(al_suite, test_delete_in_multithreading, .timeout = 2)
// {
//     ll = new_al(sizeof(student_t));
//     student_t *stu = calloc(1,sizeof(student_t));

//     for(int i = 0 ; i < 5 ; i++)
//     {
//         stu = calloc(1,sizeof(student_t));
//         insert_al(somelist,stu);
//     }

//     pthread_t threads[500];
//     int i;
//     for(i = 0  ; i < 500 ; i++)
//     {
//         int* y = malloc(sizeof(int));
//         *y = i;
//         pthread_create(&threads[i],NULL,help3, NULL);
//     }

//     for(int i = 0 ; i < 500; i++)
//     {
//         pthread_join(threads[i],NULL);
//     }
// }

typedef struct{
int* i;
arraylist_t* al;
}mt_fe_t1_struct;

void* mt_fe_t1_func(void* index){
    mt_fe_t1_struct* new_struct = ((mt_fe_t1_struct*)index);
    int i = *(new_struct->i);
    // printf("HI, i = %d\n", i);
    // fflush(stdout);
    arraylist_t* al = new_struct->al;
    // free(new_struct->i);
    // free(index);
    // printf("See, i = %d\n", i);
    bool b = false;
    foreach(student_t, value, al){
        if(b)
            value->id = i + 1;
        else
            value = NULL;
        b = !b;
    }
    cr_assert(true);
    // printf("Bye, i = %d\n", i);
    return NULL;
}



Test(al_suite, mt_fe_t1, .timeout = 10){
    arraylist_t* g1 = new_al(sizeof(student_t));
    student_t *student = calloc(1,sizeof(student_t));
    for(int i = 0; i < TEMP2; i++){
        student->id = i + NUM_THREADS + TEMP1;
        insert_al(g1,student);
    }
    // printf("%lu\n", g1->length);
    // for(int i = 0; i < 10; i++){
    //     student_t* student = g1->base;
    //     printf("%d\n", student[i].id);
    //     fflush(stdout);
    // }
    // fflush(stdout);

    pthread_t threads[NUM_THREADS];
    for(int i = 0 ; i < NUM_THREADS; i++)
    {
        mt_fe_t1_struct* new_struct = malloc(sizeof(mt_fe_t1_struct));
        new_struct->i = malloc(sizeof(int));
        *(new_struct->i) = i;
        new_struct->al = g1;
        pthread_create(&threads[i], NULL, mt_fe_t1_func, new_struct);
    }

    for(int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    bool b = false;
    int i = 0;
    foreach(student_t, value, g1){
        if(b)
            cr_assert(value->id <= NUM_THREADS);
        else
            cr_assert(value->id == NUM_THREADS + TEMP1 + i);
        b = !b;
        i++;
        // student_t* newVal = value + 1;
        // printf("%d and newVal = NULL? %d\n", value->id, newVal == NULL);
    }
    cr_assert(i == TEMP2, "i = %d", i);
    // fflush(stdout);
    // printf("\ng1->length == %lu\n", g1->length);
    // student_t* st = g1->base;
    // st++;
    // printf("st[1] == NULL? %d\n", st == NULL);
    // cr_assert(g1->length == 0);
}

// Test(al_suite, test_d, .timeout = 2)
// {
//     arraylist_t *t = new_al(sizeof(test_item_t));

//     for(int i = 0 ; i < 100 ; i++)
//     {
//         test_item_t *test = calloc(1,sizeof(test_item_t));
//         insert_al(t,test);
//     }

//     delete_al(t,(test_item_t_free_func));

//     cr_assert(t->length == 0);
//     cr_assert(t->capacity == 0);
//     cr_assert(t->item_size == 0);
// }



