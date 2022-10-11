#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("/proc/55245/status", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1)
    {
        if (strstr(line, "State:") != NULL)
        {
            printf("%s", line);
            break;
        }
    }

    fclose(fp);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);

    return 0;
}
