#ifndef HELPERS_H
#define HELPERS_H

/* Libraries */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/* Definitions */
#define SIGNATURE 0x89504E470D0A1A0A
#define IHDR 0x49484452
#define IDAT 0x49444154
#define IEND 0x49454E44

/* Structures */
typedef uint8_t BYTE;
typedef uint32_t DWORD;
typedef uint64_t QWORD;

typedef struct PNG_Chunk
{
    DWORD lenght;
    DWORD type;
    BYTE* data;
} PNG_Chunk;

typedef struct Chunk_Node
{
    PNG_Chunk* chunk;
    struct Chunk_Node* next;
} Chunk_Node;

/* Global variables */
extern int file_size;
extern FILE* file;
extern fpos_t pos;

/* Functions prototypes */
DWORD convert_to_little_endian32(DWORD num);
QWORD convert_to_little_endian64(QWORD num);
int check_PNG_signature(void);
void get_file_size(void);
DWORD get_chunk_lenght(FILE* file);
BYTE* get_chunk_data(DWORD lenght);
PNG_Chunk* create_PNG_Chunk(DWORD type);
Chunk_Node* create_chunk_node(DWORD type);
void print_chunk_nodes(Chunk_Node* head);
void free_chunk_nodes(Chunk_Node* head);

#endif