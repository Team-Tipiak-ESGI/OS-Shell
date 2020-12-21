#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

#define COMMAND_SIZE 64
#define PATH_MAX 256

int countCharacters(const char * str, const char character) {
    int count = 0;

    for (unsigned int i = 0; str[i]; i++) {
        count += (str[i] == character);
    }

    return count;
}

int main(int argc, char **argv) {

    while (1) {

        // Get command
        char * command = malloc(COMMAND_SIZE);

        char cwd[PATH_MAX];
        getcwd(cwd, sizeof(cwd));

        fputs(cwd, stdout);
        fputs(" > ", stdout);
        fgets(command, COMMAND_SIZE, stdin);

        strtok(command, "\n"); // Remove \n from command

        // Space count + final argument + NULL
        const int arguments_count = countCharacters(command, ' ') + 2;
        char * arguments[arguments_count];

        // Get all arguments and place them in arguments array
        char * token = strtok(command, " ");
        unsigned int i = 0;
        while (token != NULL) {
            arguments[i++] = token;
            token = strtok(NULL, " ");
        }

        arguments[i] = NULL;

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

