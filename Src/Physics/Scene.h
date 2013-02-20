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

//HACK TOCHO


#include <vector>

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Physics
{
	class CActor;
}

namespace Physics 
{

	class CScene 
	{
	public:


		void addActor(Physics::CActor *actor);

		//bool addStaticActor(Physics::CStaticActor *actor);

		void removeActor(Physics::CActor* actor);

		//void removeStaticActor(CStaticActor* actor);

	protected:

		/**
		Clase amiga. Solo el servidor f�sico puede crear o liberar escenas f�sicas, 
		activarlas o desactivarlas y actualizar su estado.
		*/
		friend class CServer;

		/**
		Constructor de la clase.
		*/
		CScene();

		/**
		Destructor de la aplicaci�n.
		*/
		~CScene();

		/**
		Despierta la escena
		*/
		void activate();

		/**
		Duerme la escena
		*/
		void deactivate();
		
		/**
		Actualiza el estado de la escena cada ciclo.
		*/
		void simulate(int msecs);


		//Physics::CollisionManager *getCollisionMgr() {return _collisionMgr;}
		

		//Physics::CollisionManager*_collisionMgr;
		

		/**
		Tipos para la lista de entidades.
		*/
		//typedef std::list<CStaticActor*> TStaticActorList;

		/**
		Tipos para el vector de actores (a mejorar)
		*/
		typedef std::vector<CActor> TActorVector;

		TActorVector _actors;
		

	}; // class CScene

} // namespace Physics

#endif 