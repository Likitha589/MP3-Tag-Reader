MP3 Tag Reader (C Project)

The MP3 Tag Reader is a C-based utility that extracts and displays metadata from MP3 audio files by parsing the ID3v1 and ID3v2 tags. It demonstrates effective use of file handling, string manipulation, and bitwise operations in C.

-> Features

Reads and displays metadata such as:

Title

Artist

Album

Year

Genre

Supports both ID3v1 and ID3v2 tag formats.

Handles incorrect or missing tags gracefully.

Clean modular design using header and source files.

-> Files Overview

main.c – Handles user input and invokes parsing functions

mp3_tag_reader.c – Core logic for reading and decoding ID3 tags

mp3.h – Structure definitions and function prototypes

display.c – Prints the extracted information

-> Compilation & Execution

To compile and run:

gcc*.c 
./a.out -v sample.mp3

-> Example Output
Title  : Shape of You
Artist : Ed Sheeran
Album  : ÷ (Divide)
Year   : 2017
Genre  : Pop

-> Learning Outcomes

Practiced C file I/O operations and binary data parsing

Understood ID3 tag structure and metadata standards

Improved debugging and low-level data manipulation skills

-> Author

Likitha Pasam
GitHub: Likitha589
