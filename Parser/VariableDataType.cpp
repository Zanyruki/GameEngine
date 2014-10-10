#include "VariableDataType.h"

namespace gh
{
	DataType getDataTypeFromTemplate( int data )
{
	return INT_DATA_TYPE;
}


DataType getDataTypeFromTemplate( bool data )
{
	return BOOL_DATA_TYPE;
}


DataType getDataTypeFromTemplate( float data )
{
	return FLOAT_DATA_TYPE;
}


DataType getDataTypeFromTemplate( std::string data )
{
	return STRING_DATA_TYPE;
}
}