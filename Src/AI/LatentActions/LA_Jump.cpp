#include "LA_Jump.h"

#include "Application/BaseApplication.h"
#include "../StateMachines/StateMachine.h"
#include "../../Logic/Entity/Components/BaseTraveler.h"

namespace AI
{


//////////////////////////////
//	Implementaci�n de CLA_Jump
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
	CLatentAction::LAStatus CLA_Jump::OnStart()
	{
				//Desactivaci�n de componentes
		sleepComponents();
		std::cout<<"AI::StateMachine::Jumping"<<std::endl;
		
		CMessageBoolString *message = new CMessageBoolString();
		message->setType(Message::SET_ANIMATION);
		message->setAction(Message::JUMP);				
		message->setString("jump");
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
	void CLA_Jump::OnStop()
	{
				//Reactivaci�n de componentes
	
		awakeComponents();
		if (_entity->getComponent<CBaseTraveler>()!=NULL)
		{
			_entity->getComponent<CBaseTraveler>()->resetChangingBase();		
			_entity->getComponent<CBaseTraveler>()->resetChangingRing();
		}
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
	CLatentAction::LAStatus CLA_Jump::OnRun() 
	{
		// TODO PR�CTICA IA
		// En cada paso de ejecuci�n tendremos que comprobar si hemos
		// superado el tiempo de espera. Seg�n lo hayamos superado o no,
		// la acci�n tendr� que pasar a un estado de terminado con �xito o
		// quedarse en el mismo estado en ejecuci�n.
		/*if(Application::CBaseApplication::getSingletonPtr()->getAppTime() < _endingTime)
			return RUNNING;
		else 
			return SUCCESS;*/

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
	CLatentAction::LAStatus CLA_Jump::OnAbort() 
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
	bool CLA_Jump::accept(const CMessage *message)
	{		
		// La acci�n no acepta mensajes
		return false;
		return (message->getType() == Message::CONTROL && (message->getAction()==Message::LIGHT_ATTACK || message->getAction()==Message::HEAVY_ATTACK));
	}
	/**
	Procesa el mensaje recibido. El m�todo es invocado durante la
	ejecuci�n de la acci�n cuando se recibe el mensaje.

	@param msg Mensaje recibido.
	*/
	void CLA_Jump::process(CMessage *message)
	{
		// TODO PR�CTICA IA
		// La acci�n no procesa mensajes
		/*if(message->getType() == Message::CONTROL)
			 if ( message->getAction()==Message::LIGHT_ATTACK)
				finish(true); // bool = exito o fallo
			 else
				finish(false);*/
	}
	
	void CLA_Jump::tick(unsigned int msecs) 
	{
		CLatentAction::tick();
	}

	void CLA_Jump::sleepComponents()
	{
		if (_entity->getComponent<CBaseTraveler>()!=NULL)
			_entity->getComponent<CBaseTraveler>()->sleep();
	}

	void CLA_Jump::awakeComponents()
	{	
		if (_entity->getComponent<CBaseTraveler>()!=NULL)
			_entity->getComponent<CBaseTraveler>()->awake();
	}
} //namespace LOGIC