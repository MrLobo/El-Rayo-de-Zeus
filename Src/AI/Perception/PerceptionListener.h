/**
 * @file PerceptionListener.h
 *
 * Definici�n de la interfaz PerceptionListener, que implementar�n aquellas
 * clases que deseen ser notificadas cuando se active un sensor de una entidad
 * de percepci�n
 *
 * @author Gonzalo Fl�rez
 * @date 11/04/2011
 */

#pragma once

#ifndef __AI_PerceptionListener_H
#define __AI_PerceptionListener_H



namespace AI
{
	class CNotification;

	/**
	 * Interfaz que implementar�n aquellas
	 * clases que deseen ser notificadas cuando se active un sensor de una entidad
	 * de percepci�n
	 *
	 * @author Gonzalo Fl�rez
	 * @date 11/04/2011
	 */
	class IPerceptionListener
	{
	public:
		/**
		 * M�todo invocado por el gestor de percepci�n cuando recibe una notificaci�n de un
		 * sensor de la entidad de percepci�n.
		 *
		 * @param notification Notificaci�n recibida
		 */
		virtual void notificationPerceived(CNotification* notification) = 0;
		/**
		 * Destructor
		 */
		virtual ~IPerceptionListener(void) {};
	};

} // namespace AI

#endif __AI_PerceptionListener_H
