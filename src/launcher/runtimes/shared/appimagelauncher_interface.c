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

int tryForwardExecToIntegrationAssistant(int argc, char* const* argv, char* appImagePath) {
    char* assistantPath = strdup("/usr/bin/appimage-firstrun");

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

    // modify process environment to avoid drkonki handling the errors
    int assistantEnvSize = 0;
    while (__environ[assistantEnvSize] != NULL)
        ++assistantEnvSize;

    // make rom for an additional element
    assistantEnvSize += 2;

    char** assistantEnv = malloc(sizeof(char*) * assistantEnvSize);
    for (int i = 0; i < assistantEnvSize - 2; ++i)
        assistantEnv[i] = __environ[i];

    assistantEnv[assistantEnvSize - 2] = strdup("KDE_DEBUG=1");
    assistantEnv[assistantEnvSize - 1] = NULL;

    pid_t p = fork();
    if (p == -1) {
        perror("assistant fork  failed\n");
        return EXIT_FAILURE;
    } else if (p == 0) {
        execve(assistantPath, assistantArgs, assistantEnv);
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

int shouldIntegrationAssistantBeUsedOn(const char* target) {
    char* envVar = getenv("APPIMAGELAUNCHER_DISABLE");
    if (envVar != NULL)
        return false;

    // the target path points to the appimage-services binary (path ends with "appimage-services")
    unsigned long targetStrLen = strlen(target);
    if ((targetStrLen >= 17) && (0 == strcmp(target + targetStrLen - 17, "appimage-services")))
        return false;

    if (strncasecmp("/tmp/.mount_", target, 12) == 0)
        return false;

    return true;
}

