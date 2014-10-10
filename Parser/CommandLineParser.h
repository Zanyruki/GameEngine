#pragma once
#include <Windows.h>
#include <string>
#include <map>
#include <vector>
#include "..\EventSystem\VariableNamedProperties.h"
#include "CommandLineCommand.h"

namespace gh
{
	class CommandLineParser
	{
	public:
		CommandLineParser( LPTSTR arguements );
		std::string m_arguementString;
		void parseCommands();
		void getCommands( std::vector< CommandLineCommand >& outCommands );
		std::vector< CommandLineCommand > m_commands;
	private:
		void parseArguments( std::string& arguments, std::vector< std::string >& arguementList );
	};
}