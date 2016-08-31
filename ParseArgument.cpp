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
#include <string>

#include "ParseArgument.h"


static inline std::string _ParseArg_TrailingFileName(
	int & riPos, int const argc, TCHAR const * const argv[],
	TCHAR const * const pctszArgStr )
{
	if( argc <= riPos )
	{
		std::string str = _T( "Not specified parameter to argument\"" );
		str += pctszArgStr;
		str += _T( "\"." );
		throw std::exception( str.c_str() );
	}

	std::string const strResult = argv[ riPos ];
	riPos++;
	return strResult;
}

static inline void _ParseArg_Switch( bool & rbEnd, int & riPos,
	SApplicationParam & rparam, int const argc, TCHAR const * const argv[] )
{
	rbEnd = false;
	TCHAR const * const pctsz = argv[ riPos ];
	int const iLen = _tcslen( pctsz );
	if( 0 >= iLen )
	{
		throw std::exception( _T( "Not specified option charactor(s)." ) );
	}
	if( ( 0 == _tcscmp( &( pctsz[ 1 ] ), _T( "?" ) ) ) ||
		( 0 == _tcscmp( &( pctsz[ 1 ] ), _T( "h" ) ) ) )
	{
		rbEnd = true;
	}
	else if( 0 == _tcscmp( &( pctsz[ 1 ] ), _T( "i" ) ) )
	{
		riPos++;
		rparam.strSrcFile =
			_ParseArg_TrailingFileName( riPos, argc, argv, pctsz );
	}
	else if( 0 == _tcscmp( &( pctsz[ 1 ] ), _T( "o" ) ) )
	{
		riPos++;
		rparam.strDestFile =
			_ParseArg_TrailingFileName( riPos, argc, argv, pctsz );
	}
	else if( 0 == _tcscmp( &( pctsz[ 1 ] ), _T( "d" ) ) )
	{
		rparam.bDeleteEmptyLine = true;
		riPos++;
	}
	else
	{
		std::string str = _T( "Invalid argument \"" );
		str += pctsz;
		str += _T( "\" (-? to show usage)." );
		throw std::exception( str.c_str() );
	}
}


TCHAR const tctszUsage[] = _T( "\n"
	"  usage: worddictgen [option]\n\n"
	"  required\n"
	"    none\n"
	"\n  option\n"
	"    -? -h                        // Help\n"
	"    -o <output word list file>   // (default stdout)\n"
	"    -i <input word list file>    // (default stdin)\n"
	"    -d                           // Delete empty line\n"
	"\n" );

void ParseArgument( bool & rbEnd,
	SApplicationParam & rparam, int const argc, TCHAR const * const argv[] )
{
	rbEnd = false;
	int iPos = 1;
	while( ( !rbEnd ) && ( argc > iPos ) )
	{
		TCHAR const * const pctsz = argv[ iPos ];
		int const iLen = _tcslen( pctsz );
		if( 0 >= iLen )
		{
			std::string str = _T( "Argument " );
			str += std::to_string( argc );
			str += _T( " have not entity." );
			throw std::exception( str.c_str() );
		}
		if( ( _T( '-' ) == pctsz[ 0 ] ) || ( _T( '/' ) == pctsz[ 0 ] ) )
		{
			_ParseArg_Switch( rbEnd, iPos, rparam, argc, argv );
		}
		else
		{
			std::string str = _T( "Invalid argument \"" );
			str += pctsz;
			str += _T( "\" (-? to show usage)." );
			throw std::exception( str.c_str() );
		}
	}
}
