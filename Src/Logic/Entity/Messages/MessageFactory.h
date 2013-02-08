/**
@file MessageFactory.h

Contiene la declaraci�n de la clase CMessageFactory, Singleton que almacena 
los punteros a funci�n de funciones de creaci�n de mensajes (CMessage).

@see Logic::CMessageFactory
@see BaseSubsystems::CFactory
@see Logic::CMessage

@author Emilio Santalla
@date Julio, 2010
*/
#ifndef __Logic_MessageFactory_H
#define __Logic_MessageFactory_H

#include "BaseSubsystems/Factory.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Logic 
{
	class CMessage;
}

// Declaraci�n de la clase
namespace Logic
{
	/**
	Factor�a de mensajes. Extiende la plantilla BaseSubsystems::CFactory
	para CMessage haciendo de ella un singleton por lo que solo habr� una 
	instancia de esta clase.
	<p>
	Es usada para almacenar mesanjes 
	en Logic::CMessageFactory donde todo el registro de componentes se realiza 
	utilizando macros del preprocesador declarados en Logic::CMessage.

	@ingroup logicGroup
	@ingroup mapGroup

	@author Emilio Santalla
	@date Enero, 2013
	*/
	class CMessageFactory : public BaseSubsystems::CFactory<CMessage*>
	{
	public:

		/**
		Devuelve la �nica instancia de la clase CMessageFactory.
		
		@return �nica instancia de la clase CMessageFactory.
		*/
		static CMessageFactory* getSingletonPtr();

		/**
		Destructor.
		*/
		~CMessageFactory();

	protected:
		/**
		Constructor.
		*/
		CMessageFactory ();

	private:
		/**
		�nica instancia de la clase.
		*/
		static CMessageFactory* _instance;

	}; // class CMessageFactory

} // namespace Logic

#endif // __Logic_ComponentFactory_H