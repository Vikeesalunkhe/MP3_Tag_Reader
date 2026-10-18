#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "edit.h"


unsigned int swap(unsigned int val);

/* Removes newline character from string */
void remove_newline(char *str);

void edit_tag(const char *filename,
              const char *tag_flag,
              const char *tag_value)
{
    //Create backup file before modification
    create_backup(filename);

    //Open source MP3 file in binary read mode
    FILE *fp = fopen(filename, "rb");

    if (fp == NULL)
    {
        printf("Error: Cannot open source/sample file\n");
        return;
    }

    //Create temporary file
    FILE *temp = fopen("temp.mp3", "wb");

    if (temp == NULL)
    {
        printf("Error: Cannot create temp file\n");
        fclose(fp);
        return;
    }

    // Copy MP3 header
    char header[10];
    fread(header, 1, 10, fp);
    fwrite(header, 1, 10, temp);

    // Supported ID3 frame IDs
    const char *frames[6] =
    {
        "TIT2",   // Title  */
        "TPE1",   // Artist */
        "TALB",   // Album  */
        "TYER",   // Year   */
        "COMM",   // Comment*/
        "TCON"    // Genre  */
    };

    /* Stores target frame ID */
    char frame_id_to_edit[5];

    // Map tag flag to corresponding frame ID
    if (strcmp(tag_flag, "-t") == 0)
        strcpy(frame_id_to_edit, "TIT2");

    else if (strcmp(tag_flag, "-a") == 0)
        strcpy(frame_id_to_edit, "TPE1");

    else if (strcmp(tag_flag, "-l") == 0)
        strcpy(frame_id_to_edit, "TALB");

    else if (strcmp(tag_flag, "-y") == 0)
        strcpy(frame_id_to_edit, "TYER");

    else if (strcmp(tag_flag, "-c") == 0)
        strcpy(frame_id_to_edit, "COMM");

    else if (strcmp(tag_flag, "-g") == 0)
        strcpy(frame_id_to_edit, "TCON");

    else
    {
        printf("Error: Unknown tag flag %s\n",
               tag_flag);

        fclose(fp);
        fclose(temp);

        return;
    }

    // Loop through all ID3 frames
    for (int i = 0; i < 6; i++)
    {
        char frame_id[5];
        int size;

        ///*----------------------------------------------------------
        //  * Read frame ID
        //  * Example :
        //  *      TIT2
        //  *      TPE1
      
        fread(frame_id, 1, 4, fp);
        frame_id[4] = '\0';

         //  * Read frame size
        fread(&size, 4, 1, fp);
        size = swap(size);

        /*----------------------------------------------------------
         * Read frame flags
         *---------------------------------------------------------*/
        unsigned char flags[2];
        fread(flags, 1, 2, fp);

        /*----------------------------------------------------------
         * Allocate memory for frame data
         * Frame size varies dynamically
         *---------------------------------------------------------*/
        char *data = malloc(size);
        fread(data, 1, size, fp);

        /*----------------------------------------------------------
         * If target frame found
         * Replace old data with new tag value
         *---------------------------------------------------------*/
        if (strcmp(frame_id,
                   frame_id_to_edit) == 0)
        {
            printf("Editing %s...\n", frame_id);

            /*------------------------------------------------------
             * ID3 text frames contain:
             *      1 byte encoding + actual string
             *-----------------------------------------------------*/
            int new_size = strlen(tag_value) + 1;

            /* Write frame ID */
            fwrite(frame_id, 1, 4, temp);

            /* Convert size back to big-endian */
            int be_size = swap(new_size);

            fwrite(&be_size, 4, 1, temp);

            /* Write flags */
            fwrite(flags, 1, 2, temp);

            /* Write encoding byte */
            fputc(0, temp);

            /* Write actual tag value */
            fwrite(tag_value,
                   1,
                   strlen(tag_value),
                   temp);
        }

        /*----------------------------------------------------------
         * Non-target frames copied as it is
         *---------------------------------------------------------*/
        else
        {
            fwrite(frame_id, 1, 4, temp);
            int be_size = swap(size);
            fwrite(&be_size, 4, 1, temp);
            fwrite(flags, 1, 2, temp);
            fwrite(data, 1, size, temp);
        }

        /* Free allocated memory */
        free(data);
    }

    /*--------------------------------------------------------------
     * Close files
     *-------------------------------------------------------------*/
    fclose(fp);
    fclose(temp);

    /*--------------------------------------------------------------
     * Replace original file with modified file
     *-------------------------------------------------------------*/
    remove(filename);
    rename("temp.mp3", filename);
    printf("Tag updated successfully!\n");
}

/*--------------------------------------------------------------------
 * Function : swap
 * Description : Converts 4-byte integer between
 *               big-endian and little-endian format.
 *
 * Parameters :
 *      val -> Integer value
 *
 * Return :
 *      Swapped integer
 *------------------------------------------------------------------*/
unsigned int swap(unsigned int val)
{
    return ((val >> 24) & 0x000000FF) |
           ((val >> 8)  & 0x0000FF00) |
           ((val << 8)  & 0x00FF0000) |
           ((val << 24) & 0xFF000000);
}

/*--------------------------------------------------------------------
 * Function : remove_newline
 * Description : Removes newline character from string.
 *
 * Parameters :
 *      str -> Input string
 *------------------------------------------------------------------*/
void remove_newline(char *str)
{
    int i = 0;

    while (str[i] != '\0')
    {
        if (str[i] == '\n')
        {
            str[i] = '\0';
            break;
        }

        i++;
    }
}

/*--------------------------------------------------------------------
 * Function : create_backup
 * Description : Creates backup copy of original MP3 file.
 *
 * Parameters :
 *      filename -> Original MP3 file
 *
 * Example :
 *      sample.mp3 -> sample.mp3_backup.mp3
 *------------------------------------------------------------------*/
void create_backup(const char *filename)
{
    char backup_name[100];

    /*--------------------------------------------------------------
     * Create backup file name
     *-------------------------------------------------------------*/
    strcpy(backup_name, filename);

    strcat(backup_name, "_backup.mp3");

    /* Open original file */
    FILE *src = fopen(filename, "rb");

    if (src == NULL)
    {
        printf("Error: Cannot open file "
               "to create backup\n");

        return;
    }

    /* Create backup file */
    FILE *dest = fopen(backup_name, "wb");

    if (dest == NULL)
    {
        printf("Error: Cannot create backup file\n");
        fclose(src);
        return;
    }

    char buffer[1024];

    size_t bytes;

    /*--------------------------------------------------------------
     * Copy content from source to backup
     *-------------------------------------------------------------*/
    while ((bytes = fread(buffer,
                          1,
                          sizeof(buffer),
                          src)) > 0)
    {
        fwrite(buffer,
               1,
               bytes,
               dest);
    }

    fclose(src);
    fclose(dest);

    printf("Backup created: %s\n", backup_name);
}