//---------------------------------------------------------------------------
// PlayerInfo.h
//---------------------------------------------------------------------------

/**
@file PlayerInfo.h

Esta es la clase principal de gesti�n del juego. Contiene clases como son Bases, Players, Stats, etc.
@see Logic::PlayerInfo

@author Jose Luis L�pez S�nchez
*/
#pragma once
#ifndef __Logic_PlayerInfo_H
#define __Logic_PlayerInfo_H

//#include "Logic\Entity\LogicalPosition.h"
#include <string>

using namespace std;
namespace Logic
{
	class CBaseInfo;
	class CPlayerInfo;
}
namespace Logic
{
	/**
		En esta clase se guardan tambi�n datos estad�sticos del jugador. Esos datos pueden parecer completamente irrelevantes. 
		Pero m�s adelante es posible que s� que sea interesante saber cosas como quien es el que m�s altares lleva activados,
		quien ha matado a mas enemigos, quien es el que m�s viaja... �Desbloquear logros?...recompensas...
	*/
	class CPlayerInfo
	{
	public:
		CPlayerInfo(const CBaseInfo* miBase);	//CONSTRUIR TAMBI�N CON EL COLOR.

		~CPlayerInfo();
		/**
			Consulta directamente a su componente CLife para obtener getLife() y getLifeMax()
		*/
		int getLife();
		int getLifeMax();

		/**
		Devuelve si est� envenenado, aturdido, convertido en piedra...
		@return quiz� no sea un int y s� un enumerado.
		*/
		int getHealthStatus();

		
		
	protected:
		
		/**
			crea los anillos
		*/
		Logic::CRingInfo* createRing(const unsigned short altars, Ring tipo);
		/**
			Puntero al Player due�o de la base.
		*/
		const CBaseInfo* _miBase;
		
		/**
			N�mero de bases que el Player ha activado
		*/
		unsigned int _basesActivated;

		/**
			N�mero de altares que el Player ha activado
		*/
		unsigned int _altarsActivated;

		/**
			N�mero de heroes que con afilado cobre hemos dado muerte
		*/
		unsigned int _heroesKilled;  

		/**
			N�mero de criaturas matadas
		*/
		unsigned int _creaturesKilled;  

		/**
			Puntos de m�rito acumulados
		*/
		unsigned long _meritPoints;

		/**
			N�mero de fallecimientos
		*/
		unsigned int _myDeaths;
		
		/**
		Nombre del heroe: Hercules, Aquiles...
		*/
		string _heroName;

		/**
		El tercer combo se compraba, puede no tenerlo de inicio, aunque de momento no le impediremos que lo tenga
		de inicio.
		*/
		bool _hasThreeCombos;

		/**
		Puntos de m�rito gastados en la tienda
		*/
		unsigned long _meritPointsSpent;

		/**
		Criaturas que nos han matado
		*/
		unsigned int _lostCreatures;

		/**
		Nombre del jugador, en un futuro veo m�s posible que esto sea un puntero a algo como CJugador.
		Un objeto con datos como email, nick, password...
		*/
		string _playerName;

	};
}

#endif //namespace Logic