/**
@file Graphics.cpp

Contiene la implementaci�n del componente que controla la representaci�n
gr�fica de la entidad.
 
@see Logic::CGraphics
@see Logic::IComponent

@author David Llans�
@date Agosto, 2010
*/

#include "Graphics.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Graphics/Scene.h"
#include "Graphics/Entity.h"
#include "Logic/Maps/EntityFactory.h"

#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageTF.h"
#include "Logic/Entity/Messages/MessageString.h"
#include "Logic/Entity/Messages/MessageUIntString.h"

namespace Logic 
{
	IMP_FACTORY(CGraphics);
	
	//---------------------------------------------------------

	CGraphics::~CGraphics() 
	{
		if(_graphicalEntity){			
			_scene->remove(_graphicalEntity);
			delete _graphicalEntity;
		}

	} // ~CGraphics
	
	//---------------------------------------------------------

	bool CGraphics::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		_scene = map->getGraphicScene();
			assert(_scene && "Escena gr�fica es NULL");

		assert(entityInfo->hasAttribute("model"));
			_model = entityInfo->getStringAttribute("model");

		_graphicalEntity = createGraphicalEntity(entityInfo);
			if(!_graphicalEntity)
				return false;
		
		Vector3 scale = Vector3::UNIT_SCALE;

	// HACK FRS Esto lo suyo es que el modelo ya lo traiga , no?
	// o meter la escala como vector en el map
		if(_entity->getType() == "World"
			&& _entity->getRing() == LogicalPosition::CENTRAL_RING)
			scale = Vector3(1.3,1.0,1.3);
	//

		else if(entityInfo->hasAttribute("scaleFactor") )
			scale *=  entityInfo->getFloatAttribute("scaleFactor");

		_graphicalEntity->setTransform(_entity->getTransform());
		_graphicalEntity->setScale(scale);	
		

		// ATTACHs
	// TODO  FRS Esto estar�a guapo tron extraerlo directamente como lista de pares desde el map.txt
		if(entityInfo->hasAttribute("modelWeapon"))
			_graphicalEntity->attach( Graphics::TAttachPoint::HAND, 
				entityInfo->getStringAttribute("modelWeapon") );

		if(entityInfo->hasAttribute("modelHelmet"))
			_graphicalEntity->attach( Graphics::TAttachPoint::HEAD, 
				entityInfo->getStringAttribute("modelHelmet") );	
	//
		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CGraphics::accept(const CMessage *message)
	{
		return	 message->getType() == Message::SET_TRANSFORM ||
				 message->getType() == Message::SET_MATERIAL ||
				 message->getType() == Message::SET_SUBENTITY_MATERIAL ||
				 message->getType() == Message::ATTACH;

	} // accept
	
	//---------------------------------------------------------

	void CGraphics::process(CMessage *message)
	{
		switch( message->getType() ) {
		
		case Message::SET_TRANSFORM: {			
			CMessageTF *rxMsg = static_cast<CMessageTF*>(message);
			_graphicalEntity->setTransform(rxMsg->getTransform());			
		}	break;
	
		case Message::SET_MATERIAL: {			
			CMessageString *rxMsg = static_cast<CMessageString*>(message);
			_graphicalEntity->setMaterial(rxMsg->getString());			
		}	break;

		case Message::SET_SUBENTITY_MATERIAL:{
			CMessageUIntString *rxMsg = static_cast<CMessageUIntString*>(message);
			_graphicalEntity->setSubEntityMaterial(rxMsg->getString(), rxMsg->getUInt());
		}	break;

		case Message::ATTACH: {
			CMessageString *rxMsg = static_cast<CMessageString*>(message);
			switch( message->getAction() ) {
				case Message::ATTACH_TO_HEAD:	
					_graphicalEntity->attach( Graphics::TAttachPoint::HEAD, rxMsg->getString() );	
					break;
				case Message::ATTACH_TO_HAND:	
					_graphicalEntity->attach( Graphics::TAttachPoint::HAND,	rxMsg->getString() );	
					break;
				case Message::DETACH_FROM_HEAD:	
					_graphicalEntity->detach( Graphics::TAttachPoint::HEAD );	
					break;
				case Message::DETACH_FROM_HAND:	
					_graphicalEntity->detach( Graphics::TAttachPoint::HAND );	
					break;
			}
		} break;

		} // switch

	} // process

	//---------------------------------------------------------

	Graphics::CEntity* CGraphics::createGraphicalEntity(const Map::CEntity *entityInfo)
	{		
		assert( _scene && "LOGIC::GRAPHICS>> No existe escena gr�fica!");
		assert( _model.length() > 0  && "LOGIC::GRAPHICS>> No existe modelo!");	
		
		bool isStatic = false;
			if(entityInfo->hasAttribute("static"))
				isStatic = entityInfo->getBoolAttribute("static");
	
		Graphics::CEntity* graphicalEntity = new Graphics::CEntity(_entity->getName(),_model, isStatic);
			if( _scene->add(graphicalEntity) )		
				return graphicalEntity;
			else
				return 0;
		

	} // createGraphicalEntity
	
	

} // namespace Logic

