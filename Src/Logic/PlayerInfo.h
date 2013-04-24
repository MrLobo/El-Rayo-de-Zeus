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
#include <string>

using namespace std;
//#include "Logic/Entity/Entity.h"
namespace Logic
{
	class CBaseInfo;
	class CEntity;
}

namespace ScriptManager
{
	class CServer;
}

namespace Logic
{
	/**
		En esta clase se guardan tambi�n datos estad�sticos del jugador. Esos datos pueden parecer completamente irrelevantes. 
		Pero m�s adelante es posible que s� que sea interesante saber cosas como quien es el que m�s altares lleva activados,
		quien ha matado a mas enemigos, quien es el que m�s viaja... �Desbloquear logros?...recompensas...
	*/
	/*enum Hero : string
		{	
			AQUILES ,	
			ATALANTA,
			HECTOR	,
			HERCULES,
			HIPOLITA,
			PERSEO	,
			TESEO	, 
			ULISES	,
		};*/

	class CPlayerInfo
	{
	public:
		CPlayerInfo(const CBaseInfo* miBase, Logic::CEntity* entity);	//CONSTRUIR TAMBI�N CON EL COLOR.

		~CPlayerInfo();
		/**
			Consulta directamente a su componente CLife para obtener getLife() y getLifeMax()
		*/
		int getLife();
		int getLifeMax();

		bool attachCEntity(CEntity* entidad)
		{
			if (entidad!=NULL)
			 {
				 _player =entidad;
				 //TODO: aqu� refresco todos los datos que puedan ser obtenidos desde la entidad _player.

				return true;
			}
			return false;
		}

		/**
		Devuelve si est� envenenado, aturdido, convertido en piedra...
		@return quiz� no sea un int y s� un enumerado.
		*/
		int getHealthStatus();
		
		/**
		getters y setters
		*/

		//por el momento todos los getters, id a�adiendo solo los setters que sean necesarios.
		unsigned int getBasesActivated() { return _basesActivated;	}
		
		void increaseAltarsActivated();

		unsigned int getAltarsActivated()
		{
			return _altarsActivated;
		}
		
		unsigned int getHeroesKilled()
		{
			return _heroesKilled;
		}

		unsigned int getCreaturesKilled()
		{
			return _creaturesKilled;
		}

		unsigned short getMeritPoints()
		{
			return _meritPoints;
		}

		unsigned int getMyDeaths()
		{
			return _myDeaths;
		}

		bool hasThreeCombos()
		{
			return _hasThreeCombos;
		}

		unsigned int getMeritPointsSpent()
		{
			return _meritPointsSpent;
		}

		unsigned int getLostCreatures() //habr� que ampliarlo despues con los escuderos
		{
			return _lostCreatures;
		}

		void setHeroName(string heroName)
		{
			_heroName=heroName;
		}

		string getHeroName()
		{
			return _heroName;
		}

		//SI el jugador se encuentra en su base origen
		bool inMyBase();

		//SI el jugador se encuentra en la base "base"
		bool inBase(unsigned short base);
		
		void setInMyBase(bool inMyBase)
		{
			_inMyBase=inMyBase;
		}

		//PT
		Logic::CEntity* getPlayer()	{return _player;}

		

	protected:
		/**
			Enumerado del tipo de heroe, es posible que al final lo quite.
		*/
		//Hero _hero ;
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
		unsigned short _meritPoints;

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
		
		/**
		Indica si est�s en tu base en un instante dado
		*/
		bool _inMyBase;

		/**
			Puntero al CEntity que representa �ste player/bot l�gico
		*/		
		Logic::CEntity* _player;
	};
}

#endif //namespace Logic