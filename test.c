#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>


static long goDir(char *dirname)
{
    DIR *dir = opendir(dirname);
    struct dirent *dit;
    struct stat st;
    int size = 0;
    int total_size = 0;
    char filePath[NAME_MAX];
	
	if (dir == 0){
		printf("NO DIR\n");
		return 0;
	}

    while ((dit = readdir(dir)) != NULL)
    {
        if ( (strcmp(dit->d_name, ".") == 0) || (strcmp(dit->d_name, "..") == 0) )
            continue;

        sprintf(filePath, "%s/%s", dirname, dit->d_name);
        if (lstat(filePath, &st) != 0)
            continue;
			
        size = st.st_size;

        if (S_ISDIR(st.st_mode))
        {
			total_size = 0;
            total_size += goDir(filePath);
		
			total_size += size;
            printf("DIR\t");
            printf("MODE: %lo\t", (unsigned long) st.st_mode);
            printf("SIZE: %d\t", total_size);
            printf("%s\n", filePath);
        }
        else
        {
            total_size += size;
            printf("FILES\t");
            printf("MODE: %lo\t", (unsigned long) st.st_mode);
            printf("SIZE: %d\t", size);
            printf("%s\n", filePath);
        }
    }

    return total_size;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <Directory>\n", argv[0]);
        return 1;
    }
	
    long size = goDir(argv[1]);
    printf("Total size: %ld\n", size);

    return 0;
}