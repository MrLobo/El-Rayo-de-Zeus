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
#ifndef __Logic_Base_H
#define __Logic_Base_H

#include "Logic\Entity\LogicalPosition.h"
#include <string>
#include <list>
namespace Logic
{
	class CRing;
	class CPlayer;
}
namespace Logic
{
	class CBase
	{
	public:
		CBase(const unsigned short numRings);

		~CBase();
		/**
			Para obtener el n�mero de defensores de la base.
		*/
		unsigned int getDefenders();

		/**
			Para obtener el n�mero de atacantes en la base. As� saber que la base est� calentita.
		*/
		unsigned int getAttakers();
				
		/**
			N�mero de altares
		*/
		const unsigned short _numAltars;

		/**
			N�mero de anillos
		*/
		const unsigned short _numRings;  //ya se que siempre ser�n 3, pero por si acaso.

		/**
			getters y setters
		*/
		unsigned short getNumActivatedAltars(){return _numActivatedAltars;}

		void setNumActivatedAltars(const unsigned short numActivatedAltars){_numActivatedAltars=numActivatedAltars;}
	
		bool getAllAltarsActivated(){return _AllAltarsActivated;}

		void setAllAltarsActivated(const bool allAltarsActivated){_AllAltarsActivated=allAltarsActivated;}
		
	protected:
		
		/**
			crea los anillos
		*/
		Logic::CRing* CBase::createRing(const unsigned short altars, Ring tipo);
		/**
			Puntero al Player due�o de la base.
		*/
		const CPlayer* _player;
				
		/**
			N�mero de altares activados
		*/
		unsigned short _numActivatedAltars;
		
		/**
			Base Activada (todos sus altares activados
		*/
		bool _AllAltarsActivated;

		/**
			Vida, numero de rayos que tiene, de 0 a 3
		*/
		unsigned short _life;
		
		/**
		Tipo para la lista de Anillos
		*/
		typedef std::list<CRing*> TRings;
		
		/**
		Lista de los anillos
		*/
		TRings _rings;
	};
}

#endif //namespace Logic