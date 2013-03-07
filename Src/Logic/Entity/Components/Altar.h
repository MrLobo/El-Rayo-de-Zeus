/**
@file Altar.h

Contiene la declaraci�n del componente que controla la
activacion/desactivaci�n de un Altar.

@see Logic::CAltar


@author Emilio Santalla
@date Marzo, 2013
*/

#ifndef __Logic_Altar_H
#define __Logic_Altar_H

#include "Logic/Entity/Component.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n

namespace Logic
{
	class CMessage;
}

//declaraci�n de la clase
namespace Logic 
{

	class CAltar : public IComponent
	{
		DEC_FACTORY(CAltar);
	public:


		CAltar() : IComponent(GetAltTypeIdOf(CAltar)), _switchingState(false), _revertingState(), _on(false), _switchingTime(3000), _acumTime(0) {}

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
		bool _on;

		/**
		*/

		int _switchingTime;

		/**
		*/
		int _acumTime;
		

	}; // class CAltar

	REG_FACTORY(CAltar);

} // namespace Logic

#endif // __Logic_Altar_H */