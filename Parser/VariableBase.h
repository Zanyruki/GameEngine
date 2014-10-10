#pragma once
#include <string>
#include "VariableDataType.h"
namespace gh
{

	class VariableBase
	{
	public:
		inline DataType getDataType()
		{
			return m_dataType;
		}

		DataType m_dataType;
		std::string m_variableName;
	};
}