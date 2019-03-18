#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <direct.h>
#include <process.h> 
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

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

int
CryptAndZip(char* path, int key, int keytime, char* zipname)
{
	char keystr[100];
	itoa(keytime,keystr,10);
	strcat(keystr,"\n");
	int desc = _open(path,_O_RDONLY);
	int towr = _open(zipname,_O_CREAT | _O_WRONLY | _O_APPEND , _S_IREAD | _S_IWRITE);
	write(towr,keystr,strlen(keystr));
	char tst[1];
	tst[0] = 1;
	while(read(desc,tst,1)){
		tst[0] += key%10;
		write(towr,tst,1);
	}
	_close(desc);
	_close(towr);
	//strcat(zipname,".rar");
	//spawnl(_P_WAIT,"C:\\Program Files\\WinRAR\\WinRAR.exe","WinRAR.exe","a",zipname,path,NULL);
	return 0;
}

int UnzipAndDecrypt(char* zipname, char* unzName)//просто протестировать, потом доделаю
{
	int desc = _open(zipname,_O_RDONLY);
	int unc = _open(unzName,_O_CREAT |_O_WRONLY | _O_APPEND, _S_IREAD | _S_IWRITE);
	char keytime[11];
	keytime[10] = '\0';
	read(desc,keytime,10);
	int keyint = atoi(keytime);
	srand(keyint);
	int key = rand();
	char tst[1];
	tst[0] = 1;
	while(tst[0] != '\n'){
		read(desc,tst,1);
	}
	
	while(read(desc,tst,1)){
		tst[0]-=key%10;
		write(unc,tst,1);
	}
}

int main(int argc, char const *argv[])
{
	char filename[100];
	char fullname[1000];
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
		int desc = _open(fullname, _O_RDONLY);
		if(desc == -1){
			printf("Critical error. U'll be delete from life (File not found)\n");
			return 0;
		}
		_close(desc);
		char flg = 0;
		if(!strcmp(argv[1],"-r")){ // if -r: try to remove
			flg = 1;
			if(remove(fullname)){
				printf("err\n");
			}
			printf("File %s was remove\n",fullname);

		}
		if(!strcmp(argv[1],"-m")){ // if -m: print file to console
			flg = 1;
			printf("%s\n",fullname);
			char command[1000] = "type ";
			strcat(command,fullname);
			system(command);
			printf("\nFile was print\n");
		}
		if(!strcmp(argv[1],"-p")){
			flg = 1;
			int keytime = time(NULL);
			srand(keytime);
			int key = rand();
			char zipname[1000] = "default";
			if(argc == 4){
				strcpy(zipname,argv[3]);
			}
			CryptAndZip(fullname,key,keytime,zipname);
			printf("\nFile was crypt and zip\n");
		}
		if(!strcmp(argv[1],"-u")){
			flg = 1;
			char unzipname[1000] = "default";
			if(argc == 4){
				strcpy(unzipname,argv[3]);
			}
			UnzipAndDecrypt(fullname,unzipname);//доделать
			printf("File was unzip and decrypt\n");
		}
		if(!flg){
			printf("Undefined options\n");
			return 0;
		}
	}
	return 0;
}
