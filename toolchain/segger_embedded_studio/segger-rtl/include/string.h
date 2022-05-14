/*********************************************************************
*                   (c) SEGGER Microcontroller GmbH                  *
*                        The Embedded Experts                        *
*                           www.segger.com                           *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------
*/

#ifndef __SEGGER_RTL_STRING_H
#define __SEGGER_RTL_STRING_H

/*********************************************************************
*
*       #include section
*
**********************************************************************
*/

#include "__SEGGER_RTL.h"

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************
*
*       Defines, fixed
*
**********************************************************************
*/

#ifndef NULL
#define NULL 0
#endif

/*********************************************************************
*
*       Types
*
**********************************************************************
*/

#ifndef __SEGGER_RTL_SIZE_T_DEFINED
#define __SEGGER_RTL_SIZE_T_DEFINED
typedef __SEGGER_RTL_SIZE_T size_t;
#endif

/*********************************************************************
*
*       Prototypes
*
**********************************************************************
*/

void   * (memcpy)      (void *__s1, const void *__s2, size_t __n);
void   * (memccpy)     (void *__s1, const void *__s2, int __c, size_t __n);
void   * (mempcpy)     (void *__s1, const void *__s2, size_t __n);
void   * (memmove)     (void *__s1, const void *__s2, size_t __n);
int      (memcmp)      (const void *__s1, const void *__s2, size_t __n);
void   * (memchr)      (const void *__s, int __c, size_t __n);
void   * (memrchr)     (const void *__s, int __c, size_t __n);
void   * (memmem)      (const void *__s1, size_t __n1, const void *__s2, size_t __n2);
void   * (memset)      (void *__s, int __c, size_t __n);
char   * (strcpy)      (char *__s1, const char *__s2);
char   * (stpcpy)      (char *__s1, const char *__s2);
char   * (strncpy)     (char *__s1, const char *__s2, size_t __n);
char   * (stpncpy)     (char *__s1, const char *__s2, size_t __n);
size_t   (strlcpy)     (char *__s1, const char *__s2, size_t __n);
char   * (strcat)      (char *__s1, const char *__s2);
char   * (strncat)     (char *__s1, const char *__s2, size_t __n);
size_t   (strlcat)     (char *__s1, const char *__s2, size_t __n);
int      (strcmp)      (const char *__s1, const char *__s2);
int      (strncmp)     (const char *__s1, const char *__s2, size_t __n);
int      (strcasecmp)  (const char *__s1, const char *__s2);
int      (strncasecmp) (const char *__s1, const char *__s2, size_t __n);
char   * (strchr)      (const char *__s, int __c);
char   * (strnchr)     (const char *__str, size_t __n, int __ch);
size_t   (strcspn)     (const char *__s1, const char *__s2);
char   * (strpbrk)     (const char *__s1, const char *__s2);
char   * (strrchr)     (const char *__s, int __c);
size_t   (strspn)      (const char *__s1, const char *__s2);
char   * (strstr)      (const char *__s1, const char *__s2);
char   * (strnstr)     (const char *__s1, const char *__s2, size_t __n);
char   * (strcasestr)  (const char *__s1, const char *__s2);
char   * (strncasestr) (const char *__s1, const char *__s2, size_t __n);
size_t   (strlen)      (const char *__s);
size_t   (strnlen)     (const char *__s, size_t __n);
char   * (strtok)      (char *__s1, const char *__s2);
char   * (strtok_r)    (char *__s1, const char *__s2, char **__s3);
char   * (strsep)      (char **__stringp, const char *__delim);
char   * (strdup)      (const char *__s1);
char   * (strndup)     (const char *__s1, size_t __n);
char   * (strerror)    (int __num);
int      (strcoll)     (const char *__s1, const char *__s2);
size_t   (strxfrm)     (char *__s1, const char *__s2, size_t __n);

#ifdef __cplusplus
}
#endif

#endif

/*************************** End of file ****************************/
