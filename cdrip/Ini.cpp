/*
** Copyright (C) 2001 Albert L. Faber
**  
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software 
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include "Ini.h"

CIni::CIni()
{
}

CIni::~CIni()
{
}

BOOL CIni::SetValue(String strSection, String strItem, String strVal)
{
	return WritePrivateProfileString(strSection, strItem, strVal, m_strFileName);
}

BOOL CIni::SetValue(String strSection, String strItem, const int iVal)
{
	String strEntry = String::FromInt(iVal);

	return SetValue(strSection, strItem, strEntry);
}

BOOL CIni::SetValue(String strSection, String strItem, const long lVal)
{
	return SetValue(strSection, strItem, (int) lVal);
}

BOOL CIni::SetValue(String strSection, String strItem, const DWORD dwVal)
{
	return SetValue(strSection, strItem, (int) dwVal);
}

String CIni::GetValue(String strSection, String strItem, String strDefault)
{
	TCHAR lpszValue[1024] = { '\0', };

	::GetPrivateProfileString(strSection, strItem, strDefault, lpszValue, sizeof(lpszValue), m_strFileName);

	return String(lpszValue);
}

INT CIni::GetValue(String strSection, String strItem, const INT nDefault)
{
	INT	 nReturn = nDefault;
	String	 strReturn = GetValue(strSection, strItem, "");

	if (strReturn != NIL)
	{
		nReturn = strReturn.ToInt();
	}

	return nReturn;
}

LONG CIni::GetValue(String strSection, String strItem, const LONG nDefault)
{
	return (LONG) GetValue(strSection, strItem, (INT) nDefault);
}

DWORD CIni::GetValue(String strSection, String strItem, const DWORD nDefault)
{
	return (DWORD) GetValue(strSection, strItem, (INT) nDefault);
}
