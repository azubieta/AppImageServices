/**************************************************************************

Copyright (c) 2004-16 Simon Peter
Copyright (c) 2007 Alexander Larsson

All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

**************************************************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <glib.h>


/* ======================================================== Start helper functions for icon extraction */
/*
Constructs the name of the thumbnail image for $HOME/.thumbnails for the executable that is itself
See http://people.freedesktop.org/~vuntz/thumbnail-spec-cache/
Partly borrowed from
http://www.google.com/codesearch#n76pnUnMG18/trunk/blender/imbuf/intern/thumbs.c&q=.thumbnails/normal%20lang:c%20md5&type=cs
*/

#include <ctype.h>
#include <time.h>

#include "md5.h"
#include "appimagelauncher_interface.h"

#define FILE_MAX                                 240
#define URI_MAX FILE_MAX*3 + 8

/* --- begin of adapted code from glib ---
 * The following code is adapted from function g_escape_uri_string from the gnome glib
 * Source: http://svn.gnome.org/viewcvs/glib/trunk/glib/gconvert.c?view=markup
 * released under the Gnu General Public License.
 * NOTE THIS DOESN'T WORK PROPERLY FOR öäüß - FIXME
 */

typedef enum {
    UNSAFE_ALL = 0x1,  /* Escape all unsafe characters   */
    UNSAFE_ALLOW_PLUS = 0x2,  /* Allows '+'  */
    UNSAFE_PATH = 0x8,  /* Allows '/', '&', '=', ':', '@', '+', '$' and ',' */
    UNSAFE_HOST = 0x10, /* Allows '/' and ':' and '@' */
    UNSAFE_SLASHES = 0x20  /* Allows all characters except for '/' and '%' */
} UnsafeCharacterSet;

static const unsigned char acceptable[96] = {
        /* A table of the ASCII chars from space (32) to DEL (127) */
        0x00, 0x3F, 0x20, 0x20, 0x28, 0x00, 0x2C, 0x3F, 0x3F, 0x3F, 0x3F, 0x2A, 0x28, 0x3F, 0x3F, 0x1C,
        0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x38, 0x20, 0x20, 0x2C, 0x20, 0x20,
        0x38, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
        0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x20, 0x20, 0x20, 0x20, 0x3F,
        0x20, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
        0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x20, 0x20, 0x20, 0x3F, 0x20
};

static const char hex[17] = "0123456789abcdef";

void escape_uri_string(const char* string, char* escaped_string, int len, UnsafeCharacterSet mask) {
#define ACCEPTABLE(a) ((a)>=32 && (a)<128 && (acceptable[(a)-32] & use_mask))

    const char* p;
    char* q;
    int c;
    UnsafeCharacterSet use_mask;
    use_mask = mask;

    for (q = escaped_string, p = string; (*p != '\0') && len; p++) {
        c = (unsigned char) *p;
        len--;

        if (!ACCEPTABLE (c)) {
            *q++ = '%'; /* means hex coming */
            *q++ = hex[c >> 4];
            *q++ = hex[c & 15];
        } else {
            *q++ = *p;
        }
    }

    *q = '\0';
}

void to_hex_char(char* hexbytes, const unsigned char* bytes, int len) {
    const unsigned char* p;
    char* q;

    for (q = hexbytes, p = bytes; len; p++) {
        const unsigned char c = (unsigned char) *p;
        len--;
        *q++ = hex[c >> 4];
        *q++ = hex[c & 15];
    }
}

/* --- end of adapted code from glib --- */

static int uri_from_filename(const char* dir, char* newuri) {
    char uri[URI_MAX];
    sprintf(uri, "file://%s", dir);
    char newstring[URI_MAX];
    strncpy(newstring, uri, URI_MAX);
    newstring[URI_MAX - 1] = 0;
    unsigned int i = 0;
    escape_uri_string(newstring, newuri, FILE_MAX * 3 + 8, UNSAFE_PATH);
    return 1;
}


static void thumbname_from_uri(const char* uri, char* thumb) {
    char hexdigest[33];
    MD5_HASH digest;
    Md5Calculate(uri, strlen(uri), &digest);
    hexdigest[0] = '\0';
    to_hex_char(hexdigest, digest.bytes, 16);
    hexdigest[32] = '\0';
    sprintf(thumb, "%s.png", hexdigest);

}

/* ======================================================== End helper functions for icon extraction */

extern int ext2_main(int argc, char* argv[], void (* mounted)(void));

extern void ext2_quit(void);

static pid_t fuse_pid;
static int keepalive_pipe[2];

static void*
write_pipe_thread(void* arg) {
    char c[32];
    int res;
//  sprintf(stderr, "Called write_pipe_thread");
    memset(c, 'x', sizeof(c));
    while (1) {
        /* Write until we block, on broken pipe, exit */
        res = write(keepalive_pipe[1], c, sizeof(c));
        if (res == -1) {
            kill(fuse_pid, SIGHUP);
            break;
        }
    }
    return NULL;
}

void
run_when_fuse_fs_mounted(void) {

//    sprintf(stderr, "Called run_when_fuse_fs_mounted");
    pthread_t thread;
    int res;

    fuse_pid = getpid();
    res = pthread_create(&thread, NULL, write_pipe_thread, keepalive_pipe);
}

char* getArg(int argc, char* argv[], char chr) {
    int i;
    for (i = 1; i < argc; ++i)
        if ((argv[i][0] == '-') && (argv[i][1] == chr))
            return &(argv[i][2]);
    return NULL;
}


int
main(int argc, char* argv[], char** envp) {
    int dir_fd, res;
    char mount_dir[] = "/tmp/.mount_XXXXXX";  /* create mountpoint */
    char filename[100]; /* enough for mount_dir + "/AppRun" */
    pid_t pid;
    char** real_argv;
    int i;

    char appimage_path[PATH_MAX];
    char argv0_path[PATH_MAX];

    if (argc < 2) {
        fprintf(stderr, "Missing target appimage\n");
        fprintf(stderr, "Usage: %s <appimage path> [args]\n", argv[0]);
        exit(1);
    }

    int envp_size = 0;
    for (char** envp_itr = envp; *envp_itr != NULL; ++envp_itr)
        envp_size++;

    char** envp_copy = calloc(envp_size + 1, sizeof(char*));
    for (int i = 0; i < envp_size; i++)
        envp_copy[i] = strdup(envp[i]);


    // Allow to hook up an integration assistant.
    if (shouldIntegrationAssistantBeUsedOn(argv[1]) &&
        tryForwardExecToIntegrationAssistant(argc, argv, envp_copy) == 0) {
        //  A '0' return value means that the assistant took care of the execution therefore we can safely exit
        fprintf(stdout, "AppImage execution was handled by the integration assistant\n");
        return 0;
    }


    strcpy(appimage_path, argv[1]);
    strcpy(argv0_path, argv[1]);
    setenv("TARGET_APPIMAGE", appimage_path, 1);
    setenv("DESKTOPINTEGRATION", "false", 1);

#ifdef ENABLE_SETPROCTITLE
    // load libbsd dynamically to change proc title
    // this is an optional feature, therefore we don't hard require it
    void* libbsd = dlopen("libbsd.so", RTLD_NOW);

    if (libbsd != NULL) {
        // clear error state
        dlerror();

        // try to load the two required symbols
        void (*setproctitle_init)(int, char**, char**) = dlsym(libbsd, "setproctitle_init");

        char* error;

        if ((error = dlerror()) == NULL) {
            void (*setproctitle)(const char*, char*) = dlsym(libbsd, "setproctitle");

            if (dlerror() == NULL) {
                char buffer[1024];
                strcpy(buffer, getenv("TARGET_APPIMAGE"));
                for (int i = 1; i < argc; i++) {
                    strcat(buffer, " ");
                    strcat(buffer, argv[i]);
                }

                (*setproctitle_init)(argc, argv, environ);
                (*setproctitle)("%s", buffer);
            }
        }

        dlclose(libbsd);
    }
#endif


    if (mkdtemp(mount_dir) == NULL) {
        exit(1);
    }

    if (pipe(keepalive_pipe) == -1) {
        perror("pipe error: ");
        exit(1);
    }

    pid = fork();
    if (pid == -1) {
        perror("fork error: ");
        exit(1);
    }

    if (pid == 0) {
        /* in child */

        char* child_argv[5];

        /* close read pipe */
        close(keepalive_pipe[0]);


        char* dir = realpath(appimage_path, NULL);

        child_argv[0] = dir;
        child_argv[1] = mount_dir;
        child_argv[2] = "-o";
        child_argv[3] = "ro";
        child_argv[4] = NULL;

        ext2_main(4, child_argv, NULL);
    } else {
        /* in parent, child is $pid */
        int c;

        /* close write pipe */
        close(keepalive_pipe[1]);

        /* Pause until mounted */
        read(keepalive_pipe[0], &c, 1);


        dir_fd = open(mount_dir, O_RDONLY);
        if (dir_fd == -1) {
            // perror ("open dir error: ");
            printf("Could not mount AppImage\n");
            printf("Please see https://github.com/probonopd/AppImageKit/wiki/FUSE\n");
            exit(1);
        }

        res = dup2(dir_fd, 1023);
        if (res == -1) {
            perror("dup2 error: ");
            exit(1);
        }
        close(dir_fd);

        strcpy(filename, mount_dir);
        strcat(filename, "/AppRun");

        real_argv = malloc(sizeof(char*) * (argc + 1));
        for (i = 0; i < argc; i++) {
            real_argv[i] = argv[i];
        }
        real_argv[i] = NULL;

        int length;
        char fullpath[FILE_MAX];
        length = readlink("/proc/self/exe", fullpath, sizeof(fullpath));
        fullpath[length] = '\0';

        /* Setting some environment variables that the app "inside" might use */
        setenv("APPIMAGE", fullpath, 1);
        setenv("APPDIR", mount_dir, 1);
        setenv("DESKTOPINTEGRATION", "False", 1);

        /* Original working directory */
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            setenv("OWD", cwd, 1);
        }

        execv(filename, real_argv);
        /* Error if we continue here */
        perror("execv error: ");
        exit(1);
    }

    return 0;
}
