/**
@file HudController.h

Contiene la declaraci�n de la clase CHudController. Se encarga de
recibir eventos del teclado (la R) para recargar el HUD

@author Pablo Terrado Contreras
@date Mayo, 2013
*/

#ifndef __GUI_HudController_H
#define __GUI_HudController_H

#include "InputManager.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Logic 
{
	class CEntity;
}

namespace ScriptManager
{
	class CServer;
}

// Declaraci�n de la clase
namespace GUI
{
	/**
	Esta clase sirve para gestionar el teclado, y recargar el HUD 
	
	@ingroup GUIGroup

	@author Pablo Terrado
	@date Mayo, 2013
	*/

	class CHudController : public CKeyboardListener
	{
	public:

		/**
		Constructor.
		*/
		CHudController();

		/**
		Destructor.
		*/
		~CHudController();
 

		/**
		Activa la la clase, se registra en el CInputManager y as� empieza a 
		escuchar eventos.
		*/
		void activate();

		/**
		Desctiva la la clase, se deregistra en el CInputManager y deja de 
		escuchar eventos.
		*/
		void deactivate();

		/***************************************************************
		M�todos de CKeyboardListener
		***************************************************************/
		
		/**
		M�todo que ser� invocado siempre que se pulse una tecla.

		@param key C�digo de la tecla pulsada.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		bool keyPressed(TKey key);
		
		/**
		M�todo que ser� invocado siempre que se termine la pulsaci�n
		de una tecla.

		@param key C�digo de la tecla pulsada.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		bool keyReleased(TKey key);


	}; // class CHudController

} // namespace GUI

#endif // __GUI_HudController_H
