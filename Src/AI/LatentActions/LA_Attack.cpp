#include "LA_Attack.h"

#include "../../Logic/Entity/Components/AnimatedGraphics.h"

#include "../../Logic/Entity/Components/Attack.h"
#include "../../Logic/Entity/Components/AvatarController.h"
#include "../../Logic/Entity/Components/Jump.h"
#include "../../Logic/Entity/Components/BaseTraveler.h"
#include "Application/BaseApplication.h"

#include "../StateMachines/StateMachine.h"
#include "Logic/Entity/Messages/MessageUShort.h"
#include "Logic/Entity/Messages/MessageBoolUShort.h"

namespace AI
{


//////////////////////////////
//	Implementaci�n de CLAIdle
//////////////////////////////

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
	CLatentAction::LAStatus CLA_Attack::OnStart()
	{
		
		//Desactivaci�n de componentes
		sleepComponents();
		std::cout<<"AI::INITIALCOMBATSTATE: "+_initialCombatState<<std::endl;		
		std::cout<<"AI::INITIAL_ACTION: "+(int)_action<<std::endl;
		_animationSetedByMe=false;
		_initialYaw=_entity->getYaw();
		_yawAmount=0;

		switch(_initialCombatState)
		{
		case 0:
		{
			CMessageBoolUShort *message = new CMessageBoolUShort();
			message->setType(Message::SET_ANIMATION);
			if (_action==Message::LIGHT_ATTACK)
				message->setUShort(Logic::ATTACK1);
			else
				message->setUShort(Logic::ATTACK2);				
			message->setAction(_action);
			message->setBool(false);
			_entity->emitMessage(message);
			break;
		}
		}
		return SUSPENDED;
	}

	/**
	M�todo invocado al final de la ejecuci�n de la acci�n,
	para que se realicen las tareas que son �nicamente necesarias
	al final (y no durante toda la vida de la acci�n).

	En la mayor�a de los casos este m�todo no hace nada.
	*/
	void CLA_Attack::OnStop()
	{
		awakeComponents();
		if (_entity->getComponent<CAttack>()!=NULL)
			_entity->getComponent<CAttack>()->resetAttackFlags();
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
	CLatentAction::LAStatus CLA_Attack::OnRun() 
	{
		return RUNNING;
	}

	/**
	M�todo invocado cuando la acci�n ha sido cancelada (el comportamiento
	al que pertenece se ha abortado por cualquier raz�n).

	La tarea puede en este momento realizar las acciones que
	considere oportunas para "salir de forma ordenada".

	@note <b>Importante:</b> el Abort <em>no</em> provoca la ejecuci�n
	de OnStop().
	*/
	CLatentAction::LAStatus CLA_Attack::OnAbort() 
	{
		// Cuando se aborta se queda en estado terminado con fallo
		awakeComponents();
		if (_entity->getComponent<CAttack>()!=NULL)
			_entity->getComponent<CAttack>()->resetAttackFlags();
	
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
	bool CLA_Attack::accept(const CMessage *message)
	{		
		// la accion latente de ataque solo acepta mensajes de ataque en el momento que la oportunidad de combo est� activada.
		return 	(message->getType()==Message::ANIMATION_FINISHED);
	}
	/**
	Procesa el mensaje recibido. El m�todo es invocado durante la
	ejecuci�n de la acci�n cuando se recibe el mensaje.

	@param msg Mensaje recibido.
	*/
	void CLA_Attack::process(CMessage *message)
	{
		switch(message->getType())
		{
		case Message::ANIMATION_FINISHED: //ConditionFail
		{
			if (_initialCombatState==0  || _animationSetedByMe  )
			{
				CMessageUShort* maux = static_cast<CMessageUShort*>(message);
				if (maux->getUShort()==Logic::ATTACK1)  //ACORDARSE DE PONER ATTACK3 O EL QUE SEA PARA QUE LO TENGA EN CUENTA
				{	
					finish(false);
				}
				else if (maux->getUShort()==Logic::ATTACK2)
				{
					finish(false);				
				}
				else if (maux->getUShort()==Logic::ATTACK3)
				{
					finish(false);				
				}
			}else
			{
			switch(_initialCombatState)
			{
				case 1:
				{
					CMessageBoolUShort *message = new CMessageBoolUShort();
					message->setType(Message::SET_ANIMATION);
					message->setUShort(Logic::ATTACK2);
					message->setAction(_action);
					message->setBool(false);
					_entity->emitMessage(message);		
					_animationSetedByMe=true;
					break;
				}	
				case 2:
				{
					if (_action==Message::HEAVY_ATTACK)
					{
						/*CMessageBoolString *message = new CMessageBoolString();
						message->setType(Message::SET_ANIMATION);
						message->setString("INDEFINIDO");
						message->setAction(_action);
						message->setBool(false);
						_entity->emitMessage(message);	
						_initialYaw=_entity->getYaw();
						_yawAmount=0;*/
						CMessageBoolUShort *message = new CMessageBoolUShort();
						message->setType(Message::SET_ANIMATION);
						message->setUShort(Logic::ATTACK3);
						message->setAction(_action);
						message->setBool(false);
						_entity->emitMessage(message);
						_animationSetedByMe=true;
					}
					else
					{
						CMessageBoolUShort *message = new CMessageBoolUShort();
						message->setType(Message::SET_ANIMATION);
						message->setUShort(Logic::ATTACK3);
						message->setAction(_action);
						message->setBool(false);
						_entity->emitMessage(message);	
						_animationSetedByMe=true;
						//finish(false);	
						//_initialYaw=_entity->getYaw();
						//_yawAmount=0;				
					}
					break;	
				}
		}
					
		}//else
		
		break;
		}
	}
	}

	void CLA_Attack::tick(unsigned int msecs) 
	{
		/*if (_initialCombatState==2 && _yawAmount>=0 && _action==Message::HEAVY_ATTACK)
		{
			_yawAmount++;
			_entity->yaw(_entity->getYaw()+0.001f*msecs);
			if (_yawAmount>40) 
				{
					_yawAmount=-10;
					_entity->setYaw(_initialYaw);
				
					finish(false);
				}				
		}*/
		CLatentAction::tick();
	}

	void CLA_Attack::sleepComponents()
	{
		if (_entity->getComponent<CAttack>()!=NULL)
		_entity->getComponent<CAttack>()->resetAttackFlags();
		if (_entity->getComponent<CAvatarController>()!=NULL)
		_entity->getComponent<CAvatarController>()->sleep();
		if (_entity->getComponent<CJump>()!=NULL)
		_entity->getComponent<Logic::CJump>()->sleep();
		if (_entity->getComponent<CBaseTraveler>()!=NULL)
		_entity->getComponent<CBaseTraveler>()->sleep();
	}

	void CLA_Attack::awakeComponents()
	{
		if (_entity->getComponent<CAttack>()!=NULL)
		_entity->getComponent<CAttack>()->resetAttackFlags();
		if (_entity->getComponent<CAvatarController>()!=NULL)
		_entity->getComponent<CAvatarController>()->awake();
		if (_entity->getComponent<CJump>()!=NULL)
		_entity->getComponent<Logic::CJump>()->awake();
		if (_entity->getComponent<CBaseTraveler>()!=NULL)
		_entity->getComponent<CBaseTraveler>()->awake();
	}
} //namespace LOGIC