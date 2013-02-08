/**
@file MessageFloat.cpp

Contiene definici�n de funciones de serializaci�n y deserializaci�n de mensajes.

@see Logic::CMessageFloat

@author Emilio Santalla Comellas
*/


#include "MessageFloat.h"


namespace Logic 
{
	IMP_MFACTORY(CMessageFloat);

	void CMessageFloat::serialize(Net::CBuffer &data)
	{
		CMessage::serialize(data);
		data.write((void*) (&_float),	sizeof(_float));
					
	} // serialize
	
	//---------------------------------------------------------

	void CMessageFloat::deserialize(Net::CBuffer &data)
	{

		CMessage::deserialize(data);
		data.read(&_float,  sizeof(_float));
			
	} // deserialize

} // namespace Logic