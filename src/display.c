#include <stdio.h>
#include "../include/display.h"

// help menu
void display_help(void)
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

// print usage if args are wrong
void display_error_message(void)
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

// view heading
void display_view_start(const char *filename)
{
    printf("Viewing mp3 file: %s\n", filename);
}

// edit heading
void display_edit_start(void)
{
    printf("-----SELECTED EDIT OPERATION-----\n");
}

// heading after edit
void display_updated_details(void)
{
    printf("\n--------UPDATED MP3 DETAILS--------\n");
}

// print all tag details
void display_tag_details(const char *title, const char *artist,
                         const char *album, const char *year,
                         const char *genre, const char *composer)
{
    printf("---------------------------------------\n");
    printf("MP3 TAG READER AND EDITOR FOR ID3V2\n");
    printf("----------------------------------------\n");
    printf("Title : %s\n", title);
    printf("Artist : %s\n", artist);
    printf("Album : %s\n", album);
    printf("Year : %s\n", year);
    printf("Genre : %s\n", genre);
    printf("Composer : %s\n", composer);
    printf("-----------------------------------------\n");
}
