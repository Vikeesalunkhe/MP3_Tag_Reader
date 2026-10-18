#ifndef DISPLAY_H
#define DISPLAY_H

// all printing functions

void display_help(void);

void display_error_message(void);

void display_view_start(const char *filename);

void display_edit_start(void);

void display_updated_details(void);

void display_tag_details(const char *title, const char *artist,
                         const char *album, const char *year,
                         const char *genre, const char *composer);

#endif // DISPLAY_H
