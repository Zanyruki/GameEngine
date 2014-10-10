#pragma once
#include "VariableBase.h"
#include <string>
#include "..\Utility\Utility.h"

namespace gh
{
	template< typename T_DataType >
	class Variable : public VariableBase
	{
		using type = T_DataType;
	public:
		inline Variable()
			: m_value(T_DataType())
		{
			m_variableName = "defaultValue";
			m_dataType = getDataTypeFromTemplate(m_value);

		}

		inline Variable( const T_DataType& data )
			: m_value(data)
		{
			m_variableName = "var";
			m_dataType = getDataTypeFromTemplate( data );
		}

		inline Variable( const std::string& name, const T_DataType& data, DataType type )
			: m_value( data )
		{
			m_variableName = name;
			m_dataType = type;
		}


		inline Variable(const std::string& name, const T_DataType& data )
			: m_value(data)
		{
			m_variableName = name;
			m_dataType = getDataTypeFromTemplate( m_value );
		}


		inline Variable( const std::string& name, std::string& valueAsString, DataType type )
		{
			m_variableName = name;
			m_dataType = type;
			setValueFromString( valueAsString );
		}

		inline void setValue( T_DataType data )
		{
			m_value = data;
		}

		inline void setValueFromString( std::string& valueAsString )
		{
			switch( m_dataType )
			{
			case BOOL_DATA_TYPE :
				setValue( convertStringToBool( valueAsString) );
				break;
			case INT_DATA_TYPE :
				setValue( convertStringToInt( valueAsString ) );
				break;
			case FLOAT_DATA_TYPE :
				setValue( convertStringToFloat( valueAsString) );
				break;
			case STRING_DATA_TYPE :
				setValue( valueAsString );
				break;
			}
		}




		inline T_DataType getValue()
		{
			return m_value;
		}

		T_DataType m_value;

	};

template<>
	inline Variable<std::string>::Variable( const std::string& name, std::string& valueAsString, DataType type )
			: m_value( valueAsString )
		{
			m_variableName = name;
			m_dataType = type;
		}

}