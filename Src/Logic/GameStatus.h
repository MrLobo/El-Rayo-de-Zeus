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

#include <vector>
#include <string> //PT
#include <list> //PT

namespace Logic
{
	class CBaseInfo;
	class CPlayerInfo;
	class CEntity;
}
namespace Logic
{
	class CGameStatus
	{		

	protected:
		//PT
		typedef std::list<std::string> TMapNameList;

	public:

		/**
		Devuelve la �nica instancia de la clase CServer.
		
		@return �nica instancia de la clase CServer.
		*/
		static CGameStatus* getSingletonPtr() {return _instance;}

	
		/**
		Inicializa la instancia

		@return Devuelve false si no se ha podido inicializar.
		*/
		//PT
		//static bool Init(const unsigned short numPlayers);
		static bool Init(TMapNameList &maplist);


		/**
		Libera la instancia de CGameNetMsgManager. Debe llamarse al finalizar la 
		aplicaci�n.
		*/
		static void Release();

		//opciones que van a ir a Ranking
		/**
			Tiempo programado como necesario en la partida creada
			para que el altar cambie desde activado a desactivado
			y viceversa.
			Se asume que es el mismo para todos los altares de la 
			partida y que este tiempo jam�s cambia.
		*/
		//const unsigned long _activationTime;

		/**
			Puntos de m�rito que supone activar un altar.
		*/
		//const unsigned int _scoreAltar;
		
		CBaseInfo* getBase(unsigned short base);
		
		CPlayerInfo* getPlayer(unsigned short player);
		
		unsigned short getNumBases(){return _numBases;}
		
		unsigned short getNumPlayers(){return _numPlayers;}
		//PT
		unsigned short CGameStatus::getEnemiesInBase(unsigned short base);
		
	protected:
	
		//PT
		//CGameStatus(const unsigned short numPlayers);
		CGameStatus(TMapNameList &mapList);

		~CGameStatus();
		/**
			crea las bases
		*/
		Logic::CBaseInfo* createBase(const unsigned short rings, const unsigned short lifeBase, const unsigned short colorBase);
		
		/**
			crea los jugadores
		*/
		Logic::CPlayerInfo* createPlayer(const CBaseInfo* miBase, Logic::CEntity* entity);

		/**
			N�mero de jugadores
		*/
		unsigned short _numPlayers;
		/**
			N�mero de bases, pueden ser diferentes, caso de la base Lobby
		*/
		unsigned short _numBases;

		/**
			N�mero de enemigos en la base del Player
		*/
		unsigned short _enemies;

		/**
		Tipo para la lista de Jugadores
		*/
		typedef std::vector<CPlayerInfo*> TPlayers;

		/**
		Lista de los Jugadores
		*/
		TPlayers _players;

		/**
		Tipo para la lista de Bases
		*/
		typedef std::vector<CBaseInfo*> TBases;

		/**
		Lista de las Bases
		*/
		TBases _bases;


	private:
				/**
		�nica instancia de la clase.
		*/
		static CGameStatus* _instance;
	};
//TBases _bases;
}

#endif //namespace Logic