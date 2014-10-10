#pragma once
#include <string>
namespace gh
{

enum DataType
{
	BOOL_DATA_TYPE,
	INT_DATA_TYPE,
	FLOAT_DATA_TYPE,
	STRING_DATA_TYPE,
	RANDOMRANGE_DATA_TYPE,
	RANDOMCHOICE_DATA_TYPE,
	NULL_DATA_TYPE
};

DataType getDataTypeFromTemplate( int data );

DataType getDataTypeFromTemplate( bool data );

DataType getDataTypeFromTemplate( float data );

DataType getDataTypeFromTemplate( std::string data );



}