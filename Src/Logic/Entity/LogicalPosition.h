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
#ifndef __Logic_LogicalPosition_H
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
		const enum Ring{	
			ANILLO_INFERIOR=0,
			ANILLO_CENTRAL=1,
			ANILLO_SUPERIOR=2
		};
		const enum Sense{ //Quiz� mejor sentido angunos positivos/negativos?	
			IZQUIERDA=0,
			DERECHA=1
		};
		

	}

	//Coordenadas comunes a todas las entidades del juego.
	
	/**
	Tipo copia para los anillos. Por simplicidad.
	*/
	typedef LogicalPosition::Ring Ring;
	typedef LogicalPosition::Sense Sense;

	
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
		Ring _ring;
		/**
			Sentido
		*/
		Sense _sense;
		/**
			N�mero de base: 1-8
		*/
		unsigned short _base;
		/**
			Grados
		*/	
		float _degrees;


		/**
			Mi intenci�n es quitarlo m�s adelante, al decir el anillo ya est�s diciendo el radio, este no cambia jam�s durante la ejecuci�n
		*/	
		//float _radio;

		/**
			Anchura de la entidad gr�fica en grados
		*/	
		float _angularBox;
	}TLogicalPosition; 

} // namespace Logic

#endif // __Logic_LogicalPosition_H