/**
@file MessageBool.h

Contiene el tipo de datos de un mensaje float.

@see Logic::CMessageBool

@author Emilio Santalla Comellas
*/

#ifndef __Logic_MessageBool_H
#define __Logic_MessageBool_H


#include "Net/buffer.h"
#include "Message.h"


// Predeclaracion de clases
namespace Logic {
	class CEntity;
};


namespace Logic
{	
	// Declaraci�n de la clase
	class CMessageBool : public CMessage
	{
		DEC_MFACTORY(CMessageBool);
	public:

		/**
		Constructor b�sico
		*/
		CMessageBool() : CMessage(GetAltTypeIdOf(CMessageBool)), _bool(false) {}
			

		/**
		Destructor de la clase. El destructor se invoca cuando la cuenta
		de referencias vuelve a 0.
		*/
		~CMessageBool() {};

		/**
		Devuelve el float del Mensaje

		@return float del mensaje.
		*/
		float getBool() const { return _bool; }

		/**
		Establece el float del mensaje

		@return float del mensaje.
		*/
		void setBool(const bool valor) {_bool=valor; }


		/**
		Serializa un mensaje. Genera un CBuffer a partir de 
		un mensaje.

		@param data CBuffer donde almacenar la deserializaci�n.
		*/
		void serialize(Net::CBuffer &data);

		/**
		Deserializa un mensaje. A partir de un CBuffer genera
		un nuevo mensaje.

		@param message Mensaje resultante de la deserializaci�n.
		*/
		void deserialize(Net::CBuffer &data);
	

	private:
		/**
		bool del mensaje
		*/
		bool _bool;



	}; //class CmessageBool

	REG_MFACTORY(CMessageBool);

} // namespace Logic

#endif // __Logic_Message_H