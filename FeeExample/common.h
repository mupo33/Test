// Common.h: Libary of common functions
//
//////////////////////////////////////////////////////////////////////

#ifndef __COMMON_H__
#define __COMMON_H__

#include <sys/timeb.h>
#include <time.h>
#include <math.h>

#define BIT_0	0x00000001
#define BIT_1	0x00000002
#define BIT_2	0x00000004
#define BIT_3	0x00000008
#define BIT_4	0x00000010
#define BIT_5	0x00000020
#define BIT_6	0x00000040
#define BIT_7	0x00000080
#define BIT_8	0x00000100
#define BIT_9	0x00000200
#define BIT_10	0x00000400
#define BIT_11	0x00000800
#define BIT_12	0x00001000
#define BIT_13	0x00002000
#define BIT_14	0x00004000
#define BIT_15	0x00008000
#define BIT_16	0x00010000
#define BIT_17	0x00020000
#define BIT_18	0x00040000
#define BIT_19	0x00080000
#define BIT_20	0x00100000
#define BIT_21	0x00200000
#define BIT_22	0x00400000
#define BIT_23	0x00800000
#define BIT_24	0x01000000
#define BIT_25	0x02000000
#define BIT_26	0x04000000
#define BIT_27	0x08000000
#define BIT_28	0x10000000
#define BIT_29	0x20000000
#define BIT_30	0x40000000
#define BIT_31	0x80000000

#define SET_ON(x, m) (x |= m)
#define SET_OFF(x, m) (x &= ~m)

#define IS_ON(x, m) (x & m)
#define IS_OFF(x, m) (!IS_ON(x, m))

#define IS_EQUAL(x, mx, y, my) (IS_ON(x, mx) == IS_ON(y, my) || IS_OFF(x, mx) == IS_OFF(y, my))

#define FORMATBUFFER_HEX_BYTE  0
#define FORMATBUFFER_HEX_WORD  1
#define FORMATBUFFER_HEX_DWORD 2
#define FORMATBUFFER_DEC_BYTE  3
#define FORMATBUFFER_DEC_WORD  4
#define FORMATBUFFER_DEC_DWORD 5

LPCSTR Trim(LPCSTR szField);
LPCSTR ConcatPath(LPCSTR szPath1, LPCSTR szPath2);
BOOL IsPathFileName(LPCSTR szPath);
void DeletePath(LPCSTR szPath1);
void ChangeProfileName();
CString GetExePath();
//CString GetTempFileName()


BOOL IsToken(LPCSTR& szBuf, LPCSTR szToken);
BOOL ExtractQuoteString(LPCSTR& szBuf, CString& s);
BOOL ExtractSeparatorString(LPCSTR& szBuf, CString& s, char chSeparator, BOOL bTrim = TRUE);
BOOL ExtractSeparatorInt(LPCSTR& szBuf, int& n, char chSeparator);
BOOL ExtractSeparatorHex(LPCSTR& szBuf, int& n, char chSeparator);
BOOL ExtractLocaleSeparatorString(LPCSTR& szBuf, CString& s, BOOL bTrim = TRUE);
BOOL ExtractLocaleSeparatorInt(LPCSTR& szBuf, int& n);

void FormatBuffer(LPSTR szBuffer, unsigned char* pPacket, int nLen, int nDataSize, BOOL bAscii, BOOL bRow, BOOL bCRLF);

void RecursiveDelete(CString szPath);

LPCSTR HexToText(UINT nHex, UINT nSize = 2);
UINT TextToHex(LPCSTR szHex);
WORD CheckSum16(LPBYTE pBuf, WORD wLen);
BYTE CheckSum8(LPBYTE pBuf, WORD wLen);
BYTE Xor8(LPBYTE pBuf, WORD wLen);
BOOL CheckHex(CString sStr, int nCorrectLen);

int Round(double dValue);

CString ByteToBitString(void* pData, int nDataLen);
CString ByteToBitString(BYTE byValue, int nBitLen = sizeof(BYTE) * 8);
CString IntToBitString(int nValue, int nBitLen = sizeof(int) * 8);

WORD BitStringToWord(LPCSTR szBitString);
UINT BitStringToByte(BYTE* byValue, LPCSTR szBitString);

int ByteStringToByte(LPCSTR szByteString, BYTE* byBuffer, int nBufSize);

CString	GetShortDateTime(BOOL bMilliseconds = FALSE);

void CBFillDirectory(CComboBox* cb, LPCSTR szPath);
void CBFillFile(CComboBox* cb, LPCSTR szPath, LPCSTR szExt = "");

BOOL WritePrivateProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue, LPCTSTR lpszFileName);
BOOL WritePrivateProfileFloat(LPCTSTR lpszSection, LPCTSTR lpszEntry, float fValue, LPCTSTR lpszFileName);
BOOL WritePrivateProfileDouble(LPCTSTR lpszSection, LPCTSTR lpszEntry, double dValue, LPCTSTR lpszFileName);
float GetPrivateProfileFloat(LPCTSTR lpszSection, LPCTSTR lpszEntry, float fDefault, LPCTSTR lpszFileName);
double GetPrivateProfileDouble(LPCTSTR lpszSection, LPCTSTR lpszEntry, double dDefault, LPCTSTR lpszFileName);


#ifndef _ftime_s
inline void _ftime_s(struct _timeb *timeptr) {_ftime(timeptr);};
#endif

#ifndef _splitpath_s
inline void _splitpath_s(const char * path, char * drive, size_t driveSizeInCharacters, char * dir, size_t dirSizeInCharacters, char * fname, size_t nameSizeInCharacters, char * ext, size_t extSizeInBytes) {_splitpath(path, drive, dir, fname, ext);};
#endif

#ifndef strcpy_s
inline void strcpy_s(char *strDestination, size_t sizeInBytes, const char *strSource) {strcpy(strDestination, strSource);};
#endif

#endif