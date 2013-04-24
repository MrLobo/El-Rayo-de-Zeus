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

#include <math.h>
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
		//_target->setPosition(Math::fromPolarToCartesian(0,60)); //esto no est� bien aqu�, pero si no est� no calcula bien el vector direcci�n.
		//_target->fromLogicalToCartesian(0,60,_target->getBase(),_target->getRing()); //esto no est� bien aqu�, pero si no est� no calcula bien el vector direcci�n.
		//anula lo que haya en el maps.txt sobre la posici�n del prota
		_currentPos = Vector3(150,150, 150);// Esto lo ponemos as� de momento para que salga desde arriba la camara.
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
	
		if(!_target)
			return;

		// Actualizamos la posici�n de la c�mara.
		//este parrafo es para conseguir que el modelo mire en direcci�n perpendicular al vector centro camara
		Vector3 centro=Vector3::NEGATIVE_UNIT_Y; // TODO [���] por que negative y no ZERO?
		centro.y=_target->getPosition().y;
		//Vector3 centro=Vector3(0,-125-250,0);
			
		Vector3 vectorCentroProtaCamara =  -(centro-_target->getPosition());
		vectorCentroProtaCamara.normalise();
		//Vector3 actualDirection=Math::getDirection(_target->getOrientation());
		//Vector3 directionPerp= Vector3::UNIT_Y.crossProduct(vectorCentroProtaCamara); // [���] Si no son necesarios, mejor limpiar (ya se buscar�n en git)
		//Quaternion rotacionDestino=actualDirection.getRotationTo(directionPerp);
			
		Matrix4 orientacion = _target->getOrientation();
		//std::cout<<vectorCentroProtaCamara<<std::endl; // TODO Para estas cosillas es mejor usar la macro LOG (ver AnimatedGraphics)
		//std::cout<<Math::getDirection(orientacion)<<std::endl;
			
		//Math::yaw(Math::fromDegreesToRadians(-90),orientacion);
		Vector3 direction = vectorCentroProtaCamara; //-_distance * (Math::getDirection(orientacion))  ;
		
		direction.normalise();
		direction.y = _targetHeight;
			
		vectorCentroProtaCamara.normalise();
		vectorCentroProtaCamara.y=direction.y;
		//std::cout<<"vectorcentroprotacamara: "<<vectorCentroProtaCamara<<std::endl;
		
		//inercia de la camara
		Vector3 cameraTarget=CServer::getSingletonPtr()->getRingPositions(0/*_target->getLogicalPosition()->getBase()*/,_target->getLogicalPosition()->getRing());
		_currentPos =_currentPos+ (
			(Vector3(_target->getPosition().x*_targetDistance,cameraTarget.y+126,_target->getPosition().z*_targetDistance) +
			Vector3(0,_targetHeight*2,0)) -
			_currentPos) * 0.035f*0.05f*msecs;//0.05*
		
			_graphicsCamera->setCameraPosition(_currentPos);

			
		//_graphicsCamera->setCameraPosition( +4*position);
		//anillo x=0 y=-125 z=0
		// Y la posici�n hacia donde mira la c�mara.
		//direction = _targetDistance * Math::getDirection(orientacion);

		direction = _targetDistance * direction;
		direction.y = _targetHeight;
		cameraTarget.y+=126;

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

