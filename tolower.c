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
	printf("Error tolower take forms:\n\ttolower \"{source_file(s)}\" -{argument(s)}\n\ttolower \"{source_file(s)}\"\nTry 'tolower -h' for help.\n");
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
	printf("Forms:\n\ttolower \"{source_file(s)}\" -{argument(s)}\n\ttolower \"{source_file(s)}\"\nArguments:\n\th prints help menu\n\te applys tolower to file extensions\n\tc prints the number of files renamed\n");
}

//returns index of last '.'
//returns index of '\0' if no '.' is found
int find_extension(char *str)
{
	int ext_index = -1;
	int str_index = 0;

	while(str[str_index] != '\0'){
		if(str[str_index] == '.'){
			ext_index = str_index;
		}
		++str_index;
	}

	if(ext_index == -1){
		return str_index;
	}

	return ext_index;
}

//returns 1 if file was moved
int mvlower(char *file_name, char *args)
{
	char lower[MAX_LINUX_FILE_NAME_BUFF];
	char command[(MAX_LINUX_FILE_NAME_BUFF * 2) + 10];// extra space added to hold mv

	int ext_index = (args['e'] == PASSED)? find_extension(file_name): MAX_LINUX_FILE_NAME_BUFF;

	int index = 0;

	for(index = 0; file_name[index] != '\0'; ++index){
		if(isalpha(file_name[index]) && !(args['e'] == PASSED && index > ext_index)){
			lower[index] = tolower(file_name[index]);
		}else{
			lower[index] = file_name[index];
		}
	}

	lower[index] = '\0';

	if(strcmp(lower, file_name)){//avoids mv same dest error
		return 1;
		sprintf(command, "mv \"%s\" \"%s\"", file_name, lower);
		system(command);
	}

	return 0;
}

int main(int argc, char **argv)
{
	if(argc <= 1){
		command_error();
	}

	char args[ASCII_TABLE_SIZE];

	for(int i = 0; i < ASCII_TABLE_SIZE; ++i){
		args[i] = 0;
	}

	args['e'] = VALID;
	args['h'] = VALID;
	args['c'] = VALID;

	//finds arguments for argv
	for(int i = 1; i < argc; ++i){
		if(argv[i][0] == '-'){
			parse_args(argv[i], args);
		}
	}

	if(args['h'] == PASSED){
		print_help_menu();
		return 0;
	}

	//finds files to rename
	int count = 0;
	for(int i = 1; i < argc; ++i){
		if(argv[i][0] != '-'){
			count += mvlower(argv[i], args);
		}
	}

	if(args['c'] == PASSED){
		printf("%d Files moved.\n", count);
	}

	return 0;
}
