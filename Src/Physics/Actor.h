//---------------------------------------------------------------------------
// Actor.h
//---------------------------------------------------------------------------

/**
@file Actor.h

Contiene la declaraci�n de la clase que representa una entidad f�sica.

@see Physics::CActor

@author Emilio Santalla
@date Febrero, 2013
*/

#ifndef __Physics_Actor_H
#define __Physics_Actor_H

#include "IShape.h"
#include "RigidBody.h"

#include "Logic\Entity\LogicalPosition.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n

namespace Physics 
{
	class IObserver;
}

namespace Physics
{

	
	class CActor
	{
	public:

		CActor();

		CActor(Vector2 position, float radius, float density, float restitution, IObserver *component); // CircleActor Constructor
		CActor(Vector2 min, Vector2 max, float density, float restitution, IObserver *component); // AABBActor Constructor
		
		virtual ~CActor() {}
		
		void move(const float degrees, const float height);

		
		/************************
			GETTER's & SETTER's
		************************/

		CRigidBody* getRigid() { return _body; }

		void setIObserver(IObserver* component) {_component=component;}
		IObserver *getIObserver() {return _component;}

		float getDegree();
		float getHeight();

	protected:


		CRigidBody* _body;

		IObserver* _component;


	}; // class CActor


	

} // namespace Physics

#endif 
