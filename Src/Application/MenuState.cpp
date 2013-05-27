//---------------------------------------------------------------------------
// MenuState.cpp
//---------------------------------------------------------------------------

/**
@file MenuState.cpp

Contiene la implementaci�n del estado de men�.

@see Application::CApplicationState
@see Application::CMenuState

@author David Llans�
@date Agosto, 2010
*/

#include "MenuState.h"
#include "BaseApplication.h"
#include "Clock.h"
#include "Logic/Server.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"

#include "GUI/Server.h"

#include <CEGUISystem.h>
#include <CEGUIWindowManager.h>
#include <CEGUIWindow.h>
#include <elements/CEGUIPushButton.h>

//PT se incluye el servidor de scripts de LUA
#include "ScriptManager\Server.h"

namespace Application {

	CMenuState::~CMenuState() 
	{
	} // ~CMenuState

	//--------------------------------------------------------

	bool CMenuState::init() 
	{
		CApplicationState::init();

		// Cargamos la ventana que muestra el men�
		/*
		CEGUI::WindowManager::getSingletonPtr()->loadWindowLayout("Menu.layout");
		_menuWindow = CEGUI::WindowManager::getSingleton().getWindow("Menu");
		*/

		// Cargamos la ventana que muestra el men� con LUA
		ScriptManager::CServer::getSingletonPtr()->loadExeScript("Menu");
		ScriptManager::CServer::getSingletonPtr()->executeProcedure("initMenu");
		
		// Asociamos los botones del men� con las funciones que se deben ejecutar.
		CEGUI::WindowManager::getSingleton().getWindow("Menu/Single")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CMenuState::startReleased, this));

		CEGUI::WindowManager::getSingleton().getWindow("Menu/Multi")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CMenuState::multiplayerReleased, this));
		
		CEGUI::WindowManager::getSingleton().getWindow("Menu/Exit")->
			subscribeEvent(CEGUI::PushButton::EventClicked, 
				CEGUI::SubscriberSlot(&CMenuState::exitReleased, this));

		// Asociamos los botones del men� con las funciones que se deben ejecutar desde LUA
		//ScriptManager::CServer::getSingletonPtr()->executeProcedure("asociarEventosMenu");

		return true;

	} // init

	//--------------------------------------------------------

	void CMenuState::release() 
	{
		CApplicationState::release();

	} // release

	//--------------------------------------------------------

	void CMenuState::activate() 
	{
		CApplicationState::activate();

		CBaseApplication::getSingletonPtr()->getClock()->removeAllTimeObserver();

		// Activamos la ventana que nos muestra el men� y activamos el rat�n.
		/*
		CEGUI::System::getSingletonPtr()->setGUISheet(_menuWindow);
		_menuWindow->setVisible(true);
		_menuWindow->activate();
		CEGUI::MouseCursor::getSingleton().show();
		*/

		// Activamos la ventana que nos muestra el men� y activamos el rat�n desde LUA
		ScriptManager::CServer::getSingletonPtr()->executeProcedure("showMenu");

	} // activate

	//--------------------------------------------------------

	void CMenuState::deactivate() 
	{		
		// Desactivamos la ventana GUI con el men� y el rat�n.
		/*
		CEGUI::MouseCursor::getSingleton().hide();
		_menuWindow->deactivate();
		_menuWindow->setVisible(false);
		*/

		// Desactivamos la ventana GUI con el men� y el rat�n desde LUA
		ScriptManager::CServer::getSingletonPtr()->executeProcedure("hideMenu");

		CApplicationState::deactivate();

	} // deactivate

	//--------------------------------------------------------

	void CMenuState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

	} // tick

	//--------------------------------------------------------

	bool CMenuState::keyPressed(GUI::TKey key)
	{
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CMenuState::keyReleased(GUI::TKey key)
	{
		switch(key.keyId)
		{

		case GUI::Key::ESCAPE:
			_app->exitRequest();
			break;

		case GUI::Key::RETURN:
			_app->setState("game");
			
			//[���] CARGA de Blueprints, Arquetypes y Map adelantada
			// Cargamos el archivo con las definiciones de las entidades del nivel.
			if (!Logic::CEntityFactory::getSingletonPtr()->loadBluePrints("blueprints.txt"))
				return false;
				
			// Add - ESC
			// Cargamos el archivo con las definiciones de los archetypes
			if (!Logic::CEntityFactory::getSingletonPtr()->loadArchetypes("archetypes.txt"))
				return false;
			
			// Add - JLS
			// Cargamos los anillos a partir del nombre del mapa. 
			if (!Logic::CServer::getSingletonPtr()->setRingPositions())//[���] Esto no deberia ejecutarse como parte del loadLevel...?
				return false;

			// Cargamos el nivel a partir del nombre del mapa. 
			//if (!Logic::CServer::getSingletonPtr()->loadMap("map.txt"))
			//	return false;

			_mapsToLoad.push_back("mapRed");
			_mapsToLoad.push_back("mapBlue");
			
			_mapsToLoad.push_back("mapGreen");
			_mapsToLoad.push_back("mapYellow");

			if (!Logic::CServer::getSingletonPtr()->loadWorld(_mapsToLoad))
				return false;
		
			// Llamamos al m�todo de creaci�n del jugador. Deberemos decidir
			// si el jugador es el jugador local. Al ser el monojugador lo es.
			Logic::CServer::getSingletonPtr()->getMap("mapRed")->createPlayer("Mono", true);

			// TODO Deber�amos poder propocionar caracter�asticas  (nombre, modelo, etc.)... ==> Ampliar MenuState...
			break;

		case GUI::Key::M:
			_app->setState("netmenu");
			break;
			
		default:
			return false;
			
		}
		return true;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CMenuState::mouseMoved(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CMenuState::mousePressed(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool CMenuState::mouseReleased(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased
			
	//--------------------------------------------------------
// TODO Por qu� se devuelve  true o false?
// TODO Toda la carga y creaci�n del jugador deberia encapsularse en un startGame() reusable
	bool CMenuState::startReleased(const CEGUI::EventArgs& e)
	{
		_app->setState("menusingle");

		/*
		_app->setState("game");
			
		//[���] CARGA de Blueprints, Arquetypes y Map adelantada
		// Cargamos el archivo con las definiciones de las entidades del nivel.
		if (!Logic::CEntityFactory::getSingletonPtr()->loadBluePrints("blueprints.txt"))
			return false;
				
		// Add - ESC
		// Cargamos el archivo con las definiciones de los archetypes
		if (!Logic::CEntityFactory::getSingletonPtr()->loadArchetypes("archetypes.txt"))
			return false;
			
		// Add - JLS
		// Cargamos los anillos a partir del nombre del mapa. 
		if (!Logic::CServer::getSingletonPtr()->setRingPositions())//[���] Esto no deberia ejecutarse como parte del loadLevel...?
			return false;

		_mapsToLoad.push_back("mapRed");
		_mapsToLoad.push_back("mapBlue");
		_mapsToLoad.push_back("mapGreen");
		_mapsToLoad.push_back("mapYellow");

		if (!Logic::CServer::getSingletonPtr()->loadWorld(_mapsToLoad))
			return false;
		
		// Llamamos al m�todo de creaci�n del jugador. Deberemos decidir
		// si el jugador es el jugador local. Al ser el monojugador lo es.

		//CMap::createPlayer(entityName, isLocalPlayer, model)
		Logic::CServer::getSingletonPtr()->getMap("mapRed")->createPlayer("Mono", true);
		//PT. si se le intenta pasar otro modelo da un error en getBones porque no encuentra el hueso "paracasco"

		// TODO Deber�amos poder proporcionar caracter�sticas  (nombre, modelo, etc.)... ==> Ampliar MenuState...

		*/
		return true;

	} // startReleased
			
	//--------------------------------------------------------

	bool CMenuState::exitReleased(const CEGUI::EventArgs& e)
	{
		_app->exitRequest();
		return true;

	} // exitReleased

	//--------------------------------------------------------

	bool CMenuState::multiplayerReleased(const CEGUI::EventArgs& e)
	{
		_app->setState("netmenu");
		return true;

	} // multiplayerReleased


} // namespace Application
