/**
@file Physics.h

Contiene la implementaci�n del componente encargado de representar entidades f�sicas simples,
que son aquellas representadas mediante un �nico actor de PhysX. Este componente no sirve
para representar character controllers.

@see Logic::CPhysics
@see Logic::IComponent
@see Logic::CPhysicController

@author ���
@date 26/02/2013
*/

#include "Physics.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Messages/MessageUInt.h"
#include "Logic/Maps/Map.h"

#include "Map/MapEntity.h"

#include "Physics/Scene.h"
#include "Physics/Actor.h"
#include "Physics/ActorTrigger.h"

#define DEBUG 1
#if DEBUG
#	include <iostream>
#	define LOG(msg) std::cout << "LOGIC::PHYSIC>> " << msg << std::endl;
#else
#	define LOG(msg)
#endif

namespace Logic {

	IMP_FACTORY(CPhysics);

	//---------------------------------------------------------

	CPhysics::~CPhysics() 
	{
		if ( _physicalActor ) { // TODO FRS Quiz� este tipo de comprobaci�n sucia deber�a hacerla la propia scene en su remove
			_isTrigger ? 
				_scene->removeActor(  static_cast<Physics::CActorTrigger*>(_physicalActor) ):
				_scene->removeActor(  _physicalActor ); // Eliminar el actor de la escena	

			delete _physicalActor;
		}
		
	} 

	//---------------------------------------------------------

	bool CPhysics::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		// Invocar al m�todo de la clase padre
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		_scene = map->getPhysicScene();
			assert(_scene && "Escena f�sica es NULL");

		_physicalActor = createActor(entityInfo); // Crear el actor asociado al componente
			if(!_physicalActor)
				return false;

		return true;
	} // spawn

	//---------------------------------------------------------

	// Crear el actor f�sico
	Physics::CActor* CPhysics::createActor(const Map::CEntity *entityInfo)
	{
		assert(_scene && "LOGIC::PHYSICS>> No existe escena f�sica!");		

		// Obtenemos la posici�n de la entidad. 
		const TLogicalPosition logicPos = _entity->getLogicalPosition();
	
		// Leer el ancho del angular box
		assert(entityInfo->hasAttribute("physicWidth")); 
		const float physicWidth = entityInfo->getFloatAttribute("physicWidth");

		// Leer la altura del angular box
		assert(entityInfo->hasAttribute("physicHeight"));
		const float physicHeight = entityInfo->getFloatAttribute("physicHeight");

		// TRIGGER: Leer si es un trigger (por defecto no)	
		if (entityInfo->hasAttribute("physicTrigger"))
			_isTrigger = entityInfo->getBoolAttribute("physicTrigger");
		
		// TRIGGER
		if(_isTrigger)  {
			Physics::CActorTrigger* trigger = new Physics::CActorTrigger(logicPos, physicWidth, physicHeight, this);
				if( _scene->addActor(trigger ) ) // A�adir el actor a la escena
					return trigger ;
				else
					return 0;

		// COLLIDER
		} else {
			Physics::CActor* collider = new Physics::CActor(logicPos, physicWidth, physicHeight, this);
				if(_scene->addActor(collider) ) // A�adir el actor a la escena
					return collider;
				else
					return 0;
		}

	} // createActor 

	//---------------------------------------------------------

	void  CPhysics::onTrigger (Physics::IObserver* other, bool enter) 
	{
		// Construimos un mensaje de tipo TOUCHED o UNTOUCHED 
		// y lo enviamos a todos los componentes de la entidad.

		CMessageUInt* txMsg = new CMessageUInt();		
			txMsg->setType( Message::TRIGGER ); 	
			txMsg->setAction( 
				enter ? 
				Message::TRIGGER_ENTER : 
				Message::TRIGGER_EXIT
			);			
			txMsg->setUInt( static_cast<CPhysics*>(other)->getEntity()->getEntityID() );
		_entity->emitMessage(txMsg);


		// FRS: Adjuntamos el entityID para aquellos componentes que necesitan
		// conocer la entidad que ha entrado en este trigger para causarle
		// su efecto correspondiente (p.e enviarle un LIFE_MODIFIER desde CLifeModifier)
		// EntityID mejor que CEntity* para evitar que el receptor del mensaje 
		// se encuentre con un puntero a una entidad borrada (muerta)

	} // onTrigger

	

	
}
