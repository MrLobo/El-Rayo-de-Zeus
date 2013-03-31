//---------------------------------------------------------------------------
// GameStatus.cpp
//---------------------------------------------------------------------------


/**
@file GameStatus.cpp

Contiene la implementaci�n de la clase que maneja las posiciones l�gicas

@see Logic::GameStatus

@author Jose Luis L�pez S�nchez
*/

#include "Logic\GameStatus.h"
#include "Logic\BaseInfo.h"

#include <assert.h>
//using namespace Logic;

namespace Logic 
{
	CGameStatus::CGameStatus(const unsigned short numPlayers)
	{

		_numPlayers=numPlayers;
		_numBases=numPlayers+1;
		
		for(int i=0; i<_numBases; i++)//creamos las bases con 3 anillos cada una
		{
			Logic::CBaseInfo* base=createBase(3);
			if (base!=0)	
				_bases.push_back(base);
		}
	}
	
	CGameStatus::~CGameStatus()
	{	
		delete &_bases;	//me teneis que ense�ar a destruir bien...		
		delete &_players;
	}

	//---------------------------------------------------------

	Logic::CBaseInfo* CGameStatus::createBase(const unsigned short rings)
	{		
		assert( &_bases );//&& "LOGIC::GRAPHICS>> No existe escena gr�fica!");

		Logic::CBaseInfo* base=new Logic::CBaseInfo(3);
		if (base)
			return base;

		return 0;
	} // createBase
}