#include <stdio.h>
#include <sys/inotify.h>
#include <dirent.h>
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "logger.h"

int get_dirs(const char* path, char* strings[])
{
    int dir_count = 0;
    struct dirent* dent;
    DIR* srcdir = opendir(path);

    char** tmp;

    if (srcdir == NULL)
    {
        perror("opendir");
        return -1;
    }

    while((dent = readdir(srcdir)) != NULL)
    {
        struct stat st;

        if(strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0)
            continue;

        if (fstatat(dirfd(srcdir), dent->d_name, &st, 0) < 0)
        {
            perror(dent->d_name);
            continue;
        }

        if (S_ISDIR(st.st_mode)) {
            dir_count++;
            tmp = strings;
            strings = malloc(dir_count * sizeof(char*));
            for(int i=0;i<dir_count-1;i++){
                strings[i] = malloc((strlen(tmp[i])+1) * sizeof(char));
                strcpy(strings[i], tmp[i]);
            }
            strings[dir_count] = malloc((strlen(dent->d_name)+1) * sizeof(char));
            strcpy(strings[dir_count], dent->d_name);
        };
    }
    closedir(srcdir);

    // opendir()

    return dir_count;
}

//Maybe change this for the logger?
static void displayInotifyEvent(struct inotify_event *i)
 {
     printf("-------------------\n");
     printf("    wd =%2d; ", i->wd);
     if (i->cookie > 0)
         printf("cookie =%4d; ", i->cookie);
 
     printf("mask = ");
    //  if (i->mask & IN_ACCESS)        printf("IN_ACCESS ");
    //  if (i->mask & IN_ATTRIB)        printf("IN_ATTRIB ");
    //  if (i->mask & IN_CLOSE_NOWRITE) printf("IN_CLOSE_NOWRITE ");
    //  if (i->mask & IN_CLOSE_WRITE)   printf("IN_CLOSE_WRITE ");
     if (i->mask & IN_CREATE)        printf("IN_CREATE ");
     if (i->mask & IN_DELETE)        printf("IN_DELETE ");
    //  if (i->mask & IN_DELETE_SELF)   printf("IN_DELETE_SELF ");
    //  if (i->mask & IN_IGNORED)       printf("IN_IGNORED ");
    //  if (i->mask & IN_ISDIR)         printf("IN_ISDIR ");
    //  if (i->mask & IN_MODIFY)        printf("IN_MODIFY ");
    //  if (i->mask & IN_MOVE_SELF)     printf("IN_MOVE_SELF ");
    //  if (i->mask & IN_MOVED_FROM)    printf("IN_MOVED_FROM ");
    //  if (i->mask & IN_MOVED_TO)      printf("IN_MOVED_TO ");
     if (i->mask & IN_MOVE)      printf("IN_MOVED ");
    //  if (i->mask & IN_OPEN)          printf("IN_OPEN ");
    //  if (i->mask & IN_Q_OVERFLOW)    printf("IN_Q_OVERFLOW ");
    //  if (i->mask & IN_UNMOUNT)       printf("IN_UNMOUNT ");
     printf("\n");
 
     if (i->len > 0)
         printf("        name = %s\n", i->name);
 }

 #define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))//Change this into the amount of dirs found?

int main(int argc, char *argv[]){
    
    int inotifyFd, wd, j;
    char buf[BUF_LEN] __attribute__ ((aligned(8)));
    ssize_t numRead;
    char *p;
    struct inotify_event *event;

    if (argc < 2)
        return printf("Error: %s pathname...\n", argv[0]);

    char* paths[1] = {argv[1]}; 
    //seg fault TODO
    get_dirs(argv[1],paths);

    inotifyFd = inotify_init();
    if (inotifyFd == -1)
        return printf("Error: inotify_init");

    for (j = 1; j < argc; j++) {
        printf("added watch\n");
         wd = inotify_add_watch(inotifyFd, argv[j], IN_CREATE | IN_MOVE | IN_DELETE);
         if (wd == -1)
             return printf("Error: inotify_add_watch");
 
         printf("Watching %s using wd %d\n", argv[j], wd);
     }
 
    for (;;) {                                  /* Read events forever */
        numRead = read(inotifyFd, buf, BUF_LEN);
        if (numRead == 0)
            return printf("Error: read() from inotify fd returned 0!");

        if (numRead == -1)
            return printf("Error: read");

        /*FIXME: should use %zd here, and remove (long) cast */
        printf("Read %ld bytes from inotify fd\n", (long) numRead);

        /* Process all of the events in buffer returned by read() */

        for (p = buf; p < buf + numRead; ) {
            event = (struct inotify_event *) p;
            displayInotifyEvent(event);

            p += sizeof(struct inotify_event) + event->len;
        }
    }

    return 0;
}
