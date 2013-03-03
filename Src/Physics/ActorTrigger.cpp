//---------------------------------------------------------------------------
// ActorTrigger.cpp
//---------------------------------------------------------------------------

/**
@file ActorTrigger.cpp

@see Physics::CActorTrigger

@author FRS
@date Febrero, 2013
*/

#include "ActorTrigger.h"


namespace Physics
{
	bool CActorTrigger::enters(CActor* actor){
		
		if( isOnTrigger(actor) )
			return false;

		else {
			_actorsOnTrigger.push_back(actor);
			return true;
		}
	}

	bool CActorTrigger::exits(CActor* actor){

		std::vector<CActor*>::iterator actorOnTrigger = std::find(_actorsOnTrigger.begin(), _actorsOnTrigger.end(), actor);
			if (actorOnTrigger == _actorsOnTrigger.end()) 
				return false;	
			else {
				_actorsOnTrigger.erase(actorOnTrigger);
				return true;
			}
	}


	bool CActorTrigger::isOnTrigger(CActor* actor) {
		return std::find(_actorsOnTrigger.begin(), _actorsOnTrigger.end(), actor)
				!= _actorsOnTrigger.end();
	}



}