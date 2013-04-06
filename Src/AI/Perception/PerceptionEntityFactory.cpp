#include "PerceptionEntityFactory.h"

#include "SightSensor.h"
#include "PerceptionSignal.h"

namespace AI 
{

	/**
	 * Devuelve una entidad de percepci�n a partir de una cadena de texto que identifica su tipo.
	 *
	 * @param type Tipo de entidad de percepci�n. Por ahora s�lo admite "enemy" y "player"
	 * @param radius Radio de la entidad de percepci�n
	 * @param userData Datos de usuario asociados a la entidad de percepci�n
	 * @param listener Listener que ser� notificado cuando la entidad perciba una se�al
	 * @return CPerceptionEntity del tipo correspondiente al par�metro type
	 */
	CPerceptionEntity* CPerceptionEntityFactory::getPerceptionEntity(string type, float radius, void* userData, IPerceptionListener* listener)
	{
		if (type == "enemy") {
			return new CPerceptionEntityEnemy(userData, listener, radius);
		} else if (type == "player") {
			return new CPerceptionEntityPlayer(userData, listener, radius);
		} else {
			return NULL;
		}
	}

	/**
	 * Crea una entidad de percepci�n que representa a un enemigo.
	 *
	 * Esta clase s�lo tiene un sensor de visi�n b�sico (AI::CSightSensor), inicializado
	 * con los siguientes par�metros:
	 * � active = true
	 * � threshold = 1.0
	 * � maxDistance = 200.0
	 * � alpha = PI / 4
	 *
	 * Cuidado: estos par�metros son datos y, por lo tanto, deber�an declararse en un archivo de configuraci�n externo.
	 *
	 *
	 * @author Gonzalo Fl�rez
	 * @date 11/04/2011
	 */
	CPerceptionEntityEnemy::CPerceptionEntityEnemy(void* userData, IPerceptionListener* listener, float radius) :
		CPerceptionEntity(userData, listener, radius)
	{
		// Esta clase s�lo tiene un sensor de visi�n b�sico (AI::CSightSensor), inicializado
		// con los siguientes par�metros:
		// � active = true
		// � threshold = 1.0
		// � maxDistance = 200.0
		// � alpha = PI / 4
		//
		// Cuidado: estos par�metros son datos y, por lo tanto, deber�an declararse en un archivo de configuraci�n externo.

		CSightSensor * sight = new CSightSensor(this, true, 1.0f, 200.0f, Math::PI / 4.0f);
		this->addSensor(sight);
	}

	/**
	 * Clase de entidad de percepci�n que representa al jugador
	 *
	 * Esta clase s�lo tiene una se�al con los siguientes par�metros:
	 * � type = PERCEPTION_SIGHT (es decir, que la se�al es de visibilidad)
	 * � intensity = 1.0
	 * � delay = 0.0 (instant�nea)
	 * � isActive = true
	 * � keepAlive = true (la se�al no se destruye despu�s de un ciclo de percepci�n, sino que sigue activa)
	 *
	 * Cuidado: estos par�metros son datos y, por lo tanto, deber�an declararse en un archivo de configuraci�n aparte.
	 *
	 *
	 * @author Gonzalo Fl�rez
	 * @date 11/04/2011
	 */
	CPerceptionEntityPlayer::CPerceptionEntityPlayer(void* userData, IPerceptionListener* listener, float radius) :
		CPerceptionEntity(userData, listener, radius)
	{
		// Esta clase s�lo tiene una se�al con los siguientes par�metros:
		// � type = PERCEPTION_SIGHT (es decir, que la se�al es de visibilidad)
		// � intensity = 1.0
		// � delay = 0.0 (instant�nea)
		// � isActive = true
		// � keepAlive = true (la se�al no se destruye despu�s de un ciclo de percepci�n, sino que sigue activa)
		//
		// Cuidado: estos par�metros son datos y, por lo tanto, deber�an declararse en un archivo de configuraci�n aparte.

		CPerceptionSignal* signal = new CPerceptionSignal(this, PERCEPTION_SIGHT, 1.0f, 0.0f, true, true);
		this->addSignal(signal);
	}


}
