//---------------------------------------------------------------------------
// AltarInfo.h
//---------------------------------------------------------------------------

/**
@file AltarInfo.h

Clase que guarda informaci�n sobre el altar
@see Logic::AltarInfo

@author Jose Luis L�pez S�nchez
*/
#pragma once
#ifndef __Logic_RingInfo_H
#define __Logic_RingInfo_H

#include <string>
#include <list>

#include "Logic\Entity\LogicalPosition.h"
namespace Logic
{
	class CEntity;
	class CPlayer;

}
namespace Logic
{
	class CAltarInfo
	{
	public:
		CAltarInfo(const unsigned short numAltars, Ring tipo);

		~CAltarInfo();
		
		/**
			 Para saber quien es el que est� usando el altar
		*/
		CPlayer* getPlayer();

		/**
			 Para notificar quien es el que est� usando el altar
		*/
		void setPlayer(const CPlayer* playerChanging);

		//quiz� queramos poner un metodo que te devuelva quien es el due�o del altar.

		/**
			getters y setters
		*/

		
	protected:
		
		/**
			Flag de activado
		*/
		bool _activated;

		/**
			Indica si est� en tr�nsito de activaci�n
		*/
		bool _toActivation;
		
		
		/**
			Indica si est� en tr�nsito de desactivaci�n
		*/
		bool _toDesActivation;
		
		/**
			Puntero al CEntity que representa �ste altar l�gico.
		*/		
		Logic::CEntity* _cAltar;

	};
}

#endif //namespace Logic