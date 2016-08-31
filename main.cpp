// UniqLineFilter.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <iostream>

#include "FilterUniqLine.h"
#include "ParseArgument.h"


int main( int const argc, TCHAR const * const argv[] )
{
	int iResult = -1;
	try
	{
		bool bEnd = false;
		SApplicationParam param;
		ParseArgument( bEnd, param, argc, argv );
		if( bEnd )
		{
			std::cerr << _T( "Unique line filter ver?.??\n" );
			std::cerr << tctszUsage;
			iResult = -1;
		}
		else
		{
			FilterUniqLine( param );
			iResult = 0;
		}
	}
	catch( std::exception e )
	{
		std::cerr << _T( "Unique line filter ver?.??\n" );
		std::cerr << _T( "  Error : " ) << e.what();
	}
	return iResult;
}

