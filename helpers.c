#include "helpers.h"

/* Global variables definitions */
int file_size = 0x00;
int idat_chunk = 0;
FILE* file = NULL;
fpos_t pos;

/* Functions definitions */
DWORD convert_to_little_endian32(DWORD num)
{
        return ((num & 0xFF000000) >> 24) | 
                ((num & 0x00FF0000) >> 8) | 
                ((num & 0x0000FF00) << 8) | 
                ((num & 0x000000FF) << 24);
}

QWORD convert_to_little_endian64(QWORD num)
{
    return ((num & 0xFF00000000000000) >> 56) | 
        ((num & 0x00FF000000000000) >> 40) |
        ((num & 0x0000FF0000000000) >> 24) | 
        ((num & 0x000000FF00000000) >> 8)  |
        ((num & 0x00000000FF000000) << 8)  |
        ((num & 0x0000000000FF0000) << 24) |
        ((num & 0x000000000000FF00) << 40) |
        ((num & 0x00000000000000FF) << 56);
}

int check_PNG_signature(void)
{
    QWORD sign;
    fread(&sign, sizeof(QWORD), 1, file);

    sign = convert_to_little_endian64(sign);
    if (sign != SIGNATURE)
    {
        printf("%lX\n", sign);
        printf("PNG signature was not found.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void get_file_size(void)
{
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);

    fseek(file, 0, SEEK_SET);
    return;
}

DWORD get_chunk_lenght(FILE* file)
{
    fseek(file, -8 , SEEK_CUR);

    DWORD lenght;
    fread(&lenght, sizeof(DWORD), 1, file);

    fseek(file, 4, SEEK_CUR);
    return convert_to_little_endian32(lenght);
}

BYTE* get_chunk_data(DWORD lenght)
{
    if (lenght == 0)
    {
        return NULL;
    }
    else
    {
        BYTE* data = malloc(sizeof(BYTE) * lenght);
        fread(data, sizeof(BYTE), lenght, file);

        // Get file cursor position to skip bytes that belogns to a chunk data field
        fgetpos(file, &pos);

        return data;
    }
}

PNG_Chunk* create_PNG_Chunk(DWORD type)
{
    PNG_Chunk* chunk = malloc(sizeof(PNG_Chunk));

    // Setting each field for a chunk besides CRC
    chunk->lenght = get_chunk_lenght(file);
    chunk->type = type;
    chunk->data = get_chunk_data(chunk->lenght);

    return chunk;
}

Chunk_Node* create_chunk_node(DWORD type)
{
    Chunk_Node* node = malloc(sizeof(Chunk_Node));
    node->chunk = create_PNG_Chunk(type);
    node->next = NULL;

    return node;
}

void print_chunk_nodes(Chunk_Node* head)
{
    Chunk_Node* tmp = head;
    while (tmp != NULL)
    {
        printf("├── chunk: %p\n", tmp->chunk);
        printf("│   ├── chunk lenght: %i\n", tmp->chunk->lenght);
        printf("│   ├── chunk type: %X\n", tmp->chunk->type);
        printf("│   └── chunk data pointer: %p\n", tmp->chunk->data);
        printf("└── next: %p\n", tmp->next);
        printf("---------------------------------------------\n");
        tmp = tmp->next;
    }

    return;
}

void free_chunk_nodes(Chunk_Node* head)
{
    Chunk_Node* ptr;
    while (head != NULL)
    {
        ptr = head->next;
        free(head->chunk->data);
        free(head->chunk);
        free(head);
        head = ptr;
    }

    return;
}