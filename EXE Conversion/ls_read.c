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

int main()
{
    int size;
    char** dir_names = enumerate_directory("./*", &size);

    print_str_arr(dir_names, size);
    dealloc_str_arr(dir_names, size);
    return 0;
}