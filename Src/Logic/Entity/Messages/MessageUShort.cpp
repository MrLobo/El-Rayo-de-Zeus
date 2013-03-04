/**
@file MessageShort.cpp

Contiene definici�n de funciones de serializaci�n y deserializaci�n de mensajes.

@see Logic::CMessageUShort

@author Emilio Santalla Comellas
*/


#include "MessageUShort.h"


namespace Logic 
{
	IMP_MFACTORY(CMessageUShort);

	void CMessageUShort::serialize(Net::CBuffer &data)
	{
		CMessage::serialize(data);
		data.write((void*) (&_short),	sizeof(_short));
					
	} // serialize
	
	//---------------------------------------------------------

	void CMessageUShort::deserialize(Net::CBuffer &data)
	{

		CMessage::deserialize(data);
		data.read(&_short,  sizeof(_short));
			
	} // deserialize

} // namespace Logic