// Common.cpp
//
#include "stdafx.h"
#include "Common.h"
//#include "word.h"


BOOL WritePrivateProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue, LPCTSTR lpszFileName)
{
	CString s;

	s.Format("%d", nValue);

	return WritePrivateProfileString(lpszSection, lpszEntry, s, lpszFileName);
}

BOOL WritePrivateProfileFloat(LPCTSTR lpszSection, LPCTSTR lpszEntry, float fValue, LPCTSTR lpszFileName)
{
	return WritePrivateProfileDouble(lpszSection, lpszEntry, fValue, lpszFileName);
}

BOOL WritePrivateProfileDouble(LPCTSTR lpszSection, LPCTSTR lpszEntry, double dValue, LPCTSTR lpszFileName)
{
	CString s;

	s.Format("%f", dValue);

	return WritePrivateProfileString(lpszSection, lpszEntry, s, lpszFileName);
}

float GetPrivateProfileFloat(LPCTSTR lpszSection, LPCTSTR lpszEntry, float fDefault, LPCTSTR lpszFileName)
{
	return (float)GetPrivateProfileDouble(lpszSection, lpszEntry, fDefault, lpszFileName);
}

double GetPrivateProfileDouble(LPCTSTR lpszSection, LPCTSTR lpszEntry, double dDefault, LPCTSTR lpszFileName)
{
	char szValue[100];
	
	if (GetPrivateProfileString(lpszSection, lpszEntry, "", szValue, sizeof(szValue), lpszFileName) == 0)
		return dDefault;

	return atof(szValue);
}

CString GetShortDateTime(BOOL bMilliseconds)
{
	CTime timeNow;
	
	if (bMilliseconds)
	{
		struct _timeb timebNow;

		_ftime_s(&timebNow);

		timeNow = timebNow.time;

		CString sMillisecons;

		sMillisecons.Format(".%hu", timebNow.millitm);
		
		return timeNow.Format("%Y-%m-%d_%H-%M-%S") + sMillisecons;
	}
	
	timeNow = CTime::GetCurrentTime();

	return timeNow.Format("%Y-%m-%d_%H-%M-%S");
}

CString GetExePath()
{
	char szExePath[_MAX_PATH];
	
	VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, szExePath, _MAX_PATH));
	
	char szDrive[_MAX_DRIVE];
	char szDir[_MAX_DIR];

	_splitpath_s(szExePath, szDrive, _MAX_DRIVE, szDir, _MAX_DIR, NULL, 0, NULL, 0);

	CString sExePath;
	sExePath = szDrive;
	sExePath += szDir;

	return sExePath;	
}


void ChangeProfileName()
{
	TCHAR szBuff[_MAX_PATH];
	VERIFY(::GetModuleFileName(AfxGetApp()->m_hInstance, szBuff, _MAX_PATH));
	char* pBuff = strtok(szBuff, ".");
	if (pBuff != NULL)
	{
		strcat(pBuff, ".INI");
		
		free((void*)AfxGetApp()->m_pszProfileName);

		AfxGetApp()->m_pszProfileName = _tcsdup(szBuff);
	}
}

LPCSTR Trim(LPCSTR szField)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	static char szTrimField[100];

	CString sTrimField;
	
	sTrimField = szField;
	sTrimField.TrimLeft();
	sTrimField.TrimRight();
	
	strncpy(szTrimField, sTrimField, sizeof(szTrimField));

	szTrimField[sizeof(szTrimField)-1] = '\0';
	
	return szTrimField;
}

void DeletePath(LPCSTR szPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	RecursiveDelete(CString(szPath));
	RemoveDirectory(szPath);
}

void RecursiveDelete(CString szPath)
{
	CFileFind ff;
	CString path = szPath;
	
	if(path.Right(1) != "\\")
		path += "\\";

	path += "*.*";

	BOOL res = ff.FindFile(path);

	while(res)
	{
		res = ff.FindNextFile();

		path = ff.GetFilePath();

		if (ff.IsDots())
		{
		}
		else if (ff.IsDirectory())
		{
			RecursiveDelete(path);
			RemoveDirectory(path);
		}
		else
		{
			DeleteFile(path);
		}
	}

}

BOOL IsPathFileName(LPCSTR szPath)
{
	char szDrive[_MAX_DRIVE];
	char szDir[_MAX_DIR];

	_splitpath(szPath, szDrive, szDir, NULL, NULL);

	return strlen(szDir) > 0;
}

LPCSTR ConcatPath(LPCSTR szPath1, LPCSTR szPath2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	static char szPath[MAX_PATH];

	CString sPath1;
	CString sPath2;

	sPath1 = szPath1;
	if (!sPath1.IsEmpty())
		if (sPath1.GetAt(sPath1.GetLength()-1) == '\\')
			sPath1 = sPath1.Left(sPath1.GetLength()-1);

	sPath2 = szPath2;
	if (!sPath2.IsEmpty())
		if (sPath2.GetAt(0) == '\\')
			sPath2 = sPath2.Right(sPath2.GetLength()-1);

	strcpy(szPath, sPath1 + '\\' + sPath2);

	return szPath;
}

BOOL IsToken(LPCSTR& szBuf, LPCSTR szToken)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

 	int nLen = strlen(szToken);

	if (strnicmp(szBuf, szToken, nLen) == 0)
	{
		szBuf += nLen;
		return TRUE;
	}

	return FALSE;
}

BOOL ExtractQuoteString(LPCSTR& szBuf, CString& s)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	s.Empty();
	
	while (*szBuf != '"')
	{
		if (*szBuf == '\0')
			return FALSE;

		szBuf++;
	}

	szBuf++;

	while (*szBuf != '"')
	{
		if (*szBuf == '\0')
			return FALSE;

		s += *szBuf;

		szBuf++;
	}

	return TRUE;
}

BOOL ExtractSeparatorInt(LPCSTR& szBuf, int& n, char chSeparator)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString s;
	if (!ExtractSeparatorString(szBuf, s, chSeparator, TRUE))
		return FALSE;

	n = atoi(s);

	return TRUE;
}

BOOL ExtractSeparatorHex(LPCSTR& szBuf, int& n, char chSeparator)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString s;
	if (!ExtractSeparatorString(szBuf, s, chSeparator, TRUE))
		return FALSE;

	n = TextToHex(s);

	return TRUE;
}

BOOL ExtractSeparatorString(LPCSTR& szBuf, CString& s, char chSeparator, BOOL bTrim)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	s.Empty();

	if (*szBuf == '\0')
		return FALSE;

	while (*szBuf != chSeparator)
	{
		if (*szBuf == '\0')
		{
			szBuf--;
			break;
		}

		s += *szBuf;

		szBuf++;
	}

	szBuf++;

	if (bTrim)
	{
		s.TrimLeft();
		s.TrimRight();
	}

	return TRUE;
}

BOOL ExtractLocaleSeparatorInt(LPCSTR& szBuf, int& n)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	char chSeparator;

	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SLIST, &chSeparator, 1);

	return ExtractSeparatorInt(szBuf, n, chSeparator);
}

BOOL ExtractLocaleSeparatorString(LPCSTR& szBuf, CString& s, BOOL bTrim)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	char chSeparator;

	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SLIST, &chSeparator, 1);

	return ExtractSeparatorString(szBuf, s, chSeparator, bTrim);
}

void FormatBuffer(LPSTR szBuffer, unsigned char* pPacket, int nLen, int nDataSize, BOOL bAscii, BOOL bRow, BOOL bCRLF)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (nLen == 0)
		return;

	CString sRow;
	CString sData;
	CString sAscii;
	CString sEnd;
	CString sByte;

	int nRow = 0;

	switch (nDataSize)
	{
	case FORMATBUFFER_HEX_BYTE: // Hex Byte
		{
			for (int i = 0; i < nLen; i++)
			{
				if (i % 16 == 0 && i != 0)
				{
					if (bRow)
						sRow.Format("%04X  %-48.48s%s", nRow, (const char*)sData, (const char*)sAscii);
					else
						sRow.Format("%-48.48s%s", (const char*)sData, (const char*)sAscii);

					if (bCRLF)
						sRow += '\n';

					strcat(szBuffer, sRow);

					sData.Empty();
					sAscii.Empty();
					nRow += 16;
				}

				sByte.Format("%02X", pPacket[i]);
				sData += sByte;
				sData += ' ';

				if (bAscii)
					sAscii += (char)(pPacket[i] < 32 ? '.' : pPacket[i]);
			}

			if (bRow)
				sRow.Format("%04X  %-48.48s%s", nRow, (const char*)sData, (const char*)sAscii);
			else
				sRow.Format("%-48.48s%s", (const char*)sData, (const char*)sAscii);

			if (bCRLF)
				sRow += '\n';

			strcat(szBuffer, sRow);
			break;
		}
	case FORMATBUFFER_HEX_WORD: // Hex Word
		{
			for (int i = 0; i < nLen; i++)
			{
				if (i % 16 == 0 && i != 0)
				{
					sRow.Format("%04X  %-48.48s%s\n", nRow / 2, (const char*)sData, (const char*)sAscii);

//					sBuffer += sRow;
					strcat(szBuffer, sRow);

					sData.Empty();
					sAscii.Empty();
					nRow += 16;
				}

				if (i % 2 == 0)
				{
					sByte.Format("%02X%02X", pPacket[i], pPacket[i+1]);
					sData += sByte;
					sData += ' ';
				}

				if (bAscii)
					sAscii += (char)(pPacket[i] < 32 ? '.' : pPacket[i]);
			}

			sRow.Format("%04X  %-48.48s%s\n", nRow / 2, (const char*)sData, (const char*)sAscii);

//			sBuffer += sRow;
			strcat(szBuffer, sRow);
			break;
		}
	case FORMATBUFFER_HEX_DWORD: // Hex DWord
		{
			for (int i = 0; i < nLen; i++)
			{
				if (i % 16 == 0 && i != 0)
				{
					sRow.Format("%04X  %-48.48s%s\n", nRow / 4, (const char*)sData, (const char*)sAscii);

//					sBuffer += sRow;
					strcat(szBuffer, sRow);

					sData.Empty();
					sAscii.Empty();
					nRow += 16;
				}

				if (i % 4 == 0)
				{
					sByte.Format("%02X%02X%02X%02X", pPacket[i], pPacket[i+1], pPacket[i+2], pPacket[i+3]);
					sData += sByte;
					sData += ' ';
				}

				if (bAscii)
					sAscii += (char)(pPacket[i] < 32 ? '.' : pPacket[i]);
			}

			sRow.Format("%04X  %-48.48s%s\n", nRow / 4, (const char*)sData, (const char*)sAscii);

//			sBuffer += sRow;
			strcat(szBuffer, sRow);
			break;
		}
	case FORMATBUFFER_DEC_BYTE: // Dec Byte
		{
			for (int i = 0; i < nLen; i++)
			{
				if (i % 10 == 0 && i != 0)
				{
					sRow.Format("%04X  %-48.48s%s\n", nRow, (const char*)sData, (const char*)sAscii);

//					sBuffer += sRow;
					strcat(szBuffer, sRow);

					sData.Empty();
					sAscii.Empty();
					nRow += 16;
				}

				sByte.Format("%03i", pPacket[i]);
				sData += sByte;
				sData += ' ';

				if (bAscii)
					sAscii += (char)(pPacket[i] < 32 ? '.' : pPacket[i]);
			}

			sRow.Format("%04X  %-48.48s%s\n", nRow, (const char*)sData, (const char*)sAscii);

//			sBuffer += sRow;
			strcat(szBuffer, sRow);
			break;
		}
	case FORMATBUFFER_DEC_WORD: // Dec Word
		{
			for (int i = 0; i < nLen; i++)
			{
				if (i % 10 == 0 && i != 0)
				{
					sRow.Format("%04X  %-48.48s%s\n", nRow / 2, (const char*)sData, (const char*)sAscii);

//					sBuffer += sRow;
					strcat(szBuffer, sRow);

					sData.Empty();
					sAscii.Empty();
					nRow += 16;
				}

				if (i % 2 == 0)
				{
					sByte.Format("%06i", (WORD)pPacket[i]);
					sData += sByte;
					sData += ' ';
				}

				if (bAscii)
					sAscii += (char)(pPacket[i] < 32 ? '.' : pPacket[i]);
			}

			sRow.Format("%04X  %-48.48s%s\n", nRow / 2, (const char*)sData, (const char*)sAscii);

//			sBuffer += sRow;
			strcat(szBuffer, sRow);
			break;
		}
	case FORMATBUFFER_DEC_DWORD: // Dec DWord
		{
			for (int i = 0; i < nLen; i++)
			{
				if (i % 10 == 0 && i != 0)
				{
					sRow.Format("%04X  %-48.48s%s\n", nRow / 4, (const char*)sData, (const char*)sAscii);

//					sBuffer += sRow;
					strcat(szBuffer, sRow);

					sData.Empty();
					sAscii.Empty();
					nRow += 16;
				}

				if (i % 4 == 0)
				{
					sByte.Format("%02X%02X%02X%02X", pPacket[i], pPacket[i+1], pPacket[i+2], pPacket[i+3]);
					sData += sByte;
					sData += ' ';
				}

				if (bAscii)
					sAscii += (char)(pPacket[i] < 32 ? '.' : pPacket[i]);
			}

			sRow.Format("%04X  %-48.48s%s\n", nRow / 2, (const char*)sData, (const char*)sAscii);

//			sBuffer += sRow;
			strcat(szBuffer, sRow);
			break;
		}
	}
}


LPCSTR HexToText(UINT nHex, UINT nSize)
{
	static char szHex[100];

	switch (nSize)
	{
	case 1:
		sprintf(szHex, "%2.2X", nHex);
		break;
	case 2:
		sprintf(szHex, "%4.4X", nHex);
		break;
	case 4:
		sprintf(szHex, "%8.8X", nHex);
		break;
	default:
		sprintf(szHex, "%X", nHex);
	}
	
	return szHex;
}


UINT TextToHex(LPCSTR szHex)
{
	UINT nHex;

	if (strncmp(szHex, "0x", 2) == 0 || strncmp(szHex, "0X", 2) == 0)
	{
		if (sscanf(szHex+2, "%x", &nHex) == 1)
			return nHex;
	}
	else
	{
		if (sscanf(szHex, "%x", &nHex) == 1)
			return nHex;
	}

	return 0;
}


WORD CheckSum16(LPBYTE pBuf, WORD wLen)
{
	DWORD dwCheckSum = 0;
	for (int i=0; i < wLen; i++)
		dwCheckSum += pBuf[i];

	return (WORD)dwCheckSum;
}

BYTE CheckSum8(LPBYTE pBuf, WORD wLen)
{
	DWORD dwCheckSum = 0;
	for (int i=0; i < wLen; i++)
		dwCheckSum += pBuf[i];

	return (BYTE)dwCheckSum;
}

BYTE Xor8(LPBYTE pBuf, WORD wLen)
{
	DWORD dwXor = 0;
	for (int i=0; i < wLen; i++)
		dwXor ^= pBuf[i];

	return (BYTE)dwXor;
}

BOOL CheckHex(CString sStr, int nCorrectLen)
{

   CString sByte;
    
   int i = 0;

   int nLen = sStr.GetLength();

   if (nLen > nCorrectLen)
	   return FALSE;

   while(i<nLen)
   {
	   sByte = sStr.Mid(i,1);

	   if	(!	(	(*sByte>='0' && *sByte<='9')	||
					(*sByte>='a' && *sByte<='f')|| 
					(*sByte>='A' && *sByte<='F')
				)
			)
		return FALSE;

	   i++;
   }
	
   return TRUE;

}

int Round(double dValue)
{
	if (dValue < 0) return 0;
	double dFloor = floor(dValue);
	double dCeil = ceil(dValue);

	if ((dCeil - dValue) > (dValue - dFloor))
		return (int)dFloor;
	else
		return (int)dCeil;
}

WORD BitStringToWord(LPCSTR szBitString)
{
	int nLen = strlen(szBitString);
	WORD wWord;

	wWord = 0;
	
	for (int i = 0; i < nLen; i++)
	{
		if (szBitString[nLen-1-i] == '1')
			wWord += (WORD)pow(2., i);
	}

	return wWord;
}


UINT BitStringToByte(BYTE* byValue, LPCSTR szBitString)
{
	int nBitLen = strlen(szBitString);
	int nBytePos = -1;

	for (int i = 0; i < nBitLen; i++)
	{
		if (i % 8 == 0)
		{
			nBytePos++;
			byValue[nBytePos] = 0;
		}
		
		if (szBitString[i] == '1')
			byValue[nBytePos]++;

		if ((i+1) % 8 != 0)
			byValue[nBytePos] = byValue[nBytePos] << 1;
	}

	return nBytePos + 1;
}

CString IntToBitString(int nValue, int nBitLen)
{
	CString sBitString;

	for (int i = 0; i < nBitLen; i++)
	{
		if (nValue & 0x1)
			sBitString = '1' + sBitString;
		else
			sBitString = '0' + sBitString;

		nValue = nValue >> 1;
	}
	
	return sBitString;
}

CString ByteToBitString(BYTE byValue, int nBitLen)
{
	CString sBitString;

	for (int i = 0; i < nBitLen; i++)
	{
		if (byValue & 0x1)
			sBitString = '1' + sBitString;
		else
			sBitString = '0' + sBitString;

		byValue = byValue >> 1;
	}
	
	return sBitString;
}

CString ByteToBitString(void* pData, int nDataLen)
{
	CString sBitString;
	BYTE* byByte = (BYTE*)pData;

	for (int i = 0; i < nDataLen; i++)
	{
		sBitString += ByteToBitString(byByte[i]);
	}
	
	return sBitString;
}

int ByteStringToByte(LPCSTR szByteString, BYTE* byBuffer, int nBufSize)
{
	int nByte;
	int i;

	for (i = 0; i < nBufSize; i++ )
	{
		if (sscanf(szByteString + (i*2), "%2x", &nByte) != 1)
			break;

		byBuffer[i] = nByte;
	}

	return i;
}

void CBFillDirectory(CComboBox* cb, LPCSTR szPath)
{
	CWaitCursor wait;
		
	CString sFile;
	CFileFind fileFind;
	BOOL bWorking;

	sFile = ConcatPath(szPath, "\\*.");

//	cb->ResetContent();

	bWorking = fileFind.FindFile(sFile);
	while (bWorking)
	{
		bWorking = fileFind.FindNextFile();

		if (fileFind.IsDirectory() && !fileFind.IsDots())
			cb->AddString(fileFind.GetFileName());
	}

	cb->SetCurSel(0);
}

void CBFillFile(CComboBox* cb, LPCSTR szPath, LPCSTR szExt)
{
	CWaitCursor wait;
		
	CString sFile;
	CFileFind fileFind;
	BOOL bWorking;

	sFile = ConcatPath(szPath, "\\*.");
	sFile += szExt;

//	cb->ResetContent();

	bWorking = fileFind.FindFile(sFile);
	while (bWorking)
	{
		bWorking = fileFind.FindNextFile();

		if (!fileFind.IsDots())
			cb->AddString(fileFind.GetFileName());
	}

	cb->SetCurSel(0);
}
