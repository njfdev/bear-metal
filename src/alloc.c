#include "alloc.h"

static volatile heap_table_t heap_table;

void* malloc(unsigned long n) {
  uint32_t blocks_needed = (n/HEAP_BLOCK_SIZE)+1;

  // search for contiguous number of blocks needed
  uint32_t possible_index = 0;
  for (int i = possible_index; i < HEAP_TABLE_LENGTH; i++) {
    // if current block not allocated, try again with next
    if (heap_table[i] != 0) {
      possible_index = i+1;
      continue;
    }

    // if block allocated and long enough, then we can alloc
    if (i-possible_index == blocks_needed-1) {
      break;
    }
  }

  if (possible_index >= HEAP_TABLE_LENGTH) {
    return 0; // nullptr
  }

  // assign block start addresses in table
  uint32_t address = possible_index*HEAP_BLOCK_SIZE + HEAP_START;
  for (int i = possible_index; i < possible_index+blocks_needed; i++) {
    heap_table[i] = address;
  }

  // set everything to 0
  for (int i = address; i < address+n; i++)
    *((char*)(address+i)) = 0;

  return (void*) address;
}

void* realloc(void* orig, unsigned long n) {
  uint32_t orig_address = (uint32_t) orig;
  uint32_t orig_block_count = 0;
  uint32_t orig_table_index = (uint32_t)orig / HEAP_BLOCK_SIZE - HEAP_START;

  // count blocks of orig
  for (int i = orig_table_index; i < HEAP_TABLE_LENGTH; i++) {
    if (heap_table[i] != orig_address) {
      break;
    }
    orig_block_count++;
  }

  void* new = malloc(n);
  uint32_t new_block_count = n/HEAP_BLOCK_SIZE + 1;

  uint32_t bytes_to_copy = orig_block_count <= new_block_count ? orig_block_count : new_block_count;

  // copy bytes of orig block to new address
  for (int i = 0; i < bytes_to_copy; i++) {
    *((char*)new+i) = *((char*)orig+i);
  }

  free(orig);

  return new;
}

void free(void* mem) {
  uint32_t address = (uint32_t) mem;

  uint32_t table_index = (address/HEAP_BLOCK_SIZE - HEAP_START);

  for (int i = table_index; i < HEAP_END && heap_table[i] == address; i++) {
    heap_table[i] = 0;
  }
}