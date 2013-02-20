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
	class Scene;
	class Actor;
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

		/**Clase abstracta de la que deben heredar todos los componentes f�sicos. 
		Proporciona un interfaz	com�n para recibir eventos desde el motor de f�sica.
		@author ���
		@date Febrero, 2013
		*/
		class IObserver
		{
		public: 		
			/**Este m�todo es invocado desde el motor de f�sica cuando una entidad entra o sale de un
			trigger f�sico. Se notifica tanto al componente asociado al trigger como al componente
			asociado a la otra entidad.
			@param otherComponent Componente asociado al trigger o a la otra entidad, 
					dependiendode a qui�n se est� notificando.
			@param enter True si la entidad entra en el trigger y false si sale. 
			*/
			virtual void onTrigger(IObserver *otherComponent, bool enter) {};
			virtual void onCollision(IObserver *otherComponent) {};

		}; // class IObserver

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
		bool tick(unsigned int msecs);


		//----------------------
		// Gestion de la escena
		//----------------------

		/**
		Crea la escena f�sica. Se asume que s�lo existir� una escena f�sica, por lo que 
		s�lo debe invocarse una vez.
		*/
		void createScene ();

		/**
		Destruye la escena f�sica.
		*/
		void destroyScene ();


		//------------------------------
		// Gesti�n de entidades simples
		//------------------------------



		Physics::Actor* createActor(const Logic::TLogicalPosition &position, const float angularBox, 
			                                    const float height, bool trigger, const IObserver *component); 


		void destroyActor(Physics::Actor* *actor);

		Logic::TLogicalPosition getActorLogicPosition(const Physics::Actor* actor);


		void moveActor(Physics::Actor *actor, const Logic::TLogicalPosition &position);

		// UNDONE ��� En principio no har�a falta sobrecarga, si desde el CPhysicCharacter tenemos el  TLogical entero y esto es una simple llamada (no mensaje)
		//void moveActor(Physics::Actor *actor, const float degress);

		//void moveActor(Actor *actor, const float degrees, const float height);

	private:

		/**
		Constructor de la clase.
		*/
		CServer();

		/**
		Destructor de la clase.
		*/
		virtual ~CServer();

		// Instancia �nica de la clase.
		static CServer *_instance;

		// Escena f�sica
		Scene *_scene;


	}; // class CServer

} // namespace Physics

#endif // __Physics_Server_H