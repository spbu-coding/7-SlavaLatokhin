#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define  MAX_INPUT_STRING_SIZE 1000
#define  MAX_STRING_COUNT_TO_OUTPUT 100

long long int determine_file_size(FILE *file) {
    long long int file_size = 0;
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return file_size;
}

int allocate_strings(int count_of_strings, char ***strings) {
	*strings = calloc(sizeof(char *), count_of_strings);
	if (*strings == NULL) {
		fprintf(stderr, "Can not allocate memory for strings to sort\n");
		return -1;
	}
	for (int i = 0; i < count_of_strings; i++) {
		
		(*strings)[i] = calloc(sizeof(char), MAX_INPUT_STRING_SIZE);
		if ((*strings)[i] == NULL) {
			fprintf(stderr, "Can not allocate memory for %d string to sort\n", i);
			return -1;
		}
	}
	return 0;
}

void free_strings(char **strings, int strings_count) {
	for (int i = 0; i < strings_count; ++i) {
		free(strings[i]);
	}
	free(strings);
}

int read_strings(char ***strings, int *count_of_strings, char *filename) {
	FILE *input_file;
    if ((input_file = fopen(filename, "rb")) == NULL) {
        printf("Can not open file %s.\n", filename);
        return -1;
    }
    long long int file_size = determine_file_size(input_file);
    char *buffer;
    buffer = calloc(file_size, 1);
    if (!buffer) {
        fclose(input_file);
        fprintf(stderr, "Can not create buffer\n");
        return -1;
    }
    if (!fread(buffer, file_size, sizeof(char), input_file)) {
        fclose(input_file);
        free(buffer);
        fprintf(stderr, "Can't read file\n");
        return -1;
    } 
    fclose(input_file);

    int count_of_symbols_in_string = 0;
    
    for (long long int i = 0; i < file_size; i++) {
    	if (buffer[i] == '\n') {
    		(*count_of_strings)++;
    	}
    }
    if(buffer[file_size - 1] != '\n') {
    	(*count_of_strings)++;
    }
    int count_of_strings1 = 0;
    if (allocate_strings(*count_of_strings, strings)) {
    	free(buffer);
    	return -1;
    }
    for (long long int i = 0; i < file_size; i++) {
    	if (buffer[i] != '\n' && buffer[i] != '.' && buffer[i] != ',' && buffer[i] != ';' 
    			&& buffer[i] != ':' && buffer[i] != '!' && buffer[i] != '?') {
    		(*strings)[count_of_strings1][count_of_symbols_in_string] = buffer[i];
    		count_of_symbols_in_string++;
    	}
    	else if (buffer[i] == '\n'){
    		(*strings)[count_of_strings1][count_of_symbols_in_string] = buffer[i];
    		count_of_symbols_in_string = 0;
    		count_of_strings1++;
    	}
    }
    if (buffer[file_size - 1] != '\n' && count_of_symbols_in_string != MAX_INPUT_STRING_SIZE) {
    	//проверить на счет проверки макс длины строки
    	(*strings)[count_of_strings1][count_of_symbols_in_string] = '\n'; 
    }
    free(buffer);
    return 0;
}

/*Insertion sort*/

void insertion(char **arr, size_t array_size) {
	for (int i = 1; i < (int) array_size; i++) {
		int j = i - 1;
		while(j >= 0 && strcmp(arr[j], arr[j + 1]) < 0) {
			char* for_swap = arr[j];
    		arr[j] = arr[j + 1];
    		arr[j + 1] = for_swap;
			j--;
		}
	}
}

/*Main*/

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("You must enter only name of input file.\n");
        return -1;
    }
	char **strings = NULL;
	int count_of_strings = 0;
	if (read_strings(&strings, &count_of_strings, argv[1])) {
		return -1;
	} 
    insertion(strings, count_of_strings);
    
    if (count_of_strings > 100) {
    	count_of_strings = 100;
    }

    for (long long int i = 0; i < count_of_strings; i++) {
    	printf("%s", strings[i]);
    }
    free_strings(strings, count_of_strings);
    return 0;
}