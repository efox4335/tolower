#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINUX_FILE_NAME_BUFF 5000
#define ASCII_TABLE_SIZE 128
#define VALID 1
#define PASSED 2

void command_error(void)
{
	printf("Error tolower take forms:Try 'tolower -h' for help.\n");
	exit(1);
}

void invalid_arg(char *passed_args, char invalid_arg)
{
	if(passed_args[(int) invalid_arg] == PASSED){
		printf("Error argument \"%c\" passed more than once\n", invalid_arg);
	}else{
		printf("Error argument \"%c\" unrecognized.\nTry 'tolower -h' for help.\n", invalid_arg);
	}

	exit(1);
}

void parse_args(char *in_args, char *passed_args)
{
	for(int i = 1; in_args[i] != '\0'; ++i){
		if(passed_args[(int) in_args[i]] == VALID){
			passed_args[(int) in_args[i]] = PASSED;
		}else{
			invalid_arg(passed_args, in_args[i]);
		}
	}
}

void print_help_menu(void)
{
	printf("Forms:\n\ttolower \"{source_file(s)}\" -{argument(s)}\n\ttolower \"{source_file(s)}\"\nArguments:\n\th prints help menu\n\tr applys tolower in any subdirectory that matches {source_files(s)}\n\te applys tolower to file extensions\n");
}

int main(int argc, char **argv)
{
	if(argc <= 1 || argc > 3){
		command_error();
	}

	char args[ASCII_TABLE_SIZE];

	for(int i = 0; i < ASCII_TABLE_SIZE; ++i){
		args[i] = 0;
	}

	args['r'] = VALID;
	args['e'] = VALID;
	args['h'] = VALID;

	if(argc == 3){
		if(argv[2][0] == '-'){
			parse_args(argv[2], args);
		}else{
			command_error();
		}
	}

	if(args['h'] == PASSED){
		print_help_menu();
		return 0;
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
//			system(command);
		}
	}

	return 0;
}
