//---------------------------------------------------------------------------
// LobbyClientState.h
//---------------------------------------------------------------------------

/**
@file LobbyClientState.h

Contiene la declaraci�n del estado de lobby del cliente.

@see Application::CApplicationState
@see Application::CLobbyClientState

@author David Llans�
@date Agosto, 2010
*/

#ifndef __Application_LobbyClientState_H
#define __Application_LobbyClientState_H

#include "ApplicationState.h"
#include "NET/IObserver.h"

//PT
#include <cegui.h> //para que pueda crear combobox

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Application 
{
	class CBaseApplication;
}

namespace CEGUI
{
	class EventArgs;
	class Window;
}

namespace Net
{
	class CManager;
}
//PT
namespace ScriptManager
{
	class Server;
}

namespace Application 
{
	/**
	Como su nombre indica, esta clase es la clase del men�
	principal del juego. Es muy sencilla y lo �nico que hace es cargar
	un layout de CEGUI al inicio y activarlo y desactivarlo cuando
	se activa o desactiva el estado (haci�ndo visible/invisible tambi�n
	el puntero del rat�n). Tambi�n asocia los eventos de los botones 
	del men� a las funciones C++ que se deben invocar cuando los botones
	son pulsados.
	<p>
	Este estado es CEGUI dependiente, lo cual no es deseable, la aplicaci�n
	deber�a ser independiente de las tecnolog�as usadas.

	@ingroup applicationGroup

	@author David Llans�
	@date Agosto, 2010
	*/
	class CLobbyClientState : public CApplicationState, public Net::IObserver
	{
	
	public:
		/** 
		Constructor de la clase 
		*/
		CLobbyClientState(CBaseApplication *app) : CApplicationState(app) {}	

		/**
		Funci�n llamada cuando se crea el estado (se "engancha" en la
		aplicaci�n, para que inicialice sus elementos.

		@return true si todo fue bien.
		*/
		virtual bool init();

		/**
		Funci�n llamada por la aplicaci�n cuando se activa
		el estado.
		*/
		virtual void activate();

		/**
		Funci�n llamada por la aplicaci�n cuando se desactiva
		el estado.
		*/
		virtual void deactivate();
		

		// M�todos de CKeyboardListener

		/**
		M�todo que ser� invocado siempre que se termine la pulsaci�n
		de una tecla. Ser� la aplicaci�n qui�n llame a este m�todo 
		cuando el estado est� activo. Esta clase NO se registra en
		el InputManager sino que es la aplicaci�n quien lo hace y 
		delega en los estados.

		@param key C�digo de la tecla pulsada.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		bool keyReleased(GUI::TKey key);


		/******************
			NET::IOBSERVER
		******************/
		void dataPacketReceived(Net::CPacket* packet);	
		void connectPacketReceived(Net::CPacket* packet){}
		void disconnectPacketReceived(Net::CPacket* packet){}


	private:
		
		/**
		* Funci�n que ejecuta la acci�n start. 
		Centraliza el c�digo y ser� invocada cuando se pulse la tecla correspondiente o se
		genere el evento de rat�n
		*/
		void _connect();
		
		Net::CManager* _netManager;

		


		//-------- CEGUI ------------------

		CEGUI::WindowManager* _windowManager;
		CEGUI::Window * _windowStatus;
		CEGUI::Window * _windowConnect;	
		
		//PT combobox
		CEGUI::Combobox* _cbModel;
		CEGUI::Combobox* _cbColor;

		/**
		Funci�n que se quiere realizar cuando se pulse el bot�n start.
		Simplemente cambia al estado de juego.
		*/
		bool _connectReleased(const CEGUI::EventArgs& e);

		/**
		Funci�n que se quiere realizar cuando se pulse el bot�n back.
		Simplemente cambia al estado de menu.
		*/
		bool _backReleased(const CEGUI::EventArgs& e);

		void _logStatus(const std::string& statusMsg);

	}; // CMenuState

} // namespace Application

#endif //  __Application_MenuState_H
