/**
@file Physic.h


@see Logic::IComponent
@see Physics::IObserver
@see Logic::CPhysicEntity
@see Logic::CPhysicCharacter

@author FRS
@date 23-02-13
*/

#ifndef __Logic_Physic_H
#define __Logic_Physic_H

#include "Logic/Entity/Component.h"
#include "Logic/Entity/LogicalPosition.h"
#include "Physics/IObserver.h"


// Predeclaraci�n de tipos
namespace Physics {
	class CServer;
	class CActor;
}

// Los componentes pertenecen al namespace Logic
namespace Logic 
{	
	/**

    @ingroup logicGroup

	@author FRS
@date 26-02-13
	*/
	class CPhysic : public IComponent, public Physics::IObserver
	{
		DEC_FACTORY(CPhysic);

	public:
	
		/**
		Constructor por defecto.
		*/
		CPhysic();

		/**
		Destructor. Elimina el objeto f�sico de la escena y lo destruye. 
		*/
		virtual ~CPhysic();
		
		/**	Inicializa el componente usando los atributos definidos en el fichero de mapa.*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);
		virtual bool accept(const CMessage *message) { return false; }
		virtual void process(CMessage *message) {}
		virtual void tick(unsigned int msecs) {}
 
		/**************
			IOBSERVER
		***************/
		//Se invoca cuando se produce una colisi�n entre una entidad f�sica y un trigger.
		virtual void onTrigger(IObserver* other, bool enter) {};
		virtual void onCollision(IObserver* other) {};

	protected:

		// Servidor de f�sica
		Physics::CServer* _server;

		// Actor que representa la entidad f�sica
		Physics::CActor* _physicActor;

		// Vector de deplazamiento recibido en los �ltimos mensajes de tipo MOVE.
		// Sirve para mover entidades f�sicas cinem�ticas y de character.
		TLogicalPosition _movement;

		// Crea el actor que representa la entidad f�sica a partir de la informaci�n del mapa.*/
		virtual Physics::CActor* createActor(const Map::CEntity* entityInfo);
	
	}; // class CPhysic

	REG_FACTORY(CPhysic);

} // namespace Logic

#endif // __Logic_Physic_H
