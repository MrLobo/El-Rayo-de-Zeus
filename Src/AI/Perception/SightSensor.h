/**
 * @file SightSensor.h
 *
 * En este fichero se define un sensor de visi�n b�sico
 *
 * @author Gonzalo Fl�rez
 * @date 11/04/2011
 */
#pragma once

#ifndef __AI_SightSensor_H
#define __AI_SightSensor_H

#include "sensor.h"


namespace AI 
{

	/**
	 * Clase que implementa un sensor de visi�n b�sico. Este sensor comprueba si la se�al
	 * se encuentra dentro de un cono de visi�n.
	 *
	 * El cono est� definido por dos par�metros: la distancia y el �ngulo. La distancia indica
	 * la separaci�n m�xima que puede existir entre el sensor y la se�al para que �sta sea
	 * percibida. El �ngulo da el sector circular dentro del que tiene que estar la se�al
	 * para poder percibirla. El �ngulo se da en relaci�n a la direcci�n hacia la que apunta
	 * la entidad de percepci�n del sensor. Por ejemplo, si la entidad est� mirando en la direcci�n
	 * dir y el �ngulo es PI/3 (60�), el cono abarcar�a desde dir - PI/3 hasta dir + PI/3. Los valores
	 * para el �ngulo estar�n en el intervalo [0, PI].
	 *
	 *
	 * @author Gonzalo Fl�rez
	 * @date 11/04/2011
	 */
	class CSightSensor : public CSensor
	{
	public:
		/**
		 * Constructor. Inicializa los par�metros b�sicos
		 *
		 * @param pEntity Entidad a la que pertenece el sensor
		 * @param active Indica si el sensor est� activo (puede percibir) o no
		 * @param threshold Intensidad m�nima de la se�al para que el sensor pueda percibir
		 * @param maxDistance Distancia m�xima a la que puede percibirse
		 * @param alpha �ngulo del cono.
		 * @return return
		 */
		CSightSensor(CPerceptionEntity* pEntity, bool active, float threshold, float maxDistance, float alpha)
			: CSensor(pEntity, active, threshold), _maxDistance(maxDistance), _alpha(alpha) {} ;
		/**
		 * Destructor
		 */
		~CSightSensor(void);

		/**
		 * Realiza todas las comprobaciones necesarias para averiguar si la se�al recibida
		 * se encuentra dentro del cono de visi�n.
		 *
		 * Las comprobaciones se realizan de m�s "barata" a m�s "cara"
		 * 1. Comprueba si el sensor y la se�al est�n activos
		 * 2. Comprueba si el tipo de la se�al se corresponde con el tipo que percibe el sensor.
		 * 3. Comprueba si la intensidad de la se�al est� por encima del threshold del sensor.
		 * 4. Comprueba la distancia entre la se�al y el sensor. Se realiza en 2 pasos:
		 * 4.1. Primero se comprueba si la distancia en cada dimensi�n es mayor que la distancia m�xima.
		 * 4.2. A continuaci�n se comprueba si la distancia eucl�dea es mayor que la distancia m�xima.
		 * 5. Comprobamos si la se�al se encuentra dentro del �ngulo de amplitud del cono de visi�n.
		 * 6. Comprobamos si no existe ning�n objeto f�sico entre el sensor y la se�al. Para eso usamos un rayo f�sico.
		 *
		 * En el momento en que alguna de estas comprobaciones falle se detiene el proceso y se devuelve
		 * NULL,  lo que significa que el sensor no es capaz de detectar la se�al. Por otro lado, si todas se
		 * superan con �xito se devuelve una notificaci�n.
		 *
		 * @param perceptible Se�al cuya percepci�n queremos comprobar
		 * @param time Instante en el que se realiza la comprobaci�n de percepci�n
		 * @return NULL si no se ha percibido la se�al. Una instancia de CNotification en caso contrario.
		 */
		virtual CNotification* perceives(const CPerceptionSignal * perceptible, unsigned int time);
		/**
		 * Devuelve el tipo de sensor. En este caso un sensor de visi�n.
		 */
		virtual EnmPerceptionType getType() { return PERCEPTION_SIGHT; };

	private:
		/**
		 * Distancia m�xima del cono
		 */
		float _maxDistance;
		/**
		 * Amplitud m�xima del cono
		 */
		float _alpha;
	
	}; // class CSightSensor

} // namespace AI 

#endif __AI_SightSensor_H
