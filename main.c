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

char* getFullName(char* to, char* from)//Возвращает абсолютный путь
{
	char cur_dir[1000];
	if(from[1]!=':'){
		_getcwd(cur_dir,1000);//Получить путь текущей директории
		strcpy(to,cur_dir);
		strcat(to,"\\");
	}else{
		to[0] = 0;
	}
	strcat(to,from);
}

//шифрует и архивирует
int
CryptAndZip(char* path, int key, int keytime, char* zipname)
{
	char keystr[100];
	itoa(keytime,keystr,10);
	strcat(keystr,"\n");
	char tmp[1000] = "EvilCryptFile";//создаёт файл для архивирования, записываем публичный ключ
	int desc = _open(path,_O_RDONLY);
	int towr = _open(tmp,_O_CREAT | _O_WRONLY | _O_APPEND , _S_IREAD | _S_IWRITE);
	write(towr,keystr,strlen(keystr));
	char tst[1];
	tst[0] = 1;
	while(read(desc,tst,1)){//шифрование - пока что простое и посимвольное
		tst[0] += key%10;
		write(towr,tst,1);
	}
	_close(desc);
	_close(towr);
	//архивируем файл средствами винрар
	spawnl(_P_WAIT,"C:\\Program Files\\WinRAR\\WinRAR.exe","WinRAR.exe","a",zipname,tmp,NULL);
	remove(tmp);
	remove(path);
	//удаляем исходный и временный файлы
	return 0;
}

int UnzipAndDecrypt(char* zipname, char* unzName)//просто протестировать, потом доделаю
{
	//распаковываем файл в текущую директорию
	char cur_dir[1000];
	_getcwd(cur_dir,1000);
	spawnl(_P_WAIT,"C:\\Program Files\\WinRAR\\WinRAR.exe","WinRAR.exe","x",zipname,cur_dir,NULL);
	int desc = _open("EvilCryptFile",_O_RDONLY);
	//открываем файл, считываем публичный ключ
	char keytime[11];
	keytime[10] = '\0';
	read(desc,keytime,10);
	int keyint = atoi(keytime);
	if(keyint > time(NULL)){//система распознавания свой-чужой по публичному ключу
		printf("It's not my file\n Go back, u drunk\n");
		_close(desc);
	    return 0;
	}
	//если файл свой, то создаём файл, в которой запишем расшифрованные данные
	int unc = _open(unzName,_O_CREAT |_O_WRONLY | _O_APPEND, _S_IREAD | _S_IWRITE);
	//генерим скрытый ключ по публичному ключу
	srand(keyint);
	int key = rand();
	char tst[1];
	tst[0] = 1;
	while(tst[0] != '\n'){
		read(desc,tst,1);
	}
	//посиммвольно дешифруем
	while(read(desc,tst,1)){
		tst[0]-=key%10;
		write(unc,tst,1);
	}
	_close(desc);
	_close(unc);
	//удаляем архив и временный файл
	remove("EvilCryptFile");
	remove(zipname);
	return 0;
}

int main(int argc, char const *argv[])
{
	char filename[100];
	char fullname[1000];
	//обработка в зависимости от количество введённых аргументов
	if(argc == 1){
		//quest_mode();
		printf("Congratulation!\nThe program did nothing\nYou are wasting time again\nMaybe i'll try write some arguments?\n");
	}else{
		if(argc == 2){
			printf("Write filename:\n");
			gets(filename);
		}else{
			strcpy(filename,argv[2]);
		}
		getFullName(fullname,filename);//получение абсолютного имени файла
		int desc = _open(fullname, _O_RDONLY);
		if(desc == -1){//если такого файла нет - указан некорректно, то завершить программу
			printf("Critical error. U'll be delete from life (File not found)\n");
			return 0;
		}
		_close(desc);
		char flg = 0;
		if(!strcmp(argv[1],"-r")){ // если ключ -r - удалить файл
			flg = 1;
			if(remove(fullname)){
				printf("err\n");
			}
			printf("File %s was remove\n",fullname);

		}
		if(!strcmp(argv[1],"-m")){ // если ключ -m - вызываем через system встроенную процедуру type для вывода содержимого на экран
			flg = 1;
			char command[1000] = "type ";
			strcat(command,fullname);
			system(command);
			printf("\nFile was print\n");
		}
		if(!strcmp(argv[1],"-p")){//если ключ -p - генерим публичный и скрытый ключи, если не указано имя архива - называем default 
			flg = 1;			  //вызываем функцию по шифрованию и архивированию
			int keytime = time(NULL);
			srand(keytime);
			int key = rand();
			char zipname[1000] = "defzip";
			if(argc == 4){
				strcpy(zipname,argv[3]);
			}
			CryptAndZip(fullname,key,keytime,zipname);
			printf("\nFile was crypt and zip\n");
		}
		if(!strcmp(argv[1],"-u")){//если ключ -u - если не указано имя расшифрованного файла - называем default
			flg = 1;              //вызываем функцию по разархивированию и дешифрации
			char unzipname[1000] = "defunzip";
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
