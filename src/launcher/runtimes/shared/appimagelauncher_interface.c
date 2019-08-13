// system
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

// local
#include "appimagelauncher_interface.h"

int tryForwardExecToIntegrationAssistant(int argc, char* const* argv, char* appImagePath) {
    char* assistantPath = strdup("/usr/bin/AppImageLauncher");

    // prepare command args for execv
    int assistantArgsSize = argc + 2; // AppImage path and NULL ending fields are added
    char* assistantArgs[assistantArgsSize];
    assistantArgs[0] = strdup(assistantPath);
    assistantArgs[1] = strdup(appImagePath);
    // place original arguments after the target appImagePath
    for (int i = 1; i < argc; ++i)
        assistantArgs[i + 1] = strdup(argv[i]);

    // add null ending filed
    assistantArgs[assistantArgsSize - 1] = 0;

    pid_t p = fork();
    if (p == -1) {
        perror("assistant fork  failed\n");
        return EXIT_FAILURE;
    } else if (p == 0) {
        execv(assistantPath, assistantArgs);
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

    return EXIT_FAILURE;
}

