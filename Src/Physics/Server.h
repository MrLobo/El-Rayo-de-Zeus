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
		
	protected:

		/**
		Tipo para la estructura que guarda las diferentes escenas
		*/
		typedef std::map<std::string, CScene*> TScenes;


	public:


		/**
		Devuelve la �nica instancia de la clase.

		@return Puntero al servidor f�sico.
		*/
		static CServer *getSingletonPtr() { assert(_instance && "Servidor fisico no inicializado"); return _instance; }
		
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
		void tick(float secs);

		/**
		*/
		void SwitchDebugDraw();


		//----------------------
		// Gestion de la escena
		//----------------------

		/**
		Crea la escena f�sica. Se asume que s�lo existir� una escena f�sica, por lo que 
		s�lo debe invocarse una vez.*/
		CScene* createScene(const std::string &name);

		/**
		Elimina la escena pasada por par�metro de la tabla de escenas
		y la libera.

		@param scene Escena que se desea liberar
		*/
		void removeScene(CScene* scene);

		/**
		Elimina la escena de nombre especificado por par�metro de la 
		tabla de escenas y la libera.

		@param name Nombre de la escena que se quiere liberar.
		*/
		void removeScene(const std::string& name);


		/**
		Establece una escena como escena activa. En caso de que 
		hubiese otra escena activa este m�todo la desactiva y establece
		la nueva.

		@param scene Escena que se desea poner como escena activa.
		*/
		void activate(CScene* scene);

		/**
		Establece una escena como escena activa. En caso de que 
		hubiese otra escena activa este m�todo la desactiva y establece
		la nueva.

		@param name Nombre de la escena que se quiere poner como
		escena activa.
		*/
		void activate(const std::string& name);

		/*
		*/
		void deactivate(CScene* scene);



		//------------------------------
		// Gesti�n de entidades simples
		//------------------------------
		// UNDONE FRS Cableado directamente el CPhysic con los actors
		//Physics::CActor* createActor(const Logic::TLogicalPosition &position, const float angularWidth, const float height, 
		//								bool isTrigger, IObserver *component); 

		//void destroyActor(Physics::CActor* actor);

		//void moveActor(Physics::CActor *actor, float diffDegrees, float diffHeight, char diffRing, char diffBase);
		//
		//Logic::TLogicalPosition& getActorLogicPosition(Physics::CActor* actor);

	protected:

		// Instancia �nica de la clase.
		static CServer *_instance;

		/**
		Mapa de escenas. Se asocia una escena con su nombre.
		*/
		TScenes _scenes;


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