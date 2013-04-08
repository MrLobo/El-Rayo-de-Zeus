/**
 * @file PerceptionManager.h
 *
 * Gestor de la percepci�n. En este fichero se declara la clase
 * encargada de la gesti�n de la percepci�n y algunas clases relacionadas.
 *
 *
 * @author Gonzalo Fl�rez
 * @date 11/04/2011
 */

#pragma once

#ifndef __AI_PerceptionManager_H
#define __AI_PerceptionManager_H

#include <list>

#include "BaseSubsystems/Math.h"

namespace AI 
{

using namespace std;

class CPerceptionEntity;
class CSensor;

/**
 * Tipos de percepci�n
 */
enum EnmPerceptionType 
{
	/**
	 * Tipo desconocido
	 */
	PERCEPTION_UNKNOWN,
	/**
	 * Vista
	 */
	PERCEPTION_SIGHT,
	/**
	 * O�do
	 */
	PERCEPTION_HEAR
}; // enum EnmPerceptionType 

/**
 * Clase que representa una notificaci�n que hace el gestor de
 * percepci�n a una entidad de percepci�n cuyo sensor ha percibido
 * una se�al.
 *
 * @author Gonzalo Fl�rez
 * @date 11/04/2011
 */
class CNotification
{
public:
	/**
	 * Constructor que inicializa los par�metros. Existen dos atributos que ser�n excluyentes. entity se utilizar� en los casos
	 * en los que la detecci�n implique que se est� percibiendo continuamente a la entidad que genera la se�al (por ejemplo,
	 * en el caso de la visi�n). position se utiliza cuando se detecta una se�al, pero no necesariamente a la entidad que la
	 * ha generado (por ejemplo, el ruido de una granada al estallar).
	 *
	 * @param time Tiempo de entrega. Instante de tiempo en que se tiene que realizar la notificaci�n. No tiene porqu� ser el mismo instante en que se crea, sino que puede ser m�s tarde.
	 * @param sensor Sensor que ha percibido la se�al
	 * @param entity Entidad que se ha percibido
	 * @param position Posici�n de la se�al que ha activado el sensor
	 */
	CNotification(unsigned long time, CSensor* sensor, CPerceptionEntity* perceivedEntity, Vector3 position = Vector3::ZERO) : 
		_time(time), _sensor(sensor), _perceivedEntity(perceivedEntity), _position(position) { 
		};
	/**
	 * Devuelve en qu� instante se tiene que realizar la notificaci�n
	 *
	 * @return Instante de tiempo en que se tiene que realizar la notificaci�n
	 */
	unsigned long getTime() { return _time; };
	/**
	 * Devuelve el sensor que ha realizado la percepci�n
	 *
	 * @return Sensor que ha percibido la se�al
	 */
	CSensor* getSensor() { return _sensor; };
	/**
	 * Devuelve la posici�n de la se�al
	 *
	 * @return Posici�n de la se�al que ha activado el sensor
	 */
	Vector3 getPosition() { return _position; };
	/**
	 * Devuelve la entidad que ha generado la se�al percibida
	 *
	 * @return Entidad que se ha percibido
	 */
	CPerceptionEntity* getPerceivedEntity() { return _perceivedEntity; };


private:
	/**
	 * Instante en que se tiene que hacer la notificaci�n
	 * (cu�ndo se ha percibido, no cu�ndo se ha generado la se�al)
	 */
	long _time;
	/**
	 * Sensor a trav�s del que se ha percibido
	 */
	CSensor* _sensor;
	/**
	 * Posici�n de la se�al percibida
	 */
	Vector3 _position;
	/**
	 * Entidad que se ha percibido
	 */
	CPerceptionEntity* _perceivedEntity;
}; // class CNotification

/**
 * Clase auxiliar que compara dos notificaciones usando su tiempo de entrega.
 * Se utiliza para mantener ordenada la lista de notificaciones pendientes
 *
 * @author Gonzalo Fl�rez
 * @date 11/04/2011
 */
class CNotificationComparator 
{
public:
	bool operator() (CNotification* a, CNotification* b)
	{
		return ((a->getTime()) > (b->getTime()));
	};

}; // class CNotificationComparator 


/**
 * Clase gestora de la percepci�n. En esta clase se registran
 * las entidades interesadas en percibir/ser percibidas. La clase
 * se encarga de ordenar las comprobaciones necesarias cada tick
 * para que se lleve a cabo la percepci�n y de enviar las
 * notificaciones a los sensores correspondientes cuando han
 * percibido algo.
 *
 * @author Gonzalo Fl�rez
 * @date 11/04/2011
 */
class CPerceptionManager
{

public:
	/**
	 * Constructor por defecto
	 */
	CPerceptionManager(void) {};
	/**
	 * Destructor por defecto
	 */
	~CPerceptionManager(void) {};

	/**
	 * El m�todo update se encarga de actualizar la informaci�n
	 * sobre los sensores de cada entidad de percepci�n.
	 *
	 *
	 * @param enclosing_method_arguments
	 * @return
	 */
	void update(unsigned int msecs);
	/**
	 * Registra una entidad de percepci�n en el gestor. Una vez que
	 * la entidad est� registrada, se pueden realizar comprobaciones
	 * sobre sus sensores y se�ales y puede recibir notificaciones.
	 *
	 * @param entity Entidad de percepci�n que se va a registrar.
	 */
	void registerEntity(CPerceptionEntity* entity) { 
		cout << _entities.size() << endl;
		_entities.push_back(entity); 
	};
	/**
	 * Elimina una entidad de percepci�n de la lista de entidades gestionadas.
	 * Esto evita que se realicen las comprobaciones de percepci�n sobre sus
	 * sensores y se�ales y que reciba notificaciones.
	 *
	 * @param entity Entidad de percepci�n que se va a desregistrar
	 */
	void unregisterEntity(CPerceptionEntity* entity);

private:
	/**
	 * Lista de entidades de percepci�n gestionadas
	 */
	std::list<CPerceptionEntity*> _entities;
	/**
	 * Lista de notificaciones de percepci�n pendientes de enviar
	 */
	std::priority_queue<CNotification*, vector<CNotification*>, CNotificationComparator> _notifications;

	/**
	 * Comprueba los sensores de las entidades registradas
	 *
	 * @param time Instante de tiempo en que se realiza la comprobaci�n
	 */
	void checkPerception(unsigned int time);
	/**
	 * Recorre la lista de notificaciones pendientes y env�a las
	 * que hayan caducado
	 *
	 * @param time Instante de tiempo en que se realiza la comprobaci�n
	 */
	void notifyEntities(unsigned int time);

}; // class CPerceptionManager

} // namespace AI 

#endif __AI_PerceptionManager_H
