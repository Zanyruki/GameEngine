#pragma once
#include <string>
#include <vector>

namespace gh
{
	class CommandLineCommand
	{
	public:
	
		CommandLineCommand( const CommandLineCommand& otherCommandLineCommand );

		CommandLineCommand( const std::string& functionName, std::vector< std::string >& args );

		bool operator <( const CommandLineCommand& otherCommand );

		std::string m_functionName;
		std::vector< std::string > m_arguments;

	};
}