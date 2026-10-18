#include <stdio.h>
#include "operation.h"
#include <string.h>

#include "view.h"
#include "edit.h"

int main(int argc, char *argv[])
{

    // Validate cmd line
    OperationType operation = read_and_validate_args(argc, argv);


    switch (operation)
    {
        case e_view:
            printf("Viewing mp3 file: %s\n", argv[2]);
            view_mp3(argv[2]);
            break;

        case e_edit:
            printf("-----SELECTED EDIT OPERATION-----\n");
            /*------------------------------------------------------
             * Edit MP3 tag
             *
             * argv[2] -> Tag flag
             * argv[3] -> New tag value
             * argv[4] -> MP3 file
             *-----------------------------------------------------*/            
            edit_tag(argv[4], argv[2], argv[3]);
            /*------------------------------------------------------
             * Display updated MP3 details
             *-----------------------------------------------------*/
            printf("\n--------UPDATED MP3 DETAILS--------\n");
            view_mp3(argv[4]);
            break;

        case e_help:
            display_help();
            break;

        case e_failure:
            display_error_message();
            break;

        default:
            printf("Error: Unknown operation\n");
            return 1;
    }


    return 0;



}
