#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

char typeFichier(int  m);
void strDroitU(int st_mode, char * du);

int main(int argc, char *argv[]) {
    struct dirent *direntp;
    DIR *dirp;
    char file_path[NAME_MAX+1];
    struct stat file_stat;
    off_t file_size;
    
    if (argc != 2) {
        fprintf(stderr, "Usage: %s directory_name\n", argv[0]);
        return 1;
    }

    if ((dirp = opendir(argv[1])) == NULL) {
        perror ("Failed to open directory");
        return 1;
    }

    while ((direntp = readdir(dirp)) != NULL)
    {
        sprintf( file_path, "%s/%s", argv[1], direntp->d_name );
        if ( stat( file_path, &file_stat ) != 0 ) {
            fprintf( stderr, "Failed to stat \"%s\": %s\n",
                    file_path, strerror( errno ) );
        }
        else
        {
            file_size = file_stat.st_size;
            char du[4];
            strDroitU(file_stat.st_mode,du);
            printf("%c%s %ld\t%s\n",typeFichier(file_stat.st_mode),du, file_size,direntp->d_name);
        }
    }

    while ((closedir(dirp) == -1) && (errno == EINTR)) ;
    
    return 0;
}

//calcule du type de fichier
char typeFichier(int st_mode){
    if(S_ISREG(st_mode))
        return '-';
    if (S_ISDIR(st_mode))
        return 'd';
    return '?'; //cas non traite
}

//calcul des droits utilisateurs
void strDroitU(int st_mode, char * du){
    if (st_mode&S_IRUSR)
        du[0]='r';
    else
        du[0]='-';
    if (st_mode&S_IWUSR)
        du[1]='w';
    else
        du[1]='-';
    if (st_mode&S_IXUSR)
        du[2]='r';
    else
        du[2]='-';
    du[3]='\0';
}
