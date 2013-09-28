/**
@file Server.cpp

Contiene la implementaci�n de la clase CServer, Singleton que se encarga de
la gesti�n de la l�gica del juego.

@see Logic::CServer

@author David Llans�
@date Agosto, 2010
*/

#include "Server.h"
#include "Logic/Maps/Map.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
//#include "Logic/Entity/Entity.h"

#include "Map/MapParser.h"
#include "Logic/Entity/RingStruct.h"
#include "Logic/GameNetMsgManager.h"
#include "Logic/GameStatus.h"

#include "Logic\Entity\RingStruct.h"

//PT
#include <cegui\CEGUIWindowManager.h>
#include <cegui\elements\CEGUIProgressBar.h>
#include <Graphics\Server.h>

#include <cassert>

namespace Logic {

	CServer* CServer::_instance = 0;

	//--------------------------------------------------------

	CServer::CServer() :  _gameNetMsgManager(0), _worldIsLoaded(false)
	{
		_instance = this;

	} // CServer

	//--------------------------------------------------------

	CServer::~CServer()
	{
		_instance = 0;

	} // ~CServer
	
	//--------------------------------------------------------

	bool CServer::Init()
	{
		assert(!_instance && "Segunda inicializaci�n de Logic::CServer no permitida!");

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
		assert(_instance && "Logic::CServer no est� inicializado!");

		if(_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	bool CServer::open()
	{
		// Inicializamos el parser de mapas.
		if (!Map::CMapParser::Init())
			return false;

		// Inicializamos la factor�a de entidades.
		if (!Logic::CEntityFactory::Init())
			return false;

		// Inicializamos el gestor de los mensajes de red durante el estado de juego
		if (!Logic::CGameNetMsgManager::Init())
			return false;

		_gameNetMsgManager = Logic::CGameNetMsgManager::getSingletonPtr();

		return true;

	} // open

	//--------------------------------------------------------

	void CServer::close() 
	{
		Logic::CGameNetMsgManager::Release();

		Logic::CEntityFactory::Release();
		
		Map::CMapParser::Release();
	} // close

	//--------------------------------------------------------

	
	void CServer::tick(unsigned int msecs) 
	{
		// Eliminamos las entidades que se han marcado para ser eliminadas.
		moveDefferedEntities();
		Logic::CEntityFactory::getSingletonPtr()->deleteDefferedEntities();

		TMaps::const_iterator it = _maps.begin();
		TMaps::const_iterator end = _maps.end();
		
		for (; it != end; ++it)
			it->second->tick(msecs);
	} // tick

	//---------------------------------------------------------

	// ��� Carga el map desde fichero ==> ejecuta el entity.spawn()
	bool CServer::loadMap(const std::string &filename)
	{
		// solo admitimos un mapa cargado, si iniciamos un nuevo nivel 
		// se borra el mapa anterior.
		unLoadMap(filename);

		if(_maps[filename] = CMap::createMapFromFile(filename))
			return true;

		return false;
	} // loadLevel

	//--------------------------------------------------------

	void CServer::unLoadMap(const std::string &filename)
	{
		TMaps::const_iterator it = _maps.find(filename);
		
		if(it != _maps.end())
		{
			it->second->deactivate();
			delete _maps[filename];
			_maps[filename] = 0;
			_maps.erase(it);
		}

	} // unLoadLevel

	//---------------------------------------------------------

	bool CServer::loadWorld(TMapNameList &mapList)
	{		
		if(_worldIsLoaded)
			unLoadWorld();

		// Inicializamos el gestor de los mensajes de red durante el estado de juego
		//PT Paso el mapList y no el mapList.size() para luego recuperar datos de ese mapList
		//if (!Logic::CGameStatus::Init(mapList.size()))
		if (!Logic::CGameStatus::Init(mapList))
			return false;

		TMapNameList::const_iterator it = mapList.begin();
		TMapNameList::const_iterator end = mapList.end();
		
		//PT creo la barra de progreso
		CEGUI::ProgressBar *hbar = static_cast<CEGUI::ProgressBar*> (CEGUI::WindowManager::getSingleton().getWindow("MenuSingle/Progreso"));
		float progress = hbar->getProgress();
		int nummaps = mapList.size();
		float step = (0.9f - progress) / nummaps;

		int i = 1;
		std::string texto = "";

		for (; it != end; ++it)
		{
			_worldIsLoaded = loadMap(*it);
			_mapNames.push_back(*it);

			//PT
			progress+= step;
			hbar->setProgress(progress);

			std::string result;
			std::stringstream ss;
			ss.clear();
			ss << "Loading maps: " << i << " / " << nummaps;

			result = ss.str();

			CEGUI::WindowManager::getSingleton().getWindow("MenuSingle/TextoProgreso")->setText(result);
			Graphics::CServer::getSingletonPtr()->tick(0);

			i++;
		}	

  		mapList.clear();
		return _worldIsLoaded;
	}

	//---------------------------------------------------------

	void CServer::unLoadWorld()
	{
		if(_worldIsLoaded) {

			TMaps::const_iterator it = _maps.begin();
			TMaps::const_iterator end = _maps.end();
		
			while (it != end)
				unLoadMap(it++->first);

			Logic::CGameStatus::Release(); // FRS 1304 Borramos GameStatus
			_player = 0;
			_worldIsLoaded = false;
		}
	}

	//---------------------------------------------------------

	bool CServer::activateMap(const std::string &filename) 
	{
		// Se activa la escucha del oyente de los mensajes de red para el estado de juego.
		//_gameNetMsgManager->activate();
		return _maps[filename]->activate();

	} // activateMap

	//---------------------------------------------------------

	void CServer::deactivateMap(const std::string &filename) 
	{
		TMaps::const_iterator it = _maps.find(filename);
		
		if(it != _maps.end())
			it->second->deactivate();
		//_gameNetMsgManager->deactivate(); // Se desactiva la escucha del oyente de los mensajes de red para el estado de juego.
	} // deactivateMap

	//---------------------------------------------------------

	bool CServer::activateAllMaps()
	{
		TMaps::const_iterator it = _maps.begin();
		TMaps::const_iterator end = _maps.end();

		bool activated = false;

		for (; it != end; ++it)
			activated = it->second->activate();

		_gameNetMsgManager->activate();

		
		_player->getMap()->activatePlayerCam();
		

		return activated;

	}

	//---------------------------------------------------------

	void CServer::deactivateAllMaps()
	{
		TMaps::const_iterator it = _maps.begin();
		TMaps::const_iterator end = _maps.end();

		for (; it != end; ++it)
			it->second->deactivate();

		_gameNetMsgManager->deactivate();
	}

	//---------------------------------------------------------
	
	CMap* CServer::getMap(const std::string mapName)
	{
		TMaps::const_iterator it = _maps.find(mapName);

		if (it != _maps.end())
			return it->second;
		
		return NULL;
	}

	//---------------------------------------------------------

	void CServer::deferredMoveEntity(CEntity *entity, int targetMap)
	{
		assert(entity);
		_entitiesToMove[targetMap].push_back(entity);

	}

	std::ostringstream  CServer::getBasestring(int base)
	{
		//soluci�n propuesta:
		std::ostringstream result;
			switch(base)
			{
			case 1:
				result << "mapRed";
				break;
			case 2:
				result << "mapBlue";
				break;
			case 3:
				result << "mapGreen";
				break;
			case 4:
				result << "mapYellow";
				break;
			}
			return result;
	}


	//---------------------------------------------------------

	void CServer::moveDefferedEntities()
	{
		TMapEntityLists::iterator it = _entitiesToMove.begin();
		TMapEntityLists::const_iterator end = _entitiesToMove.end();

		for (; it != end; ++it)
		{
			TEntityList::const_iterator entity = it->second.begin();
			TEntityList::const_iterator entityEnd = it->second.end();

			for (; entity != entityEnd; ++entity)
			{
				(*entity)->detachFromMap();
				(*entity)->attachToMap(_maps[ _mapNames[it->first - 1] ]);
				(*entity)->activate();	

				if ((*entity)->isPlayer())
					(*entity)->getMap()->activatePlayerCam(); 
			}
			it->second.clear();
		}

	}

	//---------------------------------------------------------

	void CServer::activateBaseCam(int targetMap)
	{
		_maps[ _mapNames[targetMap - 1] ]->activateBaseCam();
	}

	//---------------------------------------------------------

	void CServer::activatePlayerCam()
	{
		_player->getMap()->activatePlayerCam();
	}

	//---------------------------------------------------------

	Vector3 TRingPositions::_up;
	Vector3 TRingPositions::_center;
	Vector3 TRingPositions::_down;

	bool CServer::setRingPositions()
	{
		//inicializamos la estructura de posiciones de los anillos
		//el primer anillo de la primera base, empezando por abajo, ser� la base de la pila de anillos
		
		TRingPositions::_down = Logic::startingRingPosition;
		TRingPositions::_center  = TRingPositions::_down + Logic::separationBetweenRings;
		TRingPositions::_up  = TRingPositions::_center + Logic::separationBetweenRings;

		return true;
	}

	//---------------------------------------------------------
	Vector3 CServer::getRingPosition(LogicalPosition::Ring ring)

	{
		Vector3 retorno= Vector3::ZERO;
			switch (ring)
			{
				case Logic::LogicalPosition::LOWER_RING:				
					return TRingPositions::_down;
				
				case Logic::LogicalPosition::CENTRAL_RING:				
					return TRingPositions::_center;
				
				case Logic::LogicalPosition::UPPER_RING:				
					return TRingPositions::_up;
				
				default:					
					return TRingPositions::_center;
					//situaci�n an�mala, se lanzar�a una excepci�n o trazas por consola. Se le asigna el anillo central para que 
					//pese a todo no pete.					
			}

	}

	//---------------------------------------------------------

	float CServer::getRingRadio(Logic::LogicalPosition::Ring ring)
	{
			switch (ring)
			{
				case Logic::LogicalPosition::LOWER_RING:				
					return Logic::RADIO_MENOR;
				
				case Logic::LogicalPosition::CENTRAL_RING:				
					return Logic::RADIO_MAYOR;
				
				case Logic::LogicalPosition::UPPER_RING:				
					return Logic::RADIO_MENOR;
				
				default:					
					return Logic::RADIO_MAYOR;
					//situaci�n an�mala, se lanzar�a una excepci�n o trazas por consola. Se le asigna el anillo central para que 
					//pese a todo no pete.
												
			}

	}

	//---------------------------------------------------------

	void CServer::compositorEnable(const std::string &name)
	{
		Graphics::CServer::getSingletonPtr()->compositorEnable(name);
	}

	//---------------------------------------------------------

	void CServer::compositorDisable(const std::string &name)
	{
		Graphics::CServer::getSingletonPtr()->compositorDisable(name);
	}


} // namespace Logic
