/**
@file PhysicCharacter.cpp

Contiene la implementaci�n del componente que se utiliza para representar jugadores y enemigos en
el mundo f�sico usando character controllers.

@see Logic::CPhysicCharacter
@see Logic::CPhysicEntity
@see Physics::IObserver

@author ���
@date 21-02-2013
*/

#include "PhysicCharacter.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"

#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageInt.h" // TODO PeP: ser�a �ptimo enviar un unsigned short???
#include "Logic/Entity/Messages/MessageFloat.h"

#include "Physics/Server.h"
#include "Physics/Actor.h"

namespace Logic {

	IMP_FACTORY(CPhysicCharacter);

	//---------------------------------------------------------

	CPhysicCharacter::CPhysicCharacter() : IComponent(), _physicActor(0), _falling(false)
	{	
		_server = Physics::CServer::getSingletonPtr();
	}

	//---------------------------------------------------------

	CPhysicCharacter::~CPhysicCharacter() 
	{
		if (_physicActor) {
			_physicActor->release();
			_physicActor = 0;
		}
		_server = 0;
	} 

	//---------------------------------------------------------

	bool CPhysicCharacter::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo)
	{
		if(!IComponent::spawn(entity,map,entityInfo)) // Invocar al m�todo de la clase padre
			return false;

		_entityID = _entity->getEntityID();		
		_physicActor = createActor(entityInfo); // Crear el actor asociado al componente

		return true;
	}

	//---------------------------------------------------------

	bool CPhysicCharacter::accept(const CMessage *message)
	{
		return message->getType() == Message::AVATAR_MOVE;
	} 

	//---------------------------------------------------------

	// Acumulamos las diferentes coordenadas vector de desplazamiento para usarlo posteriormente en  el m�todo tick.
	// De esa forma, si recibimos varios mensajes AVATAR_WALK tendremos la �ltima coordenada de cada tipo (degrees, height, ring, base)
	void CPhysicCharacter::process(CMessage *message)
	{
		switch( message->getAction() ) {

		// UNDONE ���: Necesitamos la orientaci�n para alguna comprobaci�n f�sica? => setSense directamente en AvatarController
		//if(message->getAction() == Message::WALK_LEFT)
		//_movement._sense=Sense::LEFT;
		//else
		//_movement._sense=Sense::RIGHT;

		case Message::WALK_LEFT: 			// TODO Unificar walks en un action WALK a secas?
		case Message::WALK_RIGHT:
			_movement._degrees = static_cast<CMessageFloat*>(message)->getFloat();	
			break;

		case Message::JUMP:
			_movement._height = static_cast<CMessageFloat*>(message)->getFloat();
			break;

		case Message::CHANGE_RING:		
			_movement._ring = (Ring) static_cast<CMessageInt*>(message)->getInt();
			break;

		case Message::CHANGE_BASE:
			_movement._base= static_cast<CMessageInt*>(message)->getInt();	
			break;

		} // switch message action

			//PEP: y ahora ya tenemos la posici�n l�gica completa
			//es posible enviarla entera, enviar _logicalPosReceived, o enviar s�lamente lo que haya cambiado respecto al tick anterior
			//eso se conseguir�a de varias maneras, la primera que se me ocurre es guardar la posici�n l�gica anterior con la que se 
			//acaba de obtener y enviar s�lamente lo que haya cambiado.
			
			//��� Hombre, lo suyo es que los AVATAR_WALK que se reciban sean diferenciales ya de por s� ("vectores de 4 coordenadas" l�gicos unitarios)
			//De ese modo te evitas tener que almacenar la ultima posicion...
	} 

	//---------------------------------------------------------

	void CPhysicCharacter::tick(unsigned int msecs) 
	{
		// Llamar al m�todo de la clase padre (IMPORTANTE).
		IComponent::tick(msecs);

		// Actualizar la posici�n y orientaci�n de la entidad l�gica 
		// usando la informaci�n proporcionada por el motor de f�sica	
		// Este a genera  SET_TRANSFORM por debajo que informa al CGraphics
		_entity->setLogicalPosition( _server->getActorLogicPosition(_physicActor) );  

		// TODO Efecto de la gravedad quiz� sea necesario..?
		//if (_falling) {
		//	_movement += Vector3(0,-1,0);
		//}

		// Intentamos mover el actor seg�n los AVATAR_MOVE acumulados. 
		 _server->moveActor(_physicActor, _movement); // TODO a�adir msecs);

		// TODO Actualizamos el flag que indica si estamos cayendo
		//_falling =  !(flags & PxControllerFlag::eCOLLISION_DOWN);
		
		_movement = TLogicalPosition(); // Ponemos el movimiento a cero
	}

	//---------------------------------------------------------
	
	Physics::CActor* CPhysicCharacter::createActor(const Map::CEntity *entityInfo)
	{
		// Obtenemos la posici�n de la entidad. 
		TLogicalPosition logicalPos = _entity->getLogicalPosition();
	
		// Leer el ancho del angular box
		assert(entityInfo->hasAttribute("physicWidth")); // TODO ��� Por qu� se hacen asserts en lugar de simples if como en el spawn normal?
		float physicWidth = entityInfo->getFloatAttribute("physicWidth");

		// Leer la altura del angular box
		assert(entityInfo->hasAttribute("physicHeight"));
		float height = entityInfo->getFloatAttribute("physicHeight");

		// Crear el controller de tipo c�psula
		return _server->createActor(logicalPos, physicWidth, height, false, this);
	} // createActor 
	
	

	//---------------------------------------------------------
	/*
	void CPhysicCharacter::onShapeHit (const PxControllerShapeHit &hit)
	{
		// Si chocamos contra una entidad est�tica no hacemos nada
		PxRigidDynamic* actor = hit.shape->getActor().isRigidDynamic();
		if(!actor)
			return;

		// Si chocamos contra una entidad cinem�tica no hacemos nada
		if (_server->isKinematic(actor))
			return;
	
		// Aplicar una fuerza a la entidad en la direcci�n del movimiento
		actor->addForce(hit.dir * hit.length * 1000.0f);
	}*/
	
	//---------------------------------------------------------

}


