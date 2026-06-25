#include <stdio.h>
#include <stdlib.h>
#include "parser.h" // Importa apenas o .h

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Uso: ./dispatcher <processes.txt> <files.txt> <string.txt>\n");
        return 1;
    }

    char *processes_file = argv[1];
    char *files_file = argv[2];
    char *strings_file = argv[3];

    // Carrega os dados na ordem correta
    parse_processes(processes_file);
    parse_strings(strings_file); 
    parse_files(files_file);

    // Teste rápido para ver se leu tudo
    printf("Dados carregados!\n");
    printf("Total de Processos: %d\n", total_processes);
    printf("Total de Blocos no Disco: %d\n", total_disk_blocks);

    return 0;
}