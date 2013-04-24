//---------------------------------------------------------------------------
// BaseInfo.h
//---------------------------------------------------------------------------

/**
@file BaseInfo.h

Esta es la clase que representa una base y que contiene anillos. Contiene entre otras
cosas un puntero al due�o de la base (CPlayerInfo, lista de CRingInfo).
@see Logic::GameStatus

@author Jose Luis L�pez S�nchez
*/
#pragma once
#ifndef __Logic_BaseInfo_H
#define __Logic_BaseInfo_H

#include "Logic\Entity\LogicalPosition.h"
#include <string>
#include <vector>
namespace Logic
{
	class CRingInfo;
	class CPlayerInfo;
}
namespace Logic
{
	/**
		Cada base debe tener un color asociado. Esto no quiere decir que la base vaya a ser de ese color. 
		Simplemente que sus altares, su iluminaci�n emite ese color. Y quiz�, que los heroes tien shaders
		de ese color. Por ejemplo si dos jugadores decien cogerse el mismo personaje cada uno ser� del color
		asociado a la base.
	*/
	enum Color : unsigned short
		{	
			WHITE	=0,
			RED		=1,
			BLUE	=2,
			GREEN	=3,
			YELLOW	=4,
			BLACK	=5, //Suponiendo que sea un color v�lido, ahora por el fondo no lo es
			ORANGE	=6,
			VIOLET	=7,
			BROWN	=8
		};

	class CBaseInfo
	{
	public:
		CBaseInfo(const unsigned short numRings);	//CONSTRUIR TAMBI�N CON EL COLOR.

		~CBaseInfo();
		/**
			Para obtener el n�mero de defensores de la base.
		*/
		unsigned int getDefenders();

		/**
			Para obtener el n�mero de atacantes en la base. As� saber que la base est� calentita.
		*/
		unsigned short getAttakers() {return _numAttakers;};

		/**
			Para obtener el n�mero de vidas de la base.
		*/
		unsigned short getBaseLife() {return _life;};
				
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
		unsigned short getNumActivatedAltars();

		void setNumActivatedAltars(const unsigned short numActivatedAltars){_numActivatedAltars=numActivatedAltars;}
	
		bool areAllAltarsActivated();

		//void setAllAltarsActivated(const bool allAltarsActivated){_AllAltarsActivated=allAltarsActivated;}
		void updateAllAltarsInfo();
		CRingInfo* getRing(unsigned short ring);

		short int getNumAltars(){return _numAltars;}

		/**
		N�mero de altares que quedan para que la base sea destruida.
		*/
		short int getNumAltarsLeft(){return _numAltars-_numActivatedAltars;}
	protected:
		
		/**
			crea los anillos
		*/
		Logic::CRingInfo* createRing(const unsigned short altars, Ring tipo);
				
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
		typedef std::vector<CRingInfo*> TRings;
		
		/**
		Lista de los anillos
		*/
		TRings _rings;

		/**
		Numero de players que estan en mi base
		*/
		unsigned short _numAttakers;
	};
}

#endif //namespace Logic