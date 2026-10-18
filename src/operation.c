#include "operation.h"
#include <stdio.h>
#include <string.h>



OperationType read_and_validate_args(int argc, char *argv[]) 
{

    if (argc== 1)
    {
        return e_failure;
    }

    if (strcmp(argv[1], "--help") == 0)
    {
        return e_help;
    }

    if (strcmp(argv[1], "-v") == 0)
    {
        if (argc < 3)
        {
            printf("Error: Missing mp3 filename for view operation\n");
            return e_failure;
        }
        return e_view;
    }


    if (strcmp(argv[1], "-e") == 0)
    {
        if (argc < 5)
        {
            printf("Error: Missing arguments for edit operation\n");
            return e_failure;
        }
        

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

    return e_failure;  // If none of the conditions match, return failure

}


void display_help()
{
    printf("\n-------------HELP MENU-----------------\n");
    printf("1. -v -to view mp3 file contents\n");
    printf("2. -e -to edit mp3 file contents\n");
    printf("Flags:\n");
    printf("2.1  -t  Title (TIT2)\n");
    printf("2.2  -a  Artist (TPE1)\n");
    printf("2.3  -A  Album (TALB)\n");
    printf("2.4  -y  Year (TYER)\n");
    printf("2.5  -g  Genre/Content (TCON)\n");
    printf("2.6  -c  Composer (TCOM/COMM)\n");
    printf("------------------------------------------\n");
}



void display_error_message()
{
    printf("\n-----------------ERROR MESSAGE-------------\n");
    printf("ERROR : ./a.out : Invalid Arguments\n");
    printf("USAGE:\n");
    printf("To view please pass like : "
           "./a.out -v mp3 filename\n");
    printf("To edit please pass like : "
           "./a.out -e -t|-a|-A|-m|-y|-c "
           "changing -text mp3 filename\n");
    printf("To get help pass like : "
           "./a.out --help\n");
    printf("-----------------------------------------------\n");
}