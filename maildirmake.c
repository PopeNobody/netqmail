#include "strerr.h"
#include "exit.h"
#include "errno.h"

#define FATAL "maildirmake: fatal: "
int chdir(char *dir);
int mkdir(char *dir, int mode);
int dprintf(int, char *, ...);
int umask(int);
unsigned long strlen(char *str);
int d=0;
int mkdir_p(char *dir, int mode);
int _mkdir_p(char *dir, int mode);
int mkdir_v(char *dir, int mode);
//   int _mkdir_v(char *dir, int mode);

//   int _mkdir_v(char *dir, int mode) {
//     int res=mkdir(dir,mode);
//     return res;
//   };
int mkdir_v(char *dir, int mode) {
  dprintf(2,"%5d mkdir_v(%s,%o)\n", ++d, dir, mode);
  int res=mkdir  (dir,mode);
  char *fmt = res 
    ? "%5d mkdir_v(%s,%o) => %d (%m)\n"
    : "%5d mkdir_v(%s,%o) => %d\n";
  dprintf(2,fmt,d--,dir,mode,res);
  return res;
};
int _mkdir_p(char *dir, int mode) {
  if(mkdir_v(dir,mode)==0)
    return 0;
  if(errno==ENOENT) {
    unsigned long len=strlen(dir);
    char *end=dir+len;
    // gotta skip trailing slashes
    // to avoid getting an EEXIST after
    while(end>dir && end[-1]=='/')
      --end;
    while(end>dir && end[-1]!='/')
      --end;
    while(end>dir && end[-1]=='/')
      --end;
    if(end==dir)
      return -1;
    char *tmp=end;
    *tmp=0;
    int res=mkdir_p(dir,mode);
    *tmp='/';
    return mkdir_v(dir,mode);
//     } else if ( errno==EEXIST ) {
//       return 0;
  } else {
    return -1;
  }
};
int mkdir_p(char *dir, int mode) {
  dprintf(2,"%5d mkdir_p(%s,%o)\n", ++d, dir, mode);
  int res=_mkdir_p(dir,mode);
  char *fmt = res 
    ? "%5d mkdir_p(%s,%o) => %d (%m)\n"
    : "%5d mkdir_p(%s,%o) => %d\n";
  dprintf(2,fmt,d--,dir,mode,res);
  return res;
};
void main(argc,argv)
int argc;
char **argv;
{
  umask(077);
  if (!argv[1])
    strerr_die1x(100,"maildirmake: usage: maildirmake name");
  if (mkdir_p(argv[1],0700) == -1)
    strerr_die4sys(111,FATAL,"unable to mkdir ",argv[1],": ");
  if (chdir(argv[1]) == -1)
    strerr_die4sys(111,FATAL,"unable to chdir to ",argv[1],": ");
  if (mkdir_p("tmp",0700) == -1)
    strerr_die4sys(111,FATAL,"unable to mkdir ",argv[1],"/tmp: ");
  if (mkdir_p("new",0700) == -1)
    strerr_die4sys(111,FATAL,"unable to mkdir ",argv[1],"/new: ");
  if (mkdir_p("cur",0700) == -1)
    strerr_die4sys(111,FATAL,"unable to mkdir ",argv[1],"/cur: ");
  _exit(0);
}
