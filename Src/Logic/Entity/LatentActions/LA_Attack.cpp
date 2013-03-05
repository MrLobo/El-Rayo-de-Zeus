#include "LA_Attack.h"

#include "Application/BaseApplication.h"

#include "../StateMachines/StateMachine.h"

namespace Logic
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
		std::cout<<_initialCombatState<<std::endl;
		switch(_initialCombatState)
		{
			case 0:
				{
					CMessageBoolString *message = new CMessageBoolString();
					message->setType(Message::SET_ANIMATION);
					message->setString("FireKatana");
					message->setAction(Message::LIGHT_ATTACK);
					message->setBool(false);
					_entity->emitMessage(message);
					break;
				// TODO PR�CTICA IA
				// Al iniciar el wait, calculamos el instante de tiempo en 
				// el que se tiene que finalizar la acci�n. 
				// El tiempo de espera se guarda en _time.
				// Para obtener el instante actual podemos usar 
				// Application::CBaseApplication::getSingletonPtr()->getAppTime()
				//
				//_endingTime = Application::CBaseApplication::getSingletonPtr()->getAppTime() + _time; 
				// Otra opci�n es usar
				// BaseSubsystems::CServer::getSingletonPtr()->getAppTimer()
				// para sacar el timer de la aplicaci�n y usar su m�todo 
				// getMilliseconds.
				}
			case 1:
			{
				CMessageBoolString *message = new CMessageBoolString();
				message->setType(Message::SET_ANIMATION);
				message->setString("GetObject");
				message->setAction(Message::LIGHT_ATTACK);
				message->setBool(false);
				_entity->emitMessage(message);		
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
	CLatentAction::LAStatus CLA_Attack::OnAbort() 
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
	bool CLA_Attack::accept(const CMessage *message)
	{		
		// la accion latente de ataque solo acepta mensajes de ataque en el momento que la oportunidad de combo est� activada.
		return (_comboOportunity && (message->getType() == Message::CONTROL && 
			(message->getAction() == Message::LIGHT_ATTACK||
			message->getAction() == Message::HEAVY_ATTACK)))
			||
			((Message::ANIMATION_MOMENT) || (Message::ANIMATION_FINISHED));
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
		case Message::ANIMATION_FINISHED:
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
				}_comboOportunity=false;
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
			case Message::CONTROL: //si estamos aqu� es que el jugador ha aprovechado la oportunidad que tenia de realizar un combo.
			{

				if (_comboOportunity)
					if (message->getAction()==Message::LIGHT_ATTACK)
					{
						_comboOportunity=false;
						finish(true);
						break;
					}
			}
		}
			// TODO PR�CTICA IA
		// La acci�n no procesa mensajes
	}


} //namespace LOGIC