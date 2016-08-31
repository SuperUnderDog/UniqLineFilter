// Copyright (C) 2016 SuperUnderDog
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "stdafx.h"
#include <io.h>
#include <set>

#include "StreamContainer.h"
#include "FilterUniqLine.h"


static inline void WriteLine( CBaseOutStream * const pos, TCHAR const * const pctsz )
{
	pos->WriteLine( pctsz );
}


static inline void _FilterUniqLine_ProcLine(
	std::set<std::string> & rstrsetColledted,
	CBaseOutStream * poutstream,
	TCHAR const * const pctszLine, SApplicationParam const & rcparam )
{
	std::string str = pctszLine;
	size_t sizeIdx = 0;
	while( std::string::npos != ( sizeIdx = str.find_first_of( _T( "\t\r" ), sizeIdx ) ) )
	{
		str.erase( sizeIdx, 1 );
	}

	if( str.empty() )
	{
		if( !( rcparam.bDeleteEmptyLine ) )
		{
			WriteLine( poutstream, _T( "" ) );
		}
	}
	else
	{
		if( rstrsetColledted.end() == rstrsetColledted.find( str ) )
		{
			WriteLine( poutstream, str.c_str() );
			rstrsetColledted.insert( str );
		}
	}
}

static inline void _FilterUniqLine(
	std::set<std::string> & rstrsetColledted,
	CBaseOutStream * poutstream, CBaseInStream & rinstream,
	SApplicationParam const & rcparam )
{
	TCHAR * pctszLine = NULL;
	try
	{
		int const iLineBufSize = 0x80;
		pctszLine = new TCHAR[ iLineBufSize + 1 ];
		pctszLine[ iLineBufSize ] = 0;

		while( !( rinstream.eof() ) )
		{
			rinstream.getline( pctszLine, iLineBufSize );
			if( rinstream.fail() )
			{
				while( ( !( rinstream.eof() ) ) && rinstream.fail() )
				{
					rinstream.clear();
					rinstream.getline( pctszLine, iLineBufSize );
				}
			}
			else
			{
				_FilterUniqLine_ProcLine(
					rstrsetColledted, poutstream, pctszLine, rcparam );
			}
		}

		delete[] pctszLine;
	}
	catch( ... )
	{
		if( NULL != pctszLine ) delete[] pctszLine;
		throw;
	}
}



void FilterUniqLine( SApplicationParam const & rcparam )
{
	std::ofstream ofsDestDict;
	CBaseOutStream * poutstream = NULL;
	try
	{
		if( rcparam.strDestFile.empty() )
		{
			poutstream = new CCoutStream( std::cout );
		}
		else
		{
			ofsDestDict.open( rcparam.strDestFile,
				std::ios_base::out | std::ios_base::binary | std::ios_base::trunc );
			if( ofsDestDict.fail() )
			{
				std::string str = _T( "Failed to open \"" );
				str += rcparam.strDestFile;
				str += _T( "\"." );
				throw std::exception( str.c_str() );
			}
			poutstream = new COfstream( ofsDestDict );
		}

		std::set<std::string> strsetColledted;
		if( rcparam.strSrcFile.empty() )
		{
			CCinStream instream( std::cin );
			_FilterUniqLine( strsetColledted, poutstream, instream, rcparam );
		}
		else
		{
			std::string strDir;
			size_t const sizetLastIdx = rcparam.strSrcFile.find_last_of( _T( "\\/" ) );
			if( std::string::npos != sizetLastIdx )
			{
				strDir = rcparam.strSrcFile.substr( 0, sizetLastIdx + 1 );
			}

			_finddata_t finddata;
			long lFind = -1L;
			try
			{
				if( -1L == ( lFind = _findfirst(
					rcparam.strSrcFile.c_str(), &finddata ) ) )
				{
					std::string str = _T( "Not exist file(s) match \"" );
					str += rcparam.strSrcFile.c_str();
					str += _T( "\"." );
					throw std::exception( str.c_str() );
				}
				do
				{
					std::ifstream ifsSrcDict;
					try
					{
						ifsSrcDict.open( strDir + finddata.name, std::ios_base::in );
						if( ifsSrcDict.fail() )
						{
							std::string str = _T( "Failed to open \"" );
							str += strDir + finddata.name;
							str += _T( "\"." );
							throw std::exception( str.c_str() );
						}
						CIfstream instream( ifsSrcDict );
						_FilterUniqLine( strsetColledted, poutstream, instream, rcparam );
					}
					catch( ... )
					{
						ifsSrcDict.close();
						throw;
					}
				}
				while( _findnext( lFind, &finddata ) == 0 );
			}
			catch( ... )
			{
				if( -1 != lFind ) ::_findclose( lFind );
				throw;
			}
		}

		delete poutstream;
		ofsDestDict.close();
	}
	catch( ... )
	{
		if( NULL != poutstream ) delete poutstream;
		ofsDestDict.close();
		throw;
	}
}



