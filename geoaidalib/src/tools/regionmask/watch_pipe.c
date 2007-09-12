/***************************************************************************
                          watch_pipe.c  -  description
                             -------------------
    begin                : Tue Feb 12 2002
    copyright            : (C) 2002 by jrgen bckner
    email                : bueckner@tnt.uni-hannover.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <libgen.h>
#include <errno.h>
#include <string.h>

#undef DEBUG_MSG

void Usage(const char* prg_name)
{
  printf("Usage:\n"
         "  %s <pipename> <lockfile> <pid> <timeout>\n",prg_name);
}

/* create lockfile
 * ret=0, lockfile failed
 * ret=1, lockfile success
 * ret=2, lockfile timeout
 */
int lockfile(const char *lockname, int timeout)
{
  char hostname[256];
  char uniquefile[1024];
  char *dir;
  char buffer1[1024];
  int fd;
  struct stat buf;

  strcpy(buffer1,lockname);
  dir=dirname(buffer1);


  gethostname(hostname, 255);
  sprintf(uniquefile,"%s/%s.%d",dir,hostname,getpid());
  fd=open(uniquefile,O_WRONLY|O_CREAT|O_EXCL,S_IRUSR|S_IRGRP|S_IROTH);
  if (fd==-1) return 0;
  while (timeout>=0) {
    if (!link(uniquefile,lockname)) {
      unlink(uniquefile);
      return 1;
    }
    if (stat(uniquefile,&buf)) {
      unlink(uniquefile);
      return 0;
    }
    if (buf.st_nlink==2) {
      unlink(uniquefile);
      return 1;
    }
    if (timeout) {
      timeout-=10;
      if (!timeout) timeout=-1;
    }
    sleep(10);
  }
  return 2;
}

int checkPid(int pid)
{
  int e;
  char buffer[1024];
  sprintf(buffer,"/proc/%d",pid);
  e=mkdir(buffer,0);
  if (e==0) return 0;
  if (errno!=EEXIST) return 0;
  printf("watch_pipe: Process %d is running\n",pid);
  return 1;
}

void get_lock_or_die(char *pipename, char* lockname, int pid)
{
  int lock_state;
  do {
    lock_state=lockfile(lockname,241);
    if (!lock_state) {
      fprintf(stderr,"Can't get lock on %s\n",lockname);
      exit(1);
    }
    if (lock_state==2) {
      if (!checkPid(pid)) {
        unlink(pipename);
        unlink(lockname);
        exit(1);
      };
    }
  } while (lock_state==2);
}

int main(int argc, char **argv)
{
  struct stat buf;
  int timeout=0;
  int lock_state;
  int pid;
  time_t current_time;
  time_t file_time;
  FILE *fp;
  if (argc<4) {
    Usage(argv[0]);
    return 1;
  }
  sscanf(argv[4],"%d",&timeout);
  if (timeout<=0) {
    fprintf(stderr,"timeout(%d) must be a positive integer\n",timeout);
    return 1;
  }
  fp=fopen(argv[3],"r");
  if (!fp) {
    fprintf(stderr,"Can't open pid file %s\n",argv[3]);
    return 1;
  }
  if (fscanf(fp,"%d",&pid)!=1){
    fprintf(stderr,"Can't read pid file %s\n",argv[3]);
    return 1;
  }

  get_lock_or_die(argv[1],argv[2],pid);
  do {
    unlink(argv[2]);
    sleep(10);
    get_lock_or_die(argv[1],argv[2],pid);
    current_time=time(0);
#ifdef DEBUG_MSG
    printf("time=%d\n",current_time);
#endif
    if (stat(argv[1],&buf)) {
      fprintf(stderr,"pipe %s not found!\n",argv[1]);
      unlink(argv[2]);
      return 1;
    }
    file_time=buf.st_mtime+timeout;
#ifdef DEBUG_MSG
    printf("file_time=%d\n",file_time);
#endif
  } while (current_time<file_time);
  fp=fopen(argv[1],"w");
  if (!fp) {
    fprintf(stderr,"Can't open pipe %s!\n",argv[1]);
    return 1;
  }
  fprintf(fp,"end\n");
  fclose(fp);
  sleep(10);
  unlink(argv[1]);
  unlink(argv[2]);
  return 0;
}
