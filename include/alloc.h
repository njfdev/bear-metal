#ifndef _ALLOC_H_
#define _ALLOC_H_

// 127 MiB of heap space
#include <stdint.h>
#define HEAP_START 0x01000000
#define HEAP_END 0x80000000
#define HEAP_BLOCK_SIZE 0x1000
#define HEAP_TABLE_LENGTH (HEAP_END-HEAP_START)/HEAP_BLOCK_SIZE

typedef uint32_t heap_table_t[HEAP_TABLE_LENGTH];

void* malloc(unsigned long n);
void* realloc(void* orig, unsigned long n);
void free(void* mem);

#endif