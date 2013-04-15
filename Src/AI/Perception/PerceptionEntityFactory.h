/**
 * @file PerceptionEntityFactory.h
 *
 * En este fichero definimos una factor�a para crear los distintos tipos de entidades de percepci�n. En este caso
 * s�lo hay dos: "enemy" y "player". "enemy" tiene un sensor de visi�n b�sico (CSightSensor) y "player" tiene
 * una se�al de visi�n para ser percibido por el enemigo.
 *
 * @author Gonzalo Fl�rez
 * @date 11/04/2011
 */

#pragma once

#ifndef __AI_PerceptionEntityFactory_H
#define __AI_PerceptionEntityFactory_H

#include "PerceptionEntity.h"

using namespace std;

namespace AI
{
	/**
	 * Clase factor�a para crear las entidades de percepci�n.
	 *
	 * Tiene un m�todo est�tico, getPerceptionEntity, que recibe una cadena que identifica
	 * la entidad de percepci�n y algunos par�metros, y devuelve una nueva entidad de percepci�n
	 * del tipo correspondiente.
	 *
	 * @author Gonzalo Fl�rez
	 * @date 11/04/2011
	 */
	class CPerceptionEntityFactory
	{
	public:
		/**
		 * Devuelve una entidad de percepci�n a partir de una cadena de texto que identifica su tipo.
		 *
		 * @param type Tipo de entidad de percepci�n. Por ahora s�lo admite "enemy" y "player"
		 * @param radius Radio de la entidad de percepci�n
		 * @param userData Datos de usuario asociados a la entidad de percepci�n
		 * @param listener Listener que ser� notificado cuando la entidad perciba una se�al
		 * @return CPerceptionEntity del tipo correspondiente al par�metro type
		 */
		static CPerceptionEntity* getPerceptionEntity(string type, float radius, void* userData, IPerceptionListener* listener);

	};  // class CPerceptionEntityFactory

	/**
	 * Clase de entidad de percepci�n que representa a un enemigo
	 *
	 * Esta clase s�lo tiene un sensor de visi�n b�sico (AI::CSightSensor), inicializado
	 * con los siguientes par�metros:
	 * � active = true
	 * � threshold = 1.0
	 * � maxDistance = 200.0
	 * � alpha = PI / 4
	 *
	 * Cuidado: estos par�metros son datos y, por lo tanto, deber�an declararse en un archivo de configuraci�n aparte.
	 *
	 *
	 * @author Gonzalo Fl�rez
	 * @date 11/04/2011
	 */
	class CPerceptionEntityEnemy :
		public AI::CPerceptionEntity
	{

	public:
		CPerceptionEntityEnemy(void* userData, IPerceptionListener* listener, float radius);
		~CPerceptionEntityEnemy(void) {};

	}; // class CPerceptionEntityEnemy

	/**
	 * Clase de entidad de percepci�n que representa al jugador
	 *
	 * Esta clase s�lo tiene una se�al con los siguientes par�metros:
	 * � type = PERCEPTION_SIGHT (es decir, que la se�al es de visibilidad)
	 * � intensity = 1.0
	 * � isActive = true
	 * � keepAlive = true (la se�al no se destruye despu�s de un ciclo de percepci�n, sino que sigue activa)
	 *
	 * Cuidado: estos par�metros son datos y, por lo tanto, deber�an declararse en un archivo de configuraci�n aparte.
	 *
	 *
	 * @author Gonzalo Fl�rez
	 * @date 11/04/2011
	 */
	class CPerceptionEntityPlayer :
		public AI::CPerceptionEntity
	{
	public:
		CPerceptionEntityPlayer(void* userData, IPerceptionListener* listener, float radius);
		~CPerceptionEntityPlayer(void) {};
	}; //class CPerceptionEntityPlayer

} //namespace AI

#endif __AI_PerceptionEntityFactory_H
