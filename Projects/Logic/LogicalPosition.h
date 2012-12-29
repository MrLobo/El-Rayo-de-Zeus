//---------------------------------------------------------------------------
// LogicalPosition.h
//---------------------------------------------------------------------------


/**
@file LogicalPosition.h

Contiene el tipo de datos de un mensaje.

@see Logic::LogicalPosition

@author Jose Luis L�pez S�nchez
*/
#pragma once

#define __Logic_LogicalPosition_H

#include <string>

#include "BaseSubsystems/Math.h"

namespace Logic
{
	/**
	Namespace para los tipos de mensajes posibles.
	*/
	namespace LogicalPosition
	{					
		/**
			Ambito actual hasta que se cambie de base y/o anillo
		*/
		enum Ring{	
			ANILLO_INFERIOR,
			ANILLO_CENTRAL,
			ANILLO_SUPERIOR
		};
	}
		//Coordenadas comunes a todas las entidades del juego.
	
	/**
	Tipo copia para los anillos. Por simplicidad.
	*/
	typedef LogicalPosition::Ring Ring;
	
	/**
	Contiene el tipo de datos de una posici�n l�gica de representaci�n espacial dentro de nuestro juego
	
    @ingroup logicGroup
    @ingroup entityGroup

	@author Jos� Luis L�pez S�nchez
	@date Diciembre, 2012
    @ingroup grupoEntidad
	*/
	typedef struct
	{
		/**
			Anillo
		*/
		Ring _type;
		/**
			N�mero de base: 1-8
		*/
		short base;
		/**
			Grados en radianes: 1-8
		*/	
		float _degrees;

		/**
			Mi intenci�n es quitarlo m�s adelante, al decir el anillo ya est�s diciendo el radio, este no cambia jam�s durante la ejecuci�n
		*/	
		float _radio;

	} LogicalPosition; 

} // namespace Logic

