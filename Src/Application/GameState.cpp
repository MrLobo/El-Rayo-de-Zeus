//---------------------------------------------------------------------------
// GameState.cpp
//---------------------------------------------------------------------------

/**
@file GameState.cpp

Contiene la implementaci�n del estado de juego.

@see Application::CApplicationState
@see Application::CGameState

@author David Llans�
@date Agosto, 2010
*/

#include "GameState.h"

#include "Logic/Server.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"

#include "GUI/Server.h"
#include "GUI/PlayerController.h"

#include "Physics/Server.h"

#include <CEGUISystem.h>
#include <CEGUIWindowManager.h>
#include <CEGUIWindow.h>

namespace Application {

	// ��� Al inicializar la app
	bool CGameState::init() 
	{
		CApplicationState::init();
		// FRS el Logic:: loadLevel se mueve a la parte final del lobby/menu state
		//, ya que los mapas cargados var�an dependiendo de si somos server, client o monojudador
		
		return true;

	} // init

	//--------------------------------------------------------

	// ��� Al cerrar la app
	void CGameState::release() 
	{
		// Liberamos el nivel junto con las escenas f�sico-graficas.
		Logic::CServer::getSingletonPtr()->unLoadMap();
		Logic::CEntityFactory::getSingletonPtr()->unloadArchetypes();
		Logic::CEntityFactory::getSingletonPtr()->unloadBluePrints();

		CApplicationState::release();
	} // release

	//--------------------------------------------------------

	// ��� Al entrar en GameState (cambio de currentState)
	void CGameState::activate() 
	{
		CApplicationState::activate();
		
		// Activamos el mapa que ha sido cargado para la partida (incluye la activacion de la escenas)
		Logic::CServer::getSingletonPtr()->activateMap();

		// Queremos que el GUI maneje al jugador.
        GUI::CServer::getSingletonPtr()->getPlayerController()->activate();
		
	} // activate

	//--------------------------------------------------------

	//  ��� Al salir de gameState (cambio de currentState)
	void CGameState::deactivate() 
	{
		// Desactivamos la clase que procesa eventos de entrada para  controlar al jugador.
		GUI::CServer::getSingletonPtr()->getPlayerController()->deactivate();
		
		// Desactivamos el mapa de la partida (incluye la desactivacion de la escenas)
		Logic::CServer::getSingletonPtr()->deactivateMap();
		
		CApplicationState::deactivate();

	} // deactivate

	//--------------------------------------------------------

	void CGameState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);
		// FRS Los siguientes ticks no se colocan a nivel de C3DApplication::tick
		// porque a diferencia de otros servers, s�lo deben actualizarse durante el GameState)
		
		Physics::CServer::getSingletonPtr()->tick(msecs);// Simulaci�n f�sica 		
		Logic::CServer::getSingletonPtr()->tick(msecs);// Actualizamos la l�gica de juego.

		_time += msecs;		
	} // tick

	


	/**************
		INPUT
	*************/

	bool CGameState::keyPressed(GUI::TKey key)
	{
		return false;

	} // keyPressed

	//--------------------------------------------------------

	// TODO FRS de momento el ESC = salir de la partida, pero en un futuro podr�a ser = Pause... + New / Continue
	bool CGameState::keyReleased(GUI::TKey key)
	{
		switch(key.keyId)
		{
		case GUI::Key::ESCAPE:
			Logic::CServer::getSingletonPtr()->unLoadMap();
			Logic::CEntityFactory::getSingletonPtr()->unloadArchetypes();
			Logic::CEntityFactory::getSingletonPtr()->unloadBluePrints();
			_app->setState("menu");
			break;

		// TODO
		case GUI::Key::PAUSE:
			_app->setState("pause"); // TODO FRS no existe todav�a el estado pause
			break;

		default:
			return false;
		}
		return true;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CGameState::mouseMoved(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CGameState::mousePressed(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool CGameState::mouseReleased(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased

} // namespace Application
