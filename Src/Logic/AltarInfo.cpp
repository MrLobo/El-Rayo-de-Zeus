//---------------------------------------------------------------------------
// AltarInfo.cpp
//---------------------------------------------------------------------------

/**
@file AltarInfo.cpp

Contiene la implementaci�n de la clase AltarInfo

@see Logic::AltarInfo

@author Jose Luis L�pez S�nchez
*/

#include "Logic\AltarInfo.h"
#include "Logic\Entity\Entity.h"
#include "Logic\Entity\Components\Altar.h"
#include "Logic\Entity\Components\AnimatedAltar.h"

namespace Logic
{
	CAltarInfo::CAltarInfo(Logic::CEntity* entity):_altar(entity),_activated(false),_toActivation(false),_toDesActivation(false)// Esto es as� hasta saber como ser� la escena que emilio est� haciendo.
	{		
	}

	bool CAltarInfo::isActivated()
	{
		if (_altar) //si hay un puntero valido a un altar
			if (_altar->hasComponent<CAltar>())
				_activated=_altar->getComponent<CAltar>()->isActivated();
			if (_altar->hasComponent<CAnimatedAltar>())
				_activated=_altar->getComponent<CAnimatedAltar>()->isActivated();

		return _activated;
	}	
}