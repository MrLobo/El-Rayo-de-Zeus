/**
@file AltarController.h

Contiene la declaraci�n del componente que controla la
activacion/desactivaci�n de un Altar.

@see Logic::CAltarController


@author Emilio Santalla
@date Marzo, 2013
*/

#ifndef __Logic_AltarController_H
#define __Logic_AltarController_H

#include "Logic/Entity/Component.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n

namespace Logic
{
	class CMessage;
}

//declaraci�n de la clase
namespace Logic 
{

	class CAltarController : public IComponent
	{
		DEC_FACTORY(CAltarController);
	public:


		CAltarController() : IComponent(GetAltTypeIdOf(CAltarController)), _switchingState(false), _revertingState(), _activated(false), _switchingTime(3000), _acumTime(0) {}

		bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		bool activate();
		
		void deactivate();

		void tick(unsigned int msecs);

		bool accept(const CMessage *message);

		virtual void process(CMessage *message);

		
		void startSwitchingState();
		void stopSwitchingState();

	protected:

		/**
		*/
		bool _switchingState;

		/**
		*/
		bool _revertingState;

		/**
		*/
		bool _activated;

		/**
		*/

		int _switchingTime;

		/**
		*/
		int _acumTime;
		

	}; // class CAltarController

	REG_FACTORY(CAltarController);

} // namespace Logic

#endif // __Logic_AltarController_H */