#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

//                          (principal)
//                               |
//              +----------------+--------------+
//              |                               |
//           filho_1                         filho_2
//              |                               |
//    +---------+-----------+          +--------+--------+
//    |         |           |          |        |        |
// neto_1_1  neto_1_2  neto_1_3     neto_2_1 neto_2_2 neto_2_3

// ~~~ printfs  ~~~
//      principal (ao finalizar): "Processo principal %d finalizado\n"
// filhos e netos (ao finalizar): "Processo %d finalizado\n"
//    filhos e netos (ao inciar): "Processo %d, filho de %d\n"

// Obs:
// - netos devem esperar 5 segundos antes de imprmir a mensagem de finalizado (e terminar)
// - pais devem esperar pelos seu descendentes diretos antes de terminar

    /*************************************************
     * Dicas:                                        *
     * 1. Leia as intruções antes do main().         *
     * 2. Faça os prints exatamente como solicitado. *
     * 3. Espere o término dos filhos                *
     *************************************************/

int criandoNetos(int numeroNetos) {
    pid_t pid;

    for (int i = 0; i < numeroNetos; i++) {

        pid = fork();

        if (pid == 0) {
            printf("Processo neto %d, filho de %d\n", getpid(), getppid());
            fflush(stdout);

            sleep(5);
            printf("Processo neto %d finalizado\n", getpid());
            fflush(stdout);
            _exit(0);
        }
    }
    while (wait(NULL) >= 0);
    return 0;
}

int criandoFilhos(int numeroFilhos, int numeroNetos){
    pid_t pid;

    for (int i = 0; i < numeroFilhos; i++) {

        pid = fork();

        if (pid == 0) {
            printf("Processo filho %d, filho de %d\n", getpid(), getppid());
            fflush(stdout);

            //chama a funcao para criar netos
            criandoNetos(numeroNetos);

            printf("Processo filho %d finalizado\n", getpid());
            fflush(stdout);
            return 0;
        }
    }

    while(wait(NULL) >= 0);
    return 0;
}

int main (int argc, char** argv) {
    
    pid_t pidPrincipal = getpid();

    criandoFilhos(2, 3);

    if (getpid() ==  pidPrincipal) {
        printf("Processo principal %d finalizado\n", getpid());    
        fflush(stdout);
    }
    
    return 0;
}