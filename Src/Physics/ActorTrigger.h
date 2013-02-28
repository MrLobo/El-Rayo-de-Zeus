//---------------------------------------------------------------------------
// ActorTrigger.h
//---------------------------------------------------------------------------

/**
@file ActorTrigger.h

Contiene la declaraci�n de la clase que representa un trigger f�sico.

@see Physics::CActorTrigger

@author FRS
@date Febrero, 2013
*/

#ifndef __Physics_ActorTrigger_H
#define __Physics_ActorTrigger_H

#include "Physics/Actor.h"



namespace Physics
{
	typedef std::vector<CActorTrigger*> TTriggerVector;

	class CActorTrigger : public CActor
	{
	public:

		CActorTrigger(const Logic::TLogicalPosition &position, 
					const float angularWidth, const float height, 
					bool isTrigger, IObserver *component) 
					: CActor(position, angularWidth, height, isTrigger, component) {};
		
		// TODO FRS necesario reimpl para que el delete se ejecute sobre este this y no del padre?
		// void release();

		void enters(CActor* actor)	{ _actorsOnTrigger.push_back(actor); }; // TODO FRS no s� si me molan estos nombres de m�todos... xD
		void exits(CActor*)			{ _actorsOnTrigger.erase(actor); };
		bool isOnTrigger(CActor* actor) {}; // TODO
		
	private:

		TActorVector _actorsOnTrigger;

	}; // class CActorTrigger

} // namespace Physics

#endif 