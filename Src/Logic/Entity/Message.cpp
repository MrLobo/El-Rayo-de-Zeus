/**
@file Message.cpp

Contiene definici�n de funciones de serializaci�n y deserializaci�n de mensajes.

@see Logic::TMessage

@author David Llans�
@date Diciembre, 2010
*/

#include "Message.h"
#include "Entity.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"
#include "NET/serializable.h"

// TODO serializar y deserializar s�lo lo necesario
namespace Logic 
{
	void Message::Serialize(const TMessage &message, Net::CBuffer &txSerialMsg)
	{		
		txSerialMsg.write((void*) (&message._type),	sizeof(message._type));	// Serializamos el tipo
		
		// TODO TIPOS PRIMITIVOS: campos _int, _float y _bool.
		txSerialMsg.write((void*) (&message._int),		sizeof(message._int)); 
		txSerialMsg.write((void*) (&message._float),	sizeof(message._float));
		txSerialMsg.write((void*) (&message._bool),	sizeof(message._bool));

		// STRING
		Net::Serializable::serializeString(txSerialMsg, message._string);

		// Serializamos el campo con una posicion
		txSerialMsg.write((void*) &message._vector3.x, sizeof(message._vector3.x));
		txSerialMsg.write((void*) &message._vector3.y, sizeof(message._vector3.y));
		txSerialMsg.write((void*) &message._vector3.z, sizeof(message._vector3.z));

		/* TODO TRANSFORM serializar el campo _transform.
			Quiz� solo habr�a que hacerlo si _type == SET_TRANSFORM? */
		for(int i = 0; i < 4; ++i)
			for(int j = 0; j < 4; ++j)
				txSerialMsg.write((void*)(&message._transform[i][j]), sizeof(message._transform[i][j]));
		
		/* ENTIDAD: Serializamos el campo con una entidad. 
					Lo que hacemos es  mandar el ID para su recuperaci�n */
		Logic::TEntityID id;
			if(message._entity)
				id = message._entity->getEntityID();
			else
				id = EntityID::UNASSIGNED;
		txSerialMsg.write(&id, sizeof(id));

	} // Serialize
	
	//---------------------------------------------------------

	void Message::Deserialize(Net::CBuffer &rxSerialMsg, TMessage& message)
	{
		// Deserializamos el tipo
		rxSerialMsg.read(&message._type, sizeof(message._type));
		
		// TODO PRIMITIVOS: deserializar los campos _int, _float y _bool.
		rxSerialMsg.read(&message._int,   sizeof(message._int));
		rxSerialMsg.read(&message._float, sizeof(message._float));
		rxSerialMsg.read(&message._bool,  sizeof(message._bool));

		message._bool |= message._type == TMessageType::SET_TRANSFORM;
			 
		// STRING	
		message._string.assign(	Net::Serializable::deserializeString(rxSerialMsg) );

		// Deserializamos el campo con una posici�n
		rxSerialMsg.read(&message._vector3.x, sizeof(message._vector3.x));
		rxSerialMsg.read(&message._vector3.y, sizeof(message._vector3.y));
		rxSerialMsg.read(&message._vector3.z, sizeof(message._vector3.z));
		
		// TODO deserializar el campo _transform.
		// TODO reserva din�mica?
		// Deserializamos el campo con una transformada
		for(int i = 0; i < 4; ++i)
			for(int j = 0; j < 4; ++j)
				rxSerialMsg.read(&message._transform[i][j], sizeof(message._transform[i][j]));
		// TODO ENTIDAD: Deserializar el campo con una entidad. Con el id sacarla del mapa		 
		//HACK 	// Si en alg�n momento hay m�s de un mapa habr� que modificar esto
		Logic::TEntityID id;
			rxSerialMsg.read(&id,  sizeof(id));
			if(id == EntityID::UNASSIGNED)
				message._entity = 0;
			else
				message._entity = CServer::getSingletonPtr()->getMap()->getEntityByID(id);
	} // Deserialize

} // namespace Logic
