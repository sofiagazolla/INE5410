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

int main(int argc, char** argv) {

    for (int i = 0; i < 2; i++) {
        __pid_t pid = fork(); // cria os processos filhos
        if (pid == 0){ // verifica se ele é filho
            printf("Processo %d, filho de %d\n", getpid(), getppid()); // printa que o processo foi criado
            // usa getpid() para conseguir o pid do processo e getppid() para conseguir o do pai
            fflush(stdout);
            
            for (int j = 0; j < 3; j++){
                __pid_t pid_netos = fork(); // cria os processos netos (3 por filho)
                if (pid_netos == 0){ // verifica se é neto
                    printf("Processo %d, filho de %d\n", getpid(), getppid()); // printa que o processo foi criado
                    fflush(stdout);
                    sleep(5); // espera 5 segundos antes de finalizar
                    printf("Processo %d finalizado\n", getpid()); // finaliza o processo
                    fflush(stdout);
                    return 0;
                }
            }

            while(wait(NULL) >= 0); // garante que espere que os processos netos tenham finalizado
            printf("Processo %d finalizado\n", getpid()); // finaliza os processos filhos
            fflush(stdout);

            return 0;
        }
    }

    while(wait(NULL) >= 0); // garante que espere que os processos filhos tenham finalizado

    printf("Processo principal %d finalizado\n", getpid()); // finaliza o processo principal
    fflush(stdout);
    return 0;
    
}
