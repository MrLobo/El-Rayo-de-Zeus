#include "PerceptionManager.h"

#include "Application/BaseApplication.h"

#include "BaseSubsystems/Server.h"
#include <OgreTimer.h>

#include "Sensor.h"
#include "PerceptionEntity.h"
#include "PerceptionSignal.h"

using namespace std;

namespace AI 
{

	/**
	 * Elimina una entidad de percepci�n de la lista de entidades gestionadas.
	 * Esto evita que se realicen las comprobaciones de percepci�n sobre sus
	 * sensores y se�ales y que reciba notificaciones.
	 *
	 * @param entity Entidad de percepci�n que se va a desregistrar
	 */
	void CPerceptionManager::unregisterEntity(CPerceptionEntity* entity)
	{
		for (list<CPerceptionEntity*>::iterator it = _entities.begin();
			it != _entities.end(); it++)
		{
			if (entity == (*it))
				_entities.erase(it);
			break;
		}
	}

	/**
	 * Comprueba los sensores de las entidades registradas
	 *
	 * @param time Instante de tiempo en que se realiza la comprobaci�n
	 */
	void CPerceptionManager::checkPerception(unsigned int time)
	{
		// Para cada entidad que puede percibir
		for (list<CPerceptionEntity*>::iterator itSensers = _entities.begin();
			itSensers != _entities.end(); itSensers++)
		{
			CPerceptionEntity* senser = (*itSensers);
			// Para cada entidad que puede ser percibida
			for (list<CPerceptionEntity*>::iterator itSignalers = _entities.begin();
				itSignalers != _entities.end(); itSignalers++)
			{
				// No permitimos que una entidad se perciba a ella misma
				if (senser != (*itSignalers)) 
				{
					CPerceptionEntity* signaler = (*itSignalers);
					// Por cada signal del signaler. 
					// No incrementamos el iterador cada vuelta porque queremos borrar las se�ales temporales
					// Si borramos la se�al, el iterador se incrementa solo, si no, hay que hacerlo manualmente
					list<CPerceptionSignal*> signals = signaler->getSignals();
					list<CPerceptionSignal*>::iterator itSignal = signals.begin();
					while (itSignal != signals.end()) 
					{
						CPerceptionSignal* signal = (*itSignal);
						// Comprobamos si la se�al est� activa
						if (signal->isActive()) 
						{
							// Por cada sensor del senser
							list<CSensor*> sensors = senser->getSensors();
							for (list<CSensor*>::iterator itSensor = sensors.begin();
								itSensor != sensors.end(); itSensor++)
							{
								CSensor* sensor = (*itSensor);
								// Comprobamos si el sensor est� activo
								if (sensor->isActive()) 
								{
									CNotification* notification = sensor->perceives(signal, time);
									if (notification != 0) 
									{
										_notifications.push(notification);
									}
								}
							}
						}
						// Si la se�al es temporal la borramos (esto nos lo indica la propiedad keepAlive).
						// Si no, pasamos directamente a la siguiente incrementando el iterador.
						if (!signal->keepAlive()) 
						{
							delete (*itSignal);
							itSignal = signals.erase(itSignal);
						} else {
							itSignal++;
						}
					}
				}
			}
		}
	}

	/**
	 * El m�todo update se encarga de actualizar la informaci�n
	 * sobre los sensores de cada entidad de percepci�n.
	 *
	 *
	 * @param enclosing_method_arguments
	 * @return
	 */
	void CPerceptionManager::update(unsigned int msecs)
	{
		// Sacamos del reloj de la aplicaci�n el instante actual para
		// gestionar los instantes de las notificaciones
		unsigned int time = Application::CBaseApplication::getSingletonPtr()->getAppTime();
		// Fase de agregaci�n y chequeo:
		// Se buscan los sensores potenciales de las se�ales actuales y se comprueba si detectan las se�ales
		// Las variables notIni, notAdded y notRemoved nos indican cu�ntas notificaciones hay en la cola de
		// pendientes al principio, despu�s de a�adir y despu�s de entregar. Podemos usar estos valores para depurar.
		// int notIni = _notifications.size();
		checkPerception(time);
		// int notAdded = _notifications.size();
		
		// Fase de notificaci�n:
		// En esta fase se env�an las notificaciones cuyo tiempo de entrega haya caducado
		notifyEntities(time);
		//int notRemoved = _notifications.size();

		//if (notAdded != notIni || notRemoved != notIni)
		//	cout << time << ": +" << (notAdded - notIni) << " - " << (notAdded - notRemoved) << endl;
	}

	/**
	 * Recorre la lista de notificaciones pendientes y env�a las
	 * que hayan caducado
	 *
	 * @param time Instante de tiempo en que se realiza la comprobaci�n
	 */
	void CPerceptionManager::notifyEntities(unsigned int time)
	{
		// Si no hay notificaciones pendientes no hacemos nada
		if (_notifications.empty())
			return;

		// Procesamos las notificaciones de la cola cuyo tiempo de entrega sea anterior al actual
		CNotification* notification;
		while (!_notifications.empty() && (notification = _notifications.top())->getTime() <= time)
		{
			_notifications.pop();
			// Entregamos la notificaci�n a la entidad que la ha percibido
			notification->getSensor()->getPerceptionEntity()->sendNotification(notification);
		}	
	}

} // namespace AI 
