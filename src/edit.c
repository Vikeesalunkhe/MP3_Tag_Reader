#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/edit.h"

unsigned int swap(unsigned int val);

void remove_newline(char *str);

// edit the selected tag with new value
void edit_tag(const char *filename,
              const char *tag_flag,
              const char *tag_value)
{
    //Create backup before modification
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

    // Copy MP3 header (10 bytes)
    char header[10];
    fread(header, 1, 10, fp);
    fwrite(header, 1, 10, temp);

    // supported frame ids
    const char *frames[6] =
    {
        "TIT2",
        "TPE1",
        "TALB",
        "TYER",
        "COMM",
        "TCON"
    };

    char frame_id_to_edit[5];

    // Map tag flag to frame id
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

        // read frame id
        fread(frame_id, 1, 4, fp);
        frame_id[4] = '\0';

        // read frame size and swap it
        fread(&size, 4, 1, fp);
        size = swap(size);

        // read flags
        unsigned char flags[2];
        fread(flags, 1, 2, fp);

        // read frame data
        char *data = malloc(size);
        fread(data, 1, size, fp);

        // target frame found, write new value
        if (strcmp(frame_id,
                   frame_id_to_edit) == 0)
        {
            printf("Editing %s...\n", frame_id);

            // +1 for encoding byte
            int new_size = strlen(tag_value) + 1;

            fwrite(frame_id, 1, 4, temp);

            // convert size back to big endian
            int be_size = swap(new_size);

            fwrite(&be_size, 4, 1, temp);

            fwrite(flags, 1, 2, temp);

            // encoding byte
            fputc(0, temp);

            fwrite(tag_value,
                   1,
                   strlen(tag_value),
                   temp);
        }
        // other frames copy as it is
        else
        {
            fwrite(frame_id, 1, 4, temp);
            int be_size = swap(size);
            fwrite(&be_size, 4, 1, temp);
            fwrite(flags, 1, 2, temp);
            fwrite(data, 1, size, temp);
        }

        free(data);
    }

    fclose(fp);
    fclose(temp);

    // Replace original file with temp file
    remove(filename);
    rename("temp.mp3", filename);
    printf("Tag updated successfully!\n");
}

// big endian <-> little endian
unsigned int swap(unsigned int val)
{
    return ((val >> 24) & 0x000000FF) |
           ((val >> 8)  & 0x0000FF00) |
           ((val << 8)  & 0x00FF0000) |
           ((val << 24) & 0xFF000000);
}

// remove newline from string
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

// create backup copy of mp3 file
void create_backup(const char *filename)
{
    char backup_name[100];

    // backup file name
    strcpy(backup_name, filename);
    strcat(backup_name, "_backup.mp3");

    // open original file
    FILE *src = fopen(filename, "rb");

    if (src == NULL)
    {
        printf("Error: Cannot open file "
               "to create backup\n");

        return;
    }

    // create backup file
    FILE *dest = fopen(backup_name, "wb");

    if (dest == NULL)
    {
        printf("Error: Cannot create backup file\n");
        fclose(src);
        return;
    }

    char buffer[1024];
    size_t bytes;

    // copy data to backup
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
