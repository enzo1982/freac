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

#ifndef INI_H_INCLUDED
#define INI_H_INCLUDED

#include <smooth.h>

using namespace smooth;

class CIni
{
	private:
		String	 m_strFileName;
	public:
		BOOL	 SetValue(String strSection, String strItem, String strVal);
		BOOL	 SetValue(String strSection, String strItem, const int iVal);
		BOOL	 SetValue(String strSection, String strItem, const long lVal);
		BOOL	 SetValue(String strSection, String strItem, const DWORD lVal);

		String	 GetValue(String strSection, String strItem, String strDefault);
		INT	 GetValue(String strSection, String strItem, const INT nDefault);
		LONG	 GetValue(String strSection, String strItem, const LONG nDefault);
		DWORD	 GetValue(String strSection, String strItem, const DWORD nDefault);

			 CIni();
		virtual	~CIni();

		void	 SetIniFileName(String strFileName) { m_strFileName = strFileName; }
};

#endif
