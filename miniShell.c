/*Author: Feras Alhenashil
 *Program: miniShell (A linux bash shell simulator program)
 *Date: 4/23/2020
*/
#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <limits.h>

#define MAXINPUT 1024
#define DELIMETERS " \t\r\n\a"
#define bufferSize 64


void displayPrompt(){

    long size;
    char *buff;
    char *dir = NULL;

    size = pathconf(".", _PC_PATH_MAX);

    if ((buff = (char *) malloc((size_t) size)) != NULL)
        dir = getcwd(buff, (size_t)size);
    char computer [HOST_NAME_MAX];
    char *username = getenv("USER");
    gethostname(computer, HOST_NAME_MAX);
    printf("%s@%s:%s>", username, computer, dir);
    free(buff);
}


int userInput(char *str) {
    char *line = NULL;
    size_t bufsize = 0;

    if(getline(&line, &bufsize, stdin) == -1){
        if(feof(stdin)){
            exit(0);
        } else {
            exit(EXIT_FAILURE);
        }
    }

    if(strlen(line) > 0){
        if(line[0] > 96 && line[0] < 123){
            strcpy(str, line);
            return 0;
        }
    }
    return 1;
}

char** parseInput(char *inputline) {
    int i = 0, buff = bufferSize;
    char **argv = malloc(buff * sizeof(char *));
    char *parsed;

    if(!argv){
        printf("allocation error");
        exit(EXIT_FAILURE);
    }

    parsed = strtok(inputline, DELIMETERS);
    while(parsed != NULL){
        argv[i] = parsed;
        i++;

        if (i >= buff){
            buff += bufferSize;
            argv = realloc(argv, buff * sizeof(char *));
            if(!argv){
                printf("allocation error");
                exit(EXIT_FAILURE);
            }
        }
        parsed = strtok(NULL, DELIMETERS);
    }
    argv[i] = NULL;
    return argv;
}

int cd(char **args){
    if( chdir(args[1]) != 0)
        perror("Error");
    return 1;
}

int shell_exit(char **args){
    if(args[1]){
    exit(atoi(args[1]));
    }else{
	exit(0);
    }
}


int cpuinfo(char **args){
    FILE *cpuinfo = fopen("/proc/cpuinfo", "rb");
    int i = 0, buff = bufferSize;
    char *line = 0, *parsed_file;
    char **content = malloc(buff * sizeof(char *));
    size_t size = 0;

    if(getdelim(&line, &size, 0, cpuinfo) != -1)
    {
        parsed_file = strtok(line, " \n");
        while(parsed_file != NULL){
            content[i] = parsed_file;
            i++;

            if (i >= buff) {
                buff += bufferSize;
                content = realloc(content, buff * sizeof(char *));
                if (!content) {
                    printf("allocation error");
                    exit(EXIT_FAILURE);
                }
            }

            parsed_file = strtok(NULL, " \n");
        }

    }
    if(args[1]){
        if(strcmp(args[1], "-c") == 0){
            printf("%s\n", content[16]);
        }else if(strcmp(args[1], "-t") == 0){
            printf("%s\n", content[11]);
        } else if(strcmp(args[1], "-n") == 0){
            printf("%s\n", content[32]);
        } else if(strcmp(args[1], "--help") == 0 || strcmp(args[1], "-h") == 0){
            puts("\nUsage: cpuinfo [option]\n"
                 "\nprovides some information about the cpu:"
                 "\n-c (for cpu clock speed)\n"
                 "\n-t (for cpu type)\n"
                 "\n-n (for number of cores)\n");
        }else{
            printf("cpuinfo: invalid option -- '%s'\n", args[1]);
            printf("Try 'cpuinfo --help' for more information.\n");
        }

    } else {
        puts("No option provided for cpuinfo\n"
             "try cpuinfo [options] -c (for cpu clock speed) \n"
             "\n -t (for cpu type) -n (for number of cores)");
    }
    free(line);
    free(content);
    fclose(cpuinfo);
    return 1;
}

int meminfo(char **args){
    FILE *meminfo = fopen("/proc/meminfo", "rb");
    FILE *L2cache = fopen("/sys/devices/system/cpu/cpu1/cache/index2/size", "rb");

    int i = 0, buff = bufferSize, used, totalMem, freeMem, L2convert, L2bytes;
    char *line = 0, *parsed_file, *L2size = NULL;
    char **content = malloc(buff * sizeof(char *));
    size_t size = 0;

    if(getdelim(&line, &size, 0, meminfo) != -1)
    {
        parsed_file = strtok(line, " \n");
        while(parsed_file != NULL){
            content[i] = parsed_file;
            i++;

            if (i >= buff) {
                buff += bufferSize;
                content = realloc(content, buff * sizeof(char *));
                if (!content) {
                    printf("allocation error");
                    exit(EXIT_FAILURE);
                }
            }

            parsed_file = strtok(NULL, " \n");
        }

    }


    if(args[1]){
        if(strcmp(args[1], "-t") == 0){
            printf("%s KiB\n", content[1]);
        }else if(strcmp(args[1], "-u") == 0){
            sscanf(content[1], "%d", &totalMem);
            sscanf(content[4], "%d", &freeMem);
            used = totalMem - freeMem;
            printf("%d KiB\n", used);
        }else if(strcmp(args[1], "-c") == 0){
            getline(&L2size, &size, L2cache);
            L2size[3] =  NULL;
            sscanf(L2size, "%d", &L2convert);
            L2bytes = L2convert * 1024;
            printf("%d B\n", L2bytes);
        } else if(strcmp(args[1], "--help") == 0  || strcmp(args[1], "-h") == 0){
            puts("\nUsage: meminfo [option]\n"
                 "\nprovides some information about the cpu:\n"
                 "\n-t (for total RAM available)\n"
                 "\n-u (for used RAM)\n"
                 "\n-c (for L2 cache size)\n");
        }else{
            printf("meminfo: invalid option -- '%s'\n", args[1]);
            printf("Try 'meminfo --help' for more information.\n");
        }

    } else {
        puts("No option provided for meminfo\n"
             "try meminfo [options] -t (for total RAM available) \n"
             "\n -u (for used RAM) -c (for L2 cache size)");
    }
    free(line);
    free(L2size);
    free(content);
    fclose(L2cache);
    fclose(meminfo);
    return 1;
}

int shell_manual (char **args){
    puts("\n ........ miniShell : This is an exercise in creating a bash like shell in C .........\n"
         "\n-- This shell supports all of the regular linux shell commands \n"
         "(internal and external commands e.g. ls, cat etc), as well as\n"
         "the following shell implemented commands:\n"
         "\n >> exit [n] where n is the exit code. 0 if no n provided"
         "\n >> cpuinfo [options] -c (for cpu clock speed) "
         "\n -t (for cpu type) -n (for number of cores)"
         "\n >> meminfo [options] -t (for total RAM) "
         "\n -u (for used RAM) -c (for L2 cached size)"
         "\n "
         "\n ");
    return 1;
}

char *ownCMD_str [] ={
      "cd",
      "exit",
      "cpuinfo",
      "meminfo",
      "manual",
      "help"
};

int (*ownCMD_func[]) (char **) = {
        &cd,
        &shell_exit,
        &cpuinfo,
        &meminfo,
        &shell_manual,
        &shell_manual
};

int ownCMD_count(){
    return sizeof(ownCMD_str) / sizeof(char *);
}

int executeCMD(char **args) {
    pid_t child_pid;
    int status, i;


    for (i = 0; i < ownCMD_count(); i++){
        if (strcmp(args[0], ownCMD_str[i]) == 0)
            return (*ownCMD_func[i])(args);
    }

    child_pid = fork();

    if (child_pid == 0){
        if(execvp(args[0], args) < 0){
            perror("Error");
            exit(1);
        }
    }else if(child_pid < 0) {
        perror("Fork failed");
        exit(1);
    }else{
        waitpid(child_pid, &status, WUNTRACED);
    }

    return 1;
}

int main() {
    char inputline[MAXINPUT];
    char **argv = NULL;
    int status = 1;

    while(status) {
        displayPrompt();
        if (userInput(inputline) || strcmp(inputline, "\n")==0){
            continue;
        }
        argv = parseInput(inputline);
        status = executeCMD(argv);

    }
    free(argv);
    return 0;
}
