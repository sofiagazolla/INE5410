#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <stdlib.h>

FILE* out;

// Criando semáforos
sem_t semaforoA;
sem_t semaforoB;

void *thread_a(void *args) {
    for (int i = 0; i < *(int*)args; ++i) {

        sem_wait(&semaforoA); // Espera até que chegue a vez de A
	
        fprintf(out, "A");
        fflush(stdout);

        sem_post(&semaforoB); // Libera a vez de B
    }
    return NULL;
}

void *thread_b(void *args) {
    for (int i = 0; i < *(int*)args; ++i) {

        sem_wait(&semaforoB); // Espera até que chegue a vez de B

        fprintf(out, "B");
        fflush(stdout);

        sem_post(&semaforoA); // Libera a vez de A
    }
    return NULL;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Uso: %s [ITERAÇÕES]\n", argv[0]);
        return 1;
    }
    int iters = atoi(argv[1]);
    srand(time(NULL));
    out = fopen("result.txt", "w");

    pthread_t ta, tb;

    // Inicializa os semáforos
    sem_init(&semaforoA, 0, 1);
    sem_init(&semaforoB, 0, 1);

    // Cria threads
    pthread_create(&ta, NULL, thread_a, &iters);
    pthread_create(&tb, NULL, thread_b, &iters);

    // Espera pelas threads
    pthread_join(ta, NULL);
    pthread_join(tb, NULL);

    //Imprime quebra de linha e fecha arquivo
    fprintf(out, "\n");
    fclose(out);

    // Destrói os semáforos
    sem_destroy(&semaforoA);
    sem_destroy(&semaforoB);

    return 0;

}