/**
@file SceneLoader.h

Contiene la declaraci�n del componente que permite cargar una escena gr�fica
desde un fichero de mapa.

@see Logic::CSceneLoader
@see Logic::IComponent

@author Emilio Santalla
*/

#ifndef __Logic_SceneLoader_H
#define __Logic_SceneLoader_H

#include "Logic/Entity/Component.h"


// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Graphics 
{
	class CScene;
}

namespace Logic
{
	class CMessage;
}
//declaraci�n de la clase
namespace Logic 
{

	class CSceneLoader : public IComponent
	{
		DEC_FACTORY(CSceneLoader);

	public:

		CSceneLoader() : 
			IComponent(GetAltTypeIdOf(CSceneLoader)), _scene(0), _sceneFile("") {}


		virtual ~CSceneLoader();
		
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);


	protected:


		/**
		*/
		Graphics::CScene* _scene;

		/**.
		*/
		std::string _sceneFile;

		/**
		*/
		std::string _skyXPresetName;

		/**
		*/
		std::string _hydraXConfigFile;


	}; // class CGraphics

	REG_FACTORY(CSceneLoader);

} // namespace Logic

#endif // __Logic_Graphics_H
