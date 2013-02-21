/**
@file PhysicCharacter.h

Contiene la declaraci�n del componente que se utiliza para representar jugadores y enemigos en
el mundo f�sico.

@see Logic::PhysicCharacter

@author Jose Luis L�pez S�nchez
@date Febrero, 2013
*/

#ifndef __Logic_PhysicCharacter_H
#define __Logic_PhysicCharacter_H


#include "Logic/Entity/Component.h"
#include "Logic/Entity/LogicalPosition.h"
#include "Physics/IObserver.h"

namespace Physics{
	class CServer;
	class CActor;
}


// Los componentes se definen dentro del namespace Logica
// TODO corregir comentarios
namespace Logic 
{
	/**
	Componente que se utiliza para representar jugadores y enemigos en el mundo f�sico usando 
	character controllers.
	<p>
	Este componente recibe mensajes de tipo AVATAR_MOVE indicando el movimiento que se quiere 
	realizar. A continuaci�n se le indica al motor de f�sica el movimiento deseado, y en el
	siguiente tick se actualiza la posici�n de la entidad l�gica. Debemos tener en cuenta que 
	la posici�n final la determina el motor de f�sica, ya que durante el movimiento podemos 
	colisionar con otra entidades, resbalar sobre paredes, etc.
	<p>
	Este componente tambi�n simula el efecto de la gravedad sobre el jugador (los controllers
	de PhysX no se ven afectados por la gravedad).
	
    @ingroup logicGroup

	@author Jose Luis L�pez S�nchez & ���
	@date Febrero, 2013
	*/
	class CPhysicCharacter : public IComponent, public Physics::IObserver
	{
		DEC_FACTORY(CPhysicCharacter);
	public:

		/**
		Constructor por defecto.
		*/
		CPhysicCharacter();

		/**
		Destructor.
		*/
		virtual ~CPhysicCharacter();
		
		/**
		Inicializa el componente usando los atributos definidos en el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Este componente s�lo acepta mensajes de tipo AVATAR_WALK.
		*/
		virtual bool accept(const CMessage *message);
		
		/**
		Cuando se recibe un mensaje de tipo AVATAR_WALK, se almacena su vector de 
		desplazamiento para mover el character controller en el pr�ximo tick.
		De esta forma, si en un ciclo se reciben varios mensaje de tipo AVATAR_WALK 
		s�lo tendr� efecto el �ltimo.
		*/
		virtual void process(CMessage *message);

		/**
		Este m�todo se invoca en cada ciclo de la simulaci�n y hace lo siguiente:
		<ul>
		<li> Actualiza la posici�n de la entidad l�gica usando la informaci�n porporcionada por
		     el motor de f�sica. </li>
		<li> Mueve el character controller de acuerdo al �ltimo mensaje AVATAR_WALK recibido. </li>
		</ul>
		<p>
		Los character controllers no tienen orientaci�n, s�lo posici�n
		*/
		virtual void tick(unsigned int msecs);

		// TODO que metodos va a necesitar un character?
		///**
		//Se invoca cuando se produce una colisi�n entre una entidad f�sica y un trigger.
		//*/
		//void  onTrigger (TEntityID otherEntity, bool enter);
		///**
		//Se invoca cuando se produce una colisi�n entre un character controller y una entidad f�sica.
		//*/
		//void onShapeHit (const physx::PxControllerShapeHit &hit);

		///**
		//Se invoca cuando se produce una colisi�n entre dos character controllers.
		//*/
		//void onControllerHit (const physx::PxControllersHit &hit);

	private:

		// Servidor de f�sica
		Physics::CServer *_server;

		// Character controller que representa la entidad f�sica en PhysX
		//	physx::PxCapsuleController *_controller;
		Physics::CActor* _physicActor;

		///**
		//Crea el character controller de PhysX que representa la entidad f�sica a partir de la
		//informaci�n del mapa.
		//*/
		//physx::PxCapsuleController* createController(const Map::CEntity *entityInfo);
		Physics::CActor* createActor(const Map::CEntity* entityInfo);

		// Estructura de desplazamiento acumulado durante los �ltimos mensajes de tipo AVATAR_WALK. 
		Logic::TLogicalPosition _movement;

		// Indica si el character controller esta apoyado sobre una superficie o cayendo.
		bool _falling;  // PeP: _entity->getHeight() tambi�n nos proporciona la misma info, si es 0 est� en el suelo.

	}; // class CPhysicCharacter

	REG_FACTORY(CPhysicCharacter);

} // namespace Logic

#endif // __Logic_PhysicCharacter_H
