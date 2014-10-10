#pragma once
#include <string>
namespace gh
{
	class Table
	{
	public:
		Table( const std::string& tableName );

		std::string m_tableName;
	};
}