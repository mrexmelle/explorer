
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(push, 1)
typedef struct {
	char iName[0xFF];
	unsigned char iType;
}	SEntry;
#pragma pack(pop)

static SEntry gFileList[0xFFFF];

int main_simple(int argc, char * argv[])
{
	if(argc<2)
	{
		printf("usage: %s <dir to explore>\n", argv[0]);
		return -1;
	}

	struct dirent * entry=NULL;
	DIR * dp=opendir(argv[1]);
	if(dp==NULL) { return -2; }

	entry=readdir(dp);
	while(entry)
	{
		if(entry->d_type!=DT_DIR) { printf("main - %s\n", entry->d_name); }
		entry=readdir(dp);
	}

	closedir(dp);

	return 0;
}

int main_recursive_scandir(const char * aDirPath, SEntry * aFileList, int * aCount)
{
	int status, entryCnt, fileListIdx;
	struct dirent ** nameList;

	entryCnt=scandir(aDirPath, &nameList, NULL, alphasort);
	if(entryCnt<0) { return -1; }

	if(entryCnt>0)
	{
		int i;
		for(i=0; i<entryCnt; i++)
		{
			// ensure listed entry is not this (.) or parent (..) directory
			if(strcmp(nameList[i]->d_name, ".")!=0 && strcmp(nameList[i]->d_name, "..")!=0)
			{
				memset(aFileList[*aCount].iName, 0x0, 0xFF);
				// set directory name
				strcpy(aFileList[*aCount].iName, aDirPath);
				// append '/'
				if(aDirPath[strlen(aDirPath)-1]!='/') { strcat(aFileList[*aCount].iName, "/"); }
				// append filename
				strcat(aFileList[*aCount].iName, nameList[i]->d_name);

				// set type
				aFileList[*aCount].iType=nameList[i]->d_type;

				// increment count
				(*aCount)++;

				// look deeper if file type is directory
				if(aFileList[*aCount-1].iType & DT_DIR)
				{
					main_recursive_scandir(aFileList[*aCount-1].iName, aFileList, aCount);
				}
			}

			free(nameList[i]);
			nameList[i]=NULL;
		}
	}

	free(nameList);
	nameList=NULL;

	return 0;
}

int main_scandir(int argc, char * argv[])
{
	if(argc<2)
	{
		printf("usage: %s <dir to explore>\n", argv[0]);
		return -1;
	}

	int status;

	memset(gFileList, 0x0, 0xFF*sizeof(SEntry));

	int count=0;

	status=main_recursive_scandir(argv[1], gFileList, &count);

	int i;
	for(i=0; i<count; i++)
	{
		printf("main_scandir - %s (%u) \n", gFileList[i].iName, gFileList[i].iType);
	}

	return 0;
}

int main(int argc, char * argv[])
{
	return main_scandir(argc, argv);
}

