#include "CommandLineParser.h"
#include <sstream>
#include <vector>
#include "..\Utility\Utility.h"
namespace gh
{
	CommandLineParser::CommandLineParser( LPTSTR arguements )
	{
		size_t size = wcstombs(NULL, arguements, 0);
		char* charArray = new char[size + 1];
		wcstombs( charArray, arguements, size + 1 );
		m_arguementString = std::string( charArray );
	}

	void CommandLineParser::parseCommands()
	{
		trace( "parse commands" );
		std::stringstream commandLineStream( m_arguementString );
		std::string stringBuffer;
		std::getline( commandLineStream, stringBuffer, '-' );
		std::string functionName;
		std::string arguements;

		do 
		{
			std::vector< std::string > argumentVector;
			std::getline( commandLineStream, functionName, ' ' );
			trace("Function name: " << functionName << "\n" );
			std::getline( commandLineStream, arguements, '-' );

			if( arguements.size() > 1 )
			{	
				
				parseArguments( arguements, argumentVector );
			}
			CommandLineCommand newCommand =  CommandLineCommand( functionName, argumentVector );
			m_commands.push_back( newCommand );

		} while ( ! commandLineStream.eof() );


	}


	void CommandLineParser::parseArguments( std::string& arguments, std::vector< std::string >& argumentList )
	{
		std::string arguementBuffer = "";
		trace("All Arguments: " << arguments << "\n" );
		/*std::stringstream arguementStream( arguments );
		do 
		{
			if( arguementStream.peek() == '"' )
			{
				trace( "Quote\n" );
				trace( arguementStream.str() );
				arguementStream.get();
				std::getline( arguementStream, arguementBuffer, '"' );
				arguementStream.get();
			}
			else
			{
				trace( "No Quotes\n" );
				trace( arguementStream.str() );
				std::getline( arguementStream, arguementBuffer, ' ' );
			}
			trace( "Arguement: " << arguementBuffer << "\n" );
		} while( ! arguementStream.eof() );*/
		char token = ' ';
		size_t startPosition = 0;
		if( arguments[0] == '"' )
		{
			token = '"';
			++startPosition;
		}
		for( size_t charIndex = startPosition; charIndex < arguments.size(); ++charIndex )
		{
			char currentChar = arguments[ charIndex ];
			if( currentChar == token )
			{
				//TODO put in a list of arguments
				trace( "Token is: " << token << "\n" );
				trace( "Arg is: " << arguementBuffer << "\n" );
				argumentList.push_back( arguementBuffer );
				arguementBuffer = "";
				if( charIndex < arguments.size() - 1 && arguments[ charIndex + 1 ] == '"' )
				{
					++charIndex;
					token = '"';
					
				}
				continue;
			}
			arguementBuffer += arguments[ charIndex ];
		}
		if( arguementBuffer.size() > 0 )
		{
			argumentList.push_back( arguementBuffer );
		}
	}


	void CommandLineParser::getCommands( std::vector< CommandLineCommand >& outCommands )
	{
		outCommands = m_commands;
	}
}