/**
@file Jump2.h

Contiene la declaraci�n del componente que controla el movimiento 
de la entidad.

@see Logic::CJump2
@see Logic::IComponent

@author
@date 
*/

#ifndef __Logic_Jump2_H
#define __Logic_Jump2_H

#include "Logic/Entity/Component.h"


namespace Logic
{
	class CMessage;
}

//declaraci�n de la clase
namespace Logic 
{


	class CJump2 : public IComponent
	{
		DEC_FACTORY(CJump2);
	public:

		CJump2() : IComponent(GetAltTypeIdOf(CJump2)), _jumping(false), _jumpSpeed(0.13f), _justJumped(false){}
		
		bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		bool activate();
		
		void deactivate();

		void tick(unsigned int msecs);

		bool accept(const CMessage *message);

		void process(CMessage *message);

		void jump();

		bool getJumping()
		{
			return _jumping;
		}

		

	protected:

		/**
		*/
		bool _jumping;

		/**
		*/
		float _jumpSpeed;

		/**
		*/
		bool _justJumped;


	}; // class CJump2

	REG_FACTORY(CJump2);

} // namespace Logic

#endif // __Logic_Jump2_H */
