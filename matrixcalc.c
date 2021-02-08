#include <pthread.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 

// naming global variables
int num=0;
int matrix[20][20];
pthread_mutex_t calc_lock; 
pthread_mutex_t create_lock; 
pthread_mutex_t main_create; 
pthread_mutex_t main_while; 
pthread_t t_calc;
pthread_t t_create;

//create function creates matrixes and unlocks the calculation function lock so created matrix 
//  can be calculated afterwards
void *create(){
while (1)
 {   
 
 pthread_mutex_lock(&main_create);   
 int i=0;
 for (i = 0; i < num; i++)
  {  
    pthread_mutex_lock(&create_lock);   
    unsigned short i, j;

    for(i = 0; i < 20;i++) {
    	for(j = 0; j < 20;j++) {
            matrix[i][j]=rand()%100;
    		printf("%d ", matrix[i][j]);
    	}
    	printf("\n");
    }
        printf("\n");
        printf("\n");

    pthread_mutex_unlock(&calc_lock);     
    
       
  }
 
   pthread_mutex_unlock(&main_while);   
 
 }
}

// calc function calculates the sum of elements of matrix after the matrix has been initialized.
// Then it unlocks create function so that new matrix can be initiated
void *calc(){
    
    while (1)
    { 
       pthread_mutex_lock(&calc_lock);  
       int i=0;
       int j=0;
       int sum=0;
       for (i = 0; i < 20; i++)
       {
           for (j = 0; j < 20; j++)
           {
               sum+=matrix[i][j];
           }
           
       }
       printf("sum is :  %d\n\n\n",sum);

       pthread_mutex_unlock(&create_lock); 
    }
}
//Handles the closing signal
void signal_handl(int sig){
 signal(SIGINT,signal_handl);
 printf("\nClosing the program\n");
 pthread_kill(t_calc,SIGKILL);
 pthread_kill(t_create,SIGKILL);
 pthread_mutex_destroy(&create_lock); 
 pthread_mutex_destroy(&calc_lock); 
 exit(0);
}

int main(){

signal(SIGINT, signal_handl);

//initializing all the locks
if (pthread_mutex_init(&create_lock, NULL) != 0) { 
        printf("\n mutex init has failed\n"); 
        return 1; 
} 
if (pthread_mutex_init(&calc_lock, NULL) != 0) { 
        printf("\n mutex init has failed\n"); 
        return 1; 
} 
if (pthread_mutex_init(&main_create, NULL) != 0) { 
        printf("\n mutex init has failed\n"); 
        return 1; 
}
if (pthread_mutex_init(&main_while, NULL) != 0) { 
        printf("\n mutex init has failed\n"); 
        return 1; 
}

//locking the create method first so main while function that asks for a number can start first.
pthread_mutex_lock(&main_create);   


pthread_create(&t_create, NULL, create, NULL); 

//locking the calculation function so that create() function can start first
pthread_mutex_lock(&calc_lock); 

pthread_create(&t_calc, NULL, calc, NULL); 

//asks for a number than unlocks the lock of create() function
while (1)
{
 pthread_mutex_lock(&main_while);   
 printf("Bir sayı giriniz :  ");
 char nums[100];
 scanf("%s",nums);
 num=atoi(nums);
 printf("\n");
 if (num<=0){
    pthread_kill(t_calc,SIGKILL);
    pthread_kill(t_create,SIGKILL);
    exit(0);}
 pthread_mutex_unlock(&main_create);   
}

pthread_join(t_create, NULL); 
pthread_join(t_calc, NULL); 
pthread_mutex_destroy(&create_lock); 
pthread_mutex_destroy(&calc_lock); 
}