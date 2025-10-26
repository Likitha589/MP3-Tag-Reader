#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mp3.h"

#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN    "\033[1;36m"
#define WHITE   "\033[1;37m"

char *decode(char *data,int size)
{
    unsigned char encoding =data[0];
    if (encoding==0x00 || encoding==0x03)
    {
        return strdup(data+1);
    }
    else if(encoding==0x01 || encoding ==0x02)
    {
        unsigned char * utf16=(unsigned char *)(data+1);
        int len=size-1;

        if(len>=2 && ((utf16[0]==0xFF && utf16[1]==0xFE)||(utf16[0]==0xFE && utf16[1]==0xFF)))
        {
            utf16+=2;
            len-=2;
        }

        char *ascii=malloc(len/2+1);
        int j;
        for(j=0;j<len;j+=2)
        {
            ascii[j/2]=utf16[j];
        }
        ascii[len/2]='\0';
        return ascii;
    }
    return strdup("");

}
void view_tags(const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp)
    {
        perror(RED "Error");
        return;
    }

    char header[10];
    if (fread(header, 1, 10, fp) < 10)
    {
        printf("Error: Cannot read header\n");
        fclose(fp);
        return;
    }

    if (strncmp(header, "ID3", 3) != 0)
    {
        printf("Error: ID3v2 tag not found\n"RESET);
        fclose(fp);
        return;
    }

    printf(MAGENTA"\n=============== VIEWING MP3 TAGS =================\n"RESET);

    while (1)
    {
        long pos = ftell(fp);
        char id[5] = {0};
        unsigned char size_bytes[4];
        unsigned char flags[2];

        if (fread(id, 1, 4, fp) < 4) break;
        if (fread(size_bytes, 1, 4, fp) < 4) break;
        if (fread(flags, 1, 2, fp) < 2) break;

        if (id[0] == 0) break;

        unsigned int size = 0;
        unsigned char *ptr = (unsigned char *)&size;
        ptr[0] = size_bytes[3];
        ptr[1] = size_bytes[2];
        ptr[2] = size_bytes[1];
        ptr[3] = size_bytes[0];

        // Skip frames with invalid size
        if (size <= 0)
        {
            fseek(fp, size, SEEK_CUR);
            continue;
        }

        // Only read text frames starting with 'T' or comment 'COMM'
        if (id[0] == 'T' || strcmp(id, "COMM") == 0)
        {
            char *data = malloc(size);
            fread(data, 1, size, fp);

            char *decoded=decode(data,size);
            // Display based on frame ID
            if (strcmp(id, "TIT2") == 0)
                printf(WHITE"Title   : %s\n"RESET, decoded);
            else if (strcmp(id, "TPE1") == 0)
                printf(WHITE"Artist  : %s\n", decoded);
            else if (strcmp(id, "TALB") == 0)
                printf(WHITE"Album   : %s\n", decoded);
            else if (strcmp(id, "TCON") == 0)
                printf(WHITE"Genre   : %s\n", decoded);
            else if (strcmp(id, "TYER") == 0)
            {
                decoded[4]='\0';
                printf(WHITE"Year    : %s\n", decoded);
            }
            else if (strcmp(id, "COMM") == 0)
                printf(WHITE"Comment : %s\n"RESET, decoded);
            // else you can skip printing unknown Txxx frames if you want
            free(data);
            free(decoded);
        }
        else
        {
            fseek(fp, size, SEEK_CUR);
        }
    }

    printf(MAGENTA"=====================================================\n\n"RESET);
    fclose(fp);
}

void edit_tag(const char *option, const char *new_value, const char *filename)
{
    FILE *fp = fopen(filename, "r+b");
    if (!fp)
    {
        perror("Error");
        return;
    }

    char header[10];
    if (fread(header, 1, 10, fp) < 10)
    {
        printf("Error: Cannot read header\n");
        fclose(fp);
        return;
    }

    if (strncmp(header, "ID3", 3) != 0)
    {
        printf("Error: ID3v2 tag not found\n");
        fclose(fp);
        return;
    }

    const char *tag = NULL;
    if (strcmp(option, "-t") == 0)
        tag = "TIT2";
    else if (strcmp(option, "-a") == 0)
        tag = "TPE1";
    else if (strcmp(option, "-A") == 0)
        tag = "TALB";
    else if (strcmp(option, "-g") == 0)
        tag = "TCON";
    else if (strcmp(option, "-c") == 0)
        tag = "COMM";
    else if (strcmp(option, "-y") == 0)
        tag = "TYER";

    if (!tag)
    {
        printf("Error: Unknown option (use -t/-a/-A/-g/-c/-y)\n");
        fclose(fp);
        return;
    }

    while (1)
    {
        long pos = ftell(fp);
        char id[5] = {0};
        unsigned char size_bytes[4];
        unsigned char flags[2];

        if (fread(id, 1, 4, fp) < 4) break;
        if (fread(size_bytes, 1, 4, fp) < 4) break;
        if (fread(flags, 1, 2, fp) < 2) break;

        if (id[0] == 0) break;

        unsigned int size = 0;
        unsigned char *ptr = (unsigned char *)&size;
        ptr[0] = size_bytes[3];
        ptr[1] = size_bytes[2];
        ptr[2] = size_bytes[1];
        ptr[3] = size_bytes[0];

        if (strcmp(id, tag) == 0)
        {
            fseek(fp, pos + 10, SEEK_SET);
            unsigned char encoding = 0;
            fwrite(&encoding, 1, 1, fp);

            int max_size = size - 1;
            int len = strlen(new_value);
            if (len > max_size)
                len = max_size;

            fwrite(new_value, 1, len, fp);

            int padding = max_size - len;
            if (padding > 0)
            {
                char *pad = calloc(padding, 1);
                fwrite(pad, 1, padding, fp);
                free(pad);
            }

            printf(GREEN"Successfully updated %s -> %s\n"RESET, tag, new_value);
            break;
        }
        else
        {
            fseek(fp, size, SEEK_CUR);
        }
    }
    fclose(fp);
    view_tags(filename);
}
