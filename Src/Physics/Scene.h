/**
@file Scene.h

Contiene la declaraci�n de la clase contenedora de los elementos
de una escena.

@see Physics::CScene

@author Emilio Santalla
@date Febrero 2013
*/

#ifndef __Physics_Scene_H
#define __Physics_Scene_H


#include <vector>


namespace Physics
{
	class CActor;
	class CActorTrigger;
	class CContactListener;	
}


class b2World;
class OgreB2DebugDraw;

namespace Physics 
{

	
	class CScene 
	{

	public:

		typedef std::vector<CActor*>		TActors;

		/************
			ACTORS
		*************/
		bool add(CActor *actor);
		void remove(CActor* actor);

		b2World* getPhysicWorld() {return _world;}

		void deferredGhostBody(CActor* actor);
		void deferredUnghostBody(CActor* actor);

		void createGhostBodies();
		void deleteGhostBodies();

	protected:

		/**
		Clase amiga. Solo el servidor f�sico puede crear o liberar escenas f�sicas, 
		activarlas o desactivarlas y actualizar su estado.
		*/
		friend class CServer;

		/**	Constructor de la clase.	*/
		CScene(const std::string& name);

		/**
		Destructor de la aplicaci�n.
		*/
		~CScene();

		/**	Despierta la escena*/
		bool activate();

		/**	Duerme la escena*/
		bool deactivate();
		void tick(float timeStep);

		void switchDebugDraw();


		/******************
			GET's & SET's
		********************/
		/**
		Devuelve el nombre de la escena.
		@return Nombre de la escena.
		*/
		const std::string& getName() {return _name;}


	private:

		/**	Nombre de la escena.*/
		std::string _name;

		TActors _actors;

		TActors _actorsToGhost;
		TActors _actorsToUnghost;

		OgreB2DebugDraw* _debugDraw;
		bool _debugDrawEnabled;

		b2World* _world;
		CContactListener* _worldListener;


		void CreateWorldEdges();


	

	}; // class CScene

} // namespace Physics

#endif 