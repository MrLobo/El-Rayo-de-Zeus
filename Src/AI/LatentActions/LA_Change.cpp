
#include "../../Logic/Entity/Components/Attack.h"
#include "../../Logic/Entity/Components/AvatarController.h"
#include "../../Logic/Entity/Components/Jump2.h"
#include "../../Logic/Entity/Components/BaseTraveler.h"
#include "Application/BaseApplication.h"

#include "../StateMachines/StateMachine.h"
#include "LA_Change.h"
#include "Logic/Entity/Messages/MessageChar.h"

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
	CLatentAction::LAStatus CLA_Change::OnStart()
	{		
		//Desactivaci�n de componentes
		sleepComponents();		
		_reloj=Application::CBaseApplication::getSingletonPtr()->getClock();
		
		std::cout<<"AI::StateMachine::Change"<<std::endl;
		switch(_action)
		{
		case Message::CHANGE_BASE:
			{	//activo un reloj
				//(int index, IClockListener* listener, unsigned long time)
				_reloj->addTimeObserver(0,this,_maxChangingBaseTime);
					//std::pair<IClockListener*,unsigned long>(this,_maxChangingBaseTime));		
				
				CMessageString *m = new CMessageString();	
				m->setType(Message::SET_MATERIAL);
				m->setString("transito");
				_entity->emitMessage(m);				
				break;
			}
			case Message::CHANGE_RING:
			{	//activo un reloj
			//	_reloj->addTimeObserver(std::pair<IClockListener*,unsigned long>(this,_maxChangingRingTime));		
				_reloj->addTimeObserver(1,this,_maxChangingRingTime);
				CMessageString *m = new CMessageString();	
				m->setType(Message::SET_MATERIAL);
				m->setString("transito");
				_entity->emitMessage(m);
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
	void CLA_Change::OnStop()
	{
		std::cout<<"AI::StateMachine::ChangeSALIENDO"<<std::endl;	
		_reloj->removeTimeObserver(0);		
		_reloj->removeTimeObserver(1);			
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
	CLatentAction::LAStatus CLA_Change::OnRun() 
	{
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
	CLatentAction::LAStatus CLA_Change::OnAbort() 
	{
		// Cuando se aborta se queda en estado terminado con fallo
		if (_entity->getComponent<CBaseTraveler>()!=NULL)
		{
			_reloj->removeTimeObserver(0);		
			_reloj->removeTimeObserver(1);		
			_entity->getComponent<CBaseTraveler>()->resetChangingBase();			
			_entity->getComponent<CBaseTraveler>()->resetChangingRing();
		}
		if (_entity->getComponent<CRingTraveler>()!=NULL)
			_entity->getComponent<CRingTraveler>()->resetChangingRing();
		
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
	bool CLA_Change::accept(const CMessage *message)
	{		
		// la accion latente de ataque solo acepta mensajes de ataque en el momento que la oportunidad de combo est� activada.
		return/* (_comboOportunity && (message->getType() == Message::CONTROL && 
			(message->getAction() == Message::LIGHT_ATTACK||
			message->getAction() == Message::HEAVY_ATTACK)))
			||*/
			/*(message->getType()==Message::ANIMATION_MOMENT) &&  _initialCombatState!=2)*/
			(message->getType()==Message::CONTROL);
	}
	/**
	Procesa el mensaje recibido. El m�todo es invocado durante la
	ejecuci�n de la acci�n cuando se recibe el mensaje.

	@param msg Mensaje recibido.
	*/
	void CLA_Change::process(CMessage *message)
	{
		switch(message->getType())
		{
			case Message::CONTROL: //si estamos aqu� es que el jugador quiere aprovechar la oportunidad que tenia de realizar un combo. Dependiendo de 
			{
				if (_action==Message::CHANGE_BASE)				{
					
					CMessageString *m2 = new CMessageString();	
					m2->setType(Message::SET_MATERIAL);
					m2->setString("marine");
					_entity->emitMessage(m2);	
					finish(false);
				}
				if (_action==Message::GO_DOWN || _action==Message::GO_UP)
				{			
					CMessageString *m2 = new CMessageString();	
					m2->setType(Message::SET_MATERIAL);
					m2->setString("marine");
					_entity->emitMessage(m2);	
					finish(false);
				}
				break;
			}
		}
			// TODO PR�CTICA IA
		// La acci�n no procesa mensajes
	}

	void CLA_Change::sleepComponents()
	{
		/*if (_entity->getComponent<CAttack>()!=NULL)
		_entity->getComponent<CAttack>()->resetAttackFlags();
		if (_entity->getComponent<CAvatarController>()!=NULL)
		_entity->getComponent<CAvatarController>()->sleep();
		if (_entity->getComponent<CJump2>()!=NULL)
		_entity->getComponent<Logic::CJump2>()->sleep();
		if (_entity->getComponent<CBaseTraveler>()!=NULL)
		_entity->getComponent<CBaseTraveler>()->sleep();*/
	}

	void CLA_Change::awakeComponents()
	{/*
		if (_entity->getComponent<CAttack>()!=NULL)
		_entity->getComponent<CAttack>()->resetAttackFlags();
		if (_entity->getComponent<CAvatarController>()!=NULL)
		_entity->getComponent<CAvatarController>()->awake();
		if (_entity->getComponent<CJump2>()!=NULL)
		_entity->getComponent<Logic::CJump2>()->awake();
		if (_entity->getComponent<CBaseTraveler>()!=NULL)
		_entity->getComponent<CBaseTraveler>()->awake();
	*/}
	void CLA_Change::timeArrived()
	{//El primer if es para ignorar eventos externos si no estoy en el estado
		if (this->getStatus()!=SUCCESS && this->getStatus()!=FAIL && this->getStatus()!=RUNNING)
		{//cancelando viaje

			return;
		}
		if (_entity->getComponent<CBaseTraveler>()!=NULL)
		{			
			_entity->getComponent<CBaseTraveler>()->timeArrived();
			//_entity->getComponent<CRingTraveler>()->timeArrived();				
		}
	
		finish(true);
	}
} //namespace LOGIC