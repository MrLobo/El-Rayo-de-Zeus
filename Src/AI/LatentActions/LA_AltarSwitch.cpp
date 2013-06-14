#include "LA_AltarSwitch.h"
#include "../../Logic/Entity/Components/AnimatedGraphics.h"
#include "Application/BaseApplication.h"

#include "../../Logic/Entity/Components/Combat.h"

#include "../StateMachines/StateMachine.h"

#include "Logic/Entity/Messages/MessageBoolUShort.h"
#include "Logic/Entity/Messages/MessageFloat.h"
#include "Logic/Entity/Messages/MessageString.h"
namespace AI
{


//////////////////////////////
//	Implementaci�n de CLA_AltarSwitch
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
	CLatentAction::LAStatus CLA_AltarSwitch::OnStart()
	{
		CMessageBoolUShort *message = new CMessageBoolUShort();
		message->setType(Message::SET_ANIMATION);
		message->setUShort(Logic::ACTIVATE_ALTAR);
		message->setBool(true);
		_entity->emitMessage(message);
		
		if (_entity->getComponent<CCombat>()!=NULL)
				_entity->getComponent<CCombat>()->setCovering(false);		

		return SUSPENDED;
	}

	/**
	M�todo invocado al final de la ejecuci�n de la acci�n,
	para que se realicen las tareas que son �nicamente necesarias
	al final (y no durante toda la vida de la acci�n).

	En la mayor�a de los casos este m�todo no hace nada.
	*/
	void CLA_AltarSwitch::OnStop()
	{
		CMessageBoolUShort *message = new CMessageBoolUShort(); //anulado
		message->setType(Message::SET_ANIMATION);		
		message->setUShort(Logic::IDLE);
		message->setBool(true);
		//_entity->emitMessage(message);
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
		CLatentAction::LAStatus CLA_AltarSwitch::OnRun() 
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
		CLatentAction::LAStatus  CLA_AltarSwitch::OnAbort() 
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
	bool CLA_AltarSwitch::accept(const CMessage *message)
	{		
		// La acci�n no acepta mensajes
		return message->getType() == Message::ALTAR_MS_ORDER &&
				message->getAction() == Message::FINISH_SUCCESS;
	}
	/**
	Procesa el mensaje recibido. El m�todo es invocado durante la
	ejecuci�n de la acci�n cuando se recibe el mensaje.

	@param msg Mensaje recibido.
	*/
	void CLA_AltarSwitch::process(CMessage *message)
	{
		finish(true);
	}
	
	void CLA_AltarSwitch::tick(unsigned int msecs) 
	{		
		CLatentAction::tick();
	}

	void CLA_AltarSwitch::sleepComponents(){}

	void CLA_AltarSwitch::awakeComponents(){}
} //namespace LOGIC