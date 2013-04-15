/**
 * @file Sensor.h
 *
 * En este fichero se define la clase que representa un sensor.
 * Los sensores representan la capacidad de percibir determinados tipos de est�mulo
 * por parte de las entidades.
 *
 * @author Gonzalo Fl�rez
 * @date 11/04/2011
 */
#pragma once

#ifndef __AI_Sensor_H
#define __AI_Sensor_H

#include "BaseSubsystems/Math.h"

// #include "PerceptionSignal.h"
#include "PerceptionManager.h"

namespace AI 
{

	class CPerceptionSignal;

	/**
	 * Clase abstracta que representa un sensor.
	 * Los sensores representan la capacidad de percibir determinados tipos de est�mulo
	 * por parte de las entidades. Los est�mulos se representan mediante se�ales (AI::CPerceptionSignal).
	 *
	 * Toda entidad que tiene que percibir un tipo de se�al debe tener al menos un sensor asociado.
	 *
	 * Cada sensor que herede de esta clase tendr� que implementar al menos el m�todo getType (identifica el
	 * tipo de se�al que puede percibir) y perceives. El m�todo perceives ser� invocado por el gestor de
	 * percepci�n para cada se�al. En este m�todo es donde se incluye todo el c�digo necesario para
	 * comprobar si un sensor detecta una se�al determinada.
	 *
	 * @author Gonzalo Fl�rez
	 * @date 11/04/2011
	 */
	class CSensor
	{
	public:
		/**
		 * Constructor
		 *
		 * @param pEntity Entidad de percepci�n a la que est� asociado el sensor
		 * @param active Valor booleano que indica si el sensor est� activo
		 * @param threshold Valor m�nimo de intensidad que debe tener una se�al para activar el sensor
		 * @return return
		 */
		CSensor(CPerceptionEntity* pEntity, bool active, float threshold) : 
		  _pEntity(pEntity), _active(active), _threshold(threshold) {};
		/**
		 * Destructor
		 */
		virtual ~CSensor(void) {};
		/**
		 * Este m�todo ser� implementado por cada sensor con las comprobaciones necesarias para averiguar
		 * si el sensor es capaz de percibir una se�al determinada.
		 *
		 * Cada sensor necesitar� realizar unas comprobaciones diferentes seg�n su tipo y sus caracter�sticas.
		 *
		 * Este m�todo va a ser llamada con mucha frecuencia para cada sensor, por lo que es importante que
		 * sea bastante eficiente. En general, lo que se hace es realizar en primer lugar las comprobaciones
		 * que consumen menor tiempo de proceso, dejando las m�s pesadas para el final. De esta manera
		 * podemos detectar que el sensor no puede percibir una se�al antes de realizar las operaciones
		 * menos eficientes.
		 *
		 * Si el sensor no puede detectar la se�al devuelve NULL. Si la detecta, devuelve una nueva instancia de
		 * AI::CNotification (declarado en PerceptionManager.h) con los detalles necesarios.
		 *
		 * Es importante destacar que el Sensor produce la notificaci�n pero NO SE RESPONSABILIZA de destruirla.
		 *
		 * @param perceptible Se�al cuya percepci�n queremos comprobar
		 * @param time Instante en el que se realiza la comprobaci�n de percepci�n
		 * @return NULL si no se ha percibido la se�al. Una instancia de CNotification en caso contrario.
		 */
		virtual CNotification* perceives(const CPerceptionSignal * perceptible, unsigned int time) = 0;

		/**
		 * Tipo de sensor. Se corresponde con un elemento del enumerado AI::EnmPerceptionType (definido en
		 * PerceptionManager.h).
		 *
		 * @return Tipo de sensor
		 */
		virtual EnmPerceptionType getType() = 0;

		/**
		 * Devuelve true si el sensor est� activo, false en caso contrario
		 */
		bool isActive() { return _active; };
		/**
		 * Devuelve la entidad de percepci�n asociada al sensor
		 */
		CPerceptionEntity* getPerceptionEntity() { return _pEntity; };
		/**
		 * Devuelve el valor m�nimo de intensidad de la se�al para ser percibida por el sensor
		 */
		float getThreshold() const { return _threshold; };

	protected:
		/**
		 * Indica si el sensor est� activo
		 */
		bool _active;
		/**
		 * Entidad de percepci�n asociada al sensor
		 */
		CPerceptionEntity* _pEntity;
		/**
		 * Valor m�nimo de intensidad de la se�al para ser percibida por el sensor
		 */
		float _threshold;

	};

} // namespace AI 

#endif __AI_Sensor_H
