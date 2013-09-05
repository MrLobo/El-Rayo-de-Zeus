//---------------------------------------------------------------------------
// Server.h
//---------------------------------------------------------------------------

/**
@file Server.h

Contiene la declaraci�n de la clase principal de gr�ficos, la que permite crear
la ventana, etc.

@see Graphics::CServer

@author David Llans�
@date Julio, 2010
*/

#ifndef __Graphics_Server_H
#define __Graphics_Server_H

#include <assert.h>
#include <map>
#include <OgreCompositorManager.h>

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n

class IHHFXBase;

namespace Ogre 
{
	class Root;
	class RenderWindow;
	class Timer;
	class OverlayManager;
	class Viewport;
}
namespace Graphics 
{
	class CScene;
	class COverlay;
}

	


/**
Namespace con todo lo que se refiere a la parte gr�fica. Las diferentes clases
se apoyan en Ogre.
(para m�s informaci�n ver @ref graphicsGroup).

@author David Llans�
@date Julio, 2010
*/
namespace Graphics 
{
	/**
	Clase central del subistema gr�fico. Se encarga de gestionar todas las
	entidades que se pintan en la ventana de la aplicaci�n.
	<p>
	Est� implementado como un Singleton de inicializaci�n expl�cita; se debe 
	llamar a su m�todo Init(), para que la instancia est� creada. Al final de 
	la aplicaci�n, habr� que llamar a Release().
	<p>
	Los detalles de dibujado est�n implementados haciendo uso de Ogre.
	
	@ingroup graphicsGroup

	@author David Llans�
	@date Julio, 2010
	*/
	class CServer 
	{
	protected:

		/**
		Tipo para la estructura que guarda las diferentes escenas
		*/
		typedef std::map<std::string,CScene*> TScenes;

	public:

		/**
		Devuelve la �nica instancia de la clase.

		@return Puntero al servidor gr�fico.
		*/
		static Graphics::CServer *getSingletonPtr() { assert(_instance && "Servidor Grafico no inicializado"); return _instance; }
		/**
		Inicializa el servidor gr�fico. Dependiendo de la configuraci�n (release
		o debug), utiliza pantalla completa o no.
		<p>
		Por conveniencia, el Init crea la ventana <b>minimizada</b>, por lo
		que la aplicaci�n despu�s deber� llamar a ShowWindow(). Se hace as�
		para que la aplicaci�n no muestre una ventana en negro mientras dura
		todo el proceso de carga. La ventana est� creada y el usuario la ver�,
		pero al menos la ve minimizada.

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera el servidor gr�fico. Debe llamarse al finalizar la aplicaci�n.
		*/
		static void Release();

		/**
		Funci�n invocada en cada vuelta para actualizaci�n.

		@param secs Segundos desde que se reenderiz� el �ltimo frame.
		*/
		void tick(float secs);

		/**
		Crea una nueva escena. Puede ser de un estado como el men� o un
		nivel del juego. Su equivalente en la l�gica del juego en el caso
		de ser un nivel ser�a el mapa. la escena es un contenedor de todas
		las entidades gr�ficas que pueden ser reenderizadas por un viewport
		asociado a esa escena. 
		<p>
		@remarks El servidor gr�fico SE HACE RESPONSABLE de
		la liberaci�n de la escena al terminar la aplicaci�n. No obstante
		se puede liberar antes mediante removeScene(). Si el motor gr�fico 
		se destruye antes del objeto que cre� la escena, este objeto dejar�
		de tener la escena accesible.
		<p>
		@remarks En depuraci�n se garantiza que no se creen escenas con el 
		mismo nombre.

		@param name Nombre que se le quiere dar a la escena. En depuraci�n 
		se garantiza que no se creen escenas con el mismo nombre.
		@return Escena creada.
		*/
		CScene* createScene(const std::string& name);

		/**
		Recupera una escena previamente creada a partir de su nombre. 

		@param name Nombre de la escena que se quiere recuperar.
		@return La escena requerida.
		*/
		CScene* getScene(const std::string& name) {return (*_scenes.find(name)).second;}

		/**
		Devuelve la escena activa. 

		@return La escena activa.
		*/
		CScene* getVisibleScene() {return _visibleScene;}

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
		Inicializa la escena
		*/
		void activate(CScene* scene);

		/**
		Deinicializa la escena
		*/
		void deactivate(CScene* scene);

		/**
		*/
		void activatePlayerCam(CScene* scene);

		/**
		*/
		void activateBaseCam(CScene* scene);

		/**
		Devuelve al manager de Overlays. 

		@return La escena activa.
		*/
		Ogre::OverlayManager* getOverlayManager() {return _overlayManager;}			

	
		/**
		Metodo que devuelve el height de la pantalla. Esta tomado gracias a los overlays, puede que no sea fiable 100%

		@return devuelve el width de la pantalla
		*/
		int getScreenWidth();

		/**
		Metodo que devuelve el height de la pantalla. Esta tomado gracias a los overlays, puede que no sea fiable 100%

		@return devuelve el height de la pantalla
		*/
		int getScreenHeight();

		/**
		*/
		Ogre::Viewport* getViewport() { return _viewport; }


	protected:

		/**
		Instancia �nica de la aplicaci�n.
		*/
		static CServer *_instance;

		/**
		Mapa de escenas. Se asocia una escena con su nombre.
		*/
		TScenes _scenes;

		/**
		Escena actual. Por simplificaci�n asumimos que solo va a haber una
		escena activa al mismo tiempo. El cambio de escena activa se realiza
		a trav�s de �sta clase.
		*/
		CScene* _visibleScene;

		/**
		Escena dummy que se crea autom�ticamente. Con ella permitimos que
		siempre haya una escena para el dibujado del GUI.
		*/
		CScene* _dummyScene;

		/**
		Punto de entrada al sistema Ogre.
		*/
		Ogre::Root *_root;

		/** 
		Ventana de renderizado 
		*/
		Ogre::RenderWindow *_renderWindow;
		
		/**
		Marco com�n de la ventana de renderizado para todas las escenas
		*/
		Ogre::Viewport *_viewport;

		/**
		Manager de los Overlays
		*/
		Ogre::OverlayManager* _overlayManager;

		/**
		Constructor de la clase.
		*/
		CServer();

		/**
		Destructor del server gr�fico.
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


	/*********************
		HELL HEAVEN FX
	********************/	

	private:		
		// DICCIONARIO "short hfx name" (Blast.hfx) => "long hfx name" (/HBO/Entities/Particles/Blast.hfx)
		typedef std::map<std::string, std::string> THFXNamesMap;
			THFXNamesMap _HFX_LONG_NAMES;

	public:
		IHHFXBase* getHHFXBase() const { 
			assert(_hhfxBase && "HHFX No ha sido inicializado correctamente"); 
			return _hhfxBase; 
		}	
		const std::string& CServer::getHFXLongName(const std::string& hfxShortName) { 
			assert( !_HFX_LONG_NAMES[ hfxShortName ].empty() && "Nombre de script HFX incorrecto o no incluido en el paquete precargado");
			return _HFX_LONG_NAMES[ hfxShortName ]; 
		}
		
	private:			

		IHHFXBase* _hhfxBase;
		void _initHHFX(CScene*);
		void _preloadHHFXTextures();	

	/*******************
		COMPOSITORS
	*******************/
	public:
		void compositorEnable(const std::string &name) {	assert(_viewport && "La escena no est� activa");
			Ogre::CompositorManager::getSingletonPtr()->setCompositorEnabled(_viewport, name, true); }	
		void compositorDisable(const std::string &name)	{	assert(_viewport && "La escena no est� activa");
			Ogre::CompositorManager::getSingletonPtr()->setCompositorEnabled(_viewport, name, false); }	
	
	private:
		void _compositorReenable(const std::string &name) {	compositorDisable(name); compositorEnable(name); }
		void _resetCompositors();
		void _compositorLoad();
		void _compositorAdd(const std::string &name) {	
			assert(_viewport && "La escena no est� activa");
			Ogre::CompositorInstance* comp = Ogre::CompositorManager::getSingletonPtr()->addCompositor(_viewport, name); 
			assert(comp && "Error al cargar compositor. Revisar que esta bien definido en los assets" );
		}

	}; // class CServer

} // namespace Graphics

#endif // __Graphics_Server_H
