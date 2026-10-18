/*
Name        : Vikee Salunkhe
Project     : MP3 Tag Reader and Editor (ID3v2)
Description :
    A command-line tool to view and edit the ID3v2 metadata (tags) of MP3 files.
    - View  : reads the ID3v2 header and frames of an MP3 file and displays
              Title, Artist, Album, Year, Genre and Composer.
    - Edit  : updates a selected tag frame with a new value. A backup of the
              original file is created before modifying it, and the updated
              details are displayed afterwards.
    - Help  : prints the list of supported options and flags.
    Invalid or missing arguments are reported with a usage/error message.

    Supported edit flags:
        -t  Title            (TIT2)
        -a  Artist           (TPE1)
        -A  Album            (TALB)
        -y  Year             (TYER)
        -g  Genre / Content  (TCON)
        -c  Composer         (TCOM)

Example of cmd execution:
    View tags   : ./a.out -v test.mp3
    Edit a tag  : ./a.out -e -t "New Title" test.mp3
                  ./a.out -e -a "New Artist" test.mp3
                  ./a.out -e -y 2024 test.mp3
    Help menu   : ./a.out --help
*/

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
