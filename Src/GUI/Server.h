/**
@file Server.h

Contiene la declaraci�n de la clase CServer, Singleton que se encarga de
la gesti�n de la interfaz con el usuario (entrada de perif�ricos, CEGui...).

@see GUI::CServer

@author David Llans�
@date Agosto, 2010
*/
#ifndef __GUI_Server_H
#define __GUI_Server_H

#include "InputManager.h"

#include <CEGUI/CEGUIEventArgs.h>
#include <CEGUI/CEGUIWindowManager.h>
#include <cegui/elements/CEGUIProgressBar.h>
#include <cegui/elements/CEGUIPushButton.h>


// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Logic 
{
	class CAvatarController;
}

namespace CEGUI
{	
	class Combobox;	
	class System;	
	class Window;
}

namespace GUI
{
	class CPlayerController;
	class CCameraController;
	class CHudController; //PT
	class CShopController; //PT
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
	
	class CServer : public CKeyboardListener, public CMouseListener, public CJoystickListener
	{
	public:

		/**
		Devuelve la �nica instancia de la clase CServer.
		
		@return �nica instancia de la clase CServer.
		*/
		static CServer* getSingletonPtr() {return _instance;}

		/**
		Inicializa la instancia

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera la instancia de CServer. Debe llamarse al finalizar la 
		aplicaci�n.
		*/
		static void Release();

		/**
		Devuelve la instancia de la clase GUI que se encarga de procesar los
		eventos de entrada para controlar al jugador por si se desea configurar
		externemante.

		@return Instancia de la clase GUI que controla al jugador.
		*/
		CPlayerController *getPlayerController() {return _playerController;}

		/** PEP
		Devuelve la instancia de la clase GUI que se encarga de procesar los
		eventos de entrada para controlar al jugador por si se desea configurar
		externemante.

		@return Instancia de la clase GUI que controla al jugador.
		*/
		CCameraController *getCameraController() {return _cameraController;}

		/** PT
		Devuelve la instancia de la clase GUI que se encarga de procesar los
		eventos de entrada para recargar el HUD

		@return Instancia de la clase GUI que recarga el HUD.
		*/
		CHudController *getHudController() {return _hudController;}

		/** PT
		Devuelve la instancia de la clase GUI que se encarga de procesar la tienda SHOP

		@return Instancia de la clase GUI que recarga el SHOP.
		*/
		CShopController *getShopController() {return _shopController;}


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
		
		/***************************************************************
		M�todos de CKeyboardListener
		***************************************************************/
		
		/**
		M�todo que ser� invocado siempre que se mueva el rat�n.

		@param mouseState Estado del rat�n cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		bool mouseMoved(const CMouseState &mouseState);
		
		/**
		M�todo que ser� invocado siempre que se pulse un bot�n.

		@param mouseState Estado del rat�n cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		bool mousePressed(const CMouseState &mouseState);

		/**
		M�todo que ser� invocado siempre que se termine la pulsaci�n
		de un bot�n.

		@param mouseState Estado del rat�n cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		bool mouseReleased(const CMouseState &mouseState);


		/***************
			WINDOW
		**************/

		CEGUI::Window* getWindow(const std::string& windowName)	{ return _windowManager->getWindow(windowName); }
		std::string getWindowText(const std::string& textWindow);
		void setWindowEnabled(const std::string& windowName, bool isEnable);
		void setWindowVisible(const std::string& windowName, bool isVisible);

		/**
		 * Show a text on the GUI screen.
		 *
		 * @param msg Message that is going to be shown on screen.
		 */
		void setText(const std::string& msg);

		// ------------- BUTTONS ----------------------
		
		template <class T>
		void setCallbackButton(const std::string& buttonWindow,
			bool (T::*callback) (const CEGUI::EventArgs&), T *cbOwner)
		{			
			getWindow(buttonWindow)->subscribeEvent( 
				CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(callback, cbOwner)
			);
		}
		

		// ------------- PROGRESS ----------------------
		void updateProgress( const std::string& barWindow, const std::string& statusWindow,
			float progressAmount, const std::string& statusMsg);
		
		template <class T>
		void setCallbackProgress(const std::string& barWindow,
			bool (T::*callback) (const CEGUI::EventArgs&), T *cbOwner)
		{
			CEGUI::ProgressBar* bar = 
				static_cast<CEGUI::ProgressBar*> ( getWindow( barWindow ));
				bar->subscribeEvent( 
					CEGUI::ProgressBar::EventProgressChanged, 
					CEGUI::Event::Subscriber(callback, cbOwner)
				);
		}


		//---------- COMBOS ---------------------------------
		CEGUI::Combobox* createCombobox(const std::string& comboWindow, 
			const std::string* itemTexts, const unsigned int nItems);

		int getComboSelectedID(const std::string& comboWindow);
		std::string getComboSelectedText(const std::string& comboWindow);

		//---------- CHECKBOX ---------------------------------
		bool isCheckboxSelected(const std::string& checkBox);


	protected:

		/**
		Constructor.
		*/
		CServer ();

		/**
		Destructor.
		*/
		~CServer();

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
		Clase GUI que se encarga de controlar al jugador.
		*/
		CPlayerController *_playerController;

		/**
		Clase GUI que se encarga de controlar la c�mara.
		*/
		CCameraController *_cameraController;

		//PT
		/**
		Clase GUI que se encarga de controlar el HUD.
		*/
		CHudController *_hudController;

		//PT
		/**
		Clase GUI que se encarga de controlar el HUD.
		*/
		CShopController *_shopController;


		/**
		Sistema de la interfaz gr�fica de usuario CEGUI.
		*/
		CEGUI::System *_GUISystem;

	/**
	* Puntero a la ventana CEGUI que esta actualmente siendo renderizada.
	* Si no se esta renderizando ninguna interfaz de usuario es NULL
	*/
		CEGUI::Window *_currentWindow;

	private:
		/**
		�nica instancia de la clase.
		*/
		static CServer* _instance;

		CEGUI::WindowManager* _windowManager;

	}; // class CServer

} // namespace GUI

#endif // __GUI_Server_H
