/**
@file Camera.cpp

Contiene la implementaci�n del componente que controla la c�mara gr�fica
de una escena.
 
@see Logic::CCamera
@see Logic::IComponent

@author David Llans�
@date Agosto, 2010
*/
#pragma warning (disable : 4244 ) 
#include "Camera.h"

#include "Logic/Server.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Graphics/Scene.h"
#include "Graphics/Camera.h"
#include "Logic/Entity/LogicalPosition.h"
#include "GUI/InputManager.h"

#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageBoolFloat.h"


#include "Application/BaseApplication.h"

/*para tener un acceso directo al gamestatus*/
#include "Logic/GameStatus.h"
#include "Logic/RingInfo.h"
#include "Logic/BaseInfo.h"
#include "Logic/PlayerInfo.h"

namespace Logic 
{
	IMP_FACTORY(CCamera);
	
	//---------------------------------------------------------

	bool CCamera::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		_reloj=Application::CBaseApplication::getSingletonPtr()->getClock();
		_graphicsCamera = _entity->getMap()->getGraphicScene()->getCamera();
		if(!_graphicsCamera)
			return false;

		if(entityInfo->hasAttribute("distance"))
			_distance = entityInfo->getFloatAttribute("distance");

		if(entityInfo->hasAttribute("height"))
			_height = entityInfo->getFloatAttribute("height");

		if(entityInfo->hasAttribute("targetDistance"))
			_targetDistance = entityInfo->getFloatAttribute("targetDistance");

		if(entityInfo->hasAttribute("targetHeight"))
			_targetHeight = entityInfo->getFloatAttribute("targetHeight");

		if(entityInfo->hasAttribute("cameraSpeed"))
			_cameraSpeed = entityInfo->getFloatAttribute("cameraSpeed");
		
		if(entityInfo->hasAttribute("trembleSpeed")) {
			_trembleSpeed = entityInfo->getFloatAttribute("trembleSpeed");
		}

		if(entityInfo->hasAttribute("trembleOffset")) {
			_trembleOffset = entityInfo->getFloatAttribute("trembleOffset");
		}
		//_trembleOffset+=1;
		//_trembleSpeed*=1.3;
		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CCamera::activate()
	{		
		_target = CServer::getSingletonPtr()->getPlayer(); 
		
 
		_currentPos = (2000, 600, 2000);// Esto lo ponemos as� de momento para que salga desde arriba la camara

		_graphicsCamera->setCameraPosition(_currentPos);

		
		return true;
	} // activate
	
	//---------------------------------------------------------

	void CCamera::deactivate()
	{
		_target = 0;
	} // deactivate
	
	//---------------------------------------------------------

	void CCamera::changeHeight(Message::TActionType) //go_up activa y go_down desactiva.
	{
		
	}

	//---------------------------------------------------------

	bool CCamera::accept(const CMessage *message)
	{
		return message->getType() == Message::CAMERA;
	}

	//---------------------------------------------------------

	 void CCamera::process(CMessage *message)
	{
		switch(message->getType())
		{
			case Message::CAMERA:
				if (message->getAction()!=Message::GO_DOWN && message->getAction()!=Message::GO_UP)
				{
				CMessageBoolFloat *maux = static_cast<CMessageBoolFloat*>(message);
				if (maux->getBool())
					_targetDistance+=maux->getFloat();
				else
					_targetDistance-=maux->getFloat();
				break;
				}
				else
					//changeHeight(message->getAction());
					message->getAction()==Message::GO_DOWN?_tremble=false:_tremble=true;
		}
	 }
	//---------------------------------------------------------

	void CCamera::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);
	
		if(!_target || (msecs > 1000))
			return;
		

	
		Vector3 position = _target->getPosition();
		Vector3 direction = _distance * position.normalisedCopy(); ;
		direction.y = _height;

		Vector3 finalCameraPosition = position + direction;
		
		//inercia de la camara
		Vector3 deltaMove = ( finalCameraPosition 
			- _currentPos ) * _cameraSpeed * msecs * 0.001;


		_currentPos += deltaMove;


		_graphicsCamera->setCameraPosition(_currentPos);
		
		
		position = (_targetDistance +  _target->getLogicalPosition()->getRadio()) * _currentPos.normalisedCopy() ;
		position.y = CServer::getSingletonPtr()->getRingPosition(_target->getLogicalPosition()->getRing()).y;
		
		_graphicsCamera->setTargetCameraPosition(position);
		//a partir de aqu� es principalmente para el efecto de que tiemble la camara.
		 short riesgo=0;
		riesgo=CGameStatus::getSingletonPtr()->getBase(_entity->getLogicalPosition()->getBase())->getNumAltarsLeft();
		riesgo=3-riesgo;
		if (riesgo<0)riesgo=0;

	//	if (riesgo>0) _contador++;
		//if (medida>=3) _entity->getMap()->getGraphicScene()->activateCompositor("BW"); AQUI ACTIVAMOS COMPOSITOR DE QUE TU BASE EST� EN PELIGRO INMINENTE
		if (riesgo>0 && _calm)
		{
			_tremble=false;
			_calm=false;
			timeArrived();
			//_reloj->addTimeObserver(_entity->getEntityID(),this,3000);
		}
		if (riesgo==0) _calm=true;
		short factor=riesgo*_tremble;
		Vector3 offset=Vector3(cameraTarget.x+factor*estimateOffset(cameraTarget.x,msecs),cameraTarget.y+factor*estimateOffset(cameraTarget.y,msecs),cameraTarget.z+factor*estimateOffset(cameraTarget.z,msecs));
		_graphicsCamera->setTargetCameraPosition(offset);
		
	} // tick

	float CCamera::estimateOffset(float height, unsigned int msecs) {
		_currentTremblePos += _trembleSpeed * msecs;
		if(_currentTremblePos > 6.283) _currentTremblePos = 0;
		return (sin(_currentTremblePos) * _trembleOffset);
	}
	void CCamera::timeArrived()
	{
	    _tremble=!_tremble;
		//if (_tremble)
		_reloj->addTimeObserver(_entity->getEntityID()+_tremble,this,1000+!_tremble*1000);
		std::cout<<_tremble<<std::endl;
	}
} // namespace Logic

