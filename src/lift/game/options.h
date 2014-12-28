#ifndef _OPTIONS_H
#define _OPTIONS_H

enum EGameOption
{
	EOption1, EOption2, 
	ENumOptions
};


class COptions
{
public:
	COptions();

	void LoadOptions( const char* file );

	bool CheckBoolOption( EGameOption o );
	int CheckIntOption( EGameOption o );
	float CheckFloatOption( EGameOption o );
};


COptions gOptions;


#endif
