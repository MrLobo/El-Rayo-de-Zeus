/**
@file AnimatedGraphics.cpp

Contiene la implementaci�n del componente que controla la representaci�n
gr�fica de una entidad est�tica.
 
@see Logic::CAnimatedGraphics
@see Logic::IComponent

@author David Llans�
@date Agosto, 2010
*/

#include "AnimatedGraphics.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Graphics/Scene.h"

#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageBoolString.h"
#include "Logic/Entity/Messages/MessageString.h"

#define DEBUG 0
#if DEBUG
#	include <iostream>
#	define LOG(msg) std::cout << "LOGIC::ANIMATED>> " << msg << std::endl;
#else
#	define LOG(msg)
#endif

namespace Logic 
{
	IMP_FACTORY(CAnimatedGraphics);
	
	//---------------------------------------------------------

	Graphics::CEntity* CAnimatedGraphics::createGraphicsEntity(const Map::CEntity *entityInfo)
	{
		_animatedGraphicsEntity = new Graphics::CAnimatedEntity(_entity->getName(),_model);
		if(!_scene->addEntity(_animatedGraphicsEntity))
			return 0;

		_animatedGraphicsEntity->setTransform(_entity->getTransform());
		
		if(entityInfo->hasAttribute("defaultAnimation"))
		{
			_defaultAnimation = entityInfo->getStringAttribute("defaultAnimation");
			_animatedGraphicsEntity->setAnimation(_defaultAnimation,true);
			_animatedGraphicsEntity->setObserver(this);
		}

		float scale = 1.0;
		if (entityInfo->hasAttribute("scale"))
			scale = entityInfo->getFloatAttribute("scale");
		_animatedGraphicsEntity->setScale(scale);

		return _animatedGraphicsEntity;

	} // createGraphicsEntity
	
	//---------------------------------------------------------

	bool CAnimatedGraphics::accept(const CMessage *message)
	{
		return CGraphics::accept(message) ||
			   message->getType() == Message::SET_ANIMATION ||
			   message->getType() == Message::STOP_ANIMATION;

	} // accept
	
	//---------------------------------------------------------

	void CAnimatedGraphics::process(CMessage *message)
	{
		CGraphics::process(message);
		CMessageBoolString *maux = static_cast<CMessageBoolString*>(message);
		CMessageString *maux2 = static_cast<CMessageString*>(message);
		switch(message->getType())
		{
			case Message::SET_ANIMATION:
				// Paramos todas las animaciones antes de poner una nueva.
				// Un control m�s sofisticado deber�a permitir interpolaci�n
				// de animaciones. Galeon no lo plantea.
				_animatedGraphicsEntity->stopAllAnimations();
				_animatedGraphicsEntity->setAnimation(maux->getString(),maux->getBool());
				LOG("SET_ANIMATION: " << maux->getString());
				break;
			case Message::STOP_ANIMATION:
				_animatedGraphicsEntity->stopAnimation(maux->getString());
				LOG("STOP_ANIMATION: " << maux2->getString());
				break;
		}

	} // process
	
	//---------------------------------------------------------
	
	void CAnimatedGraphics::animationFinished(const std::string &animation)
	{
		// [���] Ejemplo de gesti�n de eventos de animaci�n -> En este caso se avisa de que animaci�n ha finalizado (necesario en CDeath)
		CMessageString *msg = new CMessageString();
		msg->setType(Message::ANIMATION_FINISHED);
		msg->setString(animation);
		_entity->emitMessage(msg);		

		// Si acaba una animaci�n y tenemos una por defecto la ponemos
		_animatedGraphicsEntity->stopAllAnimations();
		_animatedGraphicsEntity->setAnimation(_defaultAnimation,true);
	}

} // namespace Logic

