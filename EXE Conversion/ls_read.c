#include <stdio.h>
#include <string.h>
#include <windows.h>            // You must import this to interact with Windows API

/**
 * Given a directory, enumerate all the files there
 * @param directory A string representing the directory path (relative)
 * @note Be very careful with the directory path in C. This matches FILES, not actual paths. (Basically, you will need "./*"). Damn.
*/
void enumerate_directory(char* directory)
{
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
        // If there are files to enumerate, print the current data. Then continue looping until there are no more files left.
        do
        {
            // If statement prevents us from printing the "." and ".." we usually would expect from running $ls -a
            if (strcmp(file_data.cFileName, ".") != 0 && strcmp(file_data.cFileName, "..") != 0)
            {
                printf("%s\n", file_data.cFileName);
            }
        } while (FindNextFileA(search_handle, &file_data) != 0);     // FindNextFile returns 0 when it fails

    }
}

int main()
{
    enumerate_directory("./*");
    return 0;
}