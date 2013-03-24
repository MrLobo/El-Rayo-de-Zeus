/**
@file Map.cpp

Contiene la implementaci�n de la clase CMap, Un mapa l�gico.

@see Logic::Map

@author David Llans�
@date Agosto, 2010
*/

#include "Map.h"

#include "Logic/Server.h"
#include "Logic/Entity/Entity.h"
#include "EntityFactory.h"

#include "Map/MapParser.h"
#include "Map/MapEntity.h"

#include "Graphics/Server.h"
#include "Graphics/Scene.h"

#include "Physics/Server.h"
#include "Physics/Scene.h"

#include <cassert>


// HACK. Deber�a leerse de alg�n fichero de configuraci�n
#define MAP_FILE_PATH "./media/maps/"

namespace Logic {
		

	// ��� Creaci�n de un mapa con nombre name (normalmente el propio filename). => Creaci�n de escenas f�sica y gr�fica
	CMap::CMap(const std::string &name) : _name(name), _isActive(false)
	{
		_graphicScene = Graphics::CServer::getSingletonPtr()->createScene(name);
		_physicScene  = Physics::CServer::getSingletonPtr()->createScene(name); 
		// ��� aunque se creen las escenas, la escena activa debe ser la dummy hasta la activaci�n del map

	} // CMap

	//--------------------------------------------------------

	// TODO FRS Revisar estos if (necesarios?)
	CMap::~CMap()
	{
		deactivate();
		destroyAllEntities();
		if(Graphics::CServer::getSingletonPtr())
			Graphics::CServer::getSingletonPtr()->removeScene(_graphicScene);
		if(Physics::CServer::getSingletonPtr() )
			Physics::CServer::getSingletonPtr()->removeScene(_physicScene);

	} // ~CMap

	//--------------------------------------------------------

	bool CMap::activate()
	{			
		if(_isActive)
			return true;

		Graphics::CServer::getSingletonPtr()->setActiveScene(_graphicScene);	
		Physics::CServer::getSingletonPtr()->setActiveScene(_physicScene);	

		// Activamos todas las entidades registradas en el mapa.
		_isActive = true;
		TEntityList::const_iterator it = _entityList.begin();
		TEntityList::const_iterator end = _entityList.end();
			for(; it != end; ++it)
				_isActive = (*it)->activate() && _isActive;

		return _isActive;
	} // getEntity

	//--------------------------------------------------------

	void CMap::deactivate()
	{
		if(!_isActive)
			return;

		// Desactivamos todas las entidades activas registradas en el mapa.
		TEntityList::const_iterator it  = _entityList.begin();
		TEntityList::const_iterator end = _entityList.end();
			for(; it != end; it++)
				if((*it)->isActivated())
					(*it)->deactivate();

		Graphics::CServer::getSingletonPtr()->setActiveScene(0);
		Physics::CServer::getSingletonPtr()->setActiveScene(0);

		_isActive = false;
	} // getEntity

	//---------------------------------------------------------

	void CMap::tick(unsigned int msecs) 
	{
		TEntityList::const_iterator it = _entityList.begin();
		TEntityList::const_iterator end = _entityList.end();
			for(; it != end; ++it )
				(*it)->tick(msecs);
	} // tick

	//--------------------------------------------------------

	CMap* CMap::createMapFromFile(const std::string &filename)
	{
		// Completamos la ruta con el nombre proporcionado
		std::string completePath(MAP_FILE_PATH);
			completePath = completePath + filename + ".txt";
				
				if(!Map::CMapParser::getSingletonPtr()->parseFile(completePath)){ // Parseamos el fichero
					assert(!"No se ha podido parsear el mapa.");
					return false;
				}

		// Si se ha realizado con �xito el parseo creamos el mapa.
		CMap *map = new CMap(filename); // Desencadena la creaci�n de las escenas f�sica y gr�fica

		// Extraemos las entidades del parseo.
		Map::CMapParser::TEntityList entityList = 
			Map::CMapParser::getSingletonPtr()->getEntityList();

		CEntityFactory* entityFactory = CEntityFactory::getSingletonPtr();
		
		
		// Creamos todas las entidades l�gicas.
		Map::CMapParser::TEntityList::const_iterator it = entityList.begin();
		Map::CMapParser::TEntityList::const_iterator end = entityList.end();
			for(; it != end; it++)		{			
				CEntity *entity = entityFactory->createMergedEntity((*it),map); // La propia factor�a se encarga de a�adir la entidad al mapa.
				assert(entity && "No se pudo crear una entidad del mapa");
			}


		return map;

	} // createMapFromFile

	//--------------------------------------------------------

	void CMap::createPlayer(std::string entityName, std::string model, bool isLocalPlayer)
	{
		// [���] Creamos un nuevo jugador. Deber�amos tener la info del player
		// almacenada en _playerInfo as� que solo habr�a que modificarle el
		// "name". Luego se crea la entidad del jugador con la factor�a de 
		// entidades y se le dice si es o no el jugador local (con setIsPlayer())
		// Para que no salgan todos los jugadores unos encima de otros podemos
		// cambiar la posici�n de �stos.

		Map::CEntity playerInfo(entityName);
			playerInfo.setType("Player");			
		if(model.length() > 0)
			playerInfo.setAttribute("model", model);
			playerInfo.setAttribute("isPlayer", isLocalPlayer? "true" : "false");
			
		CEntity* newPlayer = CEntityFactory::getSingletonPtr()->createMergedEntity(&playerInfo, this);
			//newPlayer->setPosition( newPlayer->getPosition() + (rand()%50-25) * Vector3(1, 0, 1) ); // TODO calibrar esta pos
	
	} // createPlayer

	



	/**********************
		ENTITY MANAGEMENT
	**********************/

	void CMap::destroyAllEntities()
	{
		CEntityFactory* entityFactory = CEntityFactory::getSingletonPtr();
		
		// Eliminamos todas las entidades. La factor�a se encarga de
		// desactivarlas y sacarlas previamente del mapa.
		// FRS No podemos usar el removeEntity ya que modificar�a la lista mientras la recorremos
		TEntityList::iterator it = _entityList.begin();
		TEntityList::iterator end = _entityList.end();
			while(it != end)			
				entityFactory->deleteEntity( *it++ ); 
			// FRS Incrementamos el iterador antes del deleteEntity
			// ya que el m�todo modifica el _entityList

		_entityList.clear();
		_entityMap.clear();
	} // destroyAllEntities

	//--------------------------------------------------------	

	void CMap::addEntity(CEntity *entity)
	{
		if( !_entityMap.count(entity->getEntityID() ) )
		{			
			_entityMap[entity->getEntityID()] = entity;
			_entityList.push_back(entity); 
		}

	} // addEntity

	//--------------------------------------------------------

	void CMap::removeEntity(CEntity *entity)
	{
		if(_entityMap.count( entity->getEntityID() ) )
		{
			if(entity->isActivated())
				entity->deactivate();
			entity->_map = 0;
			_entityMap.erase(entity->getEntityID());
			_entityList.remove(entity);
		}

	} // removeEntity


	//--------------------------------------------------------

	CEntity* CMap::getEntityByName(const std::string &name, CEntity *start)
	{
		// Si se defini� entidad desde la que comenzar la b�squeda 
		// cogemos su posici�n y empezamos desde la siguiente.
		TEntityMap::const_iterator end = _entityMap.end();
		TEntityMap::const_iterator it;
			if (!start)
				it = _entityMap.begin();
			else {
				it = _entityMap.find(start->getEntityID());				
					if(it == end)
						return 0;// si la entidad no existe devolvemos NULL.
					else	
						++it;
			}		
			
		// BUSQUEDA => TODO FRS Lo mismo esto con el list es m�s eficiente
		for(; it != end; ++it){			
			if ( (*it).second->getName() == name)// si hay coincidencia de nombres devolvemos la entidad.
				return (*it).second;
		}		
		return 0;// si no se encontr� la entidad devolvemos NULL.
	} // getEntityByName

	//--------------------------------------------------------

	CEntity* CMap::getEntityByType(const std::string &type, CEntity *start)
	{
		// Si se defini� entidad desde la que comenzar la b�squeda 
		// cogemos su posici�n y empezamos desde la siguiente.
		TEntityMap::const_iterator end = _entityMap.end();
		TEntityMap::const_iterator it;
			if (!start)
				it = _entityMap.begin();
			else {
				it = _entityMap.find(start->getEntityID());		
					if(it == end)
						return 0;// si la entidad no existe devolvemos NULL.
					else
						++it;
			}		

		for(; it != end; ++it)	{// si hay coincidencia de nombres devolvemos la entidad			
			if ( (*it).second->getType() == type )
				return (*it).second;
		}
	
		return 0;	// si no se encontr� la entidad devolvemos NULL.

	} // getEntityByType

	

} // namespace Logic
