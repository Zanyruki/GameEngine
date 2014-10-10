#pragma once

//#include "Database.h"
#include "ScoredDatabaseCategory.h"
#include "DatabaseQuery.h"

namespace gh
{
	typedef std::vector< DatabaseObject* > ObjectList;

	class ScoreBasedDatabase //: public Database
	{
	public:
		ScoreBasedDatabase();
		DatabaseObject* getHighestScoringObject( const std::string& databaseName, DatabaseQuery* query );
		DatabaseObject* getObjectByName( const std::string& databaseName, const std::string& nameKey );
		void getListOfHighestScoringObjects( const std::string& databaseName, DatabaseQuery* query, int maxNumber, ObjectList& outList );
		void createNewDatabaseCategory( const std::string& databaseName );
		void addObjectToDatabase( const std::string& databaseName, DatabaseObject* object );
		void addNewDatabaseResponse( const std::string& databaseName, DatabaseResponse* response );
		void addNewDatabaseRule( const std::string& databaseName, DatabaseRule* rule );
	private:
		ScoredDatabaseCategory& getSpecificDataList( const std::string& key );
		std::map< std::string, ScoredDatabaseCategory > m_databaseMap;
	};

	ScoreBasedDatabase& getTheDatabase();
	void createNewDatabaseCategory( const std::string& databaseName );
	void addObjectToDatabase( const std::string& databaseName, DatabaseObject* object );
	DatabaseObject* getHighestScoringObject( const std::string& databaseName, DatabaseQuery* query );
	DatabaseObject* getObjectByName( const std::string& databaseName, const std::string& nameKey );
	void getListOfHighestScoringObjects( const std::string& databaseName, DatabaseQuery* query, int maxNumber, ObjectList& outList );
	void addNewDatabaseResponse( const std::string& databaseName, DatabaseResponse* response );
	void addNewDatabaseRule( const std::string& databaseName, DatabaseRule* rule );
}