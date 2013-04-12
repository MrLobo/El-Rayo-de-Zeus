/**
@file Server.cpp

Contiene la implementaci�n de la clase CServer, Singleton que se encarga de
la gesti�n de la interfaz con el usuario (entrada de perif�ricos, CEGui...).

@see GUI::CServer

@author David Llans�
@date Agosto, 2010
*/

#include "Server.h"

#include "PlayerController.h"

#include "CameraController.h"
#include "BaseSubsystems/Server.h"

#include <cassert>
#include <CEGUISystem.h>
#include <CEGUIWindowManager.h>
#include <CEGUIWindow.h>
#include <CEGUISchemeManager.h>
#include <CEGUIFontManager.h>
#include <ScriptingModules\LuaScriptModule\CEGUILua.h> //PT
#include "ScriptManager\Server.h" //PT

namespace GUI {

	CServer* CServer::_instance = 0;

	//--------------------------------------------------------

	CServer::CServer() : _playerController(0)
	{
		_instance = this;

	} // CServer

	//--------------------------------------------------------

	CServer::~CServer()
	{
		_instance = 0;
		_currentWindow = NULL;

	} // ~CServer
	
	//--------------------------------------------------------

	bool CServer::Init()
	{
		assert(!_instance && "Segunda inicializaci�n de GUI::CServer no permitida!");

		new CServer();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;

	} // Init

	//--------------------------------------------------------

	void CServer::Release()
	{
		assert(_instance && "GUI::CServer no est� inicializado!");

		if(_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	bool CServer::open()
	{
		_playerController = new CPlayerController();
		_cameraController = new CCameraController();

		_GUISystem = BaseSubsystems::CServer::getSingletonPtr()->getGUISystem();


		//PT
		//Carga de las plantillas y archivos de fuentes con LUA en lugar de con CEGUI
		//No hace falta pasarle toda la ruta del script porque se supone que todos los scripts de LUA
		//van a estar en media/scripts . Tampoco hace falta pasarle la extension .lua.
		//Todo ello se hace en la carga del script. ScriptManager::CServer::loadScript()
		ScriptManager::CServer::getSingletonPtr()->loadExeScript("GUI");

		/*
		//Carga de las plantillas y archivos de fuentes directamente con CEGUI
		// Cargamos las distintas plantillas o esquemas de fichero
		// que usaremos en nuestro GUI.
		// (autom�ticamente cargan los archivos looknfeel e imageset)
		CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
		CEGUI::SchemeManager::getSingleton().create("OgreTray.scheme");
		

		// Cargamos los archivos con las fuentes que usaremos.
		CEGUI::FontManager::getSingleton().create("DejaVuSans-10.font");
		CEGUI::FontManager::getSingleton().create("FairChar-30.font");
		CEGUI::FontManager::getSingleton().create("Batang-26.font");
		CEGUI::FontManager::getSingleton().create("handshop-30.font");


		//Establecemos cual sera el puntero del raton
		_GUISystem->setDefaultMouseCursor("OgreTrayImages","MouseArrow");
		*/

#ifndef NON_EXCLUSIVE_MODE_IN_WINDOW_MODE 
		// Establecemos cual ser� el puntero del rat�n.
		_GUISystem->setDefaultMouseCursor("OgreTrayImages","MouseArrow");
#endif	

		CInputManager::getSingletonPtr()->addMouseListener(this);
		CInputManager::getSingletonPtr()->addKeyListener(this);

		return true;

	} // open

	//--------------------------------------------------------

	void CServer::close() 
	{
		CInputManager::getSingletonPtr()->removeKeyListener(this);
		CInputManager::getSingletonPtr()->removeMouseListener(this);

		delete _playerController;
		delete _cameraController;
	} // close

	//--------------------------------------------------------

	bool CServer::keyPressed(TKey key)
	{
		_GUISystem->injectKeyDown(key.keyId);    
		_GUISystem->injectChar(key.text);    

		
		// Queremos que si hay m�s oyentes tambi�n reciban el evento
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CServer::keyReleased(TKey key)
	{
		_GUISystem->injectKeyUp(key.keyId);

		
		// Queremos que si hay m�s oyentes tambi�n reciban el evento
		return false;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CServer::mouseMoved(const CMouseState &mouseState)
	{
#if defined NON_EXCLUSIVE_MODE_IN_WINDOW_MODE
		_GUISystem->injectMousePosition((float)mouseState.posAbsX,(float)mouseState.posAbsY);
#else 
		_GUISystem->injectMouseMove((float)mouseState.movX,(float)mouseState.movY);
#endif	
		// Queremos que si hay m�s oyentes tambi�n reciban el evento
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CServer::mousePressed(const CMouseState &mouseState)
	{
		switch (mouseState.button)
		{
		case Button::LEFT:
			_GUISystem->injectMouseButtonDown(CEGUI::LeftButton);
		case Button::RIGHT:
			_GUISystem->injectMouseButtonDown(CEGUI::RightButton);
		case Button::MIDDLE:
			_GUISystem->injectMouseButtonDown(CEGUI::MiddleButton);
		}

		// Queremos que si hay m�s oyentes tambi�n reciban el evento
		return false;

	} // mousePressed

	//--------------------------------------------------------

	bool CServer::mouseReleased(const CMouseState &mouseState)
	{
		switch (mouseState.button)
		{
		case Button::LEFT:
			_GUISystem->injectMouseButtonUp(CEGUI::LeftButton);
		case Button::RIGHT:
			_GUISystem->injectMouseButtonUp(CEGUI::RightButton);
		case Button::MIDDLE:
			_GUISystem->injectMouseButtonUp(CEGUI::MiddleButton);
		}

		// Queremos que si hay m�s oyentes tambi�n reciban el evento
		return false;

	} // mouseReleased

	void CServer::setText(const std::string& msg) {
			_currentWindow->setText(msg.c_str());
	}

} // namespace GUI
