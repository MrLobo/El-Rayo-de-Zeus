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

		/**
		Destructor. Elimina el objeto f�sico de la escena y lo destruye. 
		*/
		virtual ~CPhysicEntity();
		
		/**
		Inicializa el componente usando los atributos definidos en el fichero de mapa.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		Este componente s�lo acepta mensajes de tipo KINEMATIC_MOVE. 
		Estos mensajes s�lo se utilizan para mover entidades de tipo cinem�tico.
		*/
		virtual bool accept(const CMessage *message);

		/**
		Cuando recibe mensajes de tipo KINEMATIC_MOVE almacena los movimientos para aplicarlos 
		en el pr�ximo tick sobre la entidad cinem�tica. Si en un ciclo se reciben varios 
		mensajes KINEMATIC_MOVE se acumulan. 		*/

		virtual void process(CMessage *message);

		/**
		Este m�todo se invoca en cada ciclo de la simulaci�n y hace lo siguiente:
		<ul>
		<li> Si la entidad f�sica es de tipo est�tico no hace nada. </li>
		<li> Si la entidad f�sica es de tipo din�mico actualiza la posici�n y rotaci�n de 
		     la entidad l�gica usando la informaci�n proporcionada por el motor de f�sica. </li>
		<li> Si la entidad f�sica es de tipo cinem�tico, adem�s solicita al motor de f�sica
		     que mueva la entidad de acuerdo al �ltimo mensaje KINEMATIC_MOVE recibido. </li>
		</ul>
		*/
// TODO Impl. cuando tengamos dinamicas o cin�maticas
// De momento s�lo tenemos est�ticas
		//virtual void tick(unsigned int msecs);
 
		/**************
			IOBSERVER
		***************/
		//Se invoca cuando se produce una colisi�n entre una entidad f�sica y un trigger.
		virtual void onTrigger (IObserver *other, bool enter);

	private:

		// Servidor de f�sica
		Physics::CServer* _server;

		// Actor que representa la entidad f�sica
		Physics::CActor* _physicActor;

		// Vector de deplazamiento recibido en el �ltimo mensaje de tipo KINEMATIC_MOVE. Sirve
		// para mover entidades f�sicas cinem�ticas.
		TLogicalPosition _movement;

		// Crea el actor que representa la entidad f�sica a partir de la informaci�n del mapa.*/
		Physics::CActor* createActor(const Map::CEntity* entityInfo);
	
		//Crea un plano est�tico a partir de la informaci�n de mapa. 
		//Physics::CActor* createPlane(const Map::CEntity* entityInfo);
		
		//Crea una entidad r�gida (est�tica, din�mica o cinem�tica) a partir de la informaci�n de mapa. 
		Physics::CActor* createRigid(const Map::CEntity* entityInfo);

	}; // class CPhysicEntity

	REG_FACTORY(CPhysicEntity);

} // namespace Logic

#endif // __Logic_PhysicEntity_H
