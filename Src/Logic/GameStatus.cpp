//---------------------------------------------------------------------------
// GameStatus.cpp
//---------------------------------------------------------------------------


/**
@file GameStatus.cpp

Contiene la implementaci�n de la clase que maneja las posiciones l�gicas

@see Logic::LogicalPosition

@author Jose Luis L�pez S�nchez
*/

#include "Logic\GameStatus.h"
//using namespace Logic;

namespace Logic 
{
	CGameStatus::CGameStatus(const unsigned short numPlayers)
	{		
		_numPlayers=numPlayers;
		_numBases=numPlayers+1;
		//_bases=new Bases(_numBases);
		//_players=new Players(_numPlayers);
	}
	
	CGameStatus::~CGameStatus()
	{	
		delete &_bases;	//me teneis que ense�ar a destruir bien...		
		delete &_players;
	}

	//---------------------------------------------------------

	


}