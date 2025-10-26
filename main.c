#include <stdio.h>
#include <string.h>
#include "mp3.h"

#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN    "\033[1;36m"
#define WHITE   "\033[1;37m"

void print_help(const char *progname)
{
    printf(MAGENTA"\n===================== MP3_TAG_READER/EDITOR =========================\n"RESET);
    printf(GREEN"USAGE:\n");
    printf("---------------------------------------------------------------------------------\n");
    printf("->  %s -v <file.mp3>\n", progname);
    printf("     To View all tags (Title, Artist, Album, Year, Genre, Comment, Track)\n");
    printf("\n");
    printf("->  %s -e <option> <value> <file.mp3>\n", progname);
    printf("      To Edit a specific tag\n");
    printf("----------------------------------------------------------------------------------\n"RESET);
    printf(WHITE "\n------------OPTIONS FOR -e:--------------\n");
    printf("1.  -t   Edit Title\n");
    printf("2.  -a   Edit Artist\n");
    printf("3.  -A   Edit Album\n");
    printf("4.  -g   Edit Genre\n");
    printf("5.  -c   Edit Comment\n");
    printf("6.  -y   Edit Year\n");
    printf("--------------------------------------------\n\n"RESET);
    printf(GREEN "EXAMPLE:\n");
    printf("  %s -v song.mp3\n", progname);
    printf("  %s -e -t \"New Title\" song.mp3\n"RESET, progname);
    printf(MAGENTA"========================================================================\n\n"RESET);
}

int main(int argc, char *argv[])
{
    if(argc<2)
    {
        printf(RED"ERROR: Missing arguments. Use --help for usage info\n");
        return 1;
    }

    if(strcmp(argv[1],"--help")==0)
    {
        print_help(argv[0]);
        return 0;
    }

    if (strcmp(argv[1], "-v") == 0)
    {
        if(argc!=3)
        {
            printf("ERROR: Invalid Usage. Run with --help for info.\n");
            return 1;
        }
        view_tags(argv[2]);
    }
    else if (strcmp(argv[1], "-e") == 0)
    {
        if (argc != 5)
        {
            printf("ERROR: Invalid Usage. Run with --help for info.\n");
            return 1;
        }
        edit_tag(argv[2], argv[3], argv[4]);
    }
    else
    {
        printf("Error: Unknown option '%s'. Use --help for usage.\n"RESET, argv[1]);
    }

    return 0;
}
