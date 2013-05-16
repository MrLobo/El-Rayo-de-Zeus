/**
@file PhysicalCharacter.cpp

Contiene la implementaci�n del componente que se utiliza para representar jugadores y enemigos en
el mundo f�sico usando character controllers.

@see Logic::CPhysicalCharacter
@see Logic::CPhysics
@see Physics::IObserver

@author ���
@date 21-02-2013
*/

#include "PhysicalCharacter.h"


#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"

#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageChar.h" // TODO PeP: ser�a �ptimo enviar un unsigned short???
#include "Logic/Entity/Messages/MessageFloat.h"

#include "Physics/Actor.h"
#include "Physics/Scene.h"



#define DEBUG 1
#if DEBUG
#	include <iostream>
#	define LOG(msg) std::cout << "LOGIC::PHYSIC_CHARACTER>> " << msg << std::endl;
#else
#	define LOG(msg)
#endif


namespace Logic {

	IMP_FACTORY(CPhysicalCharacter);

	//---------------------------------------------------------

	bool CPhysicalCharacter::accept(const CMessage *message)
	{
		return message->getType() == Message::AVATAR_MOVE;
	} 

	//---------------------------------------------------------

	// Acumulamos las diferentes coordenadas vector de desplazamiento para usarlo posteriormente en  el m�todo tick.
	// De esa forma, si recibimos varios mensajes AVATAR_WALK tendremos la �ltima coordenada de cada tipo (degrees, height, ring, base)
	void CPhysicalCharacter::process(CMessage *message)
	{
		switch( message->getAction() ) {
			
		case Message::WALK_LEFT:
		case Message::WALK_RIGHT:
			_diffDegrees = static_cast<CMessageFloat*>(message)->getFloat();	
			break;
		case Message::JUMP:
			_diffHeight = static_cast<CMessageFloat*>(message)->getFloat();
			break;
		//case Message::CHANGE_RING:		// TODO ��� por seguridad quiz� habr�a que probar que _ring < MAX del enum --> asserts!
		//	_diffRing = static_cast<CMessageChar*>(message)->getChar();
		//	break;
		//case Message::CHANGE_BASE:
		//	_diffBase = static_cast<CMessageChar*>(message)->getChar();	
		//	break;

		} // switch message action

	//	LOG("Movement = " << (int) _movBase << ":" << (int) _movRing  << ":" << _movDegrees << ":" << _movHeight );
		// UNDONE_movement._base << ":" << (int) _movement._ring << ":" << _movement._degrees << ":" << _movement._height );

			//PEP: y ahora ya tenemos la posici�n l�gica completa
			//es posible enviarla entera, enviar _logicalPosReceived, o enviar s�lamente lo que haya cambiado respecto al tick anterior
			//eso se conseguir�a de varias maneras, la primera que se me ocurre es guardar la posici�n l�gica anterior con la que se 
			//acaba de obtener y enviar s�lamente lo que haya cambiado.
			
			//��� Hombre, lo suyo es que los AVATAR_WALK que se reciban sean diferenciales ya de por s� ("vectores de 4 coordenadas" l�gicos unitarios)
			//De ese modo te evitas tener que almacenar la ultima posicion...
	} 

	//---------------------------------------------------------

	void CPhysicalCharacter::tick(unsigned int msecs) 
	{
		// Llamar al m�todo de la clase padre (IMPORTANTE).
		IComponent::tick(msecs);

		// Actualizar la posici�n y orientaci�n de la entidad l�gica 
		// usando la informaci�n proporcionada por el motor de f�sica	
		// Este a genera  SET_TRANSFORM por debajo que informa al CGraphics

		_entity->yaw(Math::fromDegreesToRadians(_entity->getLogicalPosition()->getDegree() - _physicalActor->getDegree()));

		
		_auxPos->setDegree(_physicalActor->getDegree()); 
		_auxPos->setHeight(_physicalActor->getHeight()); 
		_auxPos->setRing(_entity->getLogicalPosition()->getRing());
		_auxPos->setBase(_entity->getLogicalPosition()->getBase());	
		_auxPos->setSense(_entity->getLogicalPosition()->getSense());
				
		_entity->setLogicalPosition(_auxPos); 
		

		//_diffHeight -= _negativeYVelocity * msecs * 0.001f;	//gravedad (no acelerada) simulada
		if (_diffDegrees != 0 || _diffHeight != 0)
			_physicalActor->move(_diffDegrees, _diffHeight);

		
		//Ponemos el movimiento a cero		
		_diffDegrees = 0;
		_diffHeight = 0; 
	}


	/**************
		IOBSERVER
	***************/

	//Se invoca cuando se produce una colisi�n entre una entidad f�sica y un trigger.
	void CPhysicalCharacter::onCollision(IObserver* other) {
		CPhysics::onCollision(other);
		LOG(_entity->getName() << ": \"Auch! Me he chocado!\"");
		
		
		//_entity->emitMessage(m,this);
	}

	void  CPhysicalCharacter::onTrigger (Physics::IObserver* other, bool enter) 
	{
		CPhysics::onTrigger(other, enter);

		#if DEBUG
			if(enter)
				LOG(_entity->getName() << ": \"Hora estoy dentro de " << static_cast<CPhysics*>(other)->getEntity()->getName() << "\"")
			else
				LOG(_entity->getName() << ": \"Hora estoy fuera  de " << static_cast<CPhysics*>(other)->getEntity()->getName() << "\"")

		#endif
	}

} // Logic


