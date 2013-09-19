/**
@file SceneLoader.cpp


 
@see Logic::CSceneLoader
@see Logic::IComponent

@author 
@date 
*/

#include "SceneLoader.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Graphics/DotSceneLoader/DotSceneLoader.h"
#include "Graphics/Scene.h"
#include "Logic/Maps/EntityFactory.h"


#include "Logic/Entity/Messages/Message.h"


namespace Logic 
{
	IMP_FACTORY(CSceneLoader);
	
	//---------------------------------------------------------

	CSceneLoader::~CSceneLoader() 
	{
		
	} // ~CDotSceneLoader

	//---------------------------------------------------------

	bool CSceneLoader::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		_scene = map->getGraphicScene();
			assert(_scene && "Escena gr�fica es NULL");

		assert(entityInfo->hasAttribute("sceneFile"));
			_sceneFile = entityInfo->getStringAttribute("sceneFile");

		assert(entityInfo->hasAttribute("skyXPresetName"));
			_skyXPresetName = entityInfo->getStringAttribute("skyXPresetName");

		assert(entityInfo->hasAttribute("hydraXConfigFile"));
			_hydraXConfigFile = entityInfo->getStringAttribute("hydraXConfigFile");

		Graphics::DotSceneLoader loader;
#ifdef _DEBUG

#else		
		loader.parseDotScene(_sceneFile + ".scene", "General", _scene->getSceneMgr());
#endif
		_scene->skyXLoadPreset(_skyXPresetName);
		if (_hydraXConfigFile == "")
			loader.parseDotScene(_sceneFile + "fakeWater" + ".scene", "General", _scene->getSceneMgr());
		else
			_scene->hydraXLoadCFG(_hydraXConfigFile);
		
		return true;

	} // spawn
		
} // namespace Logic

