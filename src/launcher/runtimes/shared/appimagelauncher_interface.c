// system
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>
#include <zconf.h>

// local
#include "appimagelauncher_interface.h"

char** buildAssistantArgList(int argc, char* const* argv, char* assistantPath);

char** buildAssistantEnvp(char** pString);

int tryForwardExecToIntegrationAssistant(int argc, char* const* argv, char** envp) {
    char* assistantPath = getAssistantPath();
    if (assistantPath == NULL)
        return 1;

    pid_t p = fork();
    if (p == -1) {
        perror("assistant fork  failed\n");
        return EXIT_FAILURE;
    } else if (p == 0) {
        char** assistantArgs = buildAssistantArgList(argc, argv, assistantPath);

        char** assistantEnvp = buildAssistantEnvp(envp);
        // avoid drkonki handling the errors
        setenv("KDE_DEBUG", "false", 1);

        execve(assistantPath, assistantArgs, assistantEnvp);
        // if execv fails the new process must be terminated
        exit(EXIT_FAILURE);
    }

    int status = -1;
    if (waitpid(p, &status, 0) == -1) {
        perror("waitpid failed\n");
        return EXIT_FAILURE;
    }

    if (WIFEXITED(status)) {
        const int es = WEXITSTATUS(status);
        return es;
    }

}

char** buildAssistantEnvp(char** envp) {
    int envp_size = 0;
    bool is_kde_debug_set = false;
    // inspect existent env
    for (; envp[envp_size] != NULL; ++envp_size)
        if (strncmp("KDE_DEBUG=", envp[envp_size], 10) == 0)
            is_kde_debug_set = true;

    // count NULL termination
    envp_size++;

    // count the new entry
    if (!is_kde_debug_set)
        envp_size++;

    char** assistantEnvp = calloc(envp_size, sizeof(char*));
    for (int i = 0; envp[i] != NULL; ++i)
        assistantEnvp[i] = strdup(envp[i]);

    assistantEnvp[envp_size - 2] = strdup("KDE_DEBUG=false");
    return assistantEnvp;
}

/**
 * // prepare command args for execv
 * @param argc
 * @param argv
 * @param assistantPath
 * @return NULL terminated string list
 */
char** buildAssistantArgList(int argc, char* const* argv, char* assistantPath) {

    // AppImage path and NULL ending fields are added
    char** assistantArgs = calloc(argc, sizeof(char*));
    assistantArgs[0] = assistantPath;

    // place original arguments after the target appImagePath
    for (int i = 1; i < argc; ++i)
        assistantArgs[i] = strdup(argv[i]);

    return assistantArgs;
}

char* getAssistantPath() {
    if (access("/usr/bin/appimage-firstrun", F_OK) != -1)
        return strdup("/usr/bin/appimage-firstrun");

    if (access("/usr/local/bin/appimage-firstrun", F_OK) != -1)
        return strdup("/usr/local/bin/appimage-firstrun");

    char* binPath = strcat(getenv("HOME"), "/.local/bin/appimage-firstrun");
    if (access(binPath, F_OK) != -1)
        return binPath;

    return NULL;
}

int shouldIntegrationAssistantBeUsedOn(const char* target) {
    char* envVar = getenv("APPIMAGELAUNCHER_DISABLE");
    if (envVar != NULL)
        return false;

    if (strncasecmp("/tmp/.mount_", target, 12) == 0)
        return false;

    return true;
}

