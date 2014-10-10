#pragma once
#include "..\2DGameWorld\Dialogue.h"
#include "..\2DGameWorld\Person.h"
#include "..\2DGameWorld\Area.h"
#include "DatabaseQuery.h"


namespace gh
{
	/*
	/ Dialogue Functions
	*/
	Dialogue* getBestDialogue( DatabaseQuery* query );

	Dialogue* getBestContinueMonologue( DatabaseQuery* query );

	Dialogue* getDialogueByName( const std::string& dialogueName );

	Dialogue* getStartingDialogue( DatabaseQuery* query );

	Dialogue* getBestText(DatabaseQuery* query);

	void getDialogueResponses( DatabaseQuery* query, std::vector< Dialogue* >& outResponses );
	/*
	/ Item Functions
	*/

	Item* getBestItem(DatabaseQuery* query);

	
	/*
	/ Person Functions
	*/

	Person* getBestPerson(DatabaseQuery* query);

	/*
	/ Event Functions
	*/
	bool checkForMovementEvent( DatabaseQuery* query );

	bool checkForLookEvent( DatabaseQuery* query );

	bool checkForStartChapterEvent( DatabaseQuery* query );
}