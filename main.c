#include "helpers.h"

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./program input.png\n");
        return EXIT_FAILURE;
    }
    
    file = fopen(argv[1], "rb");   
    if (file == NULL)
    {
        printf("Error to open input.png\n");
        return EXIT_FAILURE;
    }

    if (check_PNG_signature())
    {
        return EXIT_FAILURE;
    }
    
    get_file_size();

    // Finds IHDR, IDAT and IEND chunks
    DWORD buffer;
    Chunk_Node* head = NULL;
    Chunk_Node* ptr;

    while (ftell(file) != file_size)
    {
        fgetpos(file, &pos);
        fread(&buffer, sizeof(DWORD), 1, file);        
        
        buffer = convert_to_little_endian32(buffer);

        if (buffer == IHDR || buffer == IDAT || buffer == IEND)
        {
            if (head == NULL)
            {
                head = create_chunk_node(buffer);
                ptr = head;
            }
            else
            {
                Chunk_Node* node = create_chunk_node(buffer);
                ptr->next = node;
                ptr = node;
            }
        }
        
        fsetpos(file, &pos);
        fseek(file, 1, SEEK_CUR);
    }

    print_chunk_nodes(head);

    /* Free everything */
    free_chunk_nodes(head);
    fclose(file);
    return EXIT_SUCCESS;
}

