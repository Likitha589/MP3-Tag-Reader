#ifndef MP3_H
#define MP3_H

// Function to view all MP3 tags (Title, Artist, Album, Genre, Year, Comment, Track)
void view_tags(const char *filename);

// Function to edit a specific MP3 tag
// option -> "-t" (title), "-a" (artist), "-A" (album), "-g" (genre), "-c" (comment), "-y" (year)
void edit_tag(const char *option, const char *new_value, const char *filename);

#endif
