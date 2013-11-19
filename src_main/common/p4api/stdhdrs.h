/*
 * Copyright 1995, 1996 Perforce Software.  All rights reserved.
 *
 * This file is part of Perforce - the FAST SCM System.
 */

/*
 * Standard headers
 */

# include <stdio.h>
# include <string.h>
# include <stdlib.h>

# if !defined( OS_QNX ) && !defined( OS_VMS )
# include <memory.h>
# endif
# ifdef OS_NEXT
# include <libc.h>
# endif

/*
 * NEED_ACCESS - access()
 * NEED_BRK - brk()/sbrk()
 * NEED_CHDIR - chdir()
 * NEED_CHMOD - chmod()
 * NEED_EBCDIC - __etoa, __atoe
 * NEED_ERRNO - errno, strerror
 * NEED_FILE - write(), unlink(), etc
 * NEED_FCNTL - O_XXX flags
 * NEED_FLOCK - LOCK_UN, etc
 * NEED_FORK - fork(), waitpid()
 * NEED_FSYNC - fsync()
 * NEED_GETPID - getpid()
 * NEED_GETUID - getuid(),setuid() etc.
 * NEED_MKDIR - mkdir()
 * NEED_MMAP - mmap()
 * NEED_OPENDIR - opendir(), etc
 * NEED_POPEN - popen(), pclose()
 * NEED_SIGNAL - signal()
 * NEED_SLEEP - Sleep()
 * NEED_STAT - stat()
 * NEED_SYSLOG - syslog()
 * NEED_TIME - time(), etc
 * NEED_TYPES - off_t, etc (always set)
 * NEED_UTIME - utime()
 */

# if defined( NEED_ACCESS ) || \
	defined( NEED_CHDIR ) || \
	defined( NEED_EBCDIC ) || \
	defined( NEED_FILE ) || \
	defined( NEED_FSYNC ) || \
	defined( NEED_FORK ) || \
	defined( NEED_GETCWD ) || \
	defined( NEED_GETPID ) || \
	defined( NEED_GETPWUID ) || \
	defined( NEED_GETUID ) || \
	defined( NEED_BRK ) || \
	defined( NEED_SLEEP )

# ifndef OS_NT
# include <unistd.h>
# endif

# ifdef OS_VAXVMS
# include <unixio.h>
# endif

# endif

# if defined( NEED_BRK )
# if !defined( OS_NT ) && !defined( MAC_MWPEF ) && \
     !defined( OS_AS400 ) && !defined( OS_MVS ) && \
     !defined( OS_LINUX ) && !defined( OS_DARWIN )
# define HAVE_BRK
# endif
# endif

# if defined( NEED_GETUID )
# ifdef unix 
# define HAVE_GETUID
# endif
# endif

# if defined( NEED_EBCDIC ) 
# if defined( OS_AS400 )
# include <ebcdic.h>
# endif
# endif

# if defined( NEED_ACCESS ) || defined( NEED_CHDIR )
# if defined( OS_NT ) || defined( OS_OS2 )
# include <direct.h>
# endif
# endif

# if defined( NEED_ERRNO )
# ifdef OS_AS400
extern int errno;
# endif
# include <errno.h>
# endif

# if defined(NEED_FILE) || defined(NEED_FSYNC)
# ifdef OS_NT
# include <io.h>
# endif
# endif

# ifdef NEED_FCNTL
# include <fcntl.h>
# endif

# ifdef NEED_FLOCK
# ifdef OS_NT
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
# include <io.h>
# endif
# ifdef unix
# include <sys/file.h>
# ifdef LOCK_UN
// 2.96 linux ia64 declares it with a __THROW
# ifndef __THROW
extern "C" int flock( int, int );
# endif
# endif
# endif
# endif

# ifdef NEED_FORK
# ifdef OS_MACHTEN
# include "/usr/include/sys/wait.h"
# endif 
# if !defined( OS_OS2 ) && !defined( MAC_MWPEF ) && !defined( OS_NT )
# define HAVE_FORK
# include <sys/wait.h>
# endif 
# endif

# ifdef NEED_FSYNC
# if !defined( OS_BEOS ) && !defined( OS_NT ) && \
     !defined( OS_MAC ) && !defined( OS_OS2 )
# define HAVE_FSYNC
# endif
# endif

# ifdef NEED_GETCWD
# ifdef OS_NEXT
# define getcwd( b, s ) getwd( b )
# endif
# if defined(OS_OS2) || defined(OS_NT)
extern "C" char *getcwd( char *buf, size_t size );
# endif
# ifdef OS_VMS
# include <unixlib.h>
# endif
# endif 

# ifdef NEED_GETHOSTNAME

# ifdef OS_BEOS
# include <net/netdb.h>
# endif

# ifdef OS_VMS
# include <socket.h>
# endif

# if defined(OS_PTX) || \
	defined(OS_QNX) || \
	defined(OS_AIX32) || \
	defined(OS_NCR) || \
	defined(OS_UNIXWARE2)

extern "C" int gethostname( char * name, int namelen );

# endif

# if defined(OS_NT) 
extern "C" int __stdcall gethostname( char * name, int namelen );
# endif

# if !defined(OS_OS2) && !(defined(OS_MAC) || defined(USE_CARBON))
# define HAVE_GETHOSTNAME
# endif

# endif

# ifdef NEED_GETPID
# ifdef OS_NT
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
# define getprocid() GetCurrentThreadId()
# else
# ifdef OS_MAC
# define getprocid() 1000 // none on mac
# else
# define getprocid() getpid()
# if defined(OS_OS2)
# include <process.h>
# endif /* OS2 */
# endif /* MAC */
# endif	/* NT */
# endif	/* GETPID */

# ifdef NEED_GETPWUID
# if !defined( OS_VMS ) && !defined( OS_NT ) && !defined( OS_BEOS ) && \
	!defined( MAC_MWPEF ) && !defined( OS_OS2 )
# include <pwd.h>
# define HAVE_GETPWUID
# endif /* UNIX */
# endif 

# if defined(NEED_MKDIR) || defined(NEED_STAT) || defined(NEED_CHMOD)

# ifdef OS_OS2
# include <direct.h>
# endif

# if !defined( OS_MAC )
# include <sys/stat.h>
# endif

# ifdef OS_MAC
# include <stat.h>
# include <fcntl.h> /* for chmod() */
# endif

# ifdef OS_MACOSX
# include "i18napi.h"	/* for CharSetApi */
# include "macfile.h"	/* for MacFile */
# endif

# ifdef NEED_CHMOD
# if defined( OS_MACOSX ) || ( OS_DARWIN )
extern int darwin_chmod( const char *, mode_t );
# define P4CHMOD(p,m) darwin_chmod(p,m)
# elif defined( OS_NT )
extern int nt_chmod( const char *, int );
# define P4CHMOD(p,m) nt_chmod(p,m)
# else
# define P4CHMOD(p,m) chmod(p,m)
# endif
# endif

# ifndef S_ISLNK /* NCR */
# define S_ISLNK(m) (((m)&S_IFMT)==S_IFLNK)
# endif

# ifndef S_ISDIR /* NEXT */
# define S_ISDIR(m) (((m)&S_IFMT)==S_IFDIR)
# define S_ISREG(m) (((m)&S_IFMT)==S_IFREG)
# endif

# ifdef OS_NT
# define PERM_0222 (S_IWRITE)
# define PERM_0266 (S_IWRITE)
# define PERM_0666 (S_IRUSR|S_IWRITE)
# define PERM_0777 (S_IRUSR|S_IWRITE|S_IEXEC)
# ifndef S_IRUSR
# define S_IRUSR S_IREAD
# define S_IWUSR S_IWRITE
# define S_IXUSR S_IEXEC
# endif	/* S_IRUSR */
# endif

# ifndef PERM_0222
# define PERM_0222 (S_IWUSR | S_IWGRP | S_IWOTH)
# define PERM_0266 (S_IWUSR | (S_IRGRP|S_IWGRP) | (S_IROTH|S_IWOTH))
# define PERM_0666 ((S_IRUSR|S_IWUSR) | (S_IRGRP|S_IWGRP) | (S_IROTH|S_IWOTH))
# define PERM_0777 (S_IRWXU | S_IRWXG | S_IRWXO)
# endif

# endif

/* Many users don't define NEED_MMAP -- so we always find out */

# if !defined( OS_AS400 ) && !defined( OS_BEOS ) && \
	!defined( OS_HPUX68K ) && !defined( OS_MAC ) && \
	!defined( OS_MACHTEN ) && !defined( OS_MVS ) && \
	!defined( OS_VMS62 ) && !defined( OS_OS2 ) && \
	!defined( OS_NEXT ) && !defined( OS_NT ) && \
	!defined( OS_QNX ) && !defined( OS_UNICOS ) && \
	!defined( OS_MPEIX ) && !defined( OS_QNXNTO ) && \
	!defined( OS_MACOSX ) && !defined( OS_ZETA )

# define HAVE_MMAP
# ifdef NEED_MMAP
# ifdef OS_HPUX9
extern "C" caddr_t mmap(const caddr_t, size_t, int, int, int, off_t);
extern "C" int munmap(const caddr_t, size_t);
# endif /* HPUX9 */
# include <sys/mman.h>
# endif /* NEED_MMAP */
# endif /* HAVE_MMAP */

# ifdef NEED_OPENDIR
# include <dirent.h>
# endif

# ifdef NEED_POPEN
# ifdef OS_NT
# define popen _popen
# define pclose _pclose
# endif
# ifdef OS_MVS
extern "C" int pclose(FILE *);
extern "C" FILE *popen(const char *, const char *);
# endif
# endif

# ifdef NEED_SIGNAL
# ifdef OS_OSF
# include "/usr/include/sys/signal.h"
# else
# include <signal.h>
# endif
# if defined( OS_NEXT ) || defined( OS_MPEIX )
// broken under gcc 2.5.8
# undef SIG_IGN
# undef SIG_DFL
# define SIG_DFL         (void (*)(int))0
# define SIG_IGN         (void (*)(int))1
# endif
# endif

# ifdef NEED_SYSLOG
# if defined( unix )
# include <syslog.h>
# define HAVE_SYSLOG
# endif
# endif

# if defined(NEED_TIME) || defined(NEED_UTIME)
# include <time.h>
# endif

# if defined(NEED_TYPES) || 1

# if defined ( MAC_MWPEF )
# include <stat.h>
// because time_t is __std(time_t)
using namespace std;
# else
# include <sys/types.h>
# endif

# endif

# ifdef NEED_UTIME
# ifndef OS_VMS
# define HAVE_UTIME
# if ( defined( OS_NT ) || defined( OS_OS2 ) ) && !defined(__BORLANDC__)
# include <sys/utime.h>
# else
# include <utime.h>
# endif
# endif
# endif

# ifdef NEED_SLEEP
# ifdef OS_NT
# include <windows.h>
# define sleep(x) Sleep(x * 1000)
# endif
# endif

/*
 * HAVE_TRUNCATE -- working truncate() call
 * HAVE_SYMLINKS -- OS supports SYMLINKS
 */

# define HAVE_SYMLINKS
# if defined( OS_OS2 ) || \
	defined( OS_NT ) || \
	defined ( MAC_MWPEF ) || \
	defined( OS_VMS ) || \
	defined( OS_INTERIX )
# undef HAVE_SYMLINKS
# endif

# define HAVE_TRUNCATE
# if defined( OS_OS2 ) || \
	defined( OS_NT ) || \
	defined( MAC_MWPEF ) || \
	defined( OS_BEOS ) || \
	defined( OS_QNX ) || \
	defined( OS_SCO ) || \
	defined( OS_VMS ) || \
	defined( OS_INTERIX ) || \
	defined( OS_MVS ) || \
	defined( OS_MPEIX ) || \
	defined( OS_AS400 )
# undef HAVE_TRUNCATE
# endif

/* These systems have no memccpy() or a broken one */

# if defined( OS_AS400 ) || defined( OS_BEOS ) || defined( OS_FREEBSD ) || \
	defined( OS_MAC ) || defined( OS_MACHTEN ) || defined( OS_MVS ) || \
	defined( OS_VMS62 ) || defined( OS_RHAPSODY ) || defined( OS_MACOSX ) || \
	defined( OS_DARWIN ) || defined( OS_ZETA )
	
# define BAD_MEMCCPY
extern "C" void *memccpy(void *, const void *, int, size_t);
# endif

/* SUNOS has old headers, bless its heart */

# ifdef OS_SUNOS
# define memmove(d, s, c) bcopy(s, d, c)

extern "C" {
void bcopy(const void *src, void *dst, size_t len);
int closelog();
int fsync(int fd);
int ftruncate(int fd, off_t length);
int gethostname(char *name, int namelen);
int getrusage(int who, struct rusage *rusage);
int gettimeofday(struct timeval *tp, struct timezone *tzp);
int lstat(const char *path, struct stat *sb);
int munmap(void *addr, size_t len);
int openlog(const char *ident, int logopt, int facility);
int readlink(const char *path, char *buf, int bufsiz);
caddr_t sbrk(int inc);
int socketpair(int d, int type, int protocol, int *sv);
int symlink(const char *name1, const char *name2);
int syslog(int priority, const char *message ... );
int tolower(int c);
int toupper(int c);
int truncate(const char *path, off_t length);
} ;

# endif

/*
 * MT_STATIC - static multithreaded data
 */

# ifdef OS_NT
# define MT_STATIC static __declspec(thread)
# else
# define MT_STATIC static
# endif

/*
 * CASE_INSENSITIVE - strcmp redefinitions for a case-insensitive server
 */

# if defined( OS_NT ) || defined( OS_UNIXWARE ) || defined( OS_MAC ) \
	|| defined( OS_VMS ) || defined( OS_LYNX ) || defined( OS_INTERIX ) \
	|| defined( OS_MVS ) || defined( OS_SINIX ) || defined( OS_AS400 ) \
	|| defined( OS_MPEIX )

extern "C" int strcasecmp(const char *s1, const char *s2);
extern "C" int strncasecmp(const char *s1, const char *s2, size_t len);

# endif

# if defined( OS_AIX ) || defined( OS_SCO )
# include <strings.h>
# endif
# ifdef OS_OS2
# define strcasecmp(x,y) strcmpi((x),(y))
# define strncasecmp(x,y,z) strnicmp(x,y,z)
# endif
# ifdef OS_QNX
# include <unix.h>
# endif

# ifdef CASE_INSENSITIVE

# include <ctype.h>
# ifdef OS_NT
# define tolower(x) ((x)>='A'&&(x)<='Z'?(x)-'A'+'a':(x))
# endif
# define zstrcmp(x,y) strcasecmp(x,y)
# define zstrncmp(x,y,z) strncasecmp(x,y,z)
# define zcharcmp(x,y) ((unsigned char)tolower(x)-(unsigned char)tolower(y))

# else

# define zstrcmp(x,y) strcmp(x,y)
# define zstrncmp(x,y,z) strncmp(x,y,z)
# define zcharcmp(x,y) ((unsigned char)(x)-(unsigned char)(y))

# endif

/*
 * LineType - LF (raw), CR, CRLF, lfcrlf (LF/CRLF)
 */

enum LineType { LineTypeRaw, LineTypeCr, LineTypeCrLf, LineTypeLfcrlf };

# ifdef USE_CR
# define LineTypeLocal LineTypeCr
# endif
# ifdef USE_CRLF
# define LineTypeLocal LineTypeCrLf
# endif
# ifndef LineTypeLocal
# define LineTypeLocal LineTypeRaw
# endif

/*
 * const_char - what "x" is
 * LONG_LONG - a 64 bit int
 */

# ifndef const_char
# define const_char char
# endif

# ifdef OS_NT
# define LONG_LONG __int64
# else
# if !defined( OS_MVS ) && \
     !defined( OS_OS2 ) && \
     !defined( OS_QNX ) && \
     !defined( OS_UNIXWARE )
# define LONG_LONG long long
# endif
# endif
