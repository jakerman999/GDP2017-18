#include "cPersistImpSQLite.h"

#include "SQLite/sqlite3.h"

// ***************************************************************************
// These are from the SQLite shell.c file
// 
#if (defined(_WIN32) || defined(WIN32)) && !defined(_CRT_SECURE_NO_WARNINGS)
/* This needs to come before any includes for MSVC compiler */
#define _CRT_SECURE_NO_WARNINGS
#endif

/*
** Warning pragmas copied from msvc.h in the core.
*/
#if defined(_MSC_VER)
#pragma warning(disable : 4054)
#pragma warning(disable : 4055)
#pragma warning(disable : 4100)
#pragma warning(disable : 4127)
#pragma warning(disable : 4130)
#pragma warning(disable : 4152)
#pragma warning(disable : 4189)
#pragma warning(disable : 4206)
#pragma warning(disable : 4210)
#pragma warning(disable : 4232)
#pragma warning(disable : 4244)
#pragma warning(disable : 4305)
#pragma warning(disable : 4306)
#pragma warning(disable : 4702)
#pragma warning(disable : 4706)
#endif /* defined(_MSC_VER) */

//#if defined(_WIN32) || defined(WIN32)
//# include <io.h>
//# include <fcntl.h>
//# define isatty(h) _isatty(h)
//# ifndef access
//#  define access(f,m) _access((f),(m))
//# endif
//# undef popen
//# define popen _popen
//# undef pclose
//# define pclose _pclose
//#else
//// some other stuff, related to unix, maybe??
//#endif

//#if defined(_WIN32) || defined(WIN32)
//#include <windows.h>
//
///* string conversion routines only needed on Win32 */
//extern char *sqlite3_win32_unicode_to_utf8(LPCWSTR);
//extern char *sqlite3_win32_mbcs_to_utf8_v2(const char *, int);
//extern char *sqlite3_win32_utf8_to_mbcs_v2(const char *, int);
//extern LPWSTR sqlite3_win32_utf8_to_unicode(const char *zText);
//#endif

///* On Windows, we normally run with output mode of TEXT so that \n characters
//** are automatically translated into \r\n.  However, this behavior needs
//** to be disabled in some cases (ex: when generating CSV output and when
//** rendering quoted strings that contain \n characters).  The following
//** routines take care of that.
//*/
//#if defined(_WIN32) || defined(WIN32)
//static void setBinaryMode(FILE *file, int isOutput) {
//	if (isOutput) fflush(file);
//	_setmode(_fileno(file), _O_BINARY);
//}
//static void setTextMode(FILE *file, int isOutput) {
//	if (isOutput) fflush(file);
//	_setmode(_fileno(file), _O_TEXT);
//}
//#else
//# define setBinaryMode(X,Y)
//# define setTextMode(X,Y)
//#endif
// ****************************************************************************

cPersistImpSQLite::cPersistImpSQLite()
{

	return;
}

cPersistImpSQLite::~cPersistImpSQLite()
{

	return;
}

