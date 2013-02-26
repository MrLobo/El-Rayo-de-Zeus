/**
@file Server.h

Contiene la declaraci�n del servidor de f�sica. 

@see Physics::CServer

@author 
@date
*/

#ifndef __Physics_Server_H
#define __Physics_Server_H

#include "Logic/Entity/LogicalPosition.h" // ��� Si se usan refs no se puede predeclarar?

// Predeclaraci�n de tipos
namespace Physics {
	class CScene;
	class CActor;
	class IObserver;
}


// Namespace que contiene las clases relacionadas con la parte f�sica. 
namespace Physics {

	/**
	Servidor de f�sica. Se encarga de gestionar las entidades f�sicas y simular su 
	comportamiento.
	*/
	class CServer 
	{
	public:


		/**
		Devuelve la �nica instancia de la clase.

		@return Puntero al servidor f�sico.
		*/
		static CServer *getSingletonPtr() { return _instance; }
		
		/**
		Inicializa el servidor f�sico. Esta operaci�n s�lo es necesario realizarla
		una vez durante la inicializaci�n de la aplicaci�n. 

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera el servidor f�sico. Debe llamarse al finalizar la aplicaci�n.
		*/
		static void Release();

		/**
		Realiza la simulaci�n f�sica y actualiza la posici�n de todas las 
		entidades f�sicas. 
		*/
		void tick(unsigned int msecs);




		//----------------------
		// Gestion de la escena
		//----------------------

		/**
		Crea la escena f�sica. Se asume que s�lo existir� una escena f�sica, por lo que 
		s�lo debe invocarse una vez.
		*/
		CScene* createScene ();

		/**
		Destruye la escena f�sica.
		*/
		void destroyScene ();


		//------------------------------
		// Gesti�n de entidades simples
		//------------------------------


		Physics::CActor* createActor(const Logic::TLogicalPosition &position, const float angularWidth, const float height, 
										bool isTrigger, IObserver *component); 

		void destroyActor(Physics::CActor* actor);

		
		Logic::TLogicalPosition& getActorLogicPosition(Physics::CActor* actor);


		//�Por qu� llamar al server y no directamente al actor->move?
		void moveActor(Physics::CActor *actor, const Logic::TLogicalPosition &position);

		// UNDONE ��� En principio no har�a falta sobrecarga, si desde el CPhysicCharacter tenemos el  TLogical entero y esto es una simple llamada (no mensaje)
		//void moveActor(Physics::Actor *actor, const float degress);
		//void moveActor(Physics::CActor *actor, const float degrees, const float height);

	protected:


		// Instancia �nica de la clase.
		static CServer *_instance;

		// Escena f�sica
		CScene *_scene;

		/**
		Constructor de la clase.
		*/
		CServer();

		/**
		Destructor de la clase.
		*/
		virtual ~CServer();

		/**
		Segunda fase de la construcci�n del objeto. Sirve para hacer
		inicializaciones de la propia instancia en vez de inicializaciones 
		est�ticas.

		@return true si todo fue correctamente.
		*/
		bool open();

		/**
		Segunda fase de la destrucci�n del objeto. Sirve para hacer liberar 
		los recursos de la propia instancia, la liberaci�n de los recursos 
		est�ticos se hace en Release().
		*/
		void close();


	}; // class CServer

} // namespace Physics

#endif // __Physics_Server_H