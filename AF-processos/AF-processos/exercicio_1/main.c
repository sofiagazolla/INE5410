#include <stdio.h> //lib para o io, aqui printf() e fflush()
#include <unistd.h> //lib para o fork()
#include <sys/wait.h> //lib para o wait()

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

    // ....

    /*************************************************
     * Dicas:                                        *
     * 1. Leia as intruções antes do main().         *
     * 2. Faça os prints exatamente como solicitado. *
     * 3. Espere o término dos filhos                *
     *************************************************/

    pid_t pid; //var para armazenar o PID

    for (int i = 0; i < 2; i++){

        pid = fork();

        if (pid == 0){ 
            printf("Processo filho %d criado\n", getpid());
            fflush(stdout);
            break; 
        } else if (pid > 0) { 
            printf("Processo pai criou %d\n", pid); 
            fflush(stdout);
        }
    }

    if (pid > 0){ 
        while(wait(NULL) >= 0); 
        printf("Processo pai finalizado!\n");
        fflush(stdout);
    }

    return 0;
}
