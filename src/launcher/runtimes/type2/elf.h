#pragma once

#include <stdio.h>

ssize_t get_elf_size(const char* fname);

/* Return the offset, and the length of an ELF section with a given name in a given ELF file */
int get_elf_section_offset_and_length(const char* fname, const char* section_name, unsigned long* offset,
                                       unsigned long* length);

int appimage_print_binary(char* fname, unsigned long offset, unsigned long length);

char* read_file_offset_length(const char* fname, unsigned long offset, unsigned long length);
