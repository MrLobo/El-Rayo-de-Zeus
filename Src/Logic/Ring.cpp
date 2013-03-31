//---------------------------------------------------------------------------
// Ring.cpp
//---------------------------------------------------------------------------


/**
@file Ring.cpp

Contiene la implementaci�n de la clase Ring

@see Logic::Ring

@author Jose Luis L�pez S�nchez
*/

#include "Logic\Ring.h"
//using namespace Logic;

namespace Logic
{
	CRing::CRing(const unsigned short numAltars, Ring tipo):_numAltars(numAltars),_AllAltarsActivated(false),_numActivatedAltars(0),_tipo(tipo)
	{		
		//_rings=new CRingnumPlayers;
		//_numBases=numPlayers+1;
	}
	
	CRing::~CRing()
	{	
		//delete &_bases;	//me teneis que ense�ar a destruir bien...		
		//delete &_players;
	}


	//---------------------------------------------------------
}