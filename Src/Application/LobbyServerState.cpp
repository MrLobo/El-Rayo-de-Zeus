//---------------------------------------------------------------------------
// LobbyServerState.cpp
//---------------------------------------------------------------------------

/**
@file LobbyServerState.cpp

Contiene la implementaci�n del estado de lobby del servidor.

@see Application::CApplicationState
@see Application::CLobbyServerState

@author David Llans�
@date Agosto, 2010
*/

#include "LobbyServerState.h"

#include "Logic/Server.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"

#include "GUI/Server.h"
#include "net/Manager.h"
#include "net/Servidor.h"
#include "net/factoriared.h"
#include "net/paquete.h"
#include "net/buffer.h"


#include <CEGUISystem.h>
#include <CEGUIWindowManager.h>
#include <CEGUIWindow.h>
#include <elements/CEGUIPushButton.h>

namespace Application {

	CLobbyServerState::~CLobbyServerState() 
	{
	} // ~CLobbyServerState

	//--------------------------------------------------------

	bool CLobbyServerState::init() 
	{
		CApplicationState::init();

		// Cargamos la ventana que muestra el men�
		CEGUI::WindowManager::getSingletonPtr()->loadWindowLayout("NetLobbyServer.layout");
		_menuWindow = CEGUI::WindowManager::getSingleton().getWindow("NetLobbyServer");
		
		// Asociamos los botones del men� con las funciones que se deben ejecutar.
		CEGUI::WindowManager::getSingleton().getWindow("NetLobbyServer/Start")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CLobbyServerState::startReleased, this));

		CEGUI::WindowManager::getSingleton().getWindow("NetLobbyServer/Start")->setEnabled(false);
		
		CEGUI::WindowManager::getSingleton().getWindow("NetLobbyServer/Back")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CLobbyServerState::backReleased, this));
	
		return true;

	} // init

	//--------------------------------------------------------

	void CLobbyServerState::release() 
	{
		CApplicationState::release();

	} // release

	//--------------------------------------------------------

	void CLobbyServerState::activate() 
	{
		CApplicationState::activate();

		// Activamos la ventana que nos muestra el men� y activamos el rat�n.
		CEGUI::System::getSingletonPtr()->setGUISheet(_menuWindow);
		_menuWindow->setVisible(true);
		_menuWindow->activate();
		CEGUI::MouseCursor::getSingleton().show();

		// NET: Registro como observer server
		Net::CManager::getSingletonPtr()->addObserver(this);
		Net::CManager::getSingletonPtr()->activateAsServer(1234,16); // 16 conexiones

		CEGUI::WindowManager::getSingleton().getWindow("NetLobbyServer/Status")
			->setText("Status: Server up. Waiting for clients ...");

	} // activate

	//--------------------------------------------------------

	void CLobbyServerState::deactivate() 
	{		
		Net::CManager::getSingletonPtr()->removeObserver(this);
		// Desactivamos la ventana GUI con el men� y el rat�n.
		CEGUI::MouseCursor::getSingleton().hide();
		_menuWindow->deactivate();
		_menuWindow->setVisible(false);
		
		CApplicationState::deactivate();

	} // deactivate

	//--------------------------------------------------------

	void CLobbyServerState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

	} // tick



	//--------------------------------------------------------

	bool CLobbyServerState::keyPressed(GUI::TKey key)
	{
	   return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CLobbyServerState::keyReleased(GUI::TKey key)
	{
		switch(key.keyId)
		{
		case GUI::Key::ESCAPE:
			Net::CManager::getSingletonPtr()->deactivateNetwork();
			_app->setState("netmenu");
			break;
		case GUI::Key::RETURN:
			doStart();
			break;
		default:
			return false;
		}
		return true;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CLobbyServerState::mouseMoved(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CLobbyServerState::mousePressed(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool CLobbyServerState::mouseReleased(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased
			
	//--------------------------------------------------------
		
	bool CLobbyServerState::startReleased(const CEGUI::EventArgs& e)
	{
		doStart();
		return true;

	} // startReleased
			
	//--------------------------------------------------------

	bool CLobbyServerState::backReleased(const CEGUI::EventArgs& e)
	{
		Net::CManager::getSingletonPtr()->deactivateNetwork();
		_app->setState("netmenu");
		return true;

	} // backReleased

	//--------------------------------------------------------

	void CLobbyServerState::doStart()
	{
		// TODO Aqu� debemos enviar a los clientes un mensaje de tipo
		// Net::LOAD_MAP para que comiencen la carga del mapa. Tras esto 
		// se debe realizar la carga del blueprints espec�fico del 
		// servidor y el mapa. La carga por defecto para monojugador
		// se ha retrasado y se encuentra en MenuState.cpp
		_waiting = false;		
		Net::NetMessageType txMsg = Net::LOAD_MAP; //Enviamos el mensaje de que carguen el mapa a todos los clientes
			Net::CManager::getSingletonPtr()->send( &txMsg, sizeof(txMsg));
#if _DEBUG
#endif	fprintf (stdout, "NET::SERVER::TX>> LOAD_MAP.\n");
		// Cargamos el archivo con las definiciones de las entidades del nivel.
		if (!Logic::CEntityFactory::getSingletonPtr()->loadBluePrints("blueprints_server.txt")){
			CEGUI::WindowManager::getSingleton().getWindow("NetLobbyServer/Status")->setText("Error al cargar el nivel");
			Net::CManager::getSingletonPtr()->deactivateNetwork();
			_app->exitRequest();
		}							
		// Cargamos el nivel a partir del nombre del mapa. 
		else if (!Logic::CServer::getSingletonPtr()->loadLevel("map_server.txt")){
			CEGUI::WindowManager::getSingleton().getWindow("NetLobbyServer/Status")->setText("Error al cargar el nivel");
			Net::CManager::getSingletonPtr()->deactivateNetwork();
			_app->exitRequest();

		} else {
#if _DEBUG
#endif		fprintf (stdout, "NET::SERVER>> MAPA Cargado.\n");
		}
	} // doStart

	//--------------------------------------------------------

	/*******************
		NET: IObserver
	*********************/
	
	void CLobbyServerState::dataPacketReceived(Net::CPaquete* packet)
	{
		// TODO Aqu� es donde debemos recibir los mensajes de red. Hay
		// que atender al mensaje Net::MAP_LOADED para que cuando se
		// reciba, enviar un mensaje tipo Net::START_GAME a los clientes
		// y cambiar al estado "game" de la aplicaci�n
		Net::NetMessageType rxMsg;
		memcpy(&rxMsg, packet->getData(),sizeof(rxMsg));
		switch (rxMsg)
		{

			case Net::MAP_LOADED:	
#if _DEBUG
#endif		fprintf (stdout, "NET::SERVER::RX>> MAP_LOADED.\n");
			// TODO Borrar las siguientes 3 l�neas. Est�n de momento para que
			// se lance el estado de juego al cargar el mapa
			////////////// BORRAR ///////////////////////////////////
			Net::NetMessageType msg = Net::START_GAME;
			Net::CManager::getSingletonPtr()->send(&msg, sizeof(msg));
			_app->setState("game");
			////////////// BORRAR ///////////////////////////////////
				
			//Almacenamos el ID del usuario que se ha cargado el mapa.
			_mapLoadedByClients.push_back(packet->getConexion()->getId());

			//Si todos los clientes han cargado los mapas pasamos a crear jugadores.
			if(_clients.size() == _mapLoadedByClients.size()) // Lista de clientes (IDs) VS lista de clientes que han cargado el mapa
			{
				// Se debe crear un jugador por cada cliente registrado.
				for(TNetIDList::const_iterator it = _clients.begin(); it != _clients.end(); it++)
				{
					//Preparamos la lista de control de carga de jugadores.
					//Esto quiere decir que el cliente (*it) ha cargado 0 jugadores
					TNetIDCounterPair elem(*it,0);
					_playersLoadedByClients.insert(elem);

					// Server orquesta carga de cada jugador: "voy a cargar tal, vosotros tambi�n"
					// TODO Hay que enviar un paquete tipo LOAD_PLAYER con 
					// el NetID del cliente del que estamos creando el jugador (*it)

					// Creamos el player. 
					// HACK Deber�amos poder propocionar caracter�asticas
					// diferentes seg�n el cliente (nombre, modelo, etc.). Esto es una
					// aproximaci�n, solo cambiamos el nombre y decimos si es el jugador
					// local
					
					std::stringstream number;
						number << (*it);
					std::string name("Player");
						name.append(number.str());

					// TODO Llamar al m�todo de creaci�n del jugador. Deberemos decidir
					// si el jugador es el jugador local. Al ser el servidor ninguno lo es
				}
			}
			break;
		}
		case Net::PLAYER_LOADED:
		{
			//Aumentamos el n�mero de jugadores cargados por el cliente
			(*_playersLoadedByClients.find(packet->getConexion()->getId())).second++;

			// TODO Comprobar si todos los clientes han terminado de 
			// cargar todos los jugadores
			bool loadFinished = true;

			//Si todos los clientes han cargado todos los players
			if(loadFinished)
			{
				//Enviamos el mensaje de que empieza el juego a todos los clientes
				Net::NetMessageType txMsg = Net::START_GAME;
					Net::CManager::getSingletonPtr()->send( (void*) &txMsg, sizeof(txMsg));
				_app->setState("game");
#if _DEBUG
#endif			fprintf (stdout, "NET::SERVER::TX>> START_GAME.\n");

			}
			break;
		}
		}
	} // dataPacketReceived

	//--------------------------------------------------------

	void CLobbyServerState::connexionPacketReceived(Net::CPaquete* packet)
	{
		if(_waiting){
			//Mostramos un poco de informaci�n en el status		
			unsigned int ip = packet->getConexion()->getAddress();
				byte* p = (byte*)&ip;

			char id[100];
				sprintf_s(id,"Client conected: %d.%d.%d.%d:%d\nWaiting for more players...",p[0],p[1],p[2],p[3], packet->getConexion()->getPort()); 				
				CEGUI::WindowManager::getSingleton().getWindow("NetLobbyServer/Status")->setText(id);
		
			//Habilitamos el bot�n de start.
			CEGUI::WindowManager::getSingleton().getWindow("NetLobbyServer/Start")->setEnabled(true);

			//Almacenamos el ID del usuario que se ha conectado.
			_clients.push_back(packet->getConexion()->getId());
		}

	} // connexionPacketReceived

	//--------------------------------------------------------

	void CLobbyServerState::disconnexionPacketReceived(Net::CPaquete* packet)
	{
		// TODO gestionar desconexiones.
#if _DEBUG
		fprintf (stdout, "NET::SERVER::RX>> DISCONNECT.\n");
#endif		
		//Habilitamos el bot�n de start.
		CEGUI::WindowManager::getSingleton().getWindow("NetLobbyServer/Start")->setEnabled(false);
		CEGUI::WindowManager::getSingleton().getWindow("NetLobbyServer/Status")
			->setText("Client disconnected. Waiting for new clients...");		
	} // disconnexionPacketReceived

} // namespace Application
