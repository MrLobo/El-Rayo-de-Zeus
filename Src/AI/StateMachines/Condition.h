/**
@file CCondition.h

En este fichero se define la interfaz para las condiciones
de las aristas de las m�quinas de estado, as� como cuatro 
condiciones de ejemplo: CConditionTrue (una condici�n que 
siempre se eval�a como verdadera), CConditionFinished (es 
verdadera si la acci�n asociada al nodo ha terminado), 
CConditionSuccess (es  verdadera si la acci�n asociada al 
nodo ha terminado con �xito) y CConditionFail (es verdadera 
si la acci�n asociada al nodo ha terminado con fallo).

@author Gonzalo Fl�rez
@date Diciembre 2010

*/

#pragma once

#ifndef __AI_Condition_H
#define __AI_Condition_H

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Messages/Message.h"

#include "../LatentActions/LatentAction.h"

using namespace Logic;

namespace AI 
{
	/**
	Interfaz que deber�n implementar las condiciones de 
	las m�quinas de estado.
	*/
	template <class TNode>
	class ICondition
	{
	public:
		/**
		En este m�todo se comprobar� la condici�n. Recibe el 
		nodo al que pertenece la condici�n (de esta manera 
		podemos comprobar si el nodo ha terminado y en qu� 
		condiciones) y la entidad que ejecuta el comportamiento 
		(que puede ser NULL).

		@param currentNode Nodo al que pertenece la condici�n.
		@param entity Entidad que ejecuta el comportamiento
		@return true o false seg�n si se cumple la condici�n o no.
		*/
		virtual bool check(TNode* currentNode, CEntity* entity) = 0;
		/**
		Devuelve true si a la acci�n le interesa el tipo de mensaje
		enviado como par�metro.
		@param msg Mensaje que ha recibido la entidad.
		@return true Si la acci�n est� en principio interesada
		por ese mensaje.
		*/
		virtual bool accept(const CMessage *message) {return false;};
		/**
		Procesa el mensaje recibido. El m�todo es invocado durante la
		ejecuci�n de la acci�n cuando se recibe el mensaje.

		@param msg Mensaje recibido.
		*/
		virtual void process(CMessage *message) {};

		virtual void reset(){};
	}; // class CCondition

	/**
	Esta clase define una condici�n que siempre se eval�a a true.
	*/
	template <class TNode>
	class CConditionTrue : public ICondition<TNode>
	{
	public:
		bool check(TNode* currentNode, CEntity* entity) { return true; }
	};
	
	/**
	Esta clase define una condici�n que se eval�a a true si la 
	acci�n a la que pertenece ha terminado.
	*/
	class CConditionFinished : public ICondition<CLatentAction>
	{
	public:
		bool check(CLatentAction* currentNode, CEntity* entity) 
		{ 
			// TODO PR�CTICA IA
			// Comprobar si el estado del nodo que se est� ejecutando es terminado
			return currentNode->getStatus() == CLatentAction::SUCCESS ||
					currentNode->getStatus() == CLatentAction::FAIL;
		}
	};

	/**
	Esta clase define una condici�n que se eval�a a true si la 
	acci�n a la que pertenece ha terminado con �xito.
	*/
	class CConditionSuccess : public ICondition<CLatentAction>
	{
	public:
		bool check(CLatentAction* currentNode, CEntity* entity) 
		{ 
			// TODO PR�CTICA IA
			// Comprobar si el estado del nodo que se est� ejecutando es �xito

			return currentNode->getStatus() == CLatentAction::SUCCESS;
		}
	};

	/**
	Esta clase define una condici�n que se eval�a a true si la 
	acci�n a la que pertenece ha terminado con fallo.
	*/
	class CConditionFail : public ICondition<CLatentAction>
	{
	public:
		bool check(CLatentAction* currentNode, CEntity* entity) 
		{ 
			
			// TODO PR�CTICA IA
			// Comprobar si el estado del nodo que se est� ejecutando es fallo
			return currentNode->getStatus() == CLatentAction::FAIL;
		}
	};

	/**
	Esta clase define una condici�n que devuelve true 
	si se recibi� un mensaje de un tipo concreto en el �ltimo tick
	*/
	template <class TNode>
	class CConditionMessage : public ICondition<TNode>
	{
	public:
		/**
		En el constructor recibimos el tipo de mensaje que 
		har� saltar la condici�n
		@param messageType Tipo de mensaje que estamos escuchando
		*/
		//Message::TMessageType::
		CConditionMessage(Logic::Message::TMessageType messageType, Logic::Message::TActionType actionType) : _received(false) {
			_messageType = messageType;
			_actionType = actionType;
		}
		/**
		En el check s�lo tenemos que comprobar el flag _received. Este flag
		se habr� activado en process si durante este tick hemos recibido
		un mensaje del tipo adecuado.
		Una vez hecha la comprobaci�n, reseteamos el flag a false.
		@param currentNode Nodo al que pertenece la condici�n (no se usa)
		@param entity Entidad que ejecuta el comportamiento (no se usa)
		@return true o false seg�n si se cumple la condici�n o no.

		*/
		bool check(TNode* currentNode, CEntity* entity) { 
			// TODO PR�CTICA IA
			// Tenemos que comprobar el flag _received y luego actualizarlo
			// de nuevo a false para el siguiente tick
			bool receivedThisTick = _received;
			_received = false;
			return receivedThisTick;
		}
		/**
		Devuelve true si el tipo del mensaje recibido coincide con el 
		que estamos esperando
		@param msg Mensaje que ha recibido la entidad.
		@return true Si la acci�n est� en principio interesada
		por ese mensaje.
		*/
		virtual bool accept(const CMessage *message) {
			// TODO PR�CTICA IA
			// La condici�n debe aceptar mensajes del tipo indicado en _messageType
			return (message->getType() == _messageType && message->getAction()==_actionType);
		};
		/**
		Procesa el mensaje recibido. Como s�lo aceptamos mensajes del
		tipo en el que estamos interesados aqu� directamente ponemos 
		el flag a true.

		@param msg Mensaje recibido.
		*/
		virtual void process(CMessage *message) {
			// TODO PR�CTICA IA
			// Actualizamos el flag _received si el mensaje es del tipo _messageType
			//CMessageBoolString *maux = static_cast<CMessageBoolString*>(message);
			_received = (message->getType() == _messageType && message->getAction()==_actionType) || _received;
		};

	private:
		/** Flag que se activa cuando recibimos un mensaje del tipo en el que estamos interesados */
		bool _received;
		/** Tipo del mensaje que esperamos */
		Logic::Message::TMessageType _messageType;
	 	
		/** Tipo del mensaje que esperamos */
		Logic::Message::TActionType _actionType;
	};

	/**
	Esta clase define una condici�n que devuelve true 
	si se recibi� un mensaje de un tipo concreto en el �ltimo tick
	*/
	template <class CLatentAction>
	class CConditionMessageAction : public ICondition<CLatentAction>
	{
	public:
		/**
		En el constructor recibimos el tipo de mensaje que 
		har� saltar la condici�n
		@param messageType Tipo de mensaje que estamos escuchando
		*/
		//Message::TMessageType::
		CConditionMessageAction(Logic::Message::TMessageType messageType, Logic::Message::TActionType actionType, bool startActivated,Logic::Message::TMessageType messageTypeToActivate, Logic::Message::TActionType actionTypeToActivate) : _received(false) {
			_received=false;
			_messageType = messageType;
			_actionType = actionType;
			_initialStatus=startActivated;
			_enabled= _initialStatus;
			_messageTypeToActivate=messageTypeToActivate;
			_actionTypeToActivate=actionTypeToActivate;
		}
		/**
		En el check s�lo tenemos que comprobar el flag _received. Este flag
		se habr� activado en process si durante este tick hemos recibido
		un mensaje del tipo adecuado.
		Una vez hecha la comprobaci�n, reseteamos el flag a false.
		@param currentNode Nodo al que pertenece la condici�n (no se usa)
		@param entity Entidad que ejecuta el comportamiento (no se usa)
		@return true o false seg�n si se cumple la condici�n o no.

		*/
		bool check(CLatentAction* currentNode, CEntity* entity) { 
			// TODO PR�CTICA IA
			// Tenemos que comprobar el flag _received y luego actualizarlo
			// de nuevo a false para el siguiente tick
			bool receivedThisTick = _received;
			_received = false;
			return receivedThisTick;
		}

		void reset() { //PeP: para resetear el estado de la condici�n de activada o desactivada.
			_enabled=_initialStatus;
		}

		/**
		Devuelve true si el tipo del mensaje recibido coincide con el 
		que estamos esperando
		@param msg Mensaje que ha recibido la entidad.
		@return true Si la acci�n est� en principio interesada
		por ese mensaje.
		*/
		virtual bool accept(const CMessage *message) {
			// TODO PR�CTICA IA
			// La condici�n debe aceptar mensajes del tipo indicado en _messageType
			if (_enabled)
			{	
				return (message->getType() == _messageType && message->getAction()==_actionType);
			}
			else
				//return 
				if (message->getType() == _messageTypeToActivate)  //ignoramos el actiontoactivate
				{		
					_enabled=true; return false;
				}	
		};
		/**
		Procesa el mensaje recibido. Como s�lo aceptamos mensajes del
		tipo en el que estamos interesados aqu� directamente ponemos 
		el flag a true.

		@param msg Mensaje recibido.
		*/
		virtual void process(CMessage *message) {
			// TODO PR�CTICA IA
			// Actualizamos el flag _received si el mensaje es del tipo _messageType
			//CMessageBoolString *maux = static_cast<CMessageBoolString*>(message);
			if (_enabled)
			if (message->getType() == _messageType && message->getAction()==_actionType)
			{	
				_enabled=_initialStatus;
				_received = (message->getType() == _messageType && message->getAction()==_actionType) || _received;
				if (_received) reset();
			}
		};

	private:
		/** Flag que se activa cuando recibimos un mensaje del tipo en el que estamos interesados */
		bool _received;
		/** Flag que se indica si de inicio est� activada la recepci�n de ese mensaje o se precisa que antes llegue otro mensaje que lo active */
		bool _initialStatus;
		/** Flag que se indica si de inicio est� activada la recepci�n de ese mensaje o se precisa que antes llegue otro mensaje que lo active */
		bool _enabled;
		/** Tipo del mensaje que esperamos */
		Logic::Message::TMessageType _messageType;
	 	
		/** Acci�n del mensaje que esperamos */
		Logic::Message::TActionType _actionType;
		/** Tipo del mensaje que har� que se active la condici�n */
		Logic::Message::TMessageType _messageTypeToActivate;
	 	
		/** Acci�n del mensaje que har� que se active la condici�n */
		Logic::Message::TActionType _actionTypeToActivate;
	};
} // namespace Logic

#endif // __AI_Condition_H
