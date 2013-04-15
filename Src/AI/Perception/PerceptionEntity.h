/**
 * @file PerceptionEntity.h
 *
 * En este fichero se encuentra la declaraci�n de la clase CPerceptionEntity,
 * que representa a las entidades que pueden percibir o ser percibidas (o
 * ambas cosas) dentro del gestor de percepci�n.
 *
 * @author Gonzalo Fl�rez
 * @date 11/04/2011
 */

#pragma once

#ifndef __AI_PerceptionEntity_H
#define __AI_PerceptionEntity_H

#include <list>
#include "PerceptionListener.h"
#include "BaseSubsystems/Math.h"



namespace AI {

class CSensor;
class CPerceptionSignal;
class CNotification;

/**
 * La clase CPerceptionEntity representa a las entidades que pueden percibir o
 * ser percibidas (o ambas cosas) dentro del gestor de percepci�n.
 * Cada entidad de percepci�n tiene una lista de sensores, que le da la capacidad
 * de percibir, y de se�ales, que permiten a otras entidades percibirla.
 *
 * En cada tick de IA el gestor recorrer� los sensores de la entidad y comprobar� si
 * alguno de ellos se activa. Por otra parte, recorrer� las se�ales asociadas a la
 * entidad comprobando tambi�n si activan alguno de los sensores de las dem�s entidades.
 *
 * El campo userData permite asociar a la entidad de percepci�n con cualquier dato
 * que pueda ser �til durante el proceso de percepci�n (en general nos interesar�
 * asociarla a la entidad l�gica).
 *
 * La entidad de percepci�n tiene una matriz de transformaci�n que ser� utilizada
 * por los sensores que as� lo requieran para obtener datos como la posici�n o la
 * orientaci�n. Esta matriz tiene que ser actualizada de manera expl�cita (en
 * general, por la entidad l�gica a la que est� asociada la entidad de percepci�n).
 *
 *
 *
 * @author Gonzalo Fl�rez
 * @date 11/04/2011
 */
class CPerceptionEntity
{
public:
	/**
	 * Constructor
	 *
	 * @param userData Datos de usuario asociados a la entidad de percepci�n
	 * @param listener Clase que ser� notificada cuando la entidad de percepci�n reciba una notificaci�n
	 * @param radius Radio de la entidad de percepci�n
	 */
	CPerceptionEntity(void* userData, IPerceptionListener* listener, float radius) : _userData(userData), _listener(listener), _radius(radius) {};
	/**
	 * Destructor
	 * El destructor se responsabiliza de eliminar el contenido de las listas de sensores y de se�ales de la entidad
	 */
	~CPerceptionEntity(void);

	/**
	 * Devuelve los datos de usuario asociados a la entidad de percepci�n
	 */
	void* getUserData() {return _userData;};
	// TODO Esto tendr�a que devolver un const y que sea referencia
	/**
	 * Devuelve la lista de sensores de la entidad
	 */
	const std::list<CSensor*>& getSensors() { return _sensors; };
	// TODO Esto tendr�a que devolver un const y que sea referencia
	/**
	 * Devuelve la lista de se�ales de la entidad
	 */
	const std::list<CPerceptionSignal*>& getSignals() { return _signals; };
	/**
	 * A�ade un sensor a la entidad
	 */
	void addSensor(CSensor* sensor) { _sensors.push_back(sensor); };
	/**
	 * A�ade una se�al a la entidad
	 */
	void addSignal(CPerceptionSignal* signal) { _signals.push_back(signal); };
	/**
	 * Elimina un sensor de la entidad
	 */
	void removeSensor(CSensor* sensor);
	/**
	 * Elimina una se�al de la entidad
	 */
	void removeSignal(CPerceptionSignal* signal);
	/**
	 * Env�a una notificaci�n a su listener
	 */
	void sendNotification(CNotification* notification) { _listener->notificationPerceived(notification); };
	/**
	 * Actualiza la matriz de transformaci�n de la entidad de percepci�n
	 */
	void setTransform(const Matrix4& transform) { _transform = transform; };
	/**
	 * Devuelve la matriz de transformaci�n de la entidad de percepci�n
	 */
	Matrix4& getTransform() { return _transform; };
	/**
	 * Devuelve el radio de la entidad de percepci�n
	 */
	float getRadius() { return _radius; };

private:
	/**
	 * Datos de usuario
	 */
	void* _userData;
	/**
	 * Lista de sensores
	 */
	std::list<CSensor*>  _sensors;
	/**
	 * Lista de se�ales
	 */
	std::list<CPerceptionSignal*>  _signals;
	/**
	 * Clase que ser� notificada cuando alguno de los sensores se active
	 */
	IPerceptionListener* _listener;
	/**
	 * Matriz de transformaci�n
	 */
	Matrix4 _transform;
	/**
	 * Radio de la entidad de percepci�n
	 */
	float _radius;
	
}; // class CPerceptionEntity

} // namespace AI 

#endif __AI_PerceptionEntity_H
