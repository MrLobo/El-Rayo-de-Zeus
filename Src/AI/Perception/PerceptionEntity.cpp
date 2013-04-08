#include "PerceptionEntity.h"

#include "Sensor.h"
#include "PerceptionSignal.h"

using namespace std;

namespace AI 
{

	/**
	 * Destructor.
	 * El destructor se responsabiliza de eliminar el contenido de las listas de sensores y de se�ales de la entidad
	 */
	CPerceptionEntity::~CPerceptionEntity(void)
	{
		// Eliminamos los sensores
		for (list<CSensor*>::iterator it = _sensors.begin();
			it != _sensors.end(); it++)
		{
			delete (*it);
		}
		// Y las se�ales
		for (list<CPerceptionSignal*>::iterator it = _signals.begin();
			it != _signals.end(); it++)
		{
			delete (*it);
		}
	}

	/**
	 * Elimina un sensor de la entidad
	 */
	void CPerceptionEntity::removeSensor(CSensor* sensor)
	{
		for (list<CSensor*>::iterator it = _sensors.begin();
			it != _sensors.end(); it++)
		{
			if (sensor == (*it))
				_sensors.erase(it);
			break;
		}
	}

	/**
	 * Elimina una se�al de la entidad
	 */
	void CPerceptionEntity::removeSignal(CPerceptionSignal* signal)
	{
		for (list<CPerceptionSignal*>::iterator it = _signals.begin();
			it != _signals.end(); it++)
		{
			if (signal== (*it))
				_signals.erase(it);
			break;
		}
	}


} // namespace AI 
