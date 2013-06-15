/**
@file Interfaz.h

Contiene la declaraci�n de la clase CInterfaz, Singleton que se encarga de
la gesti�n de la interfaz HUD y SHOP

@see GUI::CInterfaz

@author Pablo Terrado
@date Junio, 2013
*/
#ifndef __GUI_Interfaz_H
#define __GUI_Interfaz_H

#include "InputManager.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Logic 
{
	class CAvatarController;
}

namespace CEGUI
{
	class System;
	class Window; //PT
}

// Declaraci�n de la clase
namespace GUI
{
	/**
	Servidor del m�dulo GUI que hace de interfaz con el usuario. Est� 
	implementado como un singlenton de inicializaci�n expl�cita. Sirve 
	para comunicar a CEGUI los eventos producidos ya as� hacer funcionar
	la interfaz gr�fica de usuario y permite que se establezca que entidad 
	es controlada por el GUI::CPlayerController. Este control se hace a 
	trav�s del componente Logic::CAvatarController.

	@ingroup GUIGroup

	@author David Llans�
	@date Agosto, 2010
	*/
	class CInterfaz
	{
	public:

		/**
		Devuelve la �nica instancia de la clase CInterfaz.
		
		@return �nica instancia de la clase CInterfaz.
		*/
		static CInterfaz* getSingletonPtr() {return _instance;}

		/**
		Inicializa la instancia

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera la instancia de CInterfaz. Debe llamarse al finalizar la 
		aplicaci�n.
		*/
		static void Release();

	
		/**
		 * Show a text on the GUI screen.
		 *
		 * @param msg Message that is going to be shown on screen.
		 */
		void setText(const std::string& msg);


		/**
		 * Show the HUD window
		 *
		 * @param none
		 */
		void showHUD();



		/**
		 * Hide the HUD window
		 *
		 * @param none
		 */
		void hideHUD();



		/**
		 * Show the SHOP window
		 *
		 * @param none
		 */
		void showSHOP();



		/**
		 * Hide the SHOP window
		 *
		 * @param none
		 */
		void hideSHOP();




	protected:

		/**
		Constructor.
		*/
		CInterfaz ();

		/**
		Destructor.
		*/
		~CInterfaz();

		/**
		Segunda fase de la construcci�n del objeto. Sirve para hacer
		inicializaciones de la propia instancia en vez de inicializaciones 
		est�ticas.

		@return true si todo fue correctamente.
		*/
		bool open();

		/**
		Segunda fase de la destrucci�n del objeto. Sirve para hacer liberar 
		los recursos de la propia instancia, la liberaci�n de los recursos 
		est�ticos se hace en Release().
		*/
		void close();

	
		/**
		* Puntero a la ventana CEGUI del HUD
		* Si no se esta renderizando ninguna interfaz de usuario es NULL
		*/
		CEGUI::Window* _hudWindow;

		/**
		* Puntero a la ventana CEGUI de la tienda (SHOP)
		* Si no se esta renderizando ninguna interfaz de usuario es NULL
		*/
		CEGUI::Window* _shopWindow;

	private:
		/**
		�nica instancia de la clase.
		*/
		static CInterfaz* _instance;

	}; // class CInterfaz

} // namespace GUI

#endif // __GUI_Interfaz_H
