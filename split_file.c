#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
static unsigned int file_id;

unsigned int getNextFileIndex () {
	return ++file_id;
}

void UpdateFileName ( char *fileName, char *dstFileName ) {
	if (fileName && dstFileName ) {
		sprintf (dstFileName,"%s_%d",fileName,getNextFileIndex());
	}
}

int splitBySize(char *fileName, int split_size) {
	FILE* openFile = fopen(fileName, "rb+");
	char cp_filename[255];
	FILE* newFile;
	char data;
	int byte_count = 0;
	if (openFile == NULL) {
        printf("File Not Found!\n");
        return -1;
    }

	UpdateFileName(fileName,cp_filename);
	newFile = (FILE* )fopen (cp_filename,"wb+");
	if ( newFile ) {
		while ( (data=fgetc(openFile)) != EOF ) {
			fputc(data, newFile);
			++byte_count;
			if (byte_count > split_size ) {
				fclose (newFile);
				UpdateFileName(fileName,cp_filename);
				newFile = fopen (cp_filename,"wb+");
				if (!newFile) {
					printf ("!!!!File Creation Error %d",__LINE__);
					goto FINISH;
				}
				byte_count = 0;
			}
			
		}
		fclose (newFile);
	} else {
		printf ("!!!!File Creation Error %d",__LINE__);
	}
FINISH:
	fclose(openFile);
	return 0;
}

int splitByLineCount ( char *fileName, int split_line_count ) {
	FILE* openFile = fopen(fileName, "rb+");
	char cp_filename[255];
	FILE* newFile;
	char data;
	int line_count = 0;
	if (openFile == NULL) {
        printf("File Not Found!\n");
        return -1;
    }
	UpdateFileName(fileName,cp_filename);
	newFile = (FILE* )fopen (cp_filename,"wb+");
	if ( newFile ) {
		while ( (data=fgetc(openFile)) != EOF ) {
			fputc(data, newFile);
			if ( data == '\n') {
				++line_count;
				if ( line_count == split_line_count ) {
					fclose (newFile);
					UpdateFileName(fileName,cp_filename);
					newFile = fopen (cp_filename,"wb+");
					if (!newFile) {
						printf ("!!!!File Creation Error %d",__LINE__);
						goto FINISH;
					}
					line_count = 0;
				}
			}
		}
		fclose (newFile);
	} else {
		printf ("!!!!File Creation Error %d",__LINE__);
	}
FINISH:
	fclose(openFile);
	return 0;	
}
  
int main(int argc, char **argv)
{
    char option;
	char *cvalue = NULL;
	char *fileName = argv[1];
	
	
	int split_size = 0;

	while ((option = getopt(argc, argv, ":n:s:")) != -1 ) {
		switch (option) {
			case 'n':
			{
				cvalue = optarg;
				printf  ("line number[%s]\n",cvalue);
				split_size = atoi(cvalue);
				if (split_size) {
					splitByLineCount(fileName,split_size);
				}
			}
			break;
			
			case 's':
			{
				cvalue = optarg;
				split_size = atoi(cvalue);
				printf ("split_size=%d \n",split_size);
				if (split_size) {
					splitBySize(fileName,split_size);
				} else {
					printf ("Enter correct split value\n");
				}
			}
			break;
			
			default:
			{
				printf ("Not a valid option\n");
				return -1;
			}
		}

	}
    return 0;
}
