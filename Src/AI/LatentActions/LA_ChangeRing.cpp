
#include "../../Logic/Entity/Components/Attack.h"
#include "../../Logic/Entity/Components/AvatarController.h"
#include "../../Logic/Entity/Components/Jump.h"
#include "Application/BaseApplication.h"

#include "../StateMachines/StateMachine.h"
#include "LA_ChangeRing.h"
#include "Logic/Entity/Messages/MessageFloat.h"
#include "Logic/Entity/Messages/MessageBoolString.h"
#include "Logic/Entity/Messages/MessageString.h"
#include "Logic/Entity/Messages/MessageUIntString.h"
#include "Logic/Entity/Messages/MessageUInt.h"
#include "../../Logic/Entity/Components/BaseTraveler.h"
#include "Logic/Entity/Messages/MessageChar.h"
#include "Graphics/AnimatedEntity.h"

#include "../../Logic/Server.h"

namespace AI
{
////////////////////////////////
//	Implementaci�n de CLA_ChangeRing
////////////////////////////////

	/**
	M�todo invocado al principio de la ejecuci�n de la acci�n,
	para que se realicen las tareas que son �nicamente necesarias
	al principio (y no durante toda la vida de la acci�n).
	<p>
	Al comenzar se obtiene el tiempo actual y se calcula el 
	tiempo en el que terminar� la acci�n mediante el atributo _time

	@return Estado de la funci�n; si se indica que la
	acci�n a terminado (LatentAction::Completed), se invocar�
	al OnStop().
	*/
	CLatentAction::LAStatus CLA_ChangeRing::OnStart()
	{				
		_justOneTime=false;
		_changeDone=false;
		_initialJumpSpeed=(_action==Message::GO_DOWN?0.07f:0.18f); //velocidad de salto inicial en caso de dejarse caer o saltar arriba
		_initialRing=_entity->getLogicalPosition()->getRing();
		_jumpSpeed=_initialJumpSpeed;
		sleepComponents();		//Desactivaci�n de componentes
		std::cout<<"AI::StateMachine::ChangeRing"<<std::endl;
		CMessageBoolString *message = new CMessageBoolString();
		message->setType(Message::SET_ANIMATION);
		message->setAction(Message::JUMP);				
		message->setString(Graphics::AnimNames::JUMP);
		message->setBool(false);
		_entity->emitMessage(message);		
		turn();
		return SUSPENDED;
	}

	/**
	M�todo invocado al final de la ejecuci�n de la acci�n,
	para que se realicen las tareas que son �nicamente necesarias
	al final (y no durante toda la vida de la acci�n).

	En la mayor�a de los casos este m�todo no hace nada.
	*/
	void CLA_ChangeRing::OnStop()
	{
		std::cout<<"AI::StateMachine::ChangeRing-OnStop"<<std::endl;
		awakeComponents();
	}

	/**
	M�todo invocado c�clicamente para que se contin�e con la
	ejecuci�n de la acci�n.
	<p>
	En cada paso de ejecuci�n se obtiene el tiempo actual 
	y se comprueba si se debe acabar la acci�n.

	@return Estado de la acci�n tras la ejecuci�n del m�todo;
	permite indicar si la acci�n ha terminado o se ha suspendido.
	*/
	CLatentAction::LAStatus CLA_ChangeRing::OnRun() 
	{
		if (this->getStatus()!=SUCCESS && this->getStatus()!=FAIL)
			return RUNNING;
		else 
			return this->getStatus();
	}

	/**
	M�todo invocado cuando la acci�n ha sido cancelada (el comportamiento
	al que pertenece se ha abortado por cualquier raz�n).

	La tarea puede en este momento realizar las acciones quead
	considere oportunas para "salir de forma ordenada".

	@note <b>Importante:</b> el Abort <em>no</em> provoca la ejecuci�n
	de OnStop().
	*/
	CLatentAction::LAStatus CLA_ChangeRing::OnAbort() 
	{
		// Cuando se aborta se queda en estado terminado con fallo		
		//REVISAR
	/*	if (_entity->getComponent<CRingTraveler>()!=NULL)
		{
			_entity->getComponent<CRingTraveler>()->resetChangingRing();
			return FAIL;
		}
		if (_entity->getComponent<CBaseTraveler>()!=NULL)
			_entity->getComponent<CBaseTraveler>()->resetChangingRing();
		*/
		return FAIL;
	}
	/**
	Devuelve true si a la acci�n le interesa el tipo de mensaje
	enviado como par�metro.
	<p>
	Esta acci�n no acepta mensajes de ning�n tipo.

	@param msg Mensaje que ha recibido la entidad.
	@return true Si la acci�n est� en principio interesada
	por ese mensaje.
	*/
	bool CLA_ChangeRing::accept(const CMessage *message){return false;}
	/**
	Procesa el mensaje recibido. El m�todo es invocado durante la
	ejecuci�n de la acci�n cuando se recibe el mensaje.

	@param msg Mensaje recibido.
	*/
	void CLA_ChangeRing::process(CMessage *message)	{}
		
	void CLA_ChangeRing::jump() 
	{
		if (!_jumping)
		{
			_jumping = true;
			_justJumped = true;
		}
	} // jump
	
	void CLA_ChangeRing::turn() 
	{
		if (!_turning)
			_turning = true;			
	} // jump

	//PeP
	void CLA_ChangeRing::tick(unsigned int msecs) 
	{		
		float _turningSpeed= 0.005f/7; 
		if (_turning)
		{	
			if (_entity->getLogicalPosition()->getSense() == Logic::LogicalPosition::RIGHT || _entity->getLogicalPosition()->getSense() == Logic::LogicalPosition::LEFT)
				_targetSense = Logic::LogicalPosition::LOOKING_CENTER;
		
			if (_initialRing==Logic::LogicalPosition::CENTRAL_RING)
			{
				if (_entity->getLogicalPosition()->getSense() == Logic::LogicalPosition::RIGHT)
				{
					float tickRotation = Math::PI * _turningSpeed * msecs;
					_entity->yaw(tickRotation);
					_acumRotation += tickRotation;
					_mySense=Logic::LogicalPosition::RIGHT;
					if (_acumRotation >= Math::PI/2)
					{
						_entity->yaw(-(_acumRotation - Math::PI/2));
						_entity->getLogicalPosition()->setSense(Logic::LogicalPosition::LOOKING_CENTER);
						_targetSense = Logic::LogicalPosition::UNDEFINED;
						_acumRotation = 0;
					
					}
				}
				else if (_entity->getLogicalPosition()->getSense() == Logic::LogicalPosition::LEFT)
				{
					float tickRotation = Math::PI * _turningSpeed * msecs; //0.005hack, a susituir por turnSpeed dirigida por datos
					_entity->yaw(-tickRotation);
					_acumRotation += tickRotation;
					_mySense=Logic::LogicalPosition::LEFT;
					if (_acumRotation >= Math::PI/2)
					{
						_entity->yaw(_acumRotation - Math::PI/2);
						_entity->getLogicalPosition()->setSense(Logic::LogicalPosition::LOOKING_CENTER);
						_targetSense = Logic::LogicalPosition::UNDEFINED;
						_acumRotation = 0;
					}
				}
			
			else if (_entity->getLogicalPosition()->getSense() == Logic::LogicalPosition::LOOKING_CENTER)
			{
				if (_targetSense == Logic::LogicalPosition::RIGHT)
				{   
					
					float tickRotation = Math::PI * _turningSpeed* msecs; //0.005hack, a susituir por turnSpeed dirigida por datos
					_entity->yaw(-tickRotation);
					_acumRotation += tickRotation;
					if (_acumRotation >= Math::PI/2)
					{
						_entity->yaw(_acumRotation - Math::PI/2);
						_entity->getLogicalPosition()->setSense(Logic::LogicalPosition::RIGHT);
						_targetSense = Logic::LogicalPosition::UNDEFINED;
						_acumRotation = 0;						
						if (_changeDone)
							finish(true);
					}
				}
				else if (_targetSense == Logic::LogicalPosition::LEFT)
				{
					float tickRotation = Math::PI *_turningSpeed* msecs;
					_entity->yaw(tickRotation);
					_acumRotation += tickRotation;
					if (_acumRotation >= Math::PI/2)
					{
						_entity->yaw(-(_acumRotation - Math::PI/2));
						_entity->getLogicalPosition()->setSense(Logic::LogicalPosition::LEFT);
						_targetSense = Logic::LogicalPosition::UNDEFINED;
						_acumRotation = 0;
						if (_changeDone)
							finish(true);
					}
				}
				else 
				{
					_turning=false;
					jump();
				}
			}
		}
		else
		{
				if (_entity->getLogicalPosition()->getSense() == Logic::LogicalPosition::RIGHT)
				{
					float tickRotation = Math::PI * _turningSpeed * msecs;
					_entity->yaw(-tickRotation);
					_acumRotation += tickRotation;
					_mySense=Logic::LogicalPosition::RIGHT;
					if (_acumRotation >= Math::PI/2)
					{
						_entity->yaw(+(_acumRotation - Math::PI/2));
						_entity->getLogicalPosition()->setSense(Logic::LogicalPosition::LOOKING_CENTER);
						_targetSense = Logic::LogicalPosition::UNDEFINED;
						_acumRotation = 0;
					
					}
				}
				else if (_entity->getLogicalPosition()->getSense() == Logic::LogicalPosition::LEFT)
				{
					float tickRotation = Math::PI * _turningSpeed * msecs; //0.005hack, a susituir por turnSpeed dirigida por datos
					_entity->yaw(+tickRotation);
					_acumRotation += tickRotation;
					_mySense=Logic::LogicalPosition::LEFT;
					if (_acumRotation >= Math::PI/2)
					{
						_entity->yaw(-(_acumRotation - Math::PI/2));
						_entity->getLogicalPosition()->setSense(Logic::LogicalPosition::LOOKING_CENTER);
						_targetSense = Logic::LogicalPosition::UNDEFINED;
						_acumRotation = 0;
					}
				}
			
			else if (_entity->getLogicalPosition()->getSense() == Logic::LogicalPosition::LOOKING_CENTER)
			{
				if (_targetSense == Logic::LogicalPosition::RIGHT)
				{   					
					float tickRotation = Math::PI * _turningSpeed* msecs; //0.005hack, a susituir por turnSpeed dirigida por datos
					_entity->yaw(+tickRotation);
					_acumRotation += tickRotation;
					if (_acumRotation >= Math::PI/2)
					{
						_entity->yaw(-(_acumRotation - Math::PI/2));
						_entity->getLogicalPosition()->setSense(Logic::LogicalPosition::RIGHT);
						_targetSense = Logic::LogicalPosition::UNDEFINED;
						_acumRotation = 0;
						if (_changeDone)
							finish(true);
					}
				}
				else if (_targetSense == Logic::LogicalPosition::LEFT)
				{
					float tickRotation = Math::PI *_turningSpeed* msecs;
					_entity->yaw(-tickRotation);
					_acumRotation += tickRotation;
					if (_acumRotation >= Math::PI/2)
					{
						_entity->yaw(+(_acumRotation - Math::PI/2));
						_entity->getLogicalPosition()->setSense(Logic::LogicalPosition::LEFT);
						_targetSense = Logic::LogicalPosition::UNDEFINED;
						_acumRotation = 0;
						if (_changeDone)
							finish(true);
					}
				}
				else 
				{
					_turning=false;
					jump();
				}
			}
		}//else
		}//turning

		if (_jumping)
		{
			unsigned int separationBeteewenRings=50;
			float myGlobalHeight=_entity->getLogicalPosition()->getHeight()+CServer::getSingletonPtr()->getRingPosition(_entity->getLogicalPosition()->getRing()).y;

			if (!_changeDone && 
				((_action==Message::GO_UP && myGlobalHeight > CServer::getSingletonPtr()->getRingPosition(_entity->getLogicalPosition()->getRing()).y+50) ||
				(_action==Message::GO_DOWN && myGlobalHeight > CServer::getSingletonPtr()->getRingPosition(_entity->getLogicalPosition()->getRing()).y+1)) )
			{
				_changeDone=true;
				_justOneTime=true;
				CMessageChar *m0 = new CMessageChar();	
				m0->setType(Message::AVATAR_MOVE);
				m0->setAction(Message::CHANGE_RING);		
				m0->setChar(_action==Message::GO_DOWN?-1:1);
				_entity->emitMessage(m0);
			}
			float _gravedad=0.00025f;
			_jumpSpeed -= _gravedad * msecs;  //gravedad 0.0003f
			float tickHeight = _jumpSpeed * msecs;

			//si estamos en trayectoria descendente activamos salida del salto
			if (_jumpSpeed < 0)
				_justJumped = false;
			
			if (_entity->getLogicalPosition()->getHeight() == 0 && !_justJumped) 
			{
				_jumping=false;
				_targetSense=_mySense;
				turn();
				_jumpSpeed=_initialJumpSpeed;
				if (_entity->getComponent<CAvatarController>()->isWalkingRight())
					_entity->getComponent<CAvatarController>()->walkRight();
				else if (_entity->getComponent<CAvatarController>()->isWalkingLeft())
					_entity->getComponent<CAvatarController>()->walkLeft();
			}
			else
	     	{
				Logic::CMessageFloat *m = new Logic::CMessageFloat();
				m->setType(Logic::Message::AVATAR_MOVE);
				m->setAction(Logic::Message::JUMP);
					
				//float tickResult=tickHeight-50*(_action==Message::GO_DOWN?1:0)-50*(_action==Message::GO_DOWN?-1:1)*_justOneTime;
				float tickResult;
				if (_action==Message::GO_UP)
					tickResult=tickHeight-50*_justOneTime;
				if (_action==Message::GO_DOWN)
					tickResult=tickHeight+50*_justOneTime;
				m->setFloat(tickResult);
				if (_justOneTime) 
					_justOneTime=false;
				_entity->emitMessage(m);			
			}
		}
		CLatentAction::tick();//no olvideis llamar al tick de CLatentAction
	}

	void CLA_ChangeRing::sleepComponents()
	{
		if (_entity->getComponent<CAvatarController>()!=NULL)
			_entity->getComponent<CAvatarController>()->sleep();	
		if (_entity->getComponent<CJump>()!=NULL)
			_entity->getComponent<CJump>()->sleep();
	}

	void CLA_ChangeRing::awakeComponents()
	{			
		if (_entity->getComponent<CAvatarController>()!=NULL)
			_entity->getComponent<CAvatarController>()->awake();	
		if (_entity->getComponent<CJump>()!=NULL)
			_entity->getComponent<CJump>()->awake();	
	}	
} //namespace LOGIC