#include "log.h"
#include <iostream>
#include <fstream>
using namespace std;


CLogMessage::CLogMessage()
{
	mText = "Empty Log Message";
}

CLogMessage::CLogMessage( std::string c )
{
	mText = c;
}

CLogMessage::~CLogMessage()
{
}

std::string CLogMessage::GetMessage()
{
	return mText;
}

void CLogMessage::SetTimeStamp( float t )
{
	mTimeStamp = t;
}

float CLogMessage::GetTimeStamp()
{
	return mTimeStamp;
}


CLog::CLog()
{
	mTimeStamp = new CTimer();
	mTimeStamp->Clear();

	SetLogging( true );
	LogItem( new CLogMessage("Log Created") );
}

CLog::~CLog()
{
	ClearLog();
}

void CLog::ResetTimeStamp()
{
	mTimeStamp->Clear();
}

void CLog::ClearLog()
{
	CListEntry<CLogMessage>* entry = mLogEntries.GetFirst();
	
	while ( entry )
	{
		delete entry->data;
		mLogEntries.RemoveEntry( entry );
		entry = mLogEntries.GetFirst();
	}
}

void CLog::SetLogging( bool enabled )
{
	mEnabled = enabled;
}

void CLog::LogItem( CLogMessage* message )
{
//	SaveLog("aaa.log");
	if ( mEnabled )
	{
		message->SetTimeStamp( mTimeStamp->Time() );
		mLogEntries.Add( message );
	} else {
		delete message;
	}
}

void CLog::PrintLog()
{
	if ( mEnabled )
	{
		CListEntry<CLogMessage>* entry = mLogEntries.GetFirst();
	
		while ( entry )
		{
			cout << "[" << entry->data->GetTimeStamp() << "]:\t" << entry->data->GetMessage();
			//printf( "%s\n", entry->data->GetMessage() );
			entry = entry->GetNext();
		}	
	} else {
		printf( "Logging not enabled.\n" );
	}
}

void CLog::SaveLog( std::string filename )
{
	ofstream file;
	file.open( filename.c_str() );

	if ( mEnabled )
	{
		CListEntry<CLogMessage>* entry = mLogEntries.GetFirst();
	
		while ( entry )
		{
			file << "[" << (int)entry->data->GetTimeStamp() << "]:\t" << entry->data->GetMessage() << "\n";
			entry = entry->GetNext();
		}	
	} else {
		file << "Logging not enabled.\n";
	}

	file.close();
}
