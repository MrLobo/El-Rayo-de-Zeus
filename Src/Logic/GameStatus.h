//---------------------------------------------------------------------------
// GameStatus.h
//---------------------------------------------------------------------------

/**
@file GameStatus.h

Esta es la clase principal de gesti�n del juego. Contiene clases como son Bases, Players, Stats, etc.
@see Logic::GameStatus

@author Jose Luis L�pez S�nchez
*/
#pragma once
#ifndef __Logic_GameStatus_H
#define __Logic_GameStatus_H

#include <list>
namespace Logic
{
	class CBase;
	class CPlayer;
}
namespace Logic
{
	class CGameStatus
	{
	public:
		CGameStatus(const unsigned short numPlayers);

		~CGameStatus();

	protected:
	
		/**
			crea las bases
		*/
		Logic::CBase* createBase(const unsigned short rings);

		/**
			N�mero de jugadores
		*/
		unsigned short _numPlayers;
		/**
			N�mero de bases, pueden ser diferentes, caso de la base Lobby
		*/
		unsigned short _numBases; 

		/**
		Tipo para la lista de Bases
		*/
		typedef std::list<CPlayer*> TPlayers;

		/**
		Lista de las Bases
		*/
		TPlayers _players;

		/**
		Tipo para la lista de Bases
		*/
		typedef std::list<CBase*> TBases;

		/**
		Lista de las Bases
		*/
		TBases _bases;
	};
//TBases _bases;
}

#endif //namespace Logic