/**
 * @file PerceptionSignal.h
 *
 * En este fichero se define la clase que representa una se�al de percepci�n.
 * Una se�al representa de manera abstracta a todo aquello que puede ser percibido
 * por un sensor.
 *
 * @author Gonzalo Fl�rez
 * @date 11/04/2011
 */

#pragma once

#ifndef __AI_PerceptionSignal_H
#define __AI_PerceptionSignal_H

#include "PerceptionManager.h"

namespace AI
{

	/**
	 * Clase que representa las se�ales de percepci�n.
	 * Una se�al representa de manera abstracta a todo aquello que puede ser percibido
	 * por un sensor.
	 *
	 * Las propiedades de la se�al son las que marcan las diferencias entre los tipos de se�ales.
	 *
	 * @author Gonzalo Fl�rez
	 * @date 11/04/2011
	 */
	class CPerceptionSignal
	{
	public:
		/**
		 * Constructor
		 *
		 * @param pEntity Entidad de percepci�n a la que pertenece la se�al
		 * @param type Tipo de la se�al. Los tipos est�n definidos en el enumerado AI::EnmPerceptionType en PerceptionManager.h
		 * @param intensity Intensidad de la se�al
		 * @param isActive Booleano que indica si la se�al est� activa
		 * @param keepAlive Booleano que indica si la se�al se debe mantener activa una vez que se ha procesado
		 */
		CPerceptionSignal(CPerceptionEntity* pEntity, int type, float intensity = 0.0f, float delay = 0.0f, 
			bool isActive = true, bool keepAlive = true) : _pEntity(pEntity), _active(isActive), 
			_keepAlive(keepAlive), _type(type), _intensity(intensity), _delay(delay) {};
		/**
		 * Destructor
		 */
		~CPerceptionSignal(void) {};
		/**
		 * Devuelve true si la se�al est� activa en este momento
		 */
		bool isActive() const { return _active;};
		/**
		 * Devuelve true si la se�al debe mantenerse activa
		 */
		bool keepAlive() const { return _keepAlive; };
		/**
		 * Devuelve la intensidad de la se�al
		 */
		float getIntensity() const { return _intensity; };
		/**
		 * Devuelve el tipo de la se�al
		 */
		int getType() const { return _type; };
		/**
		 * Devuelve la entidad de percepci�n que ha lanzado la se�al
		 */
		CPerceptionEntity* getPerceptionEntity() const { return _pEntity; }
		/**
		 * Devuelve el retardo de la se�al
		 */
		float getDelay() { return _delay; };

	private:
		/**
		 * Entidad a la que pertenece la se�al
		 */
		CPerceptionEntity* _pEntity;
		/**
		 * Indica si la se�al est� activa (puede ser percibida)
		 */
		bool _active;
		/**
		 * Indica si la se�al se mantiene activa para siempre o
		 * si debe ser eliminada despu�s de ser evaluada una vez
		 */
		bool _keepAlive;
		/**
		 * Tipo de la se�al. Es un elemento del enumerado AI::EnmPerceptionType,
		 * definido en PerceptionManager.h
		 */
		int _type;
		/**
		 * Intensidad de la se�al. Los sensores la utilizan para comprobar
		 * si la se�al es lo bastante fuerte para que la detecten.
		 */
		float _intensity;
		/**
		 Retardo de la se�al. Es la inversa de la velocidad a la que se 
		 desplaza la se�al. Es decir, es el tiempo que tardar�a la se�al en 
		 recorrer 1 unidad de distancia.
		 Al usar el retardo en lugar de la velocidad podemos crear se�ales de
		 transmisi�n instant�nea, d�ndole un retardo de 0 (la otra opci�n ser�a 
		 darle una velocidad infinita)
		 */
		float _delay;

	}; // class CPerceptionSignal

} // namespace AI

#endif __AI_PerceptionSignal_H
