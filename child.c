#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void read_env_file(const char *filename);
void print_env_vars(char *envp[]);

int main(int argc, char *argv[], char *envp[]) {
    printf("Child process: Name=%s, PID=%d, PPID=%d\n", argv[0], getpid(), getppid());

    if (argc > 1) {
        printf("Reading environment from file: %s\n", argv[1]);
        read_env_file(argv[1]);
    } else {
        printf("Reading environment from envp[]\n");
        print_env_vars(envp);
    }

    return 0;
}

void read_env_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open env file");
        return;
    }

    char var[128];
    while (fgets(var, sizeof(var), file)) {
        var[strcspn(var, "\n")] = '\0';
        char *value = getenv(var);
        printf("%s=%s\n", var, value ? value : "(null)");
    }

    fclose(file);
}

void print_env_vars(char *envp[]) {
    for (char **env = envp; *env; env++) {
        printf("%s\n", *env);
    }
}
