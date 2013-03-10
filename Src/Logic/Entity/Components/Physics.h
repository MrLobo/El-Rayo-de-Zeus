/**
@file PhysicEntity.h

Contiene la declaraci�n del componente encargado de representar entidades f�sicas simples.
Este componente no sirve para representar physic characters.

@see Logic::CPhysicEntity
@see Logic::IComponent
@see Logic::CPhysicCharacter
@see Physics::IObserver

@author FRS
@date 23-02-13
*/

#ifndef __Logic_PhysicEntity_H
#define __Logic_PhysicEntity_H

#include "Logic/Entity/Component.h"
#include "Physics/IObserver.h"


// Predeclaraci�n de tipos
namespace Physics {
	class CScene;
	class CActor;
}

// Los componentes pertenecen al namespace Logic
namespace Logic 
{	
	/**
	Componente encargardo de la representaci�n f�sica de una entidad simple. Entendemos por 
	entidades simples aquellas que pueden ser representadas mediante un �nico actor.
	Esta clase no sirve para representar physic character ni entidades compuestas como ragdolls.
	<p>
	Existen dos tipos de entidades f�sicas: est�ticas y din�micas. Las entidades 
	est�ticas no se pueden mover una vez creadas y s�lo sirven como vol�menes de colisi�n.
	Las entidades din�micas tienen masa y se mueven aplic�ndoles fuerzas (por ejemplo la gravedad). 
	Tambi�n existe un tipo especial de entidades din�micas llamadas cinem�ticas, que est�n pensadas 
	para representar objetos cuyo movimiento viene dictado desde la l�gica del juego. Las entidades 
	cinem�ticas se pueden mover mediante desplazamientos en lugar de fuerzas.
	<p>
	En cada tick, este componente actualiza la posici�n y rotaci�n de las entidades l�gicas usando
	la informaci�n que proporciona el motor de f�sica (s�lo entidades din�micas). 
	<p>
	Cuando este componente se utiliza para representar una entidad cinem�tica, acepta mensajes de 
	tipo KINEMATIC_MOVE indicando el movimiento que se quiere realizar. Y en cada tick el componente
	intenta mover la entidad f�sica de acuerdo a los mensajes recibidos. 
	
    @ingroup logicGroup

	@author FRS
	@date 23-02-13
	*/
	class CPhysicEntity : public IComponent, public Physics::IObserver
	{
		DEC_FACTORY(CPhysicEntity);

	public:
	
		/**
		Constructor por defecto.
		*/
		CPhysicEntity();

		CPhysicEntity(altTypeId id);

		/**
		Destructor. Elimina el objeto f�sico de la escena y lo destruye. 
		*/
		virtual ~CPhysicEntity();
		
		/**	Inicializa el componente usando los atributos definidos en el fichero de mapa.*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);
		virtual bool accept(const CMessage *message) { return false; }
		virtual void process(CMessage *message) {}
		virtual void tick(unsigned int msecs) {}
 
		/**************
			IOBSERVER
		***************/
		//Se invoca cuando se produce una colisi�n entre una entidad f�sica y un trigger.
		virtual void onTrigger(IObserver* other, bool enter);
		virtual void onCollision(IObserver* other) {};

	protected:

		// UNDONE FRS Physics::CServer* _server; // Servidor de f�sica
		Physics::CScene* _scene; // Servidor de f�sica
		Physics::CActor* _actor; // Actor que representa la entidad f�sica

		// Desplazamiento recibido en los �ltimos mensajes de tipo MOVE.
		// Sirve para mover entidades f�sicas cinem�ticas y de character.
		char _diffBase;
		char _diffRing;
		float _diffDegrees;
		float _diffHeight;

		// Crea el actor que representa la entidad f�sica a partir de la informaci�n del mapa.*/
		virtual Physics::CActor* createActor(const Map::CEntity* entityInfo);
		// TODO FRS Podr�a pasar a llamarse physicEntity (por paralelismos con CGraphics)
	
	}; // class CPhysicEntity

	REG_FACTORY(CPhysicEntity);

} // namespace Logic

#endif // __Logic_PhysicEntity_H
