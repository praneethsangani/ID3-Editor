//Praneeth Sangani (PRS79)
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

typedef struct ID3 
{
	char id[3];
	char title[30];
	char artist[30];
	char album[30];
	char year[4];
	char comment[28];
	char zero_byte_separator; 
	unsigned char track; 
	char genre_identifier; 
}ID3;

//Prints out the values in a field of stuct
void print_ID3(char* field, int field_length)
{
	int count = 0; 
	while(field[count] != '\0' && count < field_length)
	{
		printf("%c", field[count]);
		count++;
	}
	printf("\n");
}

int main(int argc, char** argv) 
{
	//Case where user didnt enter mp3 file
	if(argc == 1)
	{
		printf("Enter just the name of the MP3 to see its TAG contents\nYou can change the fields of the TAG by doing the following: Ex. ./id3edit test.mp3 -year 1988\n");
	}
	//Case where user entered mp3 file. We will read out the TAG info if it has any
	else if (argc == 2)
	{
		FILE* f = fopen(argv[1], "rb");
		if(f == NULL)
		{
			printf("The file you entered was invalid\n");
			return 1; 
		}
		//Checks offset of struct
		/*printf("tag is at offset %d\n",  (int)offsetof(ID3, tag));
		printf("title is at offset %d\n",  (int)offsetof(ID3, title));
		printf("artist is at offset %d\n",  (int)offsetof(ID3, artist));
		printf("album is at offset %d\n",  (int)offsetof(ID3, album));
		printf("year is at offset %d\n",  (int)offsetof(ID3, year));
		printf("comment is at offset %d\n",  (int)offsetof(ID3, comment));
		printf("zero_byte_separator is at offset %d\n",  (int)offsetof(ID3, zero_byte_separator));
		printf("track_number is at offset %d\n",  (int)offsetof(ID3, track));
		printf("genre_identifier is at offset %d\n",  (int)offsetof(ID3, genre_identifier));*/

		ID3 tag; 
		fseek(f, -sizeof(ID3), SEEK_END); //Seek to end of file
		fread(&tag, 128, 1, f);
		//Prints out TAG info if it is there otherwise lets user know that there isnt a tag
		if(tag.id[0] != 'T' || tag.id[1] != 'A' || tag.id[2] != 'G')
		{
			printf("The File Doesn't Have A Tag\n");
		}
		else
		{
			printf("Title: "); print_ID3(tag.title, 30);
			printf("Artist: ");print_ID3(tag.artist, 30);
			printf("Album: ");print_ID3(tag.album, 30);
			printf("Year: ");print_ID3(tag.year, 4);
			printf("Comment: ");print_ID3(tag.comment, 28);
			printf("Track: ");printf("%u\n", tag.track);
		}
		fclose(f);
	}
	//Case where user wants to change tag info or add a tag. 
	else
	{
		FILE* f = fopen(argv[1], "rb+");
		if(f == NULL)
		{
			printf("The file you entered was invalid\n");
			return 1; 
		}

		ID3 tag; 
		int has_tag = 1; 
		int count = 3; 
		fseek(f, -sizeof(ID3), SEEK_END);
		fread(&tag, 128, 1, f);

		//Checks for tag
		if(tag.id[0] != 'T' || tag.id[1] != 'A' || tag.id[2] != 'G')
		{
			has_tag = 0;
		}
		//Makes a new tag 
		if(has_tag == 0)
			{
				tag.id[0] = 'T'; tag.id[1] = 'A'; tag.id[2] = 'G'; 
				memset(tag.title, '\0', 30);
				memset(tag.artist, '\0', 30);
				memset(tag.album, '\0', 30);
				memset(tag.year, '\0', 4);
				memset(tag.comment, '\0', 28);
				tag.genre_identifier = 0; 
				tag.track = '\0';
				has_tag = 1; 
			}
		
		//Sets new info that the user entered on the command line into the TAG
		while(count < argc)
		{
			if(strcmp(argv[count - 1], "-title") == 0)
			{
				memset(tag.title, '\0', 30);
				strcpy(tag.title, argv[count]);
			}
			fwrite(&tag, 1, sizeof(tag), f);
			if(strcmp(argv[count - 1], "-artist") == 0)
			{
				memset(tag.artist, '\0', 30);
				strcpy(tag.artist, argv[count]);
			}
			fwrite(&tag, 1, sizeof(tag), f);
			if(strcmp(argv[count - 1], "-album") == 0)
			{
				memset(tag.album, '\0', 30);
				strcpy(tag.album, argv[count]);
			}
			fwrite(&tag, 1, sizeof(tag), f);
			if(strcmp(argv[count - 1], "-year") == 0)
			{
				memset(tag.year, '\0', 4);
				strcpy(tag.year, argv[count]);
			}
			fwrite(&tag, 1, sizeof(tag), f);
			if(strcmp(argv[count - 1], "-comment") == 0)
			{
				memset(tag.comment, '\0', 28);
				strcpy(tag.comment, argv[count]);
			}
			if(strcmp(argv[count - 1], "-track") == 0)
			{
				tag.track = atoi(argv[count]);
			}
			fwrite(&tag, 1, sizeof(tag), f);
			count = count + 2;  
		}
		fclose(f);
	}
	return 0; 
}