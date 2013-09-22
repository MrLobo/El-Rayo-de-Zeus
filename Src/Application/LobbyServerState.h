//---------------------------------------------------------------------------
// LobbyServerState.h
//---------------------------------------------------------------------------

/**
@file LobbyServerState.h

Contiene la declaraci�n del estado de lobby del servidor.

@see Application::CApplicationState
@see Application::CLobbyServerState

@author David Llans�
@date Agosto, 2010
*/

#ifndef __Application_LobbyServerState_H
#define __Application_LobbyServerState_H

#include "ApplicationState.h"

#include <list>
#include <map>

#include "Net/Manager.h"
#include "NET/IObserver.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Application 
{
	class CBaseApplication;
}

namespace CEGUI
{
	class EventArgs;
	class Window;
	class WindowManager;
}

namespace Net
{
	class CManager;
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
	class CLobbyServerState : public CApplicationState , public Net::IObserver
	{
	public:
		/** 
		Constructor de la clase 
		*/
		CLobbyServerState(CBaseApplication *app) : CApplicationState(app),
			_waiting(true), _nClients(0), _maskClientIds(0), _mapLoadedByClients(0) {} 	

		/**
		Funci�n llamada cuando se crea el estado (se "engancha" en la
		aplicaci�n, para que inicialice sus elementos.

		@return true si todo fue bien.
		*/
		bool init();

		/**
		Funci�n llamada por la aplicaci�n cuando se activa
		el estado.
		*/
		void activate();

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
		virtual bool keyReleased(GUI::TKey key);




		/******************
			NET::IOBSERVER
		******************/
		void dataPacketReceived(Net::CPacket* packet);
		void connectPacketReceived(Net::CPacket* packet);
		void disconnectPacketReceived(Net::CPacket* packet);


	private:

		/**
		Indica si estamos en fase de espera de jugadores
		*/
		bool _waiting;
			
		/**
		* Funci�n que ejecuta la acci�n start. 
		Centraliza el c�digo y ser� invocada cuando se pulse la tecla correspondiente o se
		genere el evento de rat�n
		*/
		void _start();

				
		//-------- NET ----------------------------

		// PLAYER INFO
		unsigned int _nClients;
		std::string _playerNicks[8]; // HACK fijamos num max de players a 8
		std::string _playerModels[8]; // TODO deber�an ser enum para menos info por red en un futuro

		Net::CManager* _netManager;

		//typedef std::list<Net::NetID> TNetIDList; UNDONE no es necesario una lista
		typedef unsigned char TMask;

		/**
		lista donde almacenamos los clientes conectados
		*/
		//TNetIDList _maskClientIds; UNDONE
		TMask _maskClientIds; // char m�scara

		/**
		lista donde almacenamos los clientes que han cargado el mapa
		*/
		//TNetIDList _mapLoadedByClients; UNDONE
		TMask _mapLoadedByClients;

		// UNDONE
		//typedef std::map<Net::NetID,unsigned int> TNetIDCounterMap;
		//typedef std::pair<Net::NetID,unsigned int> TNetIDCounterPair;

		/**
		Tabla donde almacenamos por cada cliente cuantos jugadores han creado
		*/
		//TNetIDCounterMap _playersLoadedByClients; UNDONE
		unsigned int _playersLoadedByClients[8];


		
		//-------- CEGUI ----------------------

		CEGUI::WindowManager* _windowManager;
		CEGUI::Window* _windowMenu;	
		CEGUI::Window* _windowStatus;
		CEGUI::Window* _windowStart;
		CEGUI::Window* _windowBack;

		void _logStatus(const std::string& statusMsg);
		/**
		Funci�n que se quiere realizar cuando se pulse el bot�n start.
		Simplemente cambia al estado de juego.
		*/
		bool _startReleased(const CEGUI::EventArgs& e);

		/**
		Funci�n que se quiere realizar cuando se pulse el bot�n back.
		Simplemente cambia al estado de menu.
		*/
		bool _backReleased(const CEGUI::EventArgs& e);




	}; // CMenuState

} // namespace Application

#endif //  __Application_MenuState_H
