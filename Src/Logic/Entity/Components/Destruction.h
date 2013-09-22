/**
@file Destruction.h

Contiene la declaraci�n del componente que va a facilitar efectos de destrucci�n sobre el escenario cuando caiga un rayo.

@see Logic::CDestruction
@see Logic::IComponent

@author Jose Luis L�pez		
@date 18-04-2013
*/

#ifndef __Logic_Destruction_H
#define __Logic_Destruction_H

#include "Logic/Entity/Component.h"
#include "../../../Application/Clock.h"

namespace Logic
{
	class CMessage;
	class CGameStatus;
}

//declaraci�n de la clase
namespace Logic 
{
	class CDestruction : public IComponent, public Application::IClockListener
	{
		DEC_FACTORY(CDestruction);
	public:

		CDestruction() : IComponent(GetAltTypeIdOf(CDestruction)), _destroying(false), _destroyingSpeed(0.13f),_step(0), _dying(false),_cont(1){}
		
		bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		bool activate();
		
		void deactivate();

		void tick(unsigned int msecs);

		bool accept(const CMessage *message);

		void process(CMessage *message);

		void destroy();

		bool getDestroying()
		{
			return _destroying;
		}
		////////////////////////////////////////
		// M�todos de IClockListener //
		////////////////////////////////////////
		/**
		M�todo que ser� invocado siempre que se termine una animaci�n.
		Las animaciones en c�clicas no invocar�n nunca este m�todo.

		@param animation Nombre de la animaci�n terminada.
		*/
		void timeArrived();
	protected:

		/**
		*/
		bool _destroying;

		/**
		*/
		float _destroyingSpeed;


		Application::IClock* _reloj;

		unsigned short _step;

		CGameStatus* _gameStatus;

		bool _dying;

		float _cont;
	}; // class CDestruction

	REG_FACTORY(CDestruction);

} // namespace Logic

#endif // __Logic_Destruction_H */
