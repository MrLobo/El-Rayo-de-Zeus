//---------------------------------------------------------------------------
// Base.cpp
//---------------------------------------------------------------------------


/**
@file Base.cpp

Contiene la implementaci�n de la clase que maneja las posiciones l�gicas

@see Logic::Base

@author Jose Luis L�pez S�nchez
*/

#include "Logic\Base.h"
//using namespace Logic;

namespace Logic
{
	CBase::CBase(const unsigned short numRings):_numRings(numRings),_AllAltarsActivated(false),_numActivatedAltars(0),_life(1),_numAltars(11)
	{		
		//_rings=new CRingnumPlayers;
		//_numBases=numPlayers+1;
	}
	
	CBase::~CBase()
	{	
		//delete &_bases;	//me teneis que ense�ar a destruir bien...		
		//delete &_players;
	}

	//---------------------------------------------------------

	


}