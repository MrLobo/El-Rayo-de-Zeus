/**
@file MessageBoolUShort.cpp

Contiene definici�n de funciones de serializaci�n y deserializaci�n de mensajes.

@see Logic::CMessageBoolUShort

@author Jose Luis L�pez
*/


#include "MessageBoolUShort.h"


namespace Logic 
{
	IMP_MFACTORY(CMessageBoolUShort);

	void CMessageBoolUShort::serialize(Net::CBuffer &data)
	{
		CMessage::serialize(data);
		data.write((void*) (&_bool),	sizeof(_bool));
		data.write((void*) (&_short),	sizeof(_short));
					
	} // serialize
	
	//---------------------------------------------------------

	void CMessageBoolUShort::deserialize(Net::CBuffer &data)
	{
		CMessage::deserialize(data);
		data.read(&_bool,	sizeof(_bool));
		data.read(&_short,  sizeof(_short));
			
	} // deserialize

} // namespace Logic