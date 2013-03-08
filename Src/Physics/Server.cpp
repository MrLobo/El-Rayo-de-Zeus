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
	CServer *CServer::_instance = 0;

	//--------------------------------------------------------

	CServer::CServer() :  _activeScene(0) 
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
		_dummyScene = createScene("dummy_scene"); // Creamos la escena dummy para cuando no hay ninguna activa.		
		setActiveScene(_dummyScene); // Por defecto la escena activa es la dummy

		return true;
	} // open

	//--------------------------------------------------------
		
	void CServer::close()  // FRS Se ejecuta justo antes de la delete de _instance
	{
		if(_activeScene){
			_activeScene->deactivate();
			_activeScene = 0;
		}
		while(!_scenes.empty())		
			removeScene(_scenes.begin());
		
	} // close

	//--------------------------------------------------------

	
	CScene* CServer::createScene(const std::string &name)
	{
		assert(_instance && "PHYSICS::SERVER>> Servidor no inicializado");			
		assert(_scenes.find(name)==_scenes.end() && "Ya se ha creado una escena con este nombre.");

		CScene *scene = new CScene(name);
			_scenes[name] =  scene;
		return scene;
	} // createScene

	//--------------------------------------------------------

	void CServer::removeScene(CScene* scene)
	{
		assert(_instance && "PHYSICS::SERVER>> Servidor no inicializado");
		
		if(_activeScene == scene) // Si borramos la escena activa tenemos que quitarla.
			_activeScene = 0;
		_scenes.erase(scene->getName());
		delete scene;
	}

	void CServer::removeScene(const std::string& name)
	{
		removeScene( _scenes[name] );
	} // removeScene

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

	//
	Physics::CActor* CServer::createActor(
		const Logic::TLogicalPosition &position, 
		const float angularWidth, const float height, 
		bool isTrigger, IObserver *component) 
	{
		assert(_scene && "PHYSICS::SERVER>> Imposible crear un actor sin haber creado la escena f�sica previamente");

		if(isTrigger)  {

			Physics::CActorTrigger *actor =	new Physics::CActorTrigger(position, angularWidth, height, component);
			_scene->addActor(actor); // A�adir el actor a la escena
			return actor;

		} else {

			Physics::CActor *actor = new Physics::CActor(position, angularWidth, height, component);
			_scene->addActor(actor); // A�adir el actor a la escena
			return actor;

		}
	} // createActor


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