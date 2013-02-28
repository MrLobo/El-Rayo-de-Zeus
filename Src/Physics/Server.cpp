/**
@file Server.cpp

Contiene la implementaci�n del servidor de f�sica. 

@see Physics::CServer

@author Emilio Santalla
@date Marzo, 2013
*/
#include "Server.h"

#include "Physics/IObserver.h"
#include "Physics/Actor.h"
#include "Physics/Scene.h"

#include <assert.h>


namespace Physics {


	// �nica instancia del servidor
	CServer *CServer::_instance = NULL;

	//--------------------------------------------------------

	CServer::CServer() :  _scene(NULL) 
	{
		assert(!_instance && "Segunda inicializaci�n de Graphics::CServer no permitida!");

		_instance = this;
	} 

	//--------------------------------------------------------

	CServer::~CServer() 
	{
		assert(_instance);
		_instance = 0;
	} 

	//--------------------------------------------------------

	bool CServer::Init() 
	{
		assert(!_instance && "Segunda inicializaci�n de Physics::CServer no permitida!");

		new CServer();

		if (!_instance->open()) {
			Release();
			return false;
		}
		return true;
	} 

	//--------------------------------------------------------

	void CServer::Release()
	{
		if(_instance) {
			
			_instance->close();
			delete _instance;
		}
	} 

	//--------------------------------------------------------

	bool CServer::open()
	{
		_scene = createScene();		
		return true;
	} // open

	//--------------------------------------------------------

	void CServer::close() 
	{
		if(_scene)
		{
			_scene->deactivate();
			_scene = 0;
		}
	} // close

	//--------------------------------------------------------

	CScene* CServer::createScene ()
	{
		assert(_instance);
	
		// Crear la escena f�sica
		CScene *scene = new CScene();
		return scene;
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

	bool CServer::activateScene()
	{
		return _scene->activate();
	}

	//--------------------------------------------------------

	bool CServer::deactivateScene()
	{
		return _scene->deactivate();
	}

	//--------------------------------------------------------

	void CServer::tick(unsigned int msecs) 
	{
		assert(_scene);
		_scene->simulate(); // Empezar la simulaci�n f�sica.
	} 

	//--------------------------------------------------------

	Physics::CActor* CServer::createActor(
		const Logic::TLogicalPosition &position, 
		const float angularWidth, const float height, 
		bool isTrigger, IObserver *component) 
	{
		assert(_scene);

		if(isTrigger)  {

			Physics::CActorTrigger *actor =	new Physics::CActorTrigger(position, angularWidth, height, isTrigger, component);
			_scene->addActor(actor); // A�adir el actor a la escena
			return actor;

		} else {

			Physics::CActor *actor = new Physics::CActor(position, angularWidth, height, isTrigger, component);
			_scene->addActor(actor); // A�adir el actor a la escena
			return actor;

		}
	}


	//--------------------------------------------------------

	void CServer::destroyActor(Physics::CActor *actor)
	{
		assert(_scene);		
		_scene->removeActor(actor); // Eliminar el actor de la escena
		actor->release(); // Liberar recursos
	}

	//--------------------------------------------------------

	Logic::TLogicalPosition& CServer::getActorLogicPosition(CActor *actor)
	{
		assert(actor);
		return actor->getLogicPos(); // Devolver la posici�n y orientaci�n en coordenadas l�gicas
	}

	//--------------------------------------------------------


	// TODO ��� Devolver flags / eventos de sucesos? -> p.e  PxControllerFlag::eCOLLISION_DOWN / onFloor(enter/exit)
	// FRS Necesario para pasar posiciones relativas negativas (TLogicalPosition nos restring�a a unsigned's)
	void CServer::moveActor(CActor *actor, const float degrees, const float height, const char ring, const char base)
	{
		assert(actor);
		// Mover el actor tras transformar el destino a coordenadas l�gicas
		actor->move(degrees, height, ring, base);
	}

}