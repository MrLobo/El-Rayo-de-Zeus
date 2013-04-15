//---------------------------------------------------------------------------
// 3DApplication.cpp
//---------------------------------------------------------------------------

/**
@file 3DApplication.cpp

Contiene la implementaci�n de la clase aplicacion para aplicaciones
basadas en Ogre. Esta clase maneja la ejecuci�n de todo el juego.

@see Application::C3DApplication
@see Application::CBaseApplication

@author David Llans�
@date Julio, 2010
*/
#include "3DApplication.h"

#include "BaseSubsystems/Server.h"
#include "Graphics/Server.h"

#include "GUI/InputManager.h"
#include "GUI/Server.h"
#include "Logic/Server.h"
#include "Logic/Maps/ComponentFactory.h"
#include "NET/Manager.h"
#include "OgreClock.h"
#include "Physics/Server.h"
#include "Audio/Server.h"


#include <cassert>

#include <iostream>

namespace Application {

	C3DApplication::C3DApplication() : CBaseApplication()
	{

	} // C3DApplication

	//--------------------------------------------------------

	C3DApplication::~C3DApplication()
	{

	} // ~C3DApplication

	//--------------------------------------------------------

	bool C3DApplication::init() 
	{
		// Inicializamos los diferentes subsistemas, los de 
		// Ogre entre ellos.
		if (!BaseSubsystems::CServer::Init())
			return false;

		// Inicializamos el gestor de entrada de perif�ricos.
		if (!GUI::CInputManager::Init())
			return false;
		else { // FRS App es listener de cualquier input de forma centralizada; despu�s informa al currentState del evento			
			GUI::CInputManager::getSingletonPtr()->addKeyListener(this); // Nos registramos como oyentes de los eventos del teclado. 
			GUI::CInputManager::getSingletonPtr()->addMouseListener(this); // Y como oyentes de los eventos del rat�n.
		} 

		// Inicializamos el servidor gr�fico.
		if (!Graphics::CServer::Init())
			return false;

		// Inicializamos el servidor de interfaz con el usuario.
		if (!GUI::CServer::Init())
			return false;

		// Inicializaci�n del servidor de f�sica.
		if (!Physics::CServer::Init())
			return false;
			// Inicializaci�n del servidor de f�sica.
		if (!Audio::CServer::Init())
			return false;

		// Inicializamos el servidor de IA
		//if (!AI::CServer::Init())
		//	return false;

		// Inicializamos la red
		if (!Net::CManager::Init())
			return false;

		// Inicializamos el servidor de la l�gica.
		if (!Logic::CServer::Init())
			return false;

		// Inicializamos la clase base.
		if (!CBaseApplication::init())
			return false;
		
		// Creamos el reloj basado en Ogre.
		_clock = new COgreClock();

		return true;

	} // init

	//--------------------------------------------------------

	void C3DApplication::release()
	{
		// Eliminamos el reloj de la aplicaci�n.
		delete _clock;

		// FRS: BaseApp guarda todos los estados de la App
		// En el release, desactiva y libera el curState (vinculado a los motores)
		// Es necesario que todav�a no se haya liberado ning�n motor.
		CBaseApplication::release();

		// Destruimos la factor�a de componentes. La factor�a
		// de componentes no es de construcci�n y destrucci�n expl�cita
		// debido a como se registran los componentes. Por ello Init y
		// Release no son sim�tricos.

		// TODO Esto no deberia ir dentro del Logic::Server::Release?
		if(Logic::CComponentFactory::getSingletonPtr())
			delete Logic::CComponentFactory::getSingletonPtr();

		if(Logic::CServer::getSingletonPtr())
			Logic::CServer::Release();

		// Liberamos la red
		if (Net::CManager::getSingletonPtr())
			Net::CManager::Release();
		
		// Liberar servidor de IA 
		//if (AI::CServer::getSingletonPtr())
		//	AI::CServer::Release();

		// Liberar los recursos del servidor de f�sica
		if (Physics::CServer::getSingletonPtr())
			Physics::CServer::Release();

		if(GUI::CServer::getSingletonPtr())
			GUI::CServer::Release();
		
		if(Graphics::CServer::getSingletonPtr())
			Graphics::CServer::Release();

		if(GUI::CInputManager::getSingletonPtr())
		{			
			GUI::CInputManager::getSingletonPtr()->removeKeyListener(this);// Dejamos de ser oyentes de los eventos del teclado.			
			GUI::CInputManager::getSingletonPtr()->removeMouseListener(this);// Y de los eventos del rat�n
			GUI::CInputManager::Release(); // Y liberamos
		}

		if(BaseSubsystems::CServer::getSingletonPtr())
			BaseSubsystems::CServer::Release();

	} // release

	//--------------------------------------------------------

	void C3DApplication::tick(unsigned int msecs) 
	{
		CBaseApplication::tick(msecs);

		GUI::CInputManager::getSingletonPtr()->tick();

		Graphics::CServer::getSingletonPtr()->tick(msecs/1000.0f);

		Net::CManager::getSingletonPtr()->tick(msecs); 
		// ��� Necesario para tx/rx peticiones de union a partida en los lobbies

	} // tick

} // namespace Application
