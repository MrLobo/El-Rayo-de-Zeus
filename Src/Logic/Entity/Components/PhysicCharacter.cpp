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
//#include "Map/MapEntity.h"

#include "Logic/Entity/Messages/Message.h"
//#include "Logic/Entity/Messages/MessageInt.h" // TODO PeP: ser�a �ptimo enviar un unsigned short???
//#include "Logic/Entity/Messages/MessageFloat.h"

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

	void CPhysicCharacter::process(CMessage *message)
	{
		switch(message->getType())
		{
		case Message::AVATAR_MOVE:
			if(message->getAction() == Message::WALK_LEFT || message->getAction() == Message::WALK_RIGHT)
			{
				if(message->getAction() == Message::WALK_LEFT)
					_logicalPosReceived._sense=Sense::LEFT;
				else
					_logicalPosReceived._sense=Sense::RIGHT;			
			
				CMessageFloat* maux = static_cast<CMessageFloat*>(message);
				_logicalPosReceived._degrees=maux->getFloat();
			
			} else if(message->getAction() == Message::JUMP)
			{
				CMessageFloat* maux = static_cast<CMessageFloat*>(message);
				_logicalPosReceived._height=maux->getFloat();
			} else if(message->getAction()== Message::CHANGE_RING)
			{
				CMessageInt* maux = static_cast<CMessageInt*>(message);
			
				switch (maux->getInt())
					{
						case Logic::LogicalPosition::LOWER_RING:
						{
							_logicalPosReceived._ring= Logic::LogicalPosition::LOWER_RING;
							break;
						}
						case Logic::LogicalPosition::CENTRAL_RING:
						{
							_logicalPosReceived._ring = Logic::LogicalPosition::CENTRAL_RING;				
							break;
						}
						case Logic::LogicalPosition::UPPER_RING:
						{
							_logicalPosReceived._ring = Logic::LogicalPosition::UPPER_RING;
							break;
						}
						default:
						{
							_logicalPosReceived._ring = Logic::LogicalPosition::CENTRAL_RING;
							break;
						}
				}
			}else if(message->getAction()== Message::CHANGE_BASE)
			{
				CMessageInt* maux = static_cast<CMessageInt*>(message);
				_logicalPosReceived._base= maux->getInt();	
			}
			//PEP: y ahora ya tenemos la posici�n l�gica completa
			//es posible enviarla entera, enviar _logicalPosReceived, o enviar s�lamente lo que haya cambiado respecto al tick anterior
			//eso se conseguir�a de varias maneras, la primera que se me ocurre es guardar la posici�n l�gica anterior con la que se 
			//acaba de obtener y enviar s�lamente lo que haya cambiado.
			//��� Hombre, lo suyo es que los AVATAR_WALK que se reciban sean diferenciales ya de por s� (a partir de "vectores l�gicos unitarios")
			//De ese modo te evitas tener que almacenar la ultima posicion...

			// Anotamos el vector de desplazamiento para usarlo posteriormente en 
			// el m�todo tick. De esa forma, si recibimos varios mensajes AVATAR_WALK
			// en el mismo ciclo s�lo tendremos en cuenta el �ltimo.
			//_movement = message._vector3;
			break;
		}

	} 

	//---------------------------------------------------------

	void CPhysicCharacter::tick(unsigned int msecs) 
	{
		// Llamar al m�todo de la clase padre (IMPORTANTE).
		IComponent::tick(msecs);



		// Actualizar la posici�n y orientaci�n de la entidad l�gica usando la 
		// informaci�n proporcionada por el motor de f�sica	
		//_entity->setPosition(_server->getControllerPosition(_controller));

		// Si estamos cayendo modificar el vector de desplazamiento para simular el 
		// efecto de la gravedad. Lo hacemos de manera sencilla y pero poco realista.
		//if (_falling) {
			//_movement += Vector3(0,-1,0);
	//	}

		// Intentamos mover el controller a la posici�n recibida en el �ltimo mensaje 
		// de tipo AVATAR_WALK. 
		//unsigned flags = _server->moveController(_controller, _movement, msecs);

		// Actualizamos el flag que indica si estamos cayendo
		//_falling =  !(flags & PxControllerFlag::eCOLLISION_DOWN);

		// Ponemos el movimiento a cero
		//_movement = Vector3::ZERO;
		/* y aqu� pone la posici�n final, ahora sin comprobaci�n f�sica pero recogiendo todos los datos de los componentes de la entidad*/
		_entity->setLogicalPosition(_logicalPosReceived);
	
		Vector3 newPosition=_entity->fromLogicalToCartesian(_entity->getDegree(),_entity->getHeight(),_entity->getBase(),_entity->getRing());
		_entity->setPosition(newPosition);
	}

	//---------------------------------------------------------
	/*
	PxCapsuleController* CPhysicCharacter::createController(const Map::CEntity *entityInfo)
	{
		// Obtenemos la posici�n de la entidad. Inicialmente colocaremos el controller
		// un poco por encima del suelo, porque si lo ponemos justo en el suelo a veces
		// lo atraviesa en el primer ciclo de simulaci�n.
		Vector3 position = _entity->getPosition() + Vector3(0, 0.5f, 0);
	
		// Leer el volumen de colisi�n del controller. Por ahora s�lo admitimos c�psulas.
		std::string shape = "capsule";
		if (entityInfo->hasAttribute("physic_shape")) {
			shape = entityInfo->getStringAttribute("physic_shape");
			assert(shape == "capsule");
		}

		// Leer el radio de la c�psula
		assert(entityInfo->hasAttribute("physic_radius"));
		float radius = entityInfo->getFloatAttribute("physic_radius");

		// Leer la altura de la c�psula
		assert(entityInfo->hasAttribute("physic_height"));
		float height = entityInfo->getFloatAttribute("physic_height");

		// Crear el controller de tipo c�psula
		return _server->createCapsuleController(position, radius, height, this);
	} 
	*/
	//---------------------------------------------------------

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
	/*
	void CPhysicCharacter::onControllerHit (const PxControllersHit &hit)
	{

	}
	*/
	//---------------------------------------------------------

}


