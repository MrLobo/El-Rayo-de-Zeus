/**Clase abstracta de la que deben heredar todos los componentes f�sicos. 
	Proporciona un interfaz	com�n para recibir eventos desde el motor de f�sica.
	@author ���
	@date Febrero, 2013
*/

#ifndef __Physics_ContactListener_H
#define __Physics_ContactListener_H

#include "Box2D\Dynamics\b2WorldCallbacks.h"

class b2Contact;

namespace Physics
{
	class IObserver;
}

// Namespace que contiene las clases relacionadas con la parte f�sica. 
namespace Physics {
	
	class CContactListener : public b2ContactListener
	{
	public: 		

		CContactListener() : _l1(0), _l2(0) {}

		virtual void BeginContact(b2Contact* contact);
		virtual void EndContact(b2Contact* contact);


	private:

		IObserver* _l1;
		IObserver* _l2;

	}; // class IObserver

} // namespace Physics


#endif // __Physics_IObserver_H