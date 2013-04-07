/**
@file LUA_EntityFunctions.h

LUA


@author Pablo Terrado
@date Abril, 2013
*/

#ifndef __ScriptManager_LUA_EntityFunctions_H
#define __ScriptManager_LUA_EntityFunctions_H

//PT
// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace BaseSubsystems 
{
  class CServer;
  class CMath;
}

namespace Logic
{
	class CEntity;
	class CMap;
	class CEntityFactory;
	class CServer;
}

namespace Application
{
	class CGaleonApplication;
}
// Fin Predeclaraci�n de clases para ahorrar tiempo de compilaci�n


namespace ScriptManager
{

	void deleteEntity(unsigned int entityID);
	bool isPlayer (unsigned int entityID);
	unsigned int getEntityID (std::string entityName);
	std::string getName (unsigned int entityID);
	void gameOver ();
	

} // namespace ScriptManager

#endif // __ScriptManager_LUA_EntityFunctions_H