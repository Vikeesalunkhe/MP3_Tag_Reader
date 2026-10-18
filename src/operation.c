#include <stdio.h>
#include <string.h>
#include "../include/operation.h"

// check cmd line args and return operation type
OperationType read_and_validate_args(int argc, char *argv[])
{
    // no args
    if (argc == 1)
    {
        return e_failure;
    }

    // ./a.out --help
    if (strcmp(argv[1], "--help") == 0)
    {
        return e_help;
    }

    // ./a.out -v file.mp3
    if (strcmp(argv[1], "-v") == 0)
    {
        if (argc < 3)
        {
            printf("Error: Missing mp3 filename for view operation\n");
            return e_failure;
        }
        return e_view;
    }

    // ./a.out -e <flag> <new value> file.mp3
    if (strcmp(argv[1], "-e") == 0)
    {
        if (argc < 5)
        {
            printf("Error: Missing arguments for edit operation\n");
            return e_failure;
        }

        // check valid tag flag
        if (strcmp(argv[2], "-t") == 0 || strcmp(argv[2], "-a") == 0 ||
            strcmp(argv[2], "-A") == 0 || strcmp(argv[2], "-y") == 0 ||
            strcmp(argv[2], "-g") == 0 || strcmp(argv[2], "-c") == 0)
        {
            return e_edit;
        }
        else
        {
            printf("Error: Invalid flag for edit operation\n");
            return e_failure;
        }
    }

    // If none of the conditions match, return failure
    return e_failure;
}
