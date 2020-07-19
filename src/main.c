#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_ENTRIES 1000
#define MAX_PROCESSES 4


/**
 * Funcao verificadora de numeros primos
 * @param n numero a ser verificado
 * @return 0 se nao for primo, 1 caso contrario
 */
int is_prime(int64_t n) {

    // o menor primo e 2
    if (n < 2) return 0;

    // caso tenha divisor != 1 e != n: nao e primo
    for (unsigned long i = 2; i <= sqrtl(n); i++)
        if (n % i == 0)
            return 0;

    // caso contrario: e primo
    return 1;
}


int main() {

    // variaveis de controle
    int n_entries = 0;
    int n_processes = 0;
    int counter = 0;

    // buffer da entrada
    unsigned long entry;
    unsigned long entries[MAX_ENTRIES];

    // variavel da saida
    int result = 0;

    // memory map entre os processos
    int PROT = PROT_READ | PROT_WRITE;
    int FLAGS = MAP_SHARED | MAP_ANONYMOUS;
    int* primes;

    // buffer dos forks
    pid_t pid;


    // setup memory map
    assert(primes = mmap(NULL, MAX_ENTRIES * sizeof(int), PROT, FLAGS, 0, 0));

    // leitura da entrada
    while (scanf("%lu", &entry) > 0)
        entries[n_entries++] = entry;

    // processamento da entrada
    while (counter < n_entries) {

        // maximo alcancado: espera de um processo finalizar para continuar
        if (n_processes == MAX_PROCESSES) {
            waitpid(-1, NULL, 0);
            n_processes--;
        }

        // cada processo deve processar uma entrada
        pid = fork();
        n_processes++;
        if (pid == 0) {
            primes[counter] = is_prime(entries[counter]);
            exit(0);
        }
        counter++;

    }

    // espera dos processos restantes
    for (int i = 0; i < MAX_PROCESSES; i++)
        waitpid(-1, NULL, 0);


    // impressao da saida
    for (int i = 0; i < n_entries; i++)
        result += primes[i];
    printf("%d\n", result);

    return 0;
}
