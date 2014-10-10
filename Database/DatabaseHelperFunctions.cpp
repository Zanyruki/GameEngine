#include "DatabaseHelperFunctions.h"
#include "ScoreBasedDatabase.h"
#include "Event.h"
#include "..\EventSystem\EventSystem.h"

namespace gh
{
	/*
	/ Dialogue
	*/
	const int MAX_DIALOGUE_RESPONSES = 3;

	Dialogue* getBestDialogue( DatabaseQuery* query )
	{
		query->setVariable( "trigger", "onReply", STRING_DATA_TYPE );
		DatabaseObject* object = getHighestScoringObject( "Dialogue", query );
		if( object == nullptr )
		{
			reportError( "ERROR: No dialogue match found" );
		}

		return dynamic_cast< Dialogue* >( object );
	}


	Dialogue* getBestContinueMonologue( DatabaseQuery* query )
	{
		query->setVariable( "trigger", "onContinueText", STRING_DATA_TYPE );
		DatabaseObject* object = getHighestScoringObject( "Dialogue", query );
		if( object == nullptr )
		{
			reportError( "ERROR: No dialogue match found" );
		}

		return dynamic_cast< Dialogue* >( object );
	}


	Dialogue* getStartingDialogue( DatabaseQuery* query )
	{
		query->setVariable( "trigger", "onStartConversation", STRING_DATA_TYPE );
		DatabaseObject* object = getHighestScoringObject( "Dialogue", query );

		return dynamic_cast< Dialogue* >( object );
	}


	Dialogue* getBestText(DatabaseQuery* query)
	{
		DatabaseObject* objectItem = getHighestScoringObject("Dialogue", query);
		return static_cast< Dialogue* >(objectItem);
	}

	Dialogue* getDialogueByName( const std::string& dialogueName )
	{
		DatabaseObject* dialogueObject = getObjectByName( "Dialogue", dialogueName );
		return static_cast< Dialogue* >( dialogueObject );
	}


	void getDialogueResponses( DatabaseQuery* query, std::vector< Dialogue* >& outResponses )
	{
		query->setVariable( "trigger", "onResponse", STRING_DATA_TYPE );
		ObjectList list;
		getListOfHighestScoringObjects( "Dialogue", query, MAX_DIALOGUE_RESPONSES, list );
		for( auto iter = list.begin(); iter != list.end(); ++iter )
		{
			outResponses.push_back( static_cast< Dialogue* >( (*iter) ) );
		}
		
	}

	/*
	/ Item
	*/
	Item* getBestItem(DatabaseQuery* query)
	{
		DatabaseObject* objectItem = getHighestScoringObject("Item", query);
		return static_cast< Item* >(objectItem);
	}

	/*
	/ Person
	*/

	Person* getBestPerson(DatabaseQuery* query)
	{
		DatabaseObject* objectPerson = getHighestScoringObject("Person", query);
		return static_cast< Person* >(objectPerson);
	}


	/*
	/ Event
	*/
	bool checkForMovementEvent( DatabaseQuery* query )
	{
		query->setVariable( "trigger", "onMove", STRING_DATA_TYPE );
		DatabaseObject* objectEvent = getHighestScoringObject( "Event", query );
		if( objectEvent != nullptr )
		{
			Event* event = static_cast< Event* >( objectEvent );
			event->fireEvent();
			return true;
		}
		return false;
	}


	bool checkForLookEvent( DatabaseQuery* query )
	{
		query->setVariable( "trigger", "onLook", STRING_DATA_TYPE );
		DatabaseObject* objectEvent = getHighestScoringObject( "Event", query );
		if( objectEvent != nullptr )
		{
			Event* event = static_cast< Event* >( objectEvent );
			event->fireEvent();
			return true;
		}
		return false;
	}


	bool checkForStartChapterEvent( DatabaseQuery* query )
	{
		query->setVariable( "trigger", "onChapterStart", STRING_DATA_TYPE );
		DatabaseObject* objectEvent = getHighestScoringObject( "Event", query );
		if( objectEvent != nullptr )
		{
			Event* event = static_cast< Event* >( objectEvent );
			event->fireEvent();
			trace( "Event fired: " << event->getName() );
			event->fireCallbackEvents();
			return true;
		}
		return false;
	}
}