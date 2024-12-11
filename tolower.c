#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINUX_FILE_NAME_BUFF 5000

void command_error(void)
{
	printf("Error tolower take forms:\n\ttolower \"source_file(s)\" -{argument(s)}\n\ttolower -{argument(s)}\nTry tolower -h for help.\n");
	exit(1);
}

int main(int argc, char **argv)
{
	if(argc <= 1){
		command_error();
	}



	FILE *fp = NULL;
	char ls_output[MAX_LINUX_FILE_NAME_BUFF];
	char lower[MAX_LINUX_FILE_NAME_BUFF];
	char command[(MAX_LINUX_FILE_NAME_BUFF * 2) + 10];// extra space added to hold mv

	fp = popen("ls -a", "r");

	while(NULL != fgets(ls_output, MAX_LINUX_FILE_NAME_BUFF, fp)){
		for(int i = 0; ls_output[i] != '\0'; ++i){
			if(isalpha(ls_output[i])){
				lower[i] = tolower(ls_output[i]);
			}else if(ls_output[i] == '\n'){
				lower[i] = '\0';
				ls_output[i] = '\0';
			}else if(ls_output[i] == '\r'){
				lower[i] = '\0';
				ls_output[i] = '\0';
			}else{
				lower[i] = ls_output[i];
			}
		}

		if(strcmp(lower, ls_output)){//avoids mv same dest error
			sprintf(command, "mv %s %s", ls_output, lower);
			system(command);
		}
	}

	return 0;
}
