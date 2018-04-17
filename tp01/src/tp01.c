/**
 * \file skeleton.c
 * \brief Basic parsing options skeleton.
 * \author Pierre L. <pierre1.leroy@orange.com>
 * \version 0.1
 * \date 10 septembre 2016
 *
 * Basic parsing options skeleton exemple c file.
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>

#include<getopt.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <grp.h>

#include<dirent.h>
#include<pwd.h>
#include<time.h>

#define STDOUT 1
#define STDERR 2

#define MAX_PATH_LENGTH 4096


#define USAGE_SYNTAX "[OPTIONS] -e EXONBR -i INPUT -o OUTPUT"
#define USAGE_PARAMS "OPTIONS:\n\
  -i, --input  INPUT_FILE  : input file\n\
  -o, --output OUTPUT_FILE : output file\n\
***\n\
  -v, --verbose : enable *verbose* mode\n\
  -e, --exo : exercice number (1, 2 or 3)\n\
  -h, --help    : display this help\n\
"

/**
 * Procedure which displays binary usage
 * by printing on stdout all available options
 *
 * \return void
 */
void print_usage(char* bin_name)
{
  dprintf(1, "USAGE: %s %s\n\n%s\n", bin_name, USAGE_SYNTAX, USAGE_PARAMS);
}


/**
 * Procedure checks if variable must be free
 * (check: ptr != NULL)
 *
 * \param void* to_free pointer to an allocated mem
 * \see man 3 free
 * \return void
 */
void free_if_needed(void* to_free)
{
  if (to_free != NULL) free(to_free);
}


/**
 *
 * \see man 3 strndup
 * \see man 3 perror
 * \return
 */
char* dup_optarg_str()
{
  char* str = NULL;

  if (optarg != NULL)
  {
    str = strndup(optarg, MAX_PATH_LENGTH);

    // Checking if ERRNO is set
    if (str == NULL)
      perror(strerror(errno));
  }

  return str;
}


/**
 * Binary options declaration
 * (must end with {0,0,0,0})
 *
 * \see man 3 getopt_long or getopt
 * \see struct option definition
 */
static struct option binary_opts[] =
{
  { "help",    no_argument,       0, 'h' },
  { "verbose", no_argument,       0, 'v' },
  { "exercice",  required_argument, 0, 'e' },
  { "input",   required_argument, 0, 'i' },
  { "output",  required_argument, 0, 'o' },
  { 0,         0,                 0,  0  }
};

/**
 * Binary options string
 * (linked to optionn declaration)
 *
 * \see man 3 getopt_long or getopt
 */
const char* binary_optstr = "hve:i:o:";

void exoOne(char* bin_input_param, char* bin_output_param) {

  char *newBuffer = malloc(sizeof(char)*4096);
  int inputFile = open(bin_input_param, O_RDONLY);
  int outputFile = open(bin_output_param, O_WRONLY | O_CREAT, S_IRWXU);
  int byteRead = 0;
  if(inputFile < 0 || outputFile < 0) {
      printf("Error opening files: %s\n", strerror(errno));
      exit(errno);
  }
  while((byteRead = read(inputFile, newBuffer, MAX_PATH_LENGTH)) > 0) {
      if(write(outputFile, newBuffer, byteRead) < 0) {
          printf("Error writing file: %s\n", strerror(errno));
          exit(errno);
      }
  }
}

void exoTwo(char* bin_input_param) {
    char exoBuffer;
    int inputFile = open(bin_input_param, O_RDONLY);
    int byteRead = 0;
    if(inputFile < 0) {
        printf("Error opening files: %s\n", strerror(errno));
        exit(errno);
    }
    if((byteRead = lseek(inputFile, (off_t) 0, SEEK_END)) < 0) {
        printf("Error placing the cursor: %s", strerror(errno));
        exit(errno);
    }
    printf("Source file size is: %d bytes\n", byteRead);
    for(int i = byteRead - 2; i >= 0; i--) {
        lseek(inputFile,  (off_t) i, SEEK_SET);
        if(read(inputFile, &exoBuffer, 1) <= 0) {
            printf("Error reading file: %s\n", strerror(errno));
        }
        printf("%c", exoBuffer);
    }
    printf("\n");
}

void exoThree(char* bin_input_param) {
    struct stat objectStat;
    struct stat fileStat;
    DIR *myDirectory;
    struct dirent *files;
    if(stat(bin_input_param, &objectStat) < 0) {
        printf("Error checking link: %s\n", strerror(errno));
    }
    int isDir = S_ISDIR(objectStat.st_mode);
    int isFile = S_ISREG(objectStat.st_mode);
    int isSymLink = S_ISLNK(objectStat.st_mode);

    if(isDir) {
        printf("This is a directory\n");
    } else if(isFile) {
        printf("This is a file\n");
    } else if (isSymLink) {
        printf("This is a symbolic link\n");
    }
    
    if(isDir == 0 && isFile == 0 && isSymLink == 0) {
        printf("Invalid type of file as argument\n");
        exit(errno);
    }
    
    if(isDir) {
        if ((myDirectory = opendir(bin_input_param)) == NULL) {
            printf("Error checking link: %s\n", strerror(errno));
        }
        struct passwd *fileUser;
        struct group *fileGroup;
        while((files = readdir(myDirectory)) != NULL) {

            // Get file full path
            char *myFilePath = (char *) malloc(1 + sizeof(bin_input_param) + sizeof(files->d_name));
            strcpy(myFilePath, bin_input_param);
            strcat(myFilePath, "/");
            strcat(myFilePath, files->d_name);

            if(stat(myFilePath, &fileStat) < 0) {
                printf("Error checking link of directory files: %s\n", strerror(errno));
            }

            // Get type of file
            // int isFileDir = S_ISDIR(fileStat.st_mode);
            int isFileFile = S_ISREG(fileStat.st_mode);
            int isFileSymLink = S_ISLNK(fileStat.st_mode);

            // Name of File
            printf("%-10s ", files->d_name);

            // Permissions of file
            printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
            printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
            printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");

            printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
            printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
            printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");

            printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
            printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
            printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");

            // User of file
            if((fileUser = getpwuid(fileStat.st_uid)) == NULL) {
                printf("Error trying to get user infos: %s\n", strerror(errno));
            }

            printf("  %10s", fileUser->pw_name);

            // Group of file
            if((fileGroup = getgrgid(fileStat.st_gid)) == NULL) {
                printf("Error trying to get user infos: %s\n", strerror(errno));
            }

            printf(":%-10s ", fileUser->pw_name);

            // Size of file
            if(isFileFile || isFileSymLink) {
                printf("%9ld  ", fileStat.st_size);
            } else {
                printf("---------  ");
            }

            // Last modification time
            char *dateFile = malloc(sizeof(char)*36);
            const struct tm *dateStruct;
            if((dateStruct = gmtime(&fileStat.st_mtime)) == NULL) {
                printf("Error trying to get time form file: %s\n", strerror(errno));
            }
            if(strftime(dateFile, 36, "%d%m%y @ %Hh%M", dateStruct) == 0) {
                printf("Error trying to convert time: %s\n", strerror(errno));
            }

            printf("%s", dateFile);

            printf("\n");
            free_if_needed(myFilePath);
        }
    }
}

void displayError() {
    dprintf(STDERR, "Bad usage! See HELP [--help|-h]\n");
    // Exiting with a failure ERROR CODE (== 1)
    exit(EXIT_FAILURE);
}

/**
 * Binary main loop
 *
 * \return 1 if it exit successfully
 */
int main(int argc, char** argv)
{
    /**
     * Binary variables
     * (could be defined in a structure)
     */
    short int is_verbose_mode = 0;
    char* bin_input_param = NULL;
    char* bin_output_param = NULL;
    short int exoNumber = 0;

    // Parsing options
    int opt = -1;
    int opt_idx = -1;

    while ((opt = getopt_long(argc, argv, binary_optstr, binary_opts, &opt_idx)) != -1)
    {
        switch (opt)
        {
            case 'i':
                //input param
                if (optarg)
                {
                    bin_input_param = dup_optarg_str();
                }
                break;
            case 'o':
                //output param
                if (optarg)
                {
                    bin_output_param = dup_optarg_str();
                }
                break;
            case 'v':
                //verbose mode
                is_verbose_mode = 1;
                break;
            case 'e':
                //exo number
                if (optarg)
                {
                    exoNumber = atoi(dup_optarg_str());
                }
                break;
            case 'h':
                print_usage(argv[0]);

                free_if_needed(bin_input_param);
                free_if_needed(bin_output_param);

                exit(EXIT_SUCCESS);

                default :
            break;
        }
    }

  /**
   * Checking binary requirements
   * (could defined in a separate function)
   */
    if (exoNumber == 0 || bin_input_param == NULL)
    {
        // Freeing allocated data
        free_if_needed(bin_input_param);
        free_if_needed(bin_output_param);
        displayError();
    }


  // Printing params
    dprintf(1, "** PARAMS **\n%-8s: %d\n%-8s: %s\n%-8s: %s\n%-8s: %d\n",
        "exo", exoNumber,
        "input",   bin_input_param,
        "output",  bin_output_param,
        "verbose", is_verbose_mode);

    // Business logic must be implemented at this point
    if(exoNumber == 1) {
        if(bin_output_param == NULL) {
            free_if_needed(bin_input_param);
            free_if_needed(bin_output_param);
            displayError();
        }
        exoOne(bin_input_param, bin_output_param);
    } else if(exoNumber == 2) {
        exoTwo(bin_input_param);
    } else if(exoNumber == 3) {
        exoThree(bin_input_param);
    }

    // Freeing allocated data
    free_if_needed(bin_input_param);
    free_if_needed(bin_output_param);

    return EXIT_SUCCESS;
}
