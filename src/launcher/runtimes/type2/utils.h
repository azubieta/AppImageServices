#pragma once

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>


char* hexlify(const char* bytes, const size_t numBytes);


/* Try to show a notification on the GUI, fall back to the command line
 * timeout is the timeout in milliseconds. timeout = NULL seems to trigger a
 * GUI error dialog rather than a notification */
int notify(char* title, char* body, int timeout);
