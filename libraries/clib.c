#include "clib.h"

int num_places (int n) {
    int r = 1;
    if (n < 0) n = (n == 0) ? 27: -n;
    while (n > 9) {
        n /= 10;
        r++;
    }
    return r;
}

int get_no_of_lines_in_file(char const * file_path) {
    int no_of_lines = 0;

    FILE *infile = fopen(file_path, "r");

    if (infile == NULL) {
        printf("Error: file pointer is null.\n");
        exit(1);
    }

    // Read file
    char line[MAXSTRING]; // contains the line

    while (fgets(line, MAXSTRING, infile) != NULL) { // for each entry
        if (line[0] != '\n')
            no_of_lines++;
    }

    fclose(infile);

    return no_of_lines;
}

char* return_filename_string(char const* output_folder, char* p) {
    char *str3 = (char*) malloc(1 + sizeof(char*) * (strlen(output_folder)+ strlen(p)));
    strcpy(str3, output_folder);
    strcat(str3, p);
    return str3;
}

// finds the position of the given IP in an array (binary search) the array needs to be sorted
int find(uint32_t *array, uint32_t data, int size) {
    int first = 0;
    int last = size - 1;
    int middle = (first+last)/2;

    while (first <= last) {
        if (array[middle] < data)
            first = middle + 1;
        else if (array[middle] == data)
            return middle; // found in the middle
        else
            last = middle - 1;

        middle = (first+last)/2;
    }

    return -1; // not found
}

char *strdup(const char *str) {
    int n = strlen(str) + 1;
    char *dup = malloc(n);

    if(dup)
        strcpy(dup, str);

    return dup;
}

char* getFileNameFromPath(const char* local_file) {
    char* ts1 = strdup(local_file);
    char* ts2 = strdup(local_file);

    char* dir = dirname(ts1);
    char* filename = basename(ts2);

    return filename;
}

char* check_whitespace (char *str) {
    char key[] = { ' ', '\n', '\t', 0 };
    return strpbrk (str, key);
}

double get_rate(int a, int b) {
    if (a == 0 || b == 0)
        return 0.0;

    double a1 = (double) a;
    double b1 = (double) b;

    return (a1/b1);
}

char* convert_int_to_char(int input) {
    // this will just output the length which is to expect
    int length = snprintf( NULL, 0, "%d", input);
    char* valueAsString = malloc(length + 1);// one more for 0-terminator
    snprintf( valueAsString, length + 1, "%d", input);
    return valueAsString;
}

// return the min of two inputs
int min(int a, int b) {
    if (a < b)
        return a;
    else
        return b;
}

// return the max of two inputs
int max(int a, int b) {
    if (a > b)
        return a;
    else
        return b;
}

uint32_t min_uint32(uint32_t a, uint32_t b) {
    if (a < b)
        return a;
    else
        return b;
}

uint32_t max_uint32(uint32_t a, uint32_t b) {
    if (a > b)
        return a;
    else
        return b;
}
