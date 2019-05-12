// system
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>

// local
#include "utils.h"
#include "elf.h"

int notify(char* title, char* body, int timeout) {
    /* http://stackoverflow.com/questions/13204177/how-to-find-out-if-running-from-terminal-or-gui */
    if (isatty(fileno(stdin))) {
        /* We were launched from the command line. */
        printf("\n%s\n", title);
        printf("%s\n", body);
    } else {
        /* We were launched from inside the desktop */
        printf("\n%s\n", title);
        printf("%s\n", body);
        /* https://debian-administration.org/article/407/Creating_desktop_notifications */
        void* handle, * n;
        typedef void  (* notify_init_t)(char*);
        typedef void* (* notify_notification_new_t)(char*, char*, char*, char*);
        typedef void  (* notify_notification_set_timeout_t)(void*, int);
        typedef void (* notify_notification_show_t)(void*, char*);
        handle = NULL;
        if (handle == NULL)
            handle = dlopen("libnotify.so.3", RTLD_LAZY);
        if (handle == NULL)
            handle = dlopen("libnotify.so.4", RTLD_LAZY);
        if (handle == NULL)
            handle = dlopen("libnotify.so.5", RTLD_LAZY);
        if (handle == NULL)
            handle = dlopen("libnotify.so.6", RTLD_LAZY);
        if (handle == NULL)
            handle = dlopen("libnotify.so.7", RTLD_LAZY);
        if (handle == NULL)
            handle = dlopen("libnotify.so.8", RTLD_LAZY);

        if (handle == NULL) {
            printf("Failed to open libnotify.\n\n");
        }
        notify_init_t init = (notify_init_t) dlsym(handle, "notify_init");
        if (init == NULL) {
            dlclose(handle);
            return 1;
        }
        init("AppImage");

        notify_notification_new_t nnn = (notify_notification_new_t) dlsym(handle, "notify_notification_new");
        if (nnn == NULL) {
            dlclose(handle);
            return 1;
        }
        n = nnn(title, body, NULL, NULL);
        notify_notification_set_timeout_t nnst = (notify_notification_set_timeout_t) dlsym(handle,
                                                                                           "notify_notification_set_timeout");
        if (nnst == NULL) {
            dlclose(handle);
            return 1;
        }
        nnst(n, timeout);
        notify_notification_show_t show = (notify_notification_show_t) dlsym(handle, "notify_notification_show");
        if (init == NULL) {
            dlclose(handle);
            return 1;
        }
        show(n, NULL);
        dlclose(handle);
    }
    return 0;
}

char* hexlify(const char* bytes, const size_t numBytes) {
    // first of all, allocate the new string
    // a hexadecimal representation works like "every byte will be represented by two chars"
    // additionally, we need to null-terminate the string
    char* hexlified = (char*) calloc((2 * numBytes + 1), sizeof(char));

    for (size_t i = 0; i < numBytes; i++) {
        char buffer[3];
        sprintf(buffer, "%02x", (unsigned char) bytes[i]);
        strcat(hexlified, buffer);
    }

    return hexlified;
}

int appimage_print_binary(char* fname, unsigned long offset, unsigned long length) {
    char* data;
    if ((data = read_file_offset_length(fname, offset, length)) == NULL) {
        return 1;
    }

    printf("%s\n", data);

    free(data);

    return 0;
}

