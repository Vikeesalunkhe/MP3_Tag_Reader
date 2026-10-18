#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "view.h"

/*--------------------------------------------------------------------
 * Function : view_mp3
 
 * Logic :
 *      - Validate .mp3 file
 *      - Check ID3 header
 *      - Read frames one by one
 *      - Extract and display tag values
 *------------------------------------------------------------------*/
void view_mp3(const char *filename)
{
    //Validate .mp3 extension
    if (strstr(filename, ".mp3") == NULL)
    {
        printf("Error: Not an MP3 file.\n");
        return;
    }

    // Open MP3 file in binary mode
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        printf("Error: Unable to open file.\n");
        return;
    }

    // Check ID3 Header
    char header[4] = {0};
    fread(header, 1, 3, fp);
    if (strcmp(header, "ID3") != 0)
    {
        printf("Error: No ID3 header found.\n");
        fclose(fp);
        return;
    }

    // Check version
    // Supports only ID3v2.3
    unsigned char ver[2];
    fread(ver, 1, 2, fp);
    if (ver[0] != 3)
    {
        printf("Error: Only ID3v2.3 supported.\n");
        fclose(fp);
        return;
    }

     // Read flags and tag size
    unsigned char flags;
    fread(&flags, 1, 1, fp);
    /* Synchsafe integer bytes */
    unsigned char size_bytes[4];
    fread(size_bytes, 1, 4, fp);

     // Convert synchsafe integer into actual tag size
    int tag_size =
        (size_bytes[0] << 21) |
        (size_bytes[1] << 14) |
        (size_bytes[2] << 7)  |
        (size_bytes[3]);

    /* Current position after header */
    long tag_start = ftell(fp);

    /* End position of tag section */
    long tag_end = tag_start + tag_size;

    //Buffers to store extracted tag values
    char title[100] = "";
    char artist[100] = "";
    char album[100] = "";
    char year[10] = "";
    char genre[50] = "";
    char composer[50] = "";

     // Read frames until tag section ends
    while (ftell(fp) < tag_end)
    {
        char frame_id[5] = {0};
        fread(frame_id, 1, 4, fp);

         // Null frame indicates no more frames
        if (frame_id[0] == 0)
            break;

        // Read frame size
        // 
        unsigned char byte_size[4];
        fread(byte_size, 1, 4, fp);

        /* Convert big-endian size */
        int size =
            (byte_size[0] << 24) |
            (byte_size[1] << 16) |
            (byte_size[2] << 8)  |
            (byte_size[3]);

        // Skip frame flags (2 bytes)
        fseek(fp, 2, SEEK_CUR);

        /*----------------------------------------------------------
         * Ignore invalid / empty frames
         *---------------------------------------------------------*/
        if (size <= 1)
        {
            fseek(fp, size, SEEK_CUR);
            continue;
        }

        /*----------------------------------------------------------
         * Allocate memory for frame data
         *---------------------------------------------------------*/
        char *data = malloc(size + 1);
        fread(data, 1, size, fp);
        data[size] = '\0';

        /*----------------------------------------------------------
         * First byte is encoding
         * Actual text starts from data[1]
         *---------------------------------------------------------*/
        char *value = data + 1;
        /*----------------------------------------------------------
         * Identify frame and store value
         *---------------------------------------------------------*/
        if (strcmp(frame_id, "TIT2") == 0)
        {
            strcpy(title, value);
        }

        else if (strcmp(frame_id, "TPE1") == 0)
        {
            strcpy(artist, value);
        }

        else if (strcmp(frame_id, "TALB") == 0)
        {
            strcpy(album, value);
        }

        else if (strcmp(frame_id, "TYER") == 0)
        {
            strcpy(year, value);
        }

        else if (strcmp(frame_id, "TCON") == 0)
        {
            strcpy(genre, value);
        }

        else if (strcmp(frame_id, "TCOM") == 0)
        {
            strcpy(composer, value);
        }

        /*----------------------------------------------------------
         * Handle COMMENT frame
         * Format :
         *      Encoding
         *      Language (3 bytes)
         *      Description
         *      Actual comment text
         *---------------------------------------------------------*/
        else if (strcmp(frame_id, "COMM") == 0)
        {
            int index = 1;

            /* Skip language bytes */
            index += 3;

            /* Skip description */
            while (index < size &&
                   data[index] != '\0')
            {
                index++;
            }

            /* Skip null byte */
            index++;

            /* Copy actual comment */
            if (index < size)
            {
                strcpy(composer,
                       data + index);
            }
        }

        /* Free allocated memory */
        free(data);
    }

    /*--------------------------------------------------------------
     * Display extracted MP3 tag details
     *-------------------------------------------------------------*/
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

    fclose(fp);
}