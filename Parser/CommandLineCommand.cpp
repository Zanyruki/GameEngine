#include "CommandLineCommand.h"

namespace gh
{
	CommandLineCommand::CommandLineCommand( const std::string& functionName, std::vector< std::string>& args )
		: m_functionName( functionName )
		, m_arguments( args )
	{

	}

	CommandLineCommand::CommandLineCommand( const CommandLineCommand& otherCommandLineCommand )
	{
		m_functionName = otherCommandLineCommand.m_functionName;
		m_arguments = otherCommandLineCommand.m_arguments;
	}

	bool CommandLineCommand::operator <( const CommandLineCommand& otherCommand )
	{
		return false;
	}
}