#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <direct.h>

char* getFullName(char* to, char* from)//return right path
{
	char cur_dir[1000];
	if(from[1]!=':'){
		_getcwd(cur_dir,1000);//get dicectory path
		strcpy(to,cur_dir);
		strcat(to,"\\");
	}else{
		to[0] = 0;
	}
	strcat(to,from);
}

int main(int argc, char const *argv[])
{
	char filename[100];
	char fullname[1000];
	printf("Hi\n");
	if(argc == 1){
		//quest_mode();
		printf("Start quest_mode\n");
	}else{
		if(argc == 2){
			printf("Write filename:\n");
			gets(filename);
		}else{
			strcpy(filename,argv[2]);
		}
		getFullName(fullname,filename);//convet filename to fullname
		char flg = 0;
		if(!strcmp(argv[1],"-r")){
			flg = 1;
			printf("File was remove\n");
		}
		if(!strcmp(argv[1],"-m")){
			flg = 1;
			printf("File was print\n");
		}
		if(!strcmp(argv[1],"-p")){
			flg = 1;
			printf("File was crypt and zip\n");
		}
		if(!strcmp(argv[1],"-u")){
			flg = 1;
			printf("File was unzip and decrypt\n");
		}
		if(!flg){
			printf("Undefined options\n");
			return 0;
		}
	}

	return 0;
}
