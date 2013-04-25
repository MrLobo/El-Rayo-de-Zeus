/**
@file DotSceneLoader.h

Contiene la declaraci�n del componente que permite cargar una escena gr�fica
desde un fichero de mapa.

@see Logic::CDotSceneLoader
@see Logic::IComponent

@author Emilio Santalla
*/

#ifndef __Logic_DotSceneLoader_H
#define __Logic_DotSceneLoader_H

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

	class CDotSceneLoader : public IComponent
	{
		DEC_FACTORY(CDotSceneLoader);

	public:

		CDotSceneLoader() : 
			IComponent(GetAltTypeIdOf(CDotSceneLoader)), _scene(0), _scenarioFile("") {}


		virtual ~CDotSceneLoader();
		
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);


	protected:


		/**
		*/
		Graphics::CScene* _scene;

		/**.
		*/
		std::string _scenarioFile;


		

	}; // class CGraphics

	REG_FACTORY(CDotSceneLoader);

} // namespace Logic

#endif // __Logic_Graphics_H
