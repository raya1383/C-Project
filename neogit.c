#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>


#define MAX_FILENAME_LENGTH 1000
#define MAX_COMMIT_MESSAGE_LENGTH 2000
#define MAX_LINE_LENGTH 1000
#define MAX_MESSAGE_LENGTH 1000
#define STAGING_FOLDER ".neogit/stagingFolder"
#define COMMIT_FOLDER ".neogit/commit/last_commit"
#define SOURCE_FOLDER ".neogit/commit/last_commit"
#define DESTINATION_FOLDER ".neogit/commit/"
#define debug(x) printf("%s", x);
int x=0;//for checking repository
int m=0;//for checking exist file to add

void print_command(int argc, char * const argv[]);
int run_init(int argc, char * const argv[]);
int create_configs(int argc, char * const argv[]);
int run_add(int argc, char * const argv[]);
int add_to_staging(char *filepath);
int run_reset(int argc, char * const argv[]);
int remove_from_staging(char *filepath);
int run_commit(int argc, char * const argv[], char string[100]);
int inc_last_commit_ID();
bool check_file_directory_exists(char *filepath);
int commit_staged_file(int commit_ID, char *filepath);
int track_file(char *filepath);
bool is_tracked(char *filepath);
int create_commit_file(int commit_ID, char *message);
int find_file_last_commit(char* filepath);
int run_checkout(int argc, char *const argv[]);
int find_file_last_change_before_commit(char *filepath, int commit_ID);
int checkout_file(char *filepath, int commit_ID);
int alias_config(int argc, char* const argv[],FILE* file_name);
int searchFileInCurrentDir(const char* file_name);
void copyFile(const char* sourceFilename, const char* destinationFilename);
void removeWordFromFile(const char* filename, const char* wordToRemove);
int check_status(int argc, char * const argv[]);
int set_shortcut(int argc, char*const argv[]);
int find_shortcut(int argc, char*const argv[]);
int commitFile(int argc, char * const argv[]);
int replace_shortcut(int argc, char * const argv[]);
int remove_shortcut(int argc, char * const argv[]);
int log_c(int argc, char * const argv[]);
int make_last_commit(int argc, char * const argv[]);
int make_branch(int argc, char * const argv[]);
int global();
void print_command(int argc, char * const argv[]) {
    for (int i = 0; i < argc; i++) {
        fprintf(stdout, "%s ", argv[i]);
    }
    fprintf(stdout, "\n");
}

int run_init(int argc, char * const argv[]) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL) return 1;

    char tmp_cwd[1024];
    bool exists = false;
    struct dirent *entry;
    do {
        // find .neogit
        DIR *dir = opendir(".");
        if (dir == NULL) {
            printf("Error opening current directory\n");
            return 1;
        }
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".neogit") == 0)
                exists = true;
        }
        closedir(dir);

        // update current working directory
        if (getcwd(tmp_cwd, sizeof(tmp_cwd)) == NULL) return 1;

        // change cwd to parent
        if (strcmp(tmp_cwd, "/") != 0) {
            if (chdir("..") != 0) return 1;
        }

    } while (strcmp(tmp_cwd, "/") != 0);

    // return to the initial cwd
    if (chdir(cwd) != 0) return 1;
        
    if (!exists) {
        if (mkdir(".neogit", 0755) != 0) return 1;

    } else {
        printf("neogit repository has already initialized\n");
    }
    return 0;
}
/*void copyFile(const char *source, const char *destination) {
    FILE *sourceFile, *destFile;
    char ch;

    sourceFile = fopen(source, "r");
    destFile = fopen(destination, "w");

    if (sourceFile == NULL || destFile == NULL) {
        perror("Error opening file");
        return;
    }

    while ((ch = fgetc(sourceFile)) != EOF) {
        fputc(ch, destFile);
    }

    fclose(sourceFile);
    fclose(destFile);
}*/
int check_neogit_directory(int argc , char * argv[]) {

    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL) return 1;

    char tmp_cwd[1024];
    bool exists = false;
    struct dirent *entry;
    do {
        // find .neogit
        DIR *dir = opendir(".");
        if (dir == NULL) {
            printf("Error opening current directory\n");
            return 1;
        }
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".neogit") == 0)
                exists = true;
        }
        closedir(dir);

        // update current working directory
        if (getcwd(tmp_cwd, sizeof(tmp_cwd)) == NULL) return 1;

        // change cwd to parent
        if (strcmp(tmp_cwd, "/") != 0) {
            if (chdir("..") != 0) return 1;
        }

    } while (strcmp(tmp_cwd, "/") != 0);

    // return to the initial cwd
    if (chdir(cwd) != 0) return 1;
        
    if (!exists) {
        printf("you have not make ant Repository Yet!\n");
        x=1;
        return 1;

    }
    return 0;
}
int create_configs(int argc, char* const argv[]) {
    if (argc==2)
    {
        printf("Enter a full commond. ");
        return 1;
    }else if (argc==3 && argv[2]=="-global")
    {
        printf("please Enter your username  correctly !");
        return 1;
    }else if (argc==8 && argv[8]=="-global")
    {
        printf("please Enter your email correctly !");
        return 1;
    }
    
    
    FILE *file = fopen(".neogit/config", "w");
    if (file == NULL) return 1;
    

    fprintf(file, "username: %s\n", argv[3]);
    fprintf(file, "email: %s\n", argv[7] );
    fprintf(file, "last_commit_ID: %d\n", 0);
    fprintf(file, "current_commit_ID: %d\n", 0);
    fprintf(file, "branch: %s", "master");

    fclose(file);
    // create commits folder
    if (mkdir(".neogit/commits", 0755) != 0) return 1;

    // create files folder
    if (mkdir(".neogit/files", 0755) != 0) return 1;

    /*file = fopen(".neogit/staging", "w");
    fclose(file);

    file = fopen(".neogit/tracks", "w");
    fclose(file);*/
    
    return 0;
}
int alias_config(int argc, char* const argv[],FILE * file_name){
     file_name=fopen("command_file", "r");
    int found=0;
    char word[20];
    if (strcmp(argv[2], "-global") == 0)
    {
        while (fscanf(file_name, "%s", word) != EOF) {
            if (strcmp(word, argv[4]) == 0 ) {
                found = 1;
                
                break;
            }}
    }else 
    {   
        while (fscanf(file_name, "%s", word) != EOF) {
            printf("%s", word);
            if (strcmp(word, argv[3]) == 0 ) {
                found = 1;
                
                break;
            }}
    }
    
    
    if (argc<3 )
    {
        printf("Enter a command you want to rename !\n");
        return 1;
    }else if (argv[2]=="-global" && argc<4 )
    {
         printf("Enter a command you want to rename !\n");
         return 1;
    }else if ( argv[2]=="-global" && found==0)
    {
        printf("there is no command name : %s\n", argv[4]);
        return 1;
    }else if (found ==0)
    {
        printf("there is no command name : %s\n", argv[3]);
        return 1;
    }
    
    
    return 0;
}
int run_add(int argc, char *const argv[]) {
    
    // TODO: handle command in non-root directories 
    if (argc <= 2) {
        printf("please specify a file");
        return 1;
    }
    if (strstr(argv[2], "*")!=NULL)
    {    char c [3];
        strcpy(c, argv[2]);
        char firstChar=c[0];
        char secondChar=c[2];
        DIR *dir;
        struct dirent *entry;
        
        dir = opendir(".");
        if (dir == NULL) {
        printf("Unable to open current directory");
        return 1;
        }

        while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR)
            continue; // Skip directories

        char *filename = entry->d_name;
        int length = strlen(filename);

        // Check if filename contains both characters in the correct order
        for (int i = 0; i < length - 1; i++) {
            for(int j=i+1 ; j<length ; j++){
            if (filename[i] == firstChar && filename[j] == secondChar) {
                printf("%s\n", filename);
                add_to_staging(filename);
                break;

            }
        }}
    }

    closedir(dir);
    return 0;
    }else if(strcmp(argv[2], "-f")==0){
        for (int i = 0; i < argc-3; i++)
        {
           searchFileInCurrentDir(argv[3+i]);
             if (m==0)
             {
                    printf("file doesnt exist!");
             }else{
                 add_to_staging(argv[3+i]);
             }
           
        }
        
      return 0;
    }else if (strcmp(argv[2],"-n")==0)
    {
        printf("NOT stage:\n");
        DIR* dir;
        struct dirent* entry;

        dir = opendir(".");
        if (dir == NULL) {
        printf("Unable to open the directory.\n");
        return 1;
        }

        while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
        }
        printf("Stage:\n");
        FILE* file =fopen (".neogit/staging","r");
        if (file ==NULL)
        {
            printf("you have not stagged anything Yet!");
        }else{
            char word[100];
                while (!feof(file)) {
                 if (fgets(word, sizeof(word), file)) {
                 printf("%s", word);
                }
                }
        }
        
    closedir(dir);
        return 0;
    }else if (strcmp(argv[2] , "-redo")==0)
    {   
        char sourceDir[100]=".neogit/unstagingFolder";
        char destinationDir[100]= ".neogit/stagingFolder";

        DIR* sourceDirectory = opendir(sourceDir);
     if (sourceDirectory == NULL) {
        perror("خطا در باز کردن دایرکتوری منبع");
        return 1;
     }

     struct dirent* entry;
     while ((entry = readdir(sourceDirectory)) != NULL) {
        if (entry->d_type == DT_REG) {
            char sourcePath[400];
            snprintf(sourcePath, sizeof(sourcePath), "%s/%s", sourceDir, entry->d_name);

            char destinationPath[400];
            snprintf(destinationPath, sizeof(destinationPath), "%s/%s", destinationDir, entry->d_name);

            if (rename(sourcePath, destinationPath) != 0) {
                perror("خطا در انتقال فایل");
            }
        }
     }

     closedir(sourceDirectory);
        

       
        
        remove(".neogit/unstaging");
        rename(".neogit/tmp2_staging", ".neogit/staging");
        
    }else{
        add_to_staging(argv[2]);
    }
    return 0;
}
int add_to_staging(char *filepath) {

    FILE *file = fopen(".neogit/staging", "a");
    if (file == NULL) return 1;
    
    FILE * last_staging_file=fopen(".neogit/last_staging_files" , "w");
    if (last_staging_file== NULL) return 1;
    fprintf(last_staging_file, "%s\n", filepath);
    fclose(last_staging_file);


    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        int length = strlen(line);

        // remove '\n'
        if (length > 0 && line[length - 1] == '\n') {
            line[length - 1] = '\0';
        }

        if (strcmp(filepath, line) == 0) {
            fclose(file);
            return 0;
        }
    }
    fclose(file);
    
    file = fopen(".neogit/staging", "a");
    if (file == NULL) return 1;
    fprintf(file, "%s\n", filepath);
    fclose(file);
    int status=mkdir(".neogit/stagingFolder", 0777);
    char adrres[100]= ".neogit/stagingFolder/";
    strcat(adrres, filepath);
    int x = rename(filepath, adrres);
    if (x == 0) {
        printf("The file path has been changed\n");
    } else {
        printf("The file path has  NOT been changed, please try again\n");
    }
    
    return 0;
}
int run_reset(int argc, char *const argv[]) {
    // TODO: handle command in non-root directories 
    if (argc < 3) {
        printf("please specify a file");
        return 1;
    }
    if(strcmp(argv[2], "-f")==0){
        for (int i = 0; i < argc-3; i++)
        {
            remove_from_staging(argv[3+i]);
        }


    }if (strstr(argv[2], "*")!=NULL)
    {    char c [3];
        strcpy(c, argv[2]);
        char firstChar=c[0];
        char secondChar=c[2];
        DIR *dir;
        struct dirent *entry;
        
        dir = opendir(".neogit/stagingFolder");
        if (dir == NULL) {
        printf("Unable to open current directory");
        return 1;
        }

        while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR)
            continue; // Skip directories

        char *filename = entry->d_name;
        int length = strlen(filename);
        
        // Check if filename contains both characters in the correct order
        for (int i = 0; i < length - 1; i++) {
            for(int j=i+1 ; j<length ; j++){
            if (filename[i] == firstChar && filename[j] == secondChar) {
                printf("%s\n", filename);
                remove_from_staging(filename);
                break;

            }
        }}
    }

    closedir(dir);
    return 0;

    }else if (strcmp(argv[2], "-undo")==0)
    {   char word[100];
        FILE * last_staging_file=fopen(".neogit/last_staging_files" , "r");
        while (fscanf(last_staging_file,"%s" , word)!=EOF)
        {   
            remove_from_staging(word);
        }
         
    }else{

    return remove_from_staging(argv[2]);
    }
}
int remove_from_file(int argc, char *const argv[]){
     FILE *file, *tempFile;
    char filename[]="hooks_list";
    strcpy(filename, argv[2]);
    char tempFilename[] = "temp.txt";
    char line[100];
    char deleteString[100]="";
    strcpy(deleteString , argv[4]);
    int found = 0;
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }
    tempFile = fopen(tempFilename, "w");
    if (tempFile == NULL) {
        printf("Error creating temporary file.\n");
        fclose(file);
        return 1;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        if (strstr(line, deleteString) == NULL) {
            fputs(line, tempFile);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(tempFile);

    if (!found) {
        printf("The string to delete was not found in the file.\n");
        remove(tempFilename); // Delete the temporary file
    } else {
        remove(filename); // Delete the original file
        rename(tempFilename, filename); // Rename the temporary file to the original file
        printf("The string '%s' was successfully deleted from the file '%s'.\n", deleteString, filename);
    }

    return 0;
}
int remove_from_staging(char *filepath) {
    char string[100];
    strcpy(string , filepath);
    FILE *file = fopen(".neogit/staging", "r");
    if (file == NULL) return 1;
    
    FILE *tmp_file = fopen(".neogit/tmp_staging", "w");
    if (tmp_file == NULL) return 1;

    FILE *tmp2_file = fopen(".neogit/tmp2_staging", "w");
    if (tmp_file == NULL) return 1;

    copyFile(".neogit/staging",".neogit/tmp2_staging");  
    copyFile(".neogit/staging",".neogit/tmp_staging");    
    removeWordFromFile(".neogit/tmp_staging" , filepath);
    fclose(file);
    fclose(tmp_file);
    remove(".neogit/staging");
    rename(".neogit/tmp_staging", ".neogit/staging");
    
    int status=mkdir(".neogit/unstagingFolder", 0777);
    char currentPath[100];
    strncpy(currentPath, ".neogit/stagingFolder/", sizeof(currentPath));
    currentPath[sizeof(currentPath) - 1] = '\0'; 
    strncat(currentPath, filepath, sizeof(currentPath) - strlen(currentPath) - 1);

    char adrres[100]= ".neogit/unstagingFolder/";
    strcat(adrres, string);
   
    int x1=rename(currentPath,adrres);
    if (x1==0) {
        printf("Directory moved successfully.\n");
    } else {
        printf("Unable to move the directory.\n");
    }



    //در نهایت فایل اولیه تغییر میکند و فایلی که نامش در ریست امده حذف میشود اما یک کپی از ان داریم به نام : temp2_staging 
    return 0;
}

int check_status(int argc, char * const argv[]){
    char word [100];
    FILE* file=fopen(".neogit/staging" , "r");
    while ((fscanf(file, "%s" , word))!=EOF)
    {
        printf("%s : +" , word);

    }
    DIR *dir;
    struct dirent *entry;
    dir = opendir("unstagingFolder");
    if (dir == NULL) {
        printf("خطا در باز کردن فولدر!");
        return 1;
    }
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {  
            printf("%s : -\n", entry->d_name);
        }
    }

    closedir(dir);
    return 0;
}

int set_shortcut(int argc, char * const argv[]){
    
    char string [100]=" ";
    FILE * file=fopen(".neogit/shortcut" , "a");
    char finalword[100]=" ";
    
    
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-m")==0)
        {
            for (int j = i+1; j < argc -2; j++)
            {
                strcat(string, argv[j]);
                strcat(string, " ");
            }
            
        }
        
    }
    
    fprintf(file, " %s :\n%s\n" ,argv[argc-1], string);
    fclose(file);
    return 0;
}

int find_shortcut(int argc, char*const argv[]){
    if (argc<4)
    {
        printf("please use the correct format");
        return 0;
    }

    FILE *file =fopen(".neogit/shortcut" , "r");
    char word[100];
    strcpy(word,argv[3]);
    char string [100];
    char line[100];
    int found =0;
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, word) != NULL) {
            found = 1;
            break;
        }
    }

    if (found) {
        if (fgets(line, sizeof(line), file)) {
            strcpy(string,line);
            run_commit(argc,argv,string);

        } 
    } else {
        printf("shortcut doesnt exist!\n");
    }

    fclose(file);

   return 0;
}

int commitFile(int argc, char * const argv[]){
    
    if (argc < 3) {
        printf("please use the correct format\n");
        return 1;
    }else{
         char string [100]=" ";
        if (argc>3)
         {
        
            for (int i = 3; i < argc; i++)
            {
            strcat(string, argv[i]);
            strcat(string , " ");
         }
         }  
    
        if (strlen(string)>72)
             {
             printf("please Enter a shorter commit massage!");
             return 1;
            }
             
        return run_commit(argc, argv, string);
    }
   
    return 0;
}
void createDirectory(const char *path) {
    mkdir(path, 0777);
}
void clearCommitFolder() {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(COMMIT_FOLDER);
    if (dir == NULL) {
        perror("Error opening commit folder");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        char filepath[256];
        snprintf(filepath, sizeof(filepath), "%s/%s", COMMIT_FOLDER, entry->d_name);
        remove(filepath);
    }

    closedir(dir);
}
void copyFilesToCommitFolder() {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(STAGING_FOLDER);
    if (dir == NULL) {
        perror("Error opening staging folder");
        return;
    }

    createDirectory(COMMIT_FOLDER);

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) { // Check if it's a regular file
            char src[256], dest[256];
            snprintf(src, sizeof(src), "%s/%s", STAGING_FOLDER, entry->d_name);
            snprintf(dest, sizeof(dest), "%s/%s", COMMIT_FOLDER, entry->d_name);

            FILE *source = fopen(src, "rb");
            if (source == NULL) {
                perror("Error opening source file");
                continue;
            }

            FILE *destination = fopen(dest, "wb");
            if (destination == NULL) {
                perror("Error opening destination file");
                fclose(source);
                continue;
            }

            char ch;
            while ((ch = fgetc(source)) != EOF) {
                fputc(ch, destination);
            }

            fclose(source);
            fclose(destination);
        }
    }

    closedir(dir);
}

int make_last_commit(int argc, char * const argv[]){
    
    clearCommitFolder();
    copyFilesToCommitFolder();

    printf("Files copied and transferred successfully.\n");
      return 0;
}
int make_branch(int argc, char * const argv[]){
    FILE * file1=fopen(".neogit/commit/branch_names" , "r");
    char word[100];
    while (fscanf(file1, "%s" , word) != EOF)
    {
        if (strcmp(word , argv[2])==0)
        {
            printf("a branch with this name already exists!");
            return 1;
        }
        
    }
    
    struct dirent *de;
    DIR *dr = opendir(SOURCE_FOLDER);

    if (dr == NULL) {
        perror("Could not open source folder");
        return 1;
    }
    
    //createDirectory(DESTINATION_FOLDER);
    char name[100]=".neogit/commit/";
    FILE*file2=fopen(".neogit/commit/branch_names" , "a");
    fprintf(file2, argv[2]);
    fprintf(file2 , "\n");
    strcat(name, argv[2]);
    

    while ((de = readdir(dr)) != NULL) {
        if (de->d_type == DT_REG) { // Check if it's a regular file
            char sourceFilePath[256], destinationFilePath[256];
            sprintf(sourceFilePath, "%s/%s", SOURCE_FOLDER, de->d_name);
            sprintf(destinationFilePath, "%s/%s", name, de->d_name);
            copyFile(sourceFilePath, destinationFilePath);
        }
    }

    closedir(dr);

    printf("Files have been copied to the new folder successfully.\n");
    fclose(file2);
    return 0;

}

int run_commit(int argc, char * const argv[], char string[100]) {

    
    const char *targetFolderName = "commit";
    DIR *dir = opendir(".neogit");
    if (dir == NULL) {
        printf("خطا در باز کردن دایرکتوری اصلی!");
        return 1;
    }

    struct dirent *entry;

    int found = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) { // بررسی نوع فولدر
            if (strcmp(entry->d_name, targetFolderName) == 0) { 
                found = 1; 
                break;
            }
        }
    }

    closedir(dir);
    if (found ==0)
    {
         const char *newFolderName = ".neogit/commit";
        if (mkdir(newFolderName , 0777) != 0) {
        printf("خطا در ساخت فولدر!");
        return 1;
    }
    }
    DIR *dir1 = opendir(".neogit/stagingFolder");
    int fileCount = 0;
        while ((entry = readdir(dir1)) != NULL) {
            if (entry->d_type == DT_REG) { 
                fileCount++;
            }
        }
      
      if (fileCount==0)
      {
        printf("staging file is empty!\n");
        return 1;
      }
      make_last_commit(argc,argv);

    const char *newFolderName = ".neogit/commit";
    const char *sourceFolderName = ".neogit/stagingFolder";

    DIR *sourceDir = opendir(sourceFolderName);
    if (sourceDir==NULL) {
        return 1;
    }
  
    
    
    DIR *newDir = opendir(newFolderName);
    if (newDir == NULL) {
        printf("خطا در باز کردن فولدر جدید!");
        return 1;
    }

    // خواندن فایل ها از فولدر اصلی و کپی به فولدر جدید
    while ((entry = readdir(sourceDir)) != NULL) {
        if (entry->d_type == DT_REG) { // بررسی نوع فایل (Regular file)
            // ساخت مسیر فولدر اصلی و فولدر جدید
            char sourcePath[1024];
            char newPath[1024];

            snprintf(sourcePath, sizeof(sourcePath), "%s/%s", sourceFolderName, entry->d_name);
            snprintf(newPath, sizeof(newPath), "%s/%s", newFolderName, entry->d_name);

            // کپی فایل
            if (rename(sourcePath, newPath) != 0) {
                printf("خطا در کپی فایل %s!", entry->d_name);
            }
        }
    }
    // بستن فولدر ها

    FILE* commit_file=fopen(".neogit/commit/commit_file" , "a");
    fprintf(commit_file , "commit massage :%s\n" , string);
    time_t currentTime = time(NULL);
    struct tm *localTime = localtime(&currentTime);

    int hour = localTime->tm_hour;
    int minute = localTime->tm_min;

    fprintf( commit_file, "commit time: %02d:%02d\n", hour, minute);
    FILE * configfile=fopen(".neogit/config" , "r");
    char line [1000];
    char goal1[20]="username";
    char goal2[20]="branch";
    while (fgets(line, sizeof(line), configfile)) {
       
        if (strstr(line, goal1) != NULL) {
            fprintf(commit_file, "commit by %s", line); 
        }

        if (strstr(line, goal2)!= NULL)
        {
            fprintf(commit_file,  "commit in %s\n" , line);
        }
        
    }
    srand(time(0));
    int rand_number=rand();
    fprintf(commit_file, "commit ID : %d\n" , rand_number);
    fprintf(commit_file, "\n" );
    


    closedir(sourceDir);
    closedir(newDir);
    
    if (argc==3)
    {
        printf("commit massage: %s\n", string);
    }else{
        printf("commit massage:\"%s\" \n", string);
    }
    printf("commit time: %02d:%02d\n", hour, minute);
    printf(" commit ID : %d\n" , rand_number);

    return 0;
}

int inc_last_commit_ID() {
    FILE *file = fopen(".neogit/config", "r");
    if (file == NULL) return -1;
    
    FILE *tmp_file = fopen(".neogit/tmp_config", "w");
    if (tmp_file == NULL) return -1;

    int last_commit_ID;
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        if (strncmp(line, "last_commit_ID", 14) == 0) {
            sscanf(line, "last_commit_ID: %d\n", &last_commit_ID);
            last_commit_ID++;
            fprintf(tmp_file, "last_commit_ID: %d\n", last_commit_ID);

        } else fprintf(tmp_file, "%s", line);
    }
    fclose(file);
    fclose(tmp_file);

    remove(".neogit/config");
    rename(".neogit/tmp_config", ".neogit/config");
    return last_commit_ID;
}

bool check_file_directory_exists(char *filepath) {
    DIR *dir = opendir(".neogit/files");
    struct dirent *entry;
    if (dir == NULL) {
        printf("Error opening current directory");
        return 1;
    }
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, filepath) == 0) return true;
    }
    closedir(dir);

    return false;
}

int commit_staged_file(int commit_ID, char* filepath) {
    FILE *read_file, *write_file;
    char read_path[MAX_FILENAME_LENGTH];
    strcpy(read_path, filepath);
    char write_path[MAX_FILENAME_LENGTH];
    strcpy(write_path, ".neogit/files/");
    strcat(write_path, filepath);
    strcat(write_path, "/");
    char tmp[10];
    sprintf(tmp, "%d", commit_ID);
    strcat(write_path, tmp);

    read_file = fopen(read_path, "r");
    if (read_file == NULL) return 1;

    write_file = fopen(write_path, "w");
    if (write_file == NULL) return 1;

    char buffer;
    buffer = fgetc(read_file);
    while(buffer != EOF) {
        fputc(buffer, write_file);
        buffer = fgetc(read_file);
    }
    fclose(read_file);
    fclose(write_file);

    return 0;
}

int track_file(char *filepath) {
    if (is_tracked(filepath)) return 0;

    FILE *file = fopen(".neogit/tracks", "a");
    if (file == NULL) return 1;
    fprintf(file, "%s\n", filepath);
    return 0;
}

bool is_tracked(char *filepath) {
    FILE *file = fopen(".neogit/tracks", "r");
    if (file == NULL) return false;
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        int length = strlen(line);

        // remove '\n'
        if (length > 0 && line[length - 1] == '\n') {
            line[length - 1] = '\0';
        }
        
        if (strcmp(line, filepath) == 0) return true;

    }
    fclose(file); 

    return false;
}

int create_commit_file(int commit_ID, char *message) {
    char commit_filepath[MAX_FILENAME_LENGTH];
    strcpy(commit_filepath, ".neogit/commits/");
    char tmp[10];
    sprintf(tmp, "%d", commit_ID);
    strcat(commit_filepath, tmp);

    FILE *file = fopen(commit_filepath, "w");
    if (file == NULL) return 1;

    fprintf(file, "message: %s\n", message);
    fprintf(file, "files:\n");
    
    DIR *dir = opendir(".");
    struct dirent *entry;
    if (dir == NULL) {
        
        printf("Error opening current directory");
        return 1;
    }
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG && is_tracked(entry->d_name)) {
            int file_last_commit_ID = find_file_last_commit(entry->d_name);
            fprintf(file, "%s %d\n", entry->d_name, file_last_commit_ID);
        }
    }
    closedir(dir); 
    fclose(file);
    return 0;
}

int find_file_last_commit(char* filepath) {
    char filepath_dir[MAX_FILENAME_LENGTH];
    strcpy(filepath_dir, ".neogit/files/");
    strcat(filepath_dir, filepath);

    int max = -1;
    
    DIR *dir = opendir(filepath_dir);
    struct dirent *entry;
    if (dir == NULL) return 1;

    while((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            int tmp = atoi(entry->d_name);
            max = max > tmp ? max: tmp;
        }
    }
    closedir(dir);

    return max;
}

int run_checkout(int argc, char * const argv[]) {
    if (argc < 3) return 1;
    
    int commit_ID = atoi(argv[2]);

    DIR *dir = opendir(".");
    struct dirent *entry;
    while((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG && is_tracked(entry->d_name)) {
            checkout_file(entry->d_name, find_file_last_change_before_commit(entry->d_name, commit_ID));
        }
    }
    closedir(dir);

    return 0;
}

int find_file_last_change_before_commit(char *filepath, int commit_ID) {
    char filepath_dir[MAX_FILENAME_LENGTH];
    strcpy(filepath_dir, ".neogit/files/");
    strcat(filepath_dir, filepath);

    int max = -1;
    
    DIR *dir = opendir(filepath_dir);
    struct dirent *entry;
    if (dir == NULL) return 1;

    while((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            int tmp = atoi(entry->d_name);
            if (tmp > max && tmp <= commit_ID) {
                max = tmp;
            }
        }
    }
    closedir(dir);

    return max;
}

int checkout_file(char *filepath, int commit_ID) {
    char src_file[MAX_FILENAME_LENGTH];
    strcpy(src_file, ".neogit/files/");
    strcat(src_file, filepath);
    char tmp[10];
    sprintf(tmp, "/%d", commit_ID);
    strcat(src_file, tmp);

    FILE *read_file = fopen(src_file, "r");
    if (read_file == NULL) return 1;
    FILE *write_file = fopen(filepath, "w");
    if (write_file == NULL) return 1;
    
    char line[MAX_LINE_LENGTH];

    while (fgets(line, sizeof(line), read_file) != NULL) {
        fprintf(write_file, "%s", line);
    }
    
    fclose(read_file);
    fclose(write_file);

    return 0;
}
int todo_check(char * filename){
   char string[200]=".neogit/stagingFolder/";
   strcat(string , filename);
   FILE * file=fopen(string, "r");
   char word[10000];
   int flag=0;
   while (fscanf(file , "%s" , word)!=EOF)
   {
         if (strcmp(word , "TODO")==0)
         {
            flag=1;
         }
         
   }
   printf("%s:\n", filename);
   if(flag==1){
     printf("todo-check..............................FAILED\n");
   }
   else{
    printf("todo-check..............................PASSED\n");
   }
 fclose(file);
 } 
 
 int eof_blanck_space_check(char * filename){
   char string[200]=".neogit/stagingFolder/";
   char c;
   int hasWhitespace = 0;
   strcat(string , filename);
   FILE*file=fopen(string , "r");
   fseek(file, -1, SEEK_END); 
    while ((c = fgetc(file)) != EOF) {
        if (!isspace(c)) {
            hasWhitespace = 1;
            break;
        }
        fseek(file, -2, SEEK_CUR); 
    }
    fclose(file);
    printf("%s:\n", filename);
    if (hasWhitespace) {
        printf("eof-blank-check..............................FAILED\n");
    } else {
        printf("eof-blank-check..............................PASSED\n");
    }
    
    return 0;

}
int format_check(char * filename){
    char string[200]=".neogit/stagingFolder/";
   strcat(string , filename);
   
}
int balance_braces(char * filename){
    char string[200]=".neogit/stagingFolder/";
    strcat(string , filename);
    FILE *file;
    char ch;
    int openBrackets = 0;
    file = fopen(string, "r");
    if (file == NULL) {
        fprintf(stderr, "Cannot open file.\n");
        return -1;
    }

    while ((ch = fgetc(file)) != EOF) {
        if (ch == '(' || ch == '[' || ch == '{') {
            openBrackets++;
        } else if (ch == ')' || ch == ']' || ch == '}') {
            if (openBrackets == 0) {
                printf("Unmatched closing bracket found.\n");
                fclose(file);
                return 0;
            }
            openBrackets--;
        }
    }

    /*if (openBrackets > 0) {
        printf("Unmatched opening bracket found.\n");
    } else {
        printf("All brackets are properly matched.\n");
    }*/
    
    fclose(file);
    return 1;
}

int indentation_check(char * filename){
     char string[200]=".neogit/stagingFolder/";
   strcat(string , filename);
}
int static_error_check(char * filename){
     char string[200]=".neogit/stagingFolder/";
   strcat(string , filename);
}
int file_size_check(char * filename){
     char string[200]=".neogit/stagingFolder/";
   strcat(string , filename);
}
int character_limit(char * filename){
     char string[200]=".neogit/stagingFolder/";
   strcat(string , filename);
}
int time_limit(char * filename){
    char string[200]=".neogit/stagingFolder/";
   strcat(string , filename);
}
int searchFileInCurrentDir(const char* filename) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(".");
    if (dir == NULL) {
        perror("Unable to open current directory.");
        return 1;
    }
    
    while ((entry = readdir(dir)) != NULL) {
        
        if (strcmp(entry->d_name, filename) == 0) {
            
            m=1;
            return 0;
        }
    }

    closedir(dir);
    
    return 1;
}
void copyFile(const char* sourceFilename, const char* destinationFilename) {
    FILE* sourceFile = fopen(sourceFilename, "rb");
    if (sourceFile == NULL) {
        printf("Unable to open the source file.\n");
        return;
    }

    FILE* destinationFile = fopen(destinationFilename, "wb");
    if (destinationFile == NULL) {
        printf("Unable to create the destination file.\n");
        fclose(sourceFile);
        return;
    }
    
    char buffer[1024];
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), sourceFile)) > 0) {
        fwrite(buffer, 1, bytesRead, destinationFile);
    }

    fclose(sourceFile);
    fclose(destinationFile);

    printf("File copied successfully.\n");
}
int run_pre_commit(int argc, char * const argv[]){
    DIR *dir;
    struct dirent *entry;

    dir = opendir(".neogit/stagingFolder"); // Open the directory
    if (dir == NULL) {
        perror("Error opening directory");
        return 1;
    }

    while ((entry = readdir(dir)) != NULL) { // Read each entry in the directory
        if (entry->d_type == DT_REG) { // Check if it is a regular file
            char *filename = entry->d_name;
            char *fileExtension = strrchr(filename, '.'); // Get the file extension
            
            if (fileExtension) {
                if  (strcmp(fileExtension+1 , "c")==0 )
                {   char line[1000];
                    FILE * file=fopen("hooks_list" , "r");
                   
                    while (fgets(line , sizeof(line), file)!=NULL)
                    {  line[strcspn(line, "\n")] = '\0';
                      if(strcmp(line , "todo-check")==0){
                         todo_check(filename);
                      }
                      if (strcmp(line ,"eof-blank-space")==0)
                      {
                       eof_blanck_space_check(filename);
                      } 
                      if (strcmp(line , "format-check")==0)
                      {
                        format_check(filename);
                      }
                      if (strcmp(line , "balance_braces")==0)
                      {
                         int result=balance_braces(filename);
                         if (result==1)
                         {
                            printf("%s\n" , filename);
                            printf("balance-braces..............................PASS");
                         }else{
                             printf("%s\n" , filename);
                             printf("balance-braces..............................FAILED");
                         }
                         
                      }
                      if (strcmp(line,"indentation-check")==0)
                      {
                        indentation_check(filename);
                      }
                      if (strcmp(line , "static-error-check")==0)
                      {
                        static_error_check(filename);
                      }
                      if (strcmp(line, "file-size-check")==0)
                      {
                        file_size_check(filename);
                      }
                      if (strcmp(line, "character-limit")==0)
                      {
                        character_limit(filename);
                      }
                      

                    }
                    
                    fclose(file);

                }
                else if (strcmp(fileExtension+1 , "cpp")==0)
                {    char line[100];
                     FILE * file=fopen("hooks_list" , "r");
                     
                     while (fgets(line , sizeof(line), file)!=NULL)
                    {   line[strcspn(line, "\n")] = '\0';
                        if(strcmp(line , "todo-check")==0){
                         todo_check(filename);
                      }
                      if (strcmp(line ,"eof-blank-space")==0)
                      {
                        eof_blanck_space_check(filename);
                      }
                      if (strcmp(line , "format-check")==0)
                      {
                        format_check(filename);
                      }
                      if (strcmp(line , "balance_braces")==0)
                      {
                        balance_braces(filename);
                      }
                      if (strcmp(line,"indentation-check")==0)
                      {
                        indentation_check(filename);
                      }
                      if (strcmp(line , "static-error-check")==0)
                      {
                        static_error_check(filename);
                      }
                      if (strcmp(line, "file-size-check")==0)
                      {
                        file_size_check(filename);
                      }
                      if (strcmp(line, "character-limit")==0)
                      {
                        character_limit(filename);
                      }
                    }
                    fclose(file);
                   
                }else if (strcmp(fileExtension+1 , "txt")==0)
                {   char line[100];
                     FILE * file=fopen("hooks_list" , "r");
                     while (fgets(line , sizeof(line), file)!=NULL)
                    { line[strcspn(line, "\n")] = '\0';
                        if(strcmp(line , "todo-check")==0){
                         todo_check(filename);
                      }
                      if (strcmp(line ,"eof-blank-space")==0)
                      {
                        eof_blanck_space_check(filename);
                      }
                      if (strcmp(line , "format-check")==0)
                      {
                        format_check(filename);
                      }
                      if (strcmp(line , "balance_braces")==0)
                      {
                        balance_braces(filename);
                      }
                     
                      if (strcmp(line, "file-size-check")==0)
                      {
                        file_size_check(filename);
                      }
                      if (strcmp(line, "character-limit")==0)
                      {
                        character_limit(filename);
                      }
                    }
                    fclose(file);
                    
                }else if (strcmp(fileExtension+1 , "mp3")==0)
                {   char line[100];
                     FILE * file=fopen("hooks_list" , "r");
                     while (fgets(line , sizeof(line), file)!=NULL)
                    {line[strcspn(line, "\n")] = '\0';
                        if (strcmp(line , "time-limit")==0)
                        {
                            time_limit(filename);
                        }
                        if (strcmp(line , "format-check")==0)
                      {
                        format_check(filename);
                      }
                      if (strcmp(line, "file-size-check")==0)
                      {
                        file_size_check(filename);
                      }
                    }
                    fclose(file);
                    
                }else if (strcmp(fileExtension+1 , "mp4")==0)
                {   char line[100];
                    FILE * file=fopen("hooks_list" , "r");
                    while (fgets(line , sizeof(line), file)!=NULL)
                    {line[strcspn(line, "\n")] = '\0';
                        if (strcmp(line , "time-limit")==0)
                        {
                            time_limit(filename);
                        }
                        if (strcmp(line , "format-check")==0)
                      {
                        format_check(filename);
                      }
                      if (strcmp(line, "file-size-check")==0)
                      {
                        file_size_check(filename);
                      }
                    }
                    fclose(file);
                }else if (strcmp(fileExtension+1 , "wav")==0)
                {   char line[100];
                     FILE * file=fopen("hooks_list" , "r");
                     while (fgets(line , sizeof(line), file)!=NULL)
                    {line[strcspn(line, "\n")] = '\0';
                        if (strcmp(line , "time-limit")==0)
                        {
                            time_limit(filename);
                        }
                        
                        if (strcmp(line , "format-check")==0)
                      {
                        format_check(filename);
                      }
                      if (strcmp(line, "file-size-check")==0)
                      {
                        file_size_check(filename);
                      }

                    }   
                    fclose(file);
                    
                }else 
                {   char line[100];
                    FILE * file=fopen("hooks_list" , "r");
                    while (fgets(line , sizeof(line), file)!=NULL)
                    {line[strcspn(line, "\n")] = '\0';
                        if (strcmp(line, "file-size-check")==0)
                      {
                        file_size_check(filename);
                      }
                    }
                    fclose(file);
                }
                
                
                
                
                
                


            }

            
        }
    }

    closedir(dir); // Close the directory
    return 0;
}
void removeWordFromFile(const char* filename, const char* wordToRemove) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Unable to open the file.\n");
        return;
    }

    // محاسبه طول کلمه مورد نظر
    size_t wordLength = strlen(wordToRemove);

    // تخصیص حافظه برای خواندن محتوای فایل
    fseek(file, 0L, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    char* buffer = (char*) malloc(fileSize);
    if (buffer == NULL) {
        printf("Memory allocation failed.\n");
        fclose(file);
        return;
    }

    // خواندن محتوای فایل به حافظه
    fread(buffer, 1, fileSize, file);

    fclose(file);

    // جستجو و حذف کلمه از محتوای فایل
    char* position = buffer;
    while ((position = strstr(position, wordToRemove)) != NULL) {
        memmove(position, position + wordLength, strlen(position + wordLength) + 1);
    }

    // باز کردن فایل برای نوشتن محتوای جدید
    file = fopen(filename, "w");
    if (file == NULL) {
        printf("Unable to open the file for writing.\n");
        free(buffer);
        return;
    }

    // نوشتن محتوای جدید به فایل
    fwrite(buffer, 1, strlen(buffer), file);

    fclose(file);

    free(buffer);

    printf("Word removed successfully.\n");
}

void replaceLine(FILE *file, int lineNumber, const char *newLine) {
    int currentLine = 1;
    char line[1000];
   
    while (fgets(line, sizeof(line), file)) {
        
        if (currentLine == lineNumber) {
            
            fseek(file, -strlen(line), SEEK_CUR);
            fputs(newLine, file);
            break;
        }
        currentLine++;
    }
}
void printSurroundingLines(FILE *file, long currentPosition, int linesBefore, int linesAfter) {
    char line[MAX_LINE_LENGTH];
    int count = 0;
    fseek(file, currentPosition, SEEK_SET);
    while (count < linesBefore) {
        if (fgets(line, sizeof(line), file) != NULL) {
            printf("%s", line);
            count++;
        } else {
            break;
        }
    }
    fseek(file, currentPosition, SEEK_SET);
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);
        count++;
        if (count == linesBefore + linesAfter + 1) {
            break;
        }
    }
}
int check_hooks_list(int argc,char * const argv[]){
    FILE *file;
    const char *filename = "hooks_list";
    bool fileExists = false;
    file = fopen(filename, "r");
    if (file != NULL) {
        fileExists = true;
        fclose(file);
    }

    if (fileExists) {
        
    } else {
        file = fopen(filename, "a");
        if (file != NULL) {
            fclose(file);
            
        } else {
            printf("Error creating the file.\n");
        }
    }
    return 0;
}
int add_hook_list(int argc ,char * const argv[]){
   FILE * file=fopen("hooks_list" , "a");
   fprintf(file , "%s\n" , argv[4]);
}
void searchAndPrintLines(const char *filename, const char *searchLine, int linesBefore, int linesAfter) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    char line[MAX_LINE_LENGTH];
    long currentPosition = ftell(file);
    while (fgets(line, sizeof(line), file) != NULL) {
        if (strstr(line, searchLine) != NULL) {
            printSurroundingLines(file, currentPosition, linesBefore, linesAfter);
        }
        currentPosition = ftell(file);
    }
    fclose(file);
}
void hook_list(){
    printf("1-todo_check\n 2-eof_blanck_space \n 3-format-chaeck \n 4-balence_braces \n 5-indentation_check \n 6-static_error_check \n 7-file_size_check \n 8-character_limit \n 9-time_limit \n ");
}
int remove_shortcut(int argc, char * const argv[]){
    FILE*file=fopen(".neogit/shortcut" , "r");
    char word[100];
    char line[1000];
    int lineNumber = 1;
    int found =0;
    int line2;
    strcpy(word,argv[3]);
    while (fgets(line, sizeof(line), file)) {
        if (lineNumber % 2 != 0 && strstr(line, word) != NULL) {
            found =1;
            line2=lineNumber;
            break;

        }
        lineNumber++;
    }
    if (found==0)
    {
       printf("shortcut doesnt exist");
       return 1;
    }
    fclose(file);
    FILE*file2=fopen(".neogit/shortcut" , "r+");
    int line3=line2+1;
    char new[100]="                                         ";
    if (found==1)
    {
        replaceLine(file2, line2 , new);
        replaceLine(file2, line3, new);

    }

    fclose(file2);
    return 0;
}
int global(){
FILE *file;
    const char *filename = "global";
    bool fileExists = false;
    file = fopen(filename, "r");
    if (file != NULL) {
        fileExists = true;
        fclose(file);
    }

    if (fileExists) {
        printf("The file 'global' exists in the current directory.\n");
    } else {
        file = fopen(filename, "a");
        if (file != NULL) {
            fclose(file);
        } else {
            printf("Error creating the file.\n");
        }
    }

    return 0;}
int log_c(int argc, char * const argv[]){

    if (argc==2)
    {
         FILE* file=fopen(".neogit/commit/commit_file", "r");
        char line[1000];
        int number=0;
        while (fgets(line, sizeof(line), file))
        {
        printf("%s" , line);
        if (strcmp("ID" , line)==0)
        {
            number ++;
        }
        
        }
        printf("\n");
        printf("number of commited file: %d\n" , number);
        fclose(file);

    }else if (strcmp(argv[2], "-n")==0)
    {   
        FILE* file=fopen(".neogit/commit/commit_file", "r");
        if (file==NULL)
        {
            printf("cant open the file");
        }
        
        char line[1000];
        int number=0;
        while(fgets(line,sizeof(line), file)){
            if (strstr(line , "ID")!=NULL)
            {
               number++;
            }
            
        } 
        fclose(file);
        int number3=atoi(argv[3]);
      int n;
      if (number< number3)
      {
        printf("you just have %d commit!" , number);
        return 1;
      }
        char line2[100];
        int count=0;
        long position=0;
        n=number- number3;
        FILE* file2=fopen(".neogit/commit/commit_file", "r");
        while(fgets(line2, sizeof(line2), file2)) {
        position = ftell(file2); 
        if (strstr(line2, "ID") != NULL) { 
            count++;
            if (count == n) { 
                break; 
            }
        }
    }
    
    if (count < n) {
        printf("Error: ID not found %d times\n", n);
        return 1;
    }

    fseek(file2, position, SEEK_SET); 
    while (fgets(line2, sizeof(line2), file2) != NULL) {
        printf("%s", line2);
    }   
        fclose(file2);
        return 0;
        
    }else if (strcmp(argv[2], "-branch")==0)
    {   char line[100];
        int found=0;
        int read_line;
        int line_number=0;
        FILE * file=fopen(".neogit/commit/commit_file" , "r");
        char targetLine[100]="commit in branch: ";
        strcat(targetLine, argv[3]);
        int number=0;
        while(fgets(line,sizeof(line), file)){
            line_number++;
            if (strstr(line , targetLine)!=NULL)
            {
               read_line=line_number;
               int current_line=1;
               char buffer[100];
               bool keep_reading=true;
               FILE * file1=fopen(".neogit/commit/commit_file" , "r");
             do
             {
             fgets(buffer , sizeof(line) , file1);

             if (feof(file1) )
             {
                keep_reading=false;
                printf("there is not any branch with this name!");
             }else if (current_line==read_line-3)
            {
                 //keep_reading=false;
                printf("%s" , buffer);
            }else if (current_line==read_line-2)
            {
               // keep_reading=false;
                printf("%s" , buffer);
            }else if(current_line==read_line-1) {
               // keep_reading=false;
                printf("%s" , buffer);
            }else if(current_line==read_line) {
               // keep_reading=false;
                printf("%s" , buffer);
            }else if(current_line==read_line+1) {
                keep_reading=false;
                printf("%s" , buffer);
                
            }
            
            current_line ++;

        } while (keep_reading);
    }
            
        } 
        fclose(file);
        return 0;
    }else if (strcmp(argv[2], "-author")==0 )
    {  char line[100];
        int found=0;
        int read_line[100];
        int line_number=0;
        FILE * file=fopen(".neogit/commit/commit_file" , "r");
        char targetLine[100]="commit by username: ";
        strcat(targetLine, argv[3]);
        int number=0;
        while(fgets(line,sizeof(line), file)){
         line_number++;
         if (strstr(line , targetLine)!=NULL)
            {
                number++;
                read_line[number]=line_number;
            }
         }
        for (int i = number; i >0 ; i--)
        {
             int current_line=1;
               char buffer[100];
               bool keep_reading=true;
               FILE * file1=fopen(".neogit/commit/commit_file" , "r");
             do
             {
             fgets(buffer , sizeof(line) , file1);

             if (feof(file1) )
             {
                keep_reading=false;
                printf("there is not any branch with this name!");
             }else if (current_line==read_line[i]-2)
            {
                 //keep_reading=false;
                printf("%s" , buffer);
            }else if (current_line==read_line[i]-1)
            {
               // keep_reading=false;
                printf("%s" , buffer);
            }else if(current_line==read_line[i]) {
               // keep_reading=false;
                printf("%s" , buffer);
            }else if(current_line==read_line[i]+1) {
               // keep_reading=false;
                printf("%s" , buffer);
            }else if(current_line==read_line[i]+2) {
                keep_reading=false;
                printf("%s" , buffer);
                
            }
            
            current_line ++;

        } while (keep_reading);
    }
            fclose(file);

    }else if (strcmp(argv[2], "-since")==0)
    {  
        


    }else if (strcmp(argv[2], "-before")==0)
    {
        

    }else if (strcmp(argv[2], "-search")==0 && argc==4) 
    {
       char line[100];
        int found=0;
        int read_line[100];
        int line_number=0;
        FILE * file=fopen(".neogit/commit/commit_file" , "r");
        char targetLine[100]="commit massage: ";
        int number=0;
        while(fgets(line,sizeof(line), file)){
         line_number++;
         if (strstr(line , argv[3])!=NULL)
            {
                number++;
                read_line[number]=line_number;
            }
         }
        for (int i = number; i >0 ; i--)
        {
             int current_line=1;
               char buffer[100];
               bool keep_reading=true;
               FILE * file1=fopen(".neogit/commit/commit_file" , "r");
             do
             {
             fgets(buffer , sizeof(line) , file1);

             if (feof(file1) )
             {
                keep_reading=false;
                printf("there is not any branch with this name!");
             }else if (current_line==read_line[i])
            {
                 //keep_reading=false;
                printf("%s" , buffer);
            }else if (current_line==read_line[i]+1)
            {
               // keep_reading=false;
                printf("%s" , buffer);
            }else if(current_line==read_line[i]+2) {
               // keep_reading=false;
                printf("%s" , buffer);
            }else if(current_line==read_line[i]+3) {
               // keep_reading=false;
                printf("%s" , buffer);
            }else if(current_line==read_line[i]+4) {
                keep_reading=false;
                printf("%s" , buffer);
                
            }
            
            current_line ++;

        } while (keep_reading);
    }
            fclose(file);
    }else if (strcmp(argv[2], "-search")==0 && argc>4)
    {    printf("%d" , argc);
        for (int k = 3; k <= argc-1; k++)
        {
                   char line[100];
        int found=0;
        int read_line[100];
        int line_number=0;
        FILE * file=fopen(".neogit/commit/commit_file" , "r");
        char targetLine[100]="commit massage: ";
        int number=0;
        while(fgets(line,sizeof(line), file)){
         line_number++;
         if (strstr(line , argv[k])!=NULL)
            {
                number++;
                read_line[number]=line_number;
            }
         }
        for (int i = number; i >0 ; i--)
        {
             int current_line=1;
               char buffer[100];
               bool keep_reading=true;
               FILE * file1=fopen(".neogit/commit/commit_file" , "r");
             do
             {
             fgets(buffer , sizeof(line) , file1);

             if (feof(file1) )
             {
                keep_reading=false;
                printf("there is not any branch with this name!");
             }else if (current_line==read_line[i])
            {
                 //keep_reading=false;
                printf("%s" , buffer);
            }else if (current_line==read_line[i]+1)
            {
               // keep_reading=false;
                printf("%s" , buffer);
            }else if(current_line==read_line[i]+2) {
               // keep_reading=false;
                printf("%s" , buffer);
            }else if(current_line==read_line[i]+3) {
               // keep_reading=false;
                printf("%s" , buffer);
            }else if(current_line==read_line[i]+4) {
                keep_reading=false;
                printf("%s" , buffer);
                
            }
            
            current_line ++;

        } while (keep_reading);
    }
        printf("Next word: \n");

        }
       
    }
    
 return 0;

}
int replace_shortcut(int argc, char * const argv[]){

  char word[100];
  char line[1000];
  int lineNumber = 1;
  int found =0;
  int line2;
  strcpy(word, argv[argc-1]);
  char string[100]=" ";
   FILE * file=fopen(".neogit/shortcut" , "r");
   while (fgets(line, sizeof(line), file)) {
        if (lineNumber % 2 != 0 && strstr(line, word) != NULL) {
            found =1;
            line2=lineNumber;
            break;

        }
        lineNumber++;
    }
   line2++;
   if (found ==0)
   {
     printf("shortcut doesnt exist");
     return 1;
   }
   for (int i = 3; i < argc-2; i++)
   {
    strcat(string , argv[i]);
    strcat(string, " ");
   }
   
   fclose(file);

   FILE * file2=fopen(".neogit/shortcut" , "r+");
   if (found==1)
   { 
     replaceLine(file2 , line2, string);
   }
   fclose(file2);
   return 0;
}//-------------------------------------------------------------------------------------------------------------------------------------------------------
int main(int argc, char * argv[]) {
    if (argc < 2) {
        fprintf(stdout, "please enter a valid command");
        return 1;
    }
    char raya[100];
    int flag=0;
    print_command(argc, argv);
    FILE *command_file=fopen("command_file" , "a");
    fprintf(command_file ,"init\nadd\nreset\ncommit\ncheckout\nconfig\n");
    FILE*file2=fopen(".neogit/commit/branch_names" , "r+");
    if (fscanf(file2, "%s" ,raya )!=EOF)
    {   
        if (strcmp(raya , "master")==0)
        {   
            flag=1;
        }
        
    }
    if (flag==0)
    {
        fprintf(file2, "master\n");
    }
    //-----------00000------------0000000000-------------00000000000----------------0000000000000-------------00000000000000---------------00000000
    if (strcmp(argv[1], "init") == 0 ) {
        return run_init(argc, argv);

    }else if (strcmp(argv[1] , "branch")==0 && argc==3)
    {    
         check_neogit_directory(argc , argv);   
         if (x==0)
         { 
           make_branch(argc,argv);
         }

    }else if (strcmp(argv[1] , "branch")==0 && argc==2)
    {   char word_1[100];
        check_neogit_directory(argc , argv);   
         if (x==0)
         { 
           FILE * file3=fopen(".neogit/commit/branch_names" , "r");
           while (fscanf(file3 , "%s" , word_1 )!=EOF)
           {
             printf("%s\n" , word_1);
           }
           fclose(file3);
         }

    }else if (strcmp(argv[1], "commit") == 0 && strcmp(argv[2], "-m") == 0) {//********************************************************************
         check_neogit_directory(argc , argv);/*---------------------------------------------------------------------------------------------------*/
         if (x==0)                           /*---------------------------------------------------------------------------------------------------*/
         {                                   /*----------------------------------------------------------------------------------------------------*/
            return commitFile(argc, argv);   /*---------------------------------------------------------------------------------------------------*/
         }                                   /*---------------------------------------------------------------------------------------------------*/
    } else if (strcmp(argv[1], "commit") ==0 && strcmp(argv[2], "-s") == 0)/*----------------------------------------------------------------------*/
    {                                        /*---------------------------------------------------------------------------------------------------*/
         check_neogit_directory(argc , argv);/*---------------------------------------------------------------------------------------------------*/
         if (x==0)                            /*---------------------------------------------------------------------------------------------------*/
         {                                    /*---------------------------------------------------------------------------------------------------*/
            return find_shortcut(argc, argv); /*---------------------------------------------------------------------------------------------------*/
         }                                    /*---------------------------------------------------------------------------------------------------*/
                                              /*---------------------------------------------------------------------------------------------------*/
    }else if (strcmp(argv[1], "replace")==0)//*******************************************************************************************************
    {
       check_neogit_directory(argc , argv);
         if (x==0)
         {  
            return replace_shortcut(argc,argv);
         }

    } else if (strcmp(argv[1], "add") == 0) {
         check_neogit_directory(argc ,argv);
         if (x==0)
         {
            return run_add(argc, argv);
         }
         printf("ll");
    } else if (strcmp(argv[1], "reset") == 0) {
         check_neogit_directory(argc,argv);
         if (x==0)
         {
            return run_reset(argc, argv);
         }
    } else if (strcmp(argv[1], "checkout") == 0) {
         check_neogit_directory(argc , argv);
         if (x==0)
         {
            return run_checkout(argc, argv);
         }
    } else if(strcmp(argv[1], "config")==0 ){
        check_neogit_directory(argc , argv);
         if (x==0)
         {
            return create_configs(argc, argv);
         }
    } else if (strcmp(argv[1], "status") == 0)
    {
        check_neogit_directory(argc , argv);
         if (x==0)
         {
            check_status(argc,argv);
         }
    }else if (strcmp(argv[1] , "log")==0)
    {
        check_neogit_directory(argc , argv);
         if (x==0)
         { 
            log_c(argc,argv);
         }

    } else if (strcmp(argv[1] , "set")==0)
    {   
        check_neogit_directory(argc , argv);
         if (x==0)
         { 
           set_shortcut(argc,argv);
         }


    }else if (strcmp(argv[1] , "remove")==0)
    {   
        check_neogit_directory(argc , argv);
         if (x==0)
         { 
           remove_shortcut(argc,argv);
         }


    }else if (strcmp("pre-commit" , argv[1])==0)
    {   
        if (argc==2)
        {   
            run_pre_commit(argc, argv);
        }else if (strcmp(argv[3] , "list")==0)
        {
            hook_list();
        }else if (strcmp(argv[2] , "applied")==0)
        {
            char line_o[100];
            FILE * file =fopen("hooks_list" , "r");
            while (fgets(line_o , sizeof(line_o) , file)!=NULL)
            {
                printf("%s" , line_o);
            }
            
        }else if (strcmp(argv[2], "add")==0)
        {
            check_hooks_list(argc, argv);
            add_hook_list(argc,argv);
        }else if (strcmp(argv[2] , "remove")==0)
        {
            remove_from_file(argc, argv);
        }

    }
    
    else if (strstr(argv[2], "alias")==0 || strstr(argv[3], "alias")==0)//------------------------------------------------------------------------------
    {   
        check_neogit_directory(argc , argv);
       
         if (x==0)
         {  
             alias_config(argc,argv,command_file);
             
             argv[1]=argv[3];
             argv[2]=argv[4];
             argc=argc-2;

    if (strcmp(argv[3], "init") == 0 ) {
        return run_init(argc, argv);

    } else if (strcmp(argv[3], "add") == 0) {
         check_neogit_directory(argc ,argv);
         if (x==0)
         {
            return run_add(argc,argv);
         }
         printf("%d" ,argc );
    } else if (strcmp(argv[3], "reset") == 0) {
         check_neogit_directory(argc,argv);
         if (x==0)
         {
            return run_reset(argc, argv);
         }
    } else if (strcmp(argv[3], "commit") == 0) {
         check_neogit_directory(argc , argv);
         if (x==0)
         {
            return commitFile(argc, argv);
         }
    } else if (strcmp(argv[3], "checkout") == 0) {
         check_neogit_directory(argc , argv);
         if (x==0)
         {
            return run_checkout(argc, argv);
         }
    } else if((strcmp(argv[3], "config ")==0 && strcmp(argv[4], "user")==0)){
        check_neogit_directory(argc , argv);
         if (x==0)
         {
            return create_configs(argc, argv);
         }

    }}
    
    
    fclose(command_file);
    fclose(file2);
    return 0;
       } }

