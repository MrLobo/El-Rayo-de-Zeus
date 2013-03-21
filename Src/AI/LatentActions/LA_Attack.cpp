#include "LA_Attack.h"
#include "../../Logic/Entity/Components/Attack.h"
#include "../../Logic/Entity/Components/AvatarController.h"
#include "../../Logic/Entity/Components/Jump2.h"
#include "../../Logic/Entity/Components/BaseTraveler.h"
#include "Application/BaseApplication.h"

#include "../StateMachines/StateMachine.h"

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
		std::cout<<_initialCombatState<<std::endl;		
		std::cout<<(int)_action<<std::endl;
		switch(_initialCombatState)
		{
			case 0:
			{
				CMessageBoolString *message = new CMessageBoolString();
				message->setType(Message::SET_ANIMATION);
				message->setString("FireKatana");
				message->setAction(_action);
				message->setBool(false);
				_entity->emitMessage(message);
				break;
			}
			case 1:
			{
				CMessageBoolString *message = new CMessageBoolString();
				message->setType(Message::SET_ANIMATION);
				message->setString("GetObject");
				message->setAction(_action);
				message->setBool(false);
				_entity->emitMessage(message);		
				break;
			}	
			case 2:
			{
				if (_action==Message::HEAVY_ATTACK)
				{CMessageBoolString *message = new CMessageBoolString();
				message->setType(Message::SET_ANIMATION);
				message->setString("INDEFINIDO");
				message->setAction(_action);
				message->setBool(false);
				_entity->emitMessage(message);	
				_initialYaw=_entity->getYaw();
				_yawAmount=0;
				}
				else
				{
				CMessageBoolString *message = new CMessageBoolString();
				message->setType(Message::SET_ANIMATION);
				message->setString("Damage");
				message->setAction(_action);
				message->setBool(false);
				_entity->emitMessage(message);	
				//_initialYaw=_entity->getYaw();
				//_yawAmount=0;				
				}
				break;
			}
		}
		return RUNNING;
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
		if (_initialCombatState==2 && _yawAmount>=0 && _action==Message::HEAVY_ATTACK)
		{
			_yawAmount++;
			_entity->yaw(_entity->getYaw()+0.3);
			if (_yawAmount>50) 
				{
					_yawAmount=-10;
					_entity->setYaw(_initialYaw);
				/*	CMessageString *msg = new CMessageString();
					msg->setType(Message::ANIMATION_FINISHED);
					msg->setAction(_action);
					msg->setString("Death");
					_entity->emitMessage(msg);*/
					finish(false);
				}				
		}
		// TODO PR�CTICA IA
		// En cada paso de ejecuci�n tendremos que comprobar si hemos
		// superado el tiempo de espera. Seg�n lo hayamos superado o no,
		// la acci�n tendr� que pasar a un estado de terminado con �xito o
		// quedarse en el mismo estado en ejecuci�n.
		/*if(Application::CBaseApplication::getSingletonPtr()->getAppTime() < _endingTime)
			return RUNNING;
		else 
			return SUCCESS;*/
		if (this->getStatus()!=SUCCESS && this->getStatus()!=FAIL)
		return RUNNING;
		else 
			return this->getStatus();
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
		return/* (_comboOportunity && (message->getType() == Message::CONTROL && 
			(message->getAction() == Message::LIGHT_ATTACK||
			message->getAction() == Message::HEAVY_ATTACK)))
			||*/
			/*(message->getType()==Message::ANIMATION_MOMENT) &&  _initialCombatState!=2)*/
			((message->getType()==Message::ANIMATION_FINISHED) || (message->getType()==Message::SET_ANIMATION));
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
				CMessageString* maux = static_cast<CMessageString*>(message);
				if (maux->getString().compare("FireKatana")==0 )
				{	
					finish(false);
					//_lightAttack=_heavyAttack=false;//stopMovement();
				}
				else if (maux->getString().compare("GetObject")==0)
				{
					finish(false);				
				}
				else if (maux->getString().compare("Damage")==0)
				{
					finish(false);				
				}_comboOportunity=false;
				break;
			}
		case Message::SET_ANIMATION: //con esto quiero ver si se ha cancelado una animaci�n
			{
				if (message->getAction()==Message::WALK_LEFT || message->getAction()==Message::WALK_RIGHT)
				{
					finish(false);					
				}
				break;
			}	
		case Message::ANIMATION_MOMENT:
			{				
				_comboOportunity=true;
				break;
				//querr� saber cual animaci�n es, de momento se que solo puedo recibir un tipo de animaci�n
				/*float punto;
				if (_entity->getSense()==Logic::LogicalPosition::RIGHT)
					punto=_entity->getDegree()-10;
				else
					punto=_entity->getDegree()+10;
					//con este metodo vemos si con la espada le estamos dando
					attackPlace(punto,_entity->getRing(),_entity->getBase(),false);*/
			}
		/*	case Message::CONTROL: //si estamos aqu� es que el jugador quiere aprovechar la oportunidad que tenia de realizar un combo. Dependiendo de 
			{

				if (_comboOportunity)
				{
					if (_initialCombatState<1)
						if (message->getAction()==Message::LIGHT_ATTACK)
						{
							_comboOportunity=false;
							finish(true);
						}					
					if (_initialCombatState==1)//en el tercer ataque del combo
						if (message->getAction()==Message::HEAVY_ATTACK)
						{
							_comboOportunity=false;
							finish(true);
						}
					}
					break;
			}*/
		}
			// TODO PR�CTICA IA
		// La acci�n no procesa mensajes
	}

	void CLA_Attack::sleepComponents()
	{
		if (_entity->getComponent<CAttack>()!=NULL)
		_entity->getComponent<CAttack>()->resetAttackFlags();
		if (_entity->getComponent<CAvatarController>()!=NULL)
		_entity->getComponent<CAvatarController>()->sleep();
		if (_entity->getComponent<CJump2>()!=NULL)
		_entity->getComponent<Logic::CJump2>()->sleep();
		if (_entity->getComponent<CBaseTraveler>()!=NULL)
		_entity->getComponent<CBaseTraveler>()->sleep();
	}

	void CLA_Attack::awakeComponents()
	{
		if (_entity->getComponent<CAttack>()!=NULL)
		_entity->getComponent<CAttack>()->resetAttackFlags();
		if (_entity->getComponent<CAvatarController>()!=NULL)
		_entity->getComponent<CAvatarController>()->awake();
		if (_entity->getComponent<CJump2>()!=NULL)
		_entity->getComponent<Logic::CJump2>()->awake();
		if (_entity->getComponent<CBaseTraveler>()!=NULL)
		_entity->getComponent<CBaseTraveler>()->awake();
	}
} //namespace LOGIC