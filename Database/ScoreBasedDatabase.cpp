#include "ScoreBasedDatabase.h"

namespace gh
{
	ScoreBasedDatabase::ScoreBasedDatabase()
	{

	}


	void ScoreBasedDatabase::createNewDatabaseCategory( const std::string& databaseName )
	{
		
		auto found = m_databaseMap.find( databaseName );
		if( found == m_databaseMap.end() )
		{
			ScoredDatabaseCategory category;
			m_databaseMap[ databaseName ] = category;
		}
		
	}


	void ScoreBasedDatabase::addObjectToDatabase( const std::string& databaseName, DatabaseObject* object )
	{
		ScoredDatabaseCategory& databaseCategory = getSpecificDataList( databaseName );
		databaseCategory.addObject( object );
	}


	DatabaseObject* ScoreBasedDatabase::getObjectByName( const std::string& databaseName, const std::string& nameKey )
	{
		ScoredDatabaseCategory& databaseCategory = getSpecificDataList( databaseName );
		return databaseCategory.getObjectByName( nameKey );
	}


	DatabaseObject* ScoreBasedDatabase::getHighestScoringObject( const std::string& databaseName, DatabaseQuery* query )
	{
		ScoredDatabaseCategory& databaseCategory = getSpecificDataList( databaseName );
		
		return databaseCategory.getBestMatch( query );
	}


	void ScoreBasedDatabase::getListOfHighestScoringObjects( const std::string& databaseName, DatabaseQuery* query, int maxNumber, ObjectList& outList )
	{
		ScoredDatabaseCategory& databaseCategory = getSpecificDataList( databaseName );
		databaseCategory.getTopMatches( query, maxNumber, outList );
	}


	void ScoreBasedDatabase::addNewDatabaseResponse( const std::string& databaseName, DatabaseResponse* response )
	{
		ScoredDatabaseCategory& databaseCategory = getSpecificDataList( databaseName );
		databaseCategory.addResponse( response );
	}


	void ScoreBasedDatabase::addNewDatabaseRule( const std::string& databaseName, DatabaseRule* rule )
	{
		ScoredDatabaseCategory& databaseCategory = getSpecificDataList( databaseName );
		databaseCategory.addRule( rule );
	}


	ScoredDatabaseCategory& ScoreBasedDatabase::getSpecificDataList( const std::string& key )
	{
		auto found = m_databaseMap.find( key );
		if( found != m_databaseMap.end() )
		{
			return found->second;
		}
		else
		{
			reportError( "ERROR: Trying to find database category that does not exist: " + key + ". Did you remember to create the category?" );
		}
	}


	//****************
	// Global Methods
	//****************

	ScoreBasedDatabase& getTheDatabase()
	{
		static ScoreBasedDatabase theDatabase;
		return theDatabase;
	}


	void createNewDatabaseCategory( const std::string& databaseName )
	{
		getTheDatabase().createNewDatabaseCategory( databaseName );
	}


	void addObjectToDatabase( const std::string& databaseName, DatabaseObject* object )
	{
		getTheDatabase().addObjectToDatabase( databaseName, object );
	}


	DatabaseObject* getObjectByName( const std::string& databaseName, const std::string& nameKey )
	{
		return getTheDatabase().getObjectByName( databaseName, nameKey );
	}


	DatabaseObject* getHighestScoringObject( const std::string& databaseName, DatabaseQuery* query )
	{
		return getTheDatabase().getHighestScoringObject( databaseName, query );
	}


	void getListOfHighestScoringObjects( const std::string& databaseName, DatabaseQuery* query, int maxNumber, ObjectList& outList )
	{
		getTheDatabase().getListOfHighestScoringObjects( databaseName, query, maxNumber, outList );
	}


	void addNewDatabaseResponse( const std::string& databaseName, DatabaseResponse* response )
	{
		getTheDatabase().addNewDatabaseResponse( databaseName, response );
	}


	void addNewDatabaseRule( const std::string& databaseName, DatabaseRule* rule )
	{
		getTheDatabase().addNewDatabaseRule( databaseName, rule );
	}
}