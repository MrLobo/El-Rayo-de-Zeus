/**
@file Server.cpp

Contiene la implementaci�n del servidor de f�sica. 

@see Physics::CServer

@author Emilio Santalla
@date Marzo, 2013
*/

#include "Server.h"
#include "CollisionManager.h"
#include "Logic/Entity/Components/Physics.h"
#include "Actor.h"
#include "Scene.h"

#include <assert.h>


namespace Physics {


	// �nica instancia del servidor
	CServer *CServer::_instance = NULL;

	//--------------------------------------------------------

	CServer::CServer() :  _scene(NULL) {} 

	//--------------------------------------------------------

	CServer::~CServer() {} 

	//--------------------------------------------------------

	bool CServer::Init() 
	{
		if (!_instance) {
			_instance = new CServer();
		}

		return true;
	} 

	//--------------------------------------------------------

	void CServer::Release()
	{
		if(_instance) {
			delete _instance;
			_instance = NULL;
		}
	} 

	//--------------------------------------------------------

	void CServer::createScene ()
	{
		assert(_instance);
	

		// Crear la escena f�sica
	_scene = new CScene();
	}

	//--------------------------------------------------------

	void CServer::destroyScene ()
	{
		assert(_instance);

		if (_scene) {
			_scene->release();
			_scene = NULL;
		}
	}

	//--------------------------------------------------------

	void CServer::tick(unsigned int msecs) 
	{
		assert(_scene);

		// Empezar la simulaci�n f�sica.

		_scene->simulate(msecs);

	} 

	//--------------------------------------------------------

Physics::CActor* CServer::createActor(const Logic::TLogicalPosition &position, const float angularWidth, 
											  const float height, bool trigger, 
													const Logic::IPhysics *component) 
	{
		assert(_scene);


	Physics::CActor *actor = new Physics::CActor(position, angularWidth, height, trigger, component);

		// A�adir el actor a la escena
		_scene->addActor(*actor);

		return actor;
	}



	//--------------------------------------------------------

void CServer::destroyActor(Physics::CActor *actor)
	{
		assert(_scene);

		// Eliminar el actor de la escena
	_scene->removeActor(actor);

		// Liberar recursos
		actor->release();
	}

	//--------------------------------------------------------

Logic::TLogicalPosition CServer::getActorLogicPosition(const CActor *actor)
	{
		assert(actor);

		// Devolver la posici�n y orientaci�n en coordenadas l�gicas
		return actor->getGlobalPose();
	}

	//--------------------------------------------------------

void CServer::moveActor(CActor *actor, const Logic::TLogicalPosition &pos)
	{
		assert(actor);

		// Mover el actor tras transformar el destino a coordenadas l�gicas
		actor->move(pos);
	}

	//--------------------------------------------------------

void CServer::moveActor(CActor *actor, const float degrees)
	{
		assert(actor);

		// Desplazar el actor
		Logic::TLogicalPosition pos = actor->getGlobalPose();
		pos._degrees += degrees;
		actor->move(pos);
	}

void CServer::moveActor(CActor *actor, const float degrees, const float height)
	{
		assert(actor);

		// Desplazar el actor
		Logic::TLogicalPosition pos = actor->getGlobalPose();
		pos._degrees += degrees;
		pos._height += height;
		actor->move(pos);
	}

	//--------------------------------------------------------


}