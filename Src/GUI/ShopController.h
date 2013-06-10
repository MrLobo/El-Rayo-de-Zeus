/**
@file ShopController.h

Contiene la declaraci�n de la clase CShopController. Se encarga de
recibir eventos del teclado (la R) para recargar el Shop

@author Pablo Terrado Contreras
@date Mayo, 2013
*/

#ifndef __GUI_ShopController_H
#define __GUI_ShopController_H

#include "InputManager.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Logic 
{
	class CEntity;
	class CMessage;
	class CMessageUShort;
}

namespace ScriptManager
{
	class CServer;
}

// Declaraci�n de la clase
namespace GUI
{
	/**
	Esta clase sirve para gestionar el teclado, y recargar el Shop 
	
	@ingroup GUIGroup

	@author Pablo Terrado
	@date Junio, 2013
	*/

	class CShopController : public CKeyboardListener, public CMouseListener
	{
	public:

		/**
		Constructor.
		*/
		CShopController();

		/**
		Destructor.
		*/
		~CShopController();

		/**
		Establece el componente del jugador al que enviaremos acciones 
		de movimiento en funci�n de las teclas pulsadas y el movimiento 
		del rat�n.

		@param setControlledShop Componente al que enviaremos acciones de 
		movimiento en funci�n de las teclas pulsadas y el movimiento del 
		rat�n.
		*/
		void setControlledShop(Logic::CEntity *controlledShop) 
										{_controlledShop = controlledShop;} 

		Logic::CEntity * getControlledShop() {return _controlledShop; } 
 

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

	protected:
		Logic::CEntity* _controlledShop;


	}; // class CShopController

} // namespace GUI

#endif // __GUI_ShopController_H
