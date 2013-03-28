
#include "../../Logic/Entity/Components/Attack.h"
#include "../../Logic/Entity/Components/AvatarController.h"
#include "../../Logic/Entity/Components/Jump2.h"
#include "../../Logic/Entity/Components/BaseTraveler.h"
#include "Application/BaseApplication.h"

#include "../StateMachines/StateMachine.h"
#include "LA_Cover.h"
#include "Logic/Entity/Messages/MessageChar.h"
#include "Logic/Entity/Messages/MessageFloat.h"

namespace AI
{


//////////////////////////////
//	Implementaci�n de CLA_Cover
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
	CLatentAction::LAStatus CLA_Cover::OnStart()
	{		
		//Desactivaci�n de componentes
		sleepComponents();		
		std::cout<<"AI::StateMachine::Cover"<<std::endl;

			CMessageBoolString *message = new CMessageBoolString();
			message->setType(Message::SET_ANIMATION);
			message->setString("CrouchKatana");
			message->setAction(Message::UNDEF);
			message->setBool(true);
			_entity->emitMessage(message);
			
		return RUNNING;
	}

	/**
	M�todo invocado al final de la ejecuci�n de la acci�n,
	para que se realicen las tareas que son �nicamente necesarias
	al final (y no durante toda la vida de la acci�n).

	En la mayor�a de los casos este m�todo no hace nada.
	*/
	void CLA_Cover::OnStop()
	{
		std::cout<<"AI::StateMachine::CoverSALIENDO"<<std::endl;	
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
	CLatentAction::LAStatus CLA_Cover::OnRun() 
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
	CLatentAction::LAStatus CLA_Cover::OnAbort() 
	{
		// Cuando se aborta se queda en estado terminado con fallo
	
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
	bool CLA_Cover::accept(const CMessage *message)
	{		
		// la accion latente de ataque solo acepta mensajes de ataque en el momento que la oportunidad de combo est� activada.
		return false;
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
	void CLA_Cover::process(CMessage *message)
	{		
	}

	void CLA_Cover::sleepComponents()
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

	void CLA_Cover::awakeComponents()
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

	
} //namespace LOGIC