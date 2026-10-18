#include <stdio.h>
#include "../include/operation.h"
#include "../include/view.h"
#include "../include/edit.h"
#include "../include/display.h"

// main function
int main(int argc, char *argv[])
{
    // Validate cmd line
    OperationType operation = read_and_validate_args(argc, argv);

    switch (operation)
    {
        case e_view:
            // argv[2] -> mp3 file
            display_view_start(argv[2]);
            view_mp3(argv[2]);
            break;

        case e_edit:
            // argv[2] -> tag flag, argv[3] -> new value, argv[4] -> mp3 file
            display_edit_start();
            edit_tag(argv[4], argv[2], argv[3]);

            // show updated details
            display_updated_details();
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
