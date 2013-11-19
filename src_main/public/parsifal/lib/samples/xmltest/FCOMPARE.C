/* +++Date last modified: 05-Jul-1997 */

/*
**  FCOMPARE.C - Compare 2 files
**
**  public domain demo by Bob Stout
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "snipfile.h"
int    fcompare(const char *fnam1, const char *fnam2);/* Fcompare.C     */
//#include "sniptype.h"
typedef enum {Error_ = -1, Success_, False_ = 0, True_} Boolean_T;

#define BUFSIZE 16384
static char buf[2][BUFSIZE];

int fcompare(const char *fname1, const char *fname2)
{
      FILE *f1, *f2;
      int retval = Success_;

      if (NULL == (f1 = fopen(fname1, "rb")))
            return Error_;
      if (NULL != (f2 = fopen(fname2, "rb")))
      {
            size_t size1, size2;

            fseek(f1, 0L, SEEK_END);
            fseek(f2, 0L, SEEK_END);

            if (ftell(f1) != ftell(f2))
                  retval = !Success_;
            else
            {
                  rewind(f1);
                  rewind(f2);
                  do
                  {
                        size1 = fread(buf[0], 1, BUFSIZE, f1);
                        size2 = fread(buf[1], 1, BUFSIZE, f2);
                        if (0 == (size1 | size2))
                              break;
                        if ((size1 != size2) || memcmp(buf[0], buf[1], size1))
                        {
                              retval = !Success_;
                              break;
                        }
                  } while (size1 && size2);
            }
            fclose(f2);
      }
      else  retval = Error_;
      fclose(f1);
      return retval;
}

#ifdef TEST

int main(int argc, char *argv[])
{
      if (3 > argc)
      {
            puts("Usage: FCOMPARE file1 file2");
            return 1;
      }
      printf("fcompare(%s, %s) returned %d\n", argv[1], argv[2],
            fcompare(argv[1], argv[2]));
      return 0;
}

#endif /* TEST */
