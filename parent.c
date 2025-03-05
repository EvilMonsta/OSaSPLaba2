#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define ENV_FILE "env"
#define MAX_CHILDREN 100
#define CHILD_PATH_ENV "CHILD_PATH"

void read_and_print_env();
void spawn_child(int id, int mode);

int main() {
    char command;
    int child_count = 0;

    read_and_print_env();

    printf("Commands:\n '+' - spawn child (execve with getenv)\n '*' - spawn child (execve with envp)\n 'q' - exit\n");

    while (1) {
        command = getchar();
        if (command == 'q') {
            printf("Exiting parent process...\n");
            break;
        } else if (command == '+') {
            if (child_count < MAX_CHILDREN) {
                spawn_child(child_count++, 0);
            } else {
                printf("Max child processes reached.\n");
            }
        } else if (command == '*') {
            if (child_count < MAX_CHILDREN) {
                spawn_child(child_count++, 1);
            } else {
                printf("Max child processes reached.\n");
            }
        }
    }

    return 0;
}

void read_and_print_env() {
    extern char **environ;
    for (char **env = environ; *env; env++) {
        printf("%s\n", *env);
    }
}

void spawn_child(int id, int mode) {
    char child_path[256];
    char child_name[32];
    char *env_path = getenv(CHILD_PATH_ENV);

    if (!env_path) {
        fprintf(stderr, "CHILD_PATH not set\n");
        return;
    }

    snprintf(child_path, sizeof(child_path), "%s/child", env_path);
    snprintf(child_name, sizeof(child_name), "child_%02d", id);

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork failed");
        return;
    } else if (pid == 0) {
        if (mode == 0) {
            execl(child_path, child_name, ENV_FILE, NULL);
        } else {
            extern char **environ;
            execve(child_path, (char *[]){child_name, NULL}, environ);
        }
        perror("exec failed");
        exit(EXIT_FAILURE);
    } else {
        wait(NULL);
    }
}
