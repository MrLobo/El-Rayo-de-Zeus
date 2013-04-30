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
#include "Logic/Entity/Messages/MessageBoolFloatString.h"

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
	
	void CAnimatedGraphics::detachFromMap()
	{
		_scene->remove(_graphicalEntity);
		_scene=NULL;
	}

	//---------------------------------------------------------

	void CAnimatedGraphics::attachToMap(CMap* map)
	{
		_scene = map->getGraphicScene();
		_scene->add(_graphicalEntity);
	}

	//---------------------------------------------------------

	bool CAnimatedGraphics::accept(const CMessage *message)
	{
		return CGraphics::accept(message) ||
			   message->getType() == Message::SET_ANIMATION ||
			   message->getType() == Message::STOP_ANIMATION ||
			   message->getType() == Message::REWIND_ANIMATION ||
			   message->getType() == Message::SET_ANIMATION_WITH_TIME
			   ;

	} // accept
	
	//---------------------------------------------------------

	void CAnimatedGraphics::process(CMessage *message)
	{
		CGraphics::process(message);
		
		
		switch(message->getType())
		{
			case Message::SET_ANIMATION: {

				CMessageBoolString *rxMsg = static_cast<CMessageBoolString*>(message);
				// Paramos todas las animaciones antes de poner una nueva.
				// Un control m�s sofisticado deber�a permitir interpolaci�n
				// de animaciones. Galeon no lo plantea.
				_graphicalEntity->stopAllAnimations();
				_graphicalEntity->setAnimation(rxMsg ->getString(), 0, rxMsg ->getBool());
				LOG("SET_ANIMATION: " << rxMsg->getString());
			}	break;

			case Message::STOP_ANIMATION:	{
				CMessageString *rxMsg = static_cast<CMessageString*>(message);
				_graphicalEntity->stopAnimation(rxMsg ->getString());
				LOG("STOP_ANIMATION: " << rxMsg->getString());
			}	break;

			case Message::REWIND_ANIMATION:	{
				CMessageBoolString *rxMsg = static_cast<CMessageBoolString*>(message);
				_graphicalEntity->rewind(rxMsg ->getString(), rxMsg->getBool() );
				LOG("REWIND_ANIMATION: " << rxMsg->getString());
			}	break;

			case Message::SET_ANIMATION_WITH_TIME:
			{
				CMessageBoolFloatString *rxMsg = static_cast<CMessageBoolFloatString*>(message);
				// de animaciones. Galeon no lo plantea.
				_graphicalEntity->stopAllAnimations();
				_graphicalEntity->setAnimation(rxMsg ->getString(), rxMsg ->getFloat(), rxMsg ->getBool());
				LOG("SET_ANIMATION_WITH_TIME: " << rxMsg->getString());
			} break;
		}

	} // process

	//---------------------------------------------------------

	Graphics::CEntity* CAnimatedGraphics::createGraphicalEntity(const Map::CEntity *entityInfo)
	{
		assert(_scene && "LOGIC::ANIMATED_GRAPHICS>> No existe escena gr�fica!");
		assert( _model.length() > 0  &&  "LOGIC::ANIMATED_GRAPHICS>> No existe modelo!");	

		std::stringstream ssAux; // FRS Importante a�adir ID para evitar entidades gr�ficas con = nombre
			ssAux << _entity->getName() << _entity->getEntityID();
			std::string	name = ssAux.str();

		_graphicalEntity = new Graphics::CAnimatedEntity(name,_model);
			if(!_scene->add(_graphicalEntity) )
				return 0;

		// DEFAULT ANIMATION
		if(entityInfo->hasAttribute("defaultAnimation")) { 
			_defaultAnimation = entityInfo->getStringAttribute("defaultAnimation");
			_graphicalEntity->setAnimation(_defaultAnimation,0,true);
			_graphicalEntity->setObserver(this);
		}

		return _graphicalEntity;
	} // createGraphicsEntity

	
	//---------------------------------------------------------
	
	void CAnimatedGraphics::animationFinished(const std::string &animation)
	{
		if (animation != Graphics::AnimNames::DEATH)
		{
			// [���] Ejemplo de gesti�n de eventos de animaci�n -> En este caso se avisa de que animaci�n ha finalizado (necesario en CDeath)
			CMessageString *txMsg = new CMessageString();
				txMsg->setType(Message::ANIMATION_FINISHED);
				txMsg->setString(animation);
				_entity->emitMessage(txMsg);

			// Si acaba una animaci�n y tenemos una por defecto la ponemos
			_graphicalEntity->stopAllAnimations();
			_graphicalEntity->setAnimation(_defaultAnimation,0,true);
		}

	}

		
	void CAnimatedGraphics::animationMomentReached(const std::string &animation)
	{
		// [���] Ejemplo de gesti�n de eventos de animaci�n -> En este caso se avisa de que animaci�n ha finalizado (necesario en CDeath)
		CMessageString *msg = new CMessageString();
		msg->setType(Message::ANIMATION_MOMENT);
		msg->setString(animation);
		_entity->emitMessage(msg);
	}

} // namespace Logic

