#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

//       (pai)
//         |
//    +----+----+
//    |         |
// filho_1   filho_2


// ~~~ printfs  ~~~
// pai (ao criar filho): "Processo pai criou %d\n"
//    pai (ao terminar): "Processo pai finalizado!\n"
//  filhos (ao iniciar): "Processo filho %d criado\n"

// Obs:
// - pai deve esperar pelos filhos antes de terminar!


int main(int argc, char** argv) {

    pid_t pid; // criamos uma variável, para poder armazenar os pid

    for (int i = 0; i < 2; i++) {
        fflush(stdout); // garante que não tenham problemas no buffer de saída
        pid = fork(); // cria um processo filho e armazena o pid
        
        if (pid == 0){ // se é igual a 0, falamos do processo filho
            printf("Processo filho %d criado\n" , getpid()); // usamos getpid pq no processo filho, o retorno de pid é 0
            return 0;
        } else { // caso seja o processo pai
            printf("Processo pai criou %d\n", pid); // não precisa usar getpid pq no processo pai o pid é retornado diretamente
        }
    }

    while(wait(NULL) >= 0); // enquanto houverem filhos terminando (pq quando terminar é -1), continua esperando que os filhos terminem

    printf("Processo pai finalizado!\n");

    return 0;
}
