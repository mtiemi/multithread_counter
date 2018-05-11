/* Lab 5
 * Contador de numero primos por multithreads
 * Nome: Mariane Tiemi Iguti RA: 147279
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define n_threads 3
#define ATIVO 1
#define INATIVO 0

int n_workers = 0;
pthread_t workers[n_threads];
int worker_status[n_threads];
int result = 0;

typedef struct 
{
  int N;
  int ID;
} thread_args;

pthread_mutex_t trava;

// verifica primo
int primo(unsigned long int entrada)
{
    unsigned long int i, div = 0;
    
    for(i = 1; i <= entrada; i++)
    {
        if(entrada % i == 0)
        {
            div++;
        }
    }
    
    if(div == 2)
    {
        return 1; // primo
    }else
    {
        return 0; // nao eh primo
    }

}

void* funcao_thread(void *arg) 
{
    thread_args *info = (thread_args *)arg;
    int aux = primo(info->N);
    pthread_mutex_lock(&trava);
    
    if(aux == 1)
    {
        result++;
    }
    
    n_workers--;
    worker_status[info->ID] = INATIVO;
    free(info);
    pthread_mutex_unlock(&trava);
    
    return NULL;
}

int main(int argc, char **argv) 
{
    unsigned long int number;
    thread_args *send_args;
    int i;
    char c;
    
    while(1) 
    { 
        if(n_workers < n_threads) 
        {
            // pegando as entradas
            scanf("%lu", &number);
            c = getchar();
            
            if(c == EOF)
            {
                break;
            }
                   
            pthread_mutex_lock(&trava);
            send_args = (thread_args*) malloc (sizeof(thread_args));
            send_args->N = number;
          
            /* Procura espaco para thread livre */
            i = 0;
            while(worker_status[i] == ATIVO)
            { 
                i++;
            }  
            
            send_args->ID = i;
            worker_status[i] = ATIVO;
            n_workers++;
            pthread_create(& (workers[i]), NULL, funcao_thread, (void*) send_args);
            pthread_mutex_unlock(&trava);
        }
    }
        
    /* Esperando threads terminarem! */
    for(i = 0; i < n_threads; i++) 
    {
        if(worker_status[i] == ATIVO)
        pthread_join(workers[i], NULL);
    }
     
    printf("%d\n", result);
     
  return 0;
}
