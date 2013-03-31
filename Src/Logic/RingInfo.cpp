//---------------------------------------------------------------------------
// RingInfo.cpp
//---------------------------------------------------------------------------


/**
@file RingInfo.cpp

Contiene la implementaci�n de la clase RingInfo

@see Logic::RingInfo

@author Jose Luis L�pez S�nchez
*/

#include "Logic\RingInfo.h"
//using namespace Logic;

namespace Logic
{
	CRingInfo::CRingInfo(const unsigned short numAltars, Ring tipo):_numAltars(numAltars),_AllAltarsActivated(false),_numActivatedAltars(0),_tipo(tipo)
	{
		//_rings=new CRingnumPlayers;
		//_numBases=numPlayers+1;
	}
	
	CRingInfo::~CRingInfo()
	{	
		//delete &_bases;	//me teneis que ense�ar a destruir bien...		
		//delete &_players;
	}


	//---------------------------------------------------------
}