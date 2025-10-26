#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>

// Lê o conteúdo do arquivo filename e retorna um vetor E o tamanho dele
// Se filename for da forma "gen:%d", gera um vetor aleatório com %d elementos
//
// +-------> retorno da função, ponteiro para vetor malloc()ado e preenchido
// |
// |         tamanho do vetor (usado <-----+
// |         como 2o retorno)              |
// v                                       v
double* load_vector(const char* filename, int* out_size);

struct vetores{
    double *a;
    double *b;

    int first;
    int last;
};

void* thread(void* arg) { //método soma de vetores
    
    struct vetores *ponteiro = (struct vetores*) arg;
    
    double *a = ponteiro->a;
    double *b = ponteiro->b;

    int first = ponteiro->first;
    int last = ponteiro->last;

    double* total = malloc(sizeof(double));
    *total = 0; // inicializa

    for (int i = first; i < last; ++i) {
        *total += a[i] * b[i];
    }

    return total;
}


// Avalia se o prod_escalar é o produto escalar dos vetores a e b. Assume-se
// que ambos a e b sejam vetores de tamanho size.
void avaliar(double* a, double* b, int size, double prod_escalar);

int main(int argc, char* argv[]) {
    srand(time(NULL));

    //Temos argumentos suficientes?
    if(argc < 4) {
        printf("Uso: %s n_threads a_file b_file\n"
            "    n_threads    número de threads a serem usadas na computação\n"
            "    *_file       caminho de arquivo ou uma expressão com a forma gen:N,\n"
            "                 representando um vetor aleatório de tamanho N\n", 
            argv[0]);
        return 1;
    }

    //Quantas threads?
    int n_threads = atoi(argv[1]);
    if (!n_threads) {
        printf("Número de threads deve ser > 0\n");
        return 1;
    }
    //Lê números de arquivos para vetores alocados com malloc
    int a_size = 0, b_size = 0;
    double* a = load_vector(argv[2], &a_size);
    if (!a) {
        //load_vector não conseguiu abrir o arquivo
        printf("Erro ao ler arquivo %s\n", argv[2]);
        return 1;
    }
    double* b = load_vector(argv[3], &b_size);
    if (!b) {
        printf("Erro ao ler arquivo %s\n", argv[3]);
        return 1;
    }
    
    //Garante que entradas são compatíveis
    if (a_size != b_size) {
        printf("Vetores a e b tem tamanhos diferentes! (%d != %d)\n", a_size, b_size);
        return 1;
    }

    if (n_threads > a_size) {
        n_threads = a_size;
    }

    pthread_t threads[n_threads];
    struct vetores arg[n_threads];

    int qtde_por_thread = a_size / n_threads;
    int resto = a_size % n_threads;

    //Calcula produto escalar. Paralelize essa parte
    double result = 0;
    for (int i = 0; i < n_threads; ++i){
        arg[i].a = a;
        arg[i].b = b;

        arg[i].first = i * qtde_por_thread;
        arg[i].last = (1 + i) * qtde_por_thread;
    
        if (i == n_threads - 1) {
            arg[i].last += resto;
        }

        pthread_create(&threads[i], NULL, thread, (void*)&arg[i]);

    }

    for (int i = 0; i < n_threads; i++){
        void * retorno;
        pthread_join(threads[i], &retorno);

        double* soma_parcial = (double*) retorno;

        result += *soma_parcial;
        free(soma_parcial);
    }

    //    +---------------------------------+
    // ** | IMPORTANTE: avalia o resultado! | **
    //    +---------------------------------+
    avaliar(a, b, a_size, result);

    //Libera memória
    free(a);
    free(b);

    return 0;
}
