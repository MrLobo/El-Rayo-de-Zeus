/**
@file MessageString.cpp

Contiene definici�n de funciones de serializaci�n y deserializaci�n de mensajes.
@see Logic::CMessageString

@author Emilio Santalla Comellas
*/


#include "MessageString.h"


namespace Logic 
{
	IMP_MFACTORY(CMessageString);

	void CMessageString::serialize(Net::CBuffer &data)
	{
		CMessage::serialize(data);

		
		unsigned int size = _string.size();  
		data.write(&size,sizeof(size));		
		data.write((void*)_string.c_str(),size);
		
					
	} // serialize
	
	//---------------------------------------------------------

	void CMessageString::deserialize(Net::CBuffer &data)
	{

		CMessage::deserialize(data);


		unsigned int size;
			data.read(&size, sizeof(size)); 		
		char* aux = new char[size];		
			data.read(aux, size);
			_string.assign(aux,size); 
			delete[] aux;

		
			
	} // deserialize

} // namespace Logic