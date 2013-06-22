/**
@file Interfaz.cpp

Contiene la declaraci�n de la clase CInterfaz, Singleton que se encarga de
la gesti�n de la interfaz HUD y SHOP

@see GUI::CInterfaz

@author Pablo Terrado
@date Junio, 2013
*/

#include "Interfaz.h"


#include <cassert>
#include <CEGUISystem.h>
#include <CEGUIWindowManager.h>
#include <CEGUIWindow.h>
#include <CEGUISchemeManager.h>
#include <CEGUIFontManager.h>

#include <BaseSubsystems/Server.h>
#include <ScriptingModules\LuaScriptModule\CEGUILua.h> //PT
#include <ScriptManager\Server.h> //PT


namespace GUI {

	CInterfaz* CInterfaz::_instance = 0;

	//--------------------------------------------------------

	CInterfaz::CInterfaz()
	{
		_instance = this;

	} // CInterfaz

	//--------------------------------------------------------

	CInterfaz::~CInterfaz()
	{
		_instance = 0;
		_hudWindow = NULL;
		_shopWindow = NULL;

	} // ~CInterfaz
	
	//--------------------------------------------------------

	bool CInterfaz::Init()
	{
		assert(!_instance && "Segunda inicializaci�n de GUI::CServer no permitida!");

		new CInterfaz();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;

	} // Init

	//--------------------------------------------------------

	void CInterfaz::Release()
	{
		assert(_instance && "GUI::CServer no est� inicializado!");

		if(_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	bool CInterfaz::open()
	{

		//PT
		//Carga de las plantillas y archivos de fuentes con LUA en lugar de con CEGUI
		//No hace falta pasarle toda la ruta del script porque se supone que todos los scripts de LUA
		//van a estar en media/scripts . Tampoco hace falta pasarle la extension .lua.
		//Todo ello se hace en la carga del script. ScriptManager::CServer::loadScript()
		ScriptManager::CServer::getSingletonPtr()->loadExeScript("GUI");

		return true;

	} // open

	//--------------------------------------------------------

	void CInterfaz::close() 
	{

	} // close

	//--------------------------------------------------------

	void CInterfaz::setText(const std::string& msg) {
		_hudWindow->setText(msg.c_str());
		_shopWindow->setText(msg.c_str());
	}

} // namespace GUI
