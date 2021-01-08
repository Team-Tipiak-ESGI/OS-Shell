// DOC : Mode canonique, ioctl, mode canonique / brut du terminal, gestion des signaux
// management du terminal

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

#define COMMAND_SIZE 64
#define PATH_MAX 256
#define TRUE 1

int main(void) {

    // Get command
    char *command = malloc(COMMAND_SIZE);

    char **arguments = malloc(COMMAND_SIZE * 4);

    while (TRUE) {

        char cwd[PATH_MAX];
        getcwd(cwd, sizeof(cwd));

        fprintf(stdout,"%s >", cwd);
        fgets(command, COMMAND_SIZE, stdin);

        unsigned long len = strlen(command);
        command[len -1] = '\0';

        // Space count + final argument + NULL
        // Get all arguments and place them in arguments array
        char *token = strtok(command, " ");
        unsigned int i = 0;
        while (token != NULL) {
            *(arguments + i++) = token;
            token = strtok(NULL, " ");
        }

        *(arguments + i)  = NULL;

        // TODO: Faire un switch case
        if (strcmp(arguments[0], "exit") == 0) {
            exit(1);
        } else if (strcmp(arguments[0], "cd") == 0) {

            chdir(arguments[1]);
            continue;
        }
        // Fork
        pid_t id = fork();

        if (id > 0) {
            /* Parent process */
            wait(NULL);
        } else if (id == 0) {
            /* Child process */
            // Exec command
            int err = execvp(arguments[0], arguments);
            if (err == -1) {
                perror("Command failed");
                exit(EXIT_FAILURE);
            }
        } else {
            /* Fork creation failed */
            printf("Fork creation failed!\n");
            break;
        }
    }

    return EXIT_SUCCESS;
}

