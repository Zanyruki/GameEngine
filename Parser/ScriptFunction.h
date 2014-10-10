#pragma once
#include "VariableBase.h"
#include "ScriptFunctionBase.h"
#include <vector>
#include <tuple>
#include "..\Utility\Utility.h"
#include "GenericContainer.h"
#include "TypeWrapper.h"
namespace gh
{
	template<int ...>
	struct seq { };

	template<int N, int ...S>
	struct gens : gens<N - 1, N - 1, S...> { };

	template<int ...S>
	struct gens<0, S...> {
		typedef seq<S...> type;
	};

	//template< typename T_func > //, typename T_ObjectType >
	template< typename T_ReturnType, typename T_ClassType, typename... ArgsTypes > 
	class ScriptFunction : public ScriptFunctionBase
	{
		using MemberFunctionPointer = T_ReturnType(T_ClassType::*)(ArgsTypes...args);
		using TupleType = std::tuple < ArgsTypes... > ;
		static const size_t m_numberOfArgs = sizeof...(ArgsTypes);
	public:
		std::tuple< ArgsTypes... > m_typeTuple;
		
		ScriptFunction( const std::string& funcName, MemberFunctionPointer funcPointer )
			: m_functionName( funcName )
			, m_functionPointer( funcPointer )
		{
			///m_numberOfArgs = sizeof...(args);
			/*m_numberOfArgs = sizeof...( ArgsTypes );
			for (m_tupleIndex = 0; m_tupleIndex < m_numberOfArgs; ++m_tupleIndex)
			{
				m_argTypes.push_back(getDataTypeFromTemplate(std::get<m_tupleIndex>(typeTuple)));
			}*/
			//extractData( funcPointer );
			//extractDataForArgTypes( new Variable<ArgsTypes>()...);
		}

		template< typename...Args >
		void extractDataForArgTypes( VariableBase* container, Args*...args)
		{
			//container::value_type tempVar;
			m_argTypes.push_back( container );
			extractDataForArgTypes(args...);
		}

		virtual VariableBase* executeFunction(FunctionOwnerBase* functionOwner, const std::vector< MathematicExpression* >& argList)
		{
			trace("executeFunction entered");
			m_listOfExpressions = argList;
			int tupleSize = sizeof...(ArgsTypes);
			
			saveTuple(argList, typename gens<sizeof...(ArgsTypes)>::type());
			return executeFunction(functionOwner, typename gens<sizeof...(ArgsTypes)>::type());

			/*if (m_numberOfArgs == 0)
			{
			}
			else
			{
				TypeWrapper< ArgsTypes... > typeWrapper;
				if (m_numberOfArgs == 1)
				{
					return executeFunction(functionOwner, evaluateExpression(argList[0], 0, std::get<0>(m_typeTuple)));
				}
				else if (m_numberOfArgs == 2)
				{
					auto arg1 = evaluateExpression(argList[0], 0, std::get<0>(m_typeTuple));
					int argIndex = 0;
					//std::get<1>(m_typeTuple);
					//auto tupleType = std::get<1>(m_typeTuple);
					//auto arg2 = evaluateExpression(argList[1], 1, tupleType);
					return executeFunction(functionOwner, arg1);
				}
				
				return nullptr;
			}*/
			
		}

		void saveTupleTypes(const std::vector < MathematicExpression* >& expressionList)
		{
			//Dummy method for functions that don't take args
		}

		template< typename T_DataType >
		void saveTupleTypes(const std::vector < MathematicExpression* >& expressionList, T_DataType type )
		{
			T_DataType var = expressionList[0]->evaluateBasedOffTemplate( T_DataType() );
			params = std::make_tuple(var);
		}

		template< typename T_DataType1, typename T_DataType2 >
		void saveTupleTypes(const std::vector < MathematicExpression* >& expressionList, T_DataType1 type1, T_DataType2 type2 )
		{
			T_DataType1 var1 = expressionList[0]->evaluateBasedOffTemplate(T_DataType1());
			T_DataType2 var2 = expressionList[1]->evaluateBasedOffTemplate(T_DataType2());
			params = std::make_tuple(var1, var2);
		}


		template<int ...S>
		void saveTuple(const std::vector < MathematicExpression* >& expressionList, seq<S...>)
		{
			saveTupleTypes( expressionList, std::get<S>(m_typeTuple) ...);
		}


		template <typename T_DataType >
		T_DataType evaluateExpression(MathematicExpression* expression, int index, T_DataType type )
		{
			T_DataType returnValue = expression->evaluateBasedOffTemplate( type );
			return returnValue;
		}

		template< typename...Args>
		VariableBase* executeFunction( FunctionOwnerBase* functionOwner, Args...args )
		{
			return executeFunction(functionOwner, typename gens<sizeof...(Args)>::type());
		}

		template<int ...S>
		VariableBase* executeFunction(FunctionOwnerBase* functionOwner, seq<S...>)
		{
			FunctionOwner< T_ClassType >* object = static_cast< FunctionOwner< T_ClassType >* >( functionOwner );
			auto memberFunc = std::mem_fn(m_functionPointer);
			Variable< T_ReturnType >* returnVar = new Variable< T_ReturnType >("var", memberFunc(object->m_functionOwner, std::get<S>(params) ...));
			
			//( (object->m_functionOwner)->*m_functionPointer)( "yes" );
			return returnVar;
			//return nullptr;
		}

		virtual DataType getReturnType()
		{
			return getDataTypeFromTemplate( T_ReturnType() );
		}

		std::string m_functionName;
		std::vector< VariableBase* > m_argTypes;
		MemberFunctionPointer m_functionPointer;
		std::tuple<ArgsTypes...> params;
		std::vector<MathematicExpression* > m_listOfExpressions;
		
	};
}