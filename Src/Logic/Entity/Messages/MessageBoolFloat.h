/**
@file MessageBoolFloat.h

Contiene el tipo de datos de un mensaje float.

@see Logic::CMessageBoolFloat

@author Emilio Santalla Comellas
*/

#ifndef __Logic_MessageBoolFloat_H
#define __Logic_MessageBoolFloat_H


#include "Net/buffer.h"
#include "Message.h"


// Predeclaracion de clases
namespace Logic {
	class CEntity;
};


namespace Logic
{	
	// Declaraci�n de la clase
	class CMessageBoolFloat : public CMessage
	{
		DEC_MFACTORY(CMessageBoolFloat);
	public:

		/**
		Constructor b�sico
		*/
		CMessageBoolFloat() :  CMessage(GetAltTypeIdOf(CMessageBoolFloat)), _bool(false), _float(0.0f) {}
			

		~CMessageBoolFloat() {};


		float getBool() const { return _bool; }
		void setBool(const bool valor) {_bool=valor; }


		float getFloat() const { return _float; }
		void setFloat(const float valor) {_float=valor; }


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


		/**
		float del mensaje
		*/
		float _float;

	}; //class CmessageBoolFloat

	REG_MFACTORY(CMessageBoolFloat);

} // namespace Logic

#endif // __Logic_Message_H