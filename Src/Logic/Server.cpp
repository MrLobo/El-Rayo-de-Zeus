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

#include "Map/MapParser.h"
#include "Logic/Entity/RingStruct.h"
#include "Logic/GameNetMsgManager.h"

#include <cassert>

namespace Logic {

	CServer* CServer::_instance = 0;

	//--------------------------------------------------------

	CServer::CServer() : _map(0), _gameNetMsgManager(0)
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
		unLoadLevel();

		Logic::CGameNetMsgManager::Release();

		Logic::CEntityFactory::Release();
		
		Map::CMapParser::Release();

	} // close

	//--------------------------------------------------------

	bool CServer::loadLevel(const std::string &filename)
	{
		// solo admitimos un mapa cargado, si iniciamos un nuevo nivel 
		// se borra el mapa anterior.
		unLoadLevel();

		if(_map = CMap::createMapFromFile(filename))
		{
			return true;
		}

		return false;

	} // loadLevel

	bool CServer::setRingPositions()
	{
		//inicializamos la estructura de posiciones de los anillos
		//el primer anillo de la primera base, empezando por abajo, ser� la base de la pila de anillos
		
		for (int i=0;i<=Logic::NUM_BASES;++i)
		{
			Logic::base.posBase[i]._down=Logic::startingBasesPosition+i*Logic::separationBetweenBases;
			Logic::base.posBase[i]._center=base.posBase[i]._down+Logic::separationBetweenRings;
			Logic::base.posBase[i]._up=base.posBase[i]._down+2*Logic::separationBetweenRings;
		}
		return true;
	}

	Vector3 CServer::getRingPositions(unsigned short base,Logic::LogicalPosition::Ring ring)
	{
		Vector3 retorno= Vector3::ZERO;
			switch (ring)
			{
				case Logic::LogicalPosition::LOWER_RING:
				{
					//return Logic::base.posBase[base]._down+Vector3(0,126,0);
					return Logic::base.posBase[base]._down;
				}
				case Logic::LogicalPosition::CENTRAL_RING:
				{
					//return Logic::base.posBase[base]._center+Vector3(0,126,0);
					return Logic::base.posBase[base]._center;
				}
				case Logic::LogicalPosition::UPPER_RING:
				{
					//return Logic::base.posBase[base]._up+Vector3(0,126,0);
					return Logic::base.posBase[base]._up;
				}
				default:
					{
					return Logic::base.posBase[base]._center+Vector3(0,126,0);
					//situaci�n an�mala, se lanzar�a una excepci�n o trazas por consola. Se le asigna el anillo central para que 
					//pese a todo no pete.
					}								
			}

	}

	float CServer::getRingRadio(unsigned short base,Logic::LogicalPosition::Ring ring)
	{
			switch (ring)
			{
				case Logic::LogicalPosition::LOWER_RING:
				{
					return Logic::RADIO_MENOR;
				}
				case Logic::LogicalPosition::CENTRAL_RING:
				{
					return Logic::RADIO_MAYOR;
				}
				case Logic::LogicalPosition::UPPER_RING:
				{
					return Logic::RADIO_MENOR;
				}
				default:
					{
						return Logic::RADIO_MAYOR;
					//situaci�n an�mala, se lanzar�a una excepci�n o trazas por consola. Se le asigna el anillo central para que 
					//pese a todo no pete.
					}								
			}

	}
	//--------------------------------------------------------

	void CServer::unLoadLevel()
	{
		if(_map)
		{
			_map->deactivate();
			delete _map;
			_map = 0;
		}
		_player = 0;

	} // unLoadLevel

	//---------------------------------------------------------

	bool CServer::activateMap() 
	{
		// Se activa la escucha del oyente de los mensajes de red para el estado de juego.
		_gameNetMsgManager->activate();
		return _map->activate();

	} // activateMap

	//---------------------------------------------------------

	void CServer::deactivateMap() 
	{
		_map->deactivate();
		_gameNetMsgManager->deactivate(); // Se desactiva la escucha del oyente de los mensajes de red para el estado de juego.

	} // deactivateMap

	//---------------------------------------------------------

	void CServer::tick(unsigned int msecs) 
	{
		// Eliminamos las entidades que se han marcado para ser eliminadas.
		Logic::CEntityFactory::getSingletonPtr()->deleteDefferedEntities();

		_map->tick(msecs);

	} // tick

} // namespace Logic
