#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>            // You must import this to interact with Windows API

/**
 * Given a directory, enumerate all the files there
 * @param directory A string representing the directory path (relative)
 * @param arr_size {Outbound} Pointer to variable to represent size of the array to be returned
 * @note Be very careful with the directory path in C. This matches FILES, not actual paths. (Basically, you will need "./*").
 * @return Array of file names (string array)
*/
/**
 * Given a directory, enumerate all the files there
 * @param directory A string representing the directory path (relative)
 * @note Be very careful with the directory path in C. This matches FILES, not actual paths. (Basically, you will need "./*"). Damn.
*/
char** enumerate_directory(char* directory, int* arr_size)
{
    // This tracks the current array size and the number of items that exists
    int num_items = 0;

    // String array that contains the file names
    char** file_arr = malloc(10 * sizeof(char*));

    // Temporarily declares a file_data variable
    WIN32_FIND_DATAA file_data; 

    // The windows API returns a handle (iterator, basically) to search through all the items.
    // Its parameters require a directory to search in, and a pointer (read: address) to the file_data to write data into.
    HANDLE search_handle = FindFirstFileA(directory, &file_data);

    if (search_handle == INVALID_HANDLE_VALUE)
    {
        puts("There are no files to enumerate");
    }
    else
    {
        // If there are files to enumerate, append the current data. Then continue looping until there are no more files left.
        do
        {
            // If statement prevents us from appending the "." and ".." we usually would expect from running $ls -a
            if (strcmp(file_data.cFileName, ".") != 0 && strcmp(file_data.cFileName, "..") != 0)
            {
                // If we exhaust all 10 items in the array, create another 10
                if (num_items % 10 == 0 && num_items > 0)
                {
                    file_arr = realloc(file_arr, (num_items + 10) * sizeof(char*));
                }

                // Allocate space for the new string and add it in
                file_arr[num_items] = malloc(sizeof(file_data.cFileName));
                strcpy(file_arr[num_items], file_data.cFileName);

                num_items++; // Increment num_items
            }

        } while (FindNextFileA(search_handle, &file_data) != 0);     // FindNextFile returns 0 when it fails

    }

    *arr_size = num_items;
    return file_arr;
}

/**
 * Prints all contents of a string array (char**)
 * @param str_arr The array to print
 * @param size Size of array
*/
void print_str_arr(char** str_arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        puts(str_arr[i]);
    }
}

/**
 * Deallocates a string array (char**)
 * @param str_arr The array to deallocate
 * @param size Size of array
*/
void dealloc_str_arr(char** str_arr, int size)
{
    // Cautionary tale: sizeof on a pointer returns the size of its first element
    // sizeof(char**) == sizeof(char*[0])
    for (int i = 0; i < size; i++)
    {
        free(str_arr[i]);
    }

    free(str_arr);
}

/**
 * Return the size (read: number of chars since 1 char == 1 byte) of a given file.
 * @param file Pointer to the file object
 * @return The number of chars in the file
*/
long get_file_size(FILE* file)
{
    // If we have nullptr, file size basically does not exist
    if (file == NULL)
    {
        return 0;
    }

    // Now, we use fseek to jump the pointer to the end of the file, and then ftell will tell us how many bytes we moved
    // fseek returns 0 if successful
    if (fseek(file, 0, SEEK_END) == 0)
    {
        long byte_size = ftell(file);

        // Return the fseek back to the start of file to not mess with fgets
        fseek(file, 0, SEEK_SET);

        return byte_size;
    }
    else
    {
        return 0;
    }
}

/**
 * Read all the files given in $file_names, and outputs their content into write_path
 * @param file_names An array of file names to read
 * @param size The size of the file_names array
 * @param write_path File path to write to
*/
void spill_file_contents(char** file_names, int size, char* write_path)
{
    FILE* write_file = fopen(write_path, "ab");

    // First, let's make sure the file we're writing to actually exists
    if (write_file == NULL) 
    { 
        puts("Write file path does not exist.");
        return;
    }

    // Now, we iterate through all the files
    for (int i = 0; i < size; i++)
    {
        FILE* read_file = fopen(file_names[i], "rb");

        // Let's actually make sure the file exists. If it doesn't, continue.
        if (read_file == NULL)
        {
            continue;
        }

        fprintf(write_file, file_names[i]);
        fprintf(write_file, "\n--------\n");

        // Read the file, and output its contents into write_file. Remember we are dynamically allocating this string.
        // Realistically, we won't need *this* much memory. This size is the "worst case scenario"
        long file_contents_size = get_file_size(read_file);
        char* file_contents = malloc(file_contents_size);

        // fread does read in binary mode, but ASCII values will still be displayed normally
        fread(file_contents, file_contents_size, 1, read_file);
        fwrite(file_contents, file_contents_size, 1, write_file);

        fprintf(write_file, "\n\n");

        // Stop them memory leaks 😤
        free(file_contents);
    }
}

int main()
{
    int size;
    char** dir_names = enumerate_directory("./*", &size);

    spill_file_contents(dir_names, size, "./ls_content.txt");
    dealloc_str_arr(dir_names, size);
    return 0;
}