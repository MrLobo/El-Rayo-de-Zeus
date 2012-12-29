/**
@file AngularMovement.h

Contiene la declaraci�n del componente que controla el movimiento 
angular de entidades.

@see Logic::CAngularMovement
@see Logic::IComponent

@author Jos� Luis L�pez
@date Diciembre, 2012
*/

#include "AngularMovement.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
 

//declaraci�n de la clase
namespace Logic 
{
	IMP_FACTORY(CAngularMovement);

	bool CAngularMovement::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){

		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
		
		if(entityInfo->hasAttribute("angularSpeed"))
			_angularSpeed = entityInfo->getFloatAttribute("angularSpeed");

		return true;
		}

	bool CAngularMovement::activate()
	{
		 _sentidoIzquierda=true;
		 //_actualRadius=
		 //if (_entity->getType().compare("Player")==0)
		 	//		std::cout<<"PlayerDegree: "<<_entity->getDegree()<<std::endl;
			return true;
	}
		

	void CAngularMovement::deactivate(){}

	
	bool CAngularMovement::accept(const TMessage &message)
	{
		return message._type == Message::CONTROL; //De momento, luego tendr� que aceptar de otras entidades NPC
	}

		
	 void CAngularMovement::process(const TMessage &message)
		 {
		switch(message._type)
		{
		case Message::CONTROL:
			if(!message._string.compare("goUp"))
				goUp();
			else if(!message._string.compare("goDown"))
				goDown();
			else if(!message._string.compare("walkLeft"))
				walkLeft();
			else if(!message._string.compare("walkRight"))
				walkRight();
			else if(!message._string.compare("walkStop"))
				stopMovement();
			else if(!message._string.compare("walkBack"))
				 {	
					_sentidoColision=message._bool;
					walkBack();
				 }
			else if(!message._string.compare("turn"))
				turn(message._float);
		}

		 }


		void CAngularMovement::walkRight()
		{
			_walkingRight = true;
			// Cambiamos la animaci�n
			TMessage message;
			message._type = Message::SET_ANIMATION;
			message._string = "Run";
			message._bool = true;
			_entity->emitMessage(message,this);

		}
		
		void CAngularMovement::walkLeft()
		{
			_walkingLeft = true;
			// Cambiamos la animaci�n
			TMessage message;
			message._type = Message::SET_ANIMATION;
			message._string = "Run";
			message._bool = true;
			_entity->emitMessage(message,this);

		}
	
		void CAngularMovement::walkBack()
		{			
			_walkBack=true; //para retroceder en las colisiones   
			// Cambiamos la animaci�n
			TMessage message;
			message._type = Message::SET_ANIMATION;
			message._string = "Idle";
			message._bool = true;
			_entity->emitMessage(message,this);
		}
		
		
		void CAngularMovement::goDown()
		{}
		
		void CAngularMovement::goUp()
		{}
		
		void CAngularMovement::stopMovement()
		{
		_walkingLeft = _walkingRight = false;

		// Cambiamos la animaci�n si no seguimos desplaz�ndonos
		// lateralmente
		
			TMessage message;
			message._type = Message::SET_ANIMATION;
			message._string = "Idle";
			message._bool = true;
			_entity->emitMessage(message,this);
		}
		

		void CAngularMovement::tick(unsigned int msecs)
	{
			IComponent::tick(msecs);

		// Si nos estamos desplazando calculamos la pr�xima posici�n
		// Calculamos si hay vectores de direcci�n de avance y strafe,
		// hayamos la direcci�n de la suma y escalamos seg�n la
		// velocidad y el tiempo transcurrido.
	

			//if (!_entity->isPlayer())


	     Vector3 direction(Vector3::ZERO);
		if(_walkingLeft || _walkingRight)
		{
			if(_walkingLeft || _walkingRight)
			{
				direction = Math::getDirection(_entity->getYaw() + Math::PI/2);
				//Matrix4 orientacion = _entity->getOrientation();
				//Math::yaw(Math::fromDegreesToRadians(_actualDegree),orientacion);
				if(_walkingRight){
					if(_sentidoIzquierda==true)
					{
						_sentidoIzquierda=false;
						_entity->yaw(Math::PI);						
					}
						//_entity->setYaw(0);
						//_actualDegree+=-_angularSpeed;
					if (!_walkBack)
					{
						_entity->setDegree(_entity->getDegree()-_angularSpeed); 
						_entity->yaw(Math::fromDegreesToRadians(_angularSpeed));
					}
				}
				else
				{
					if(_sentidoIzquierda==false)
					{
						_entity->yaw(Math::PI);					
						_sentidoIzquierda=true;
					}
					//_actualDegree+=_angularSpeed;
					if (!_walkBack)
					{
						_entity->setDegree(_entity->getDegree()+_angularSpeed);
						_entity->yaw(Math::fromDegreesToRadians(-_angularSpeed));
					}			
					
				}
				//turn(-0.02f*_angularSpeed);
				//turn(Math::PI/2);			
				//_entity->setOrientation(
				if (_walkBack)
					{
						if (_sentidoColision)
						{
							_entity->setDegree(_entity->getDegree()+(_angularSpeed)); 
							_entity->yaw(Math::fromDegreesToRadians(-(_angularSpeed)));
						}
						else
						{
							_entity->setDegree(_entity->getDegree()-(_angularSpeed)); 
							_entity->yaw(Math::fromDegreesToRadians((_angularSpeed)));
						}							
				
						stopMovement();   
						/*Logic::TMessage m;
						m._type = Logic::Message::CONTROL;						
						m._string = "walkStop";
						_entity->emitMessage(m);*/
						_sentidoColision=false;
						_walkBack=false;

					}
				if(_walkingLeft)
					direction *= -1;
			}

			direction.normalise();
			//_entity->setDegree(_actualDegree);
			//_entity->setRadio(_actualRadius);

			//std::cout<<"PlayerDegree: "<<_entity->getDegree()<<std::endl;
			Vector3 newPosition = Math::fromPolarToCartesian(_entity->getDegree(),_entity->getRadio());
			
			//newPosition.y=0;
			_entity->setPosition(newPosition);
		}
		
		}

		void CAngularMovement::turn(float amount)
		{
				_entity->yaw(amount);		
		}

} // namespace Logic