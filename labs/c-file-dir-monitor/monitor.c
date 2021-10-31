#include <stdio.h>
#include <sys/inotify.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "logger.h"


int addNotifyWatches(int __fd, const char *__name, uint32_t __mask,char** wdNameMap)
{
    int dir_count = 0, wd;
    struct dirent* dent;

    char* path = realpath(__name, NULL);
    DIR* srcdir = opendir(path);

    wd = inotify_add_watch(__fd, __name, __mask);
    if (wd == -1)
        return printf("Error: inotify_add_watch");
    
    printf("Starting File/Directory Monitor on %s\n", __name);

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
            printf("Error: %s\n",dent->d_name);
            continue;
        }

        if (S_ISDIR(st.st_mode)){
            path = realpath(dent->d_name, NULL);
            wd = inotify_add_watch(__fd, path, __mask);
            if (wd == -1)
                return printf("Error: inotify_add_watch");
            wdNameMap[wd]=malloc((strlen(dent->d_name)+1)*sizeof(char*));
            strcpy(wdNameMap[wd], dent->d_name);
            // printf("Watching %s using wd %d\n", path, wd);
        }
    }
    closedir(srcdir);

    return dir_count;
}

static void displayInotifyEvent(struct inotify_event *i,char** wdNameMap)
 {
     
    if(i->len<=0) return;

    if (i->mask & IN_MOVED_TO) {
        printf(" -> ");
        if(i->wd && i->wd>1) printf("%s/",wdNameMap[i->wd]);
        printf("%s\n",i->name);
        return;
    }
    
    printf("[ ");

     if (i->mask & IN_ISDIR){
         printf("Directory - ");
    }else{
        printf("File - ");
    }

    if (i->mask & IN_CREATE)        printf("Create");
    if (i->mask & IN_DELETE)        printf("Delete");
    if (i->mask & IN_MOVED_FROM)    printf("Rename");
    printf(" ] - ");
 
    if(i->wd && i->wd>1) printf("%s/",wdNameMap[i->wd]);

    printf("%s", i->name);
    if(!(i->mask & IN_MOVED_FROM)){
        printf("\n");
    }
 }

#define BUF_LEN (256 * (sizeof(struct inotify_event) + NAME_MAX + 1))

int main(int argc, char *argv[]){
    
    int inotifyFd, j;
    ssize_t numRead;
    char *p;
    struct inotify_event *event;

    if (argc < 2)
        return printf("Error: %s is missing pathname parameter\n", argv[0]);

    inotifyFd = inotify_init();
    if (inotifyFd == -1)
        return printf("Error: inotify_init");

    
    char** wdNameMap = malloc(256*sizeof(char*));
    addNotifyWatches(inotifyFd, argv[1], IN_CREATE | IN_MOVE | IN_DELETE, wdNameMap);
    
    char buf[BUF_LEN] __attribute__ ((aligned(8)));
 
    for (;;) {
        numRead = read(inotifyFd, buf, BUF_LEN);
        if (numRead == 0)
            return printf("Error: read() from inotify fd returned 0!");

        if (numRead == -1)
            return printf("Error: read");

        /* Process all of the events in buffer returned by read() */

        for (p = buf; p < buf + numRead; ) {
            event = (struct inotify_event *) p;
            displayInotifyEvent(event,wdNameMap);

            p += sizeof(struct inotify_event) + event->len;
        }
    }

    return 0;
}
