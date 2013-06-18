//---------------------------------------------------------------------------
// Scene.h
//---------------------------------------------------------------------------

/**
@file Scene.h

Contiene la declaraci�n de la clase contenedora de los elementos
de una escena.

@see Graphics::CScene

@author David Llans�
@date Julio, 2010
*/

#ifndef __Graphics_Scene_H
#define __Graphics_Scene_H

#include <BaseSubsystems/Math.h>
#include <OgreFrameListener.h>

#include <list>

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
class IHHFXScene;
struct SContactReport;

namespace Ogre 
{
	class Root;
	class Viewport;
	class SceneManager;
	class StaticGeometry;	
};


namespace Graphics 
{
	class CServer;
	class CCamera;
	class CEntity;
	class CSceneElement;
	class CStaticEntity;
	class CBillboard;
	class CLight;
};

namespace Physics
{
	class CScene;
};

namespace Logic
{
	class CDotSceneLoader;
};

namespace Graphics 
{
	/**
	Clase que controla todos los elementos de una escena. Puede ser de 
	un estado como el men� o un nivel del juego. Su equivalente en la 
	l�gica del juego en el caso de ser un nivel ser�a el mapa. la escena 
	es un contenedor de todas las entidades gr�ficas que pueden ser 
	reenderizadas por un viewport asociado a esa escena. Se asume que una
	escena solo va a ser reenderizada desde un punto de vista por lo que
	por defecto se crea una �nica c�mara y cuando se activa la escena se
	crea un �nico viewport mostrado en la totalidad de la ventana de 
	reenderizado. En funci�n del tipo de juego puede quererse hacer m�s de
	un reenderizado de la misma escena a la vez y mostrarlo en diferentes 
	viewports en la ventana. Por ejemplo en un juego de coches para mostrar
	la imagen de los retrovisores del coche.
	<p>
	Tr�s crear la escena se deben a�adir las diferentes entidades y 
	entidades est�ticas. Al a�adir las entidades a la escena se fuerza su
	carga. Las entidades est�ticas se almacenan en una estructura. Una vez 
	a�adidas todas las entidades se debe crear la geometr�a est�tica ya 
	que hasta que no se invoque a buildStaticGeometry() las entidades no
	pasar�n a formar parte de la geometr�a est�tica. Al activar la escena
	se fuerza la creaci�n de la geometr�a est�tica si no se hab�a creado.
	<p>
	@remarks Una vez activada la escena las modificaciones realizadas en
	los valores de las entidades est�ticas no tendr�n efecto.
	<p>
	@remarks Solo el servidor gr�fico (Graphics::CServer) puede crear o 
	liberar escenas.
	
	@ingroup graphicsGroup

	@author David Llans�
	@date Julio, 2010
	*/
	class CScene  :  public Ogre::FrameListener
	{


	public:

		/**
		Devuelve la c�mara de la escena.

		@return Puntero a la c�mara de la escena.
		*/
		CCamera *getCamera() {return _camera;}

		CCamera *getBaseCamera() {return _baseCamera;}

		

		/**
		Devuelve el nombre de la escena.

		@return Nombre de la escena.
		*/
		const std::string& getName() {return _name;}

		/**
		A�ade una entidad gr�fica a la escena.
		<p>
		@remarks La escena NO se hace responsable de destruir la
		entidad.

		@param entity Entidad gr�fica que se quiere a�adir a la escena.
		@return Cierto si la entidad se a�adi� y carg� correctamente.
		*/
		bool add(CEntity* entity);

		/**
		Elimina una entidad gr�fica de la escena. 
		<p>
		@remarks Este m�todo NO destruye la entidad, �sta solo deja de
		ser parte de la escena.

		@param entity Entidad gr�fica que se quiere eliminar de la escena.
		*/
		void remove(CEntity* entity);
	
		/**
		*/
		bool add(CLight* light);

		/**
		*/
		void remove(CLight* light);


		//---------- GENERIC SCENE ELEMENTS (p.e. billboards)-----------

		bool add(CSceneElement* sceneElement);
		void remove(CSceneElement* sceneElement);

		// PARTICLES
		void createParticleSystem(const std::string& templateName, const std::string& parentEntity); // pos relativa a otra entidad
		void createParticleSystem(const std::string& templateName, const Vector3& position); // pos absoluta
		
		/**PeP
		Activar compositor, pensado para el de blanco y negro. Tambi�n RadialBlur
		*/
	
		void activateCompositor(std::string name);

		/**PeP
		Desactivar compositor, pensado para el de blanco y negro sobre todo. Tambi�n RadialBlur
		*/
		void deactivateCompositor(std::string name);


		
	protected:

		/**
		Clase amiga. Solo el servidor gr�fico puede crear o liberar escenas, 
		activarlas o desactivarlas y actualizar su estado. Solo las entidades,, billboards y
		la c�mara pueden acceder al gestor de la escena de Ogre.
		*/
		friend class CServer;	
		friend class CCamera;
		friend class CSceneElement;
		friend class CLight;
		friend class Logic::CDotSceneLoader; // HACK FRS Logic???
		friend class Physics::CScene; // FRS esto tiene que ser asin?

		/**
		Nombre de la escena.
		*/
		std::string _name;
		
		/**
		Punto de entrada al sistema Ogre.
		*/
		Ogre::Root *_root;

		/**
		Camara desde la que se ver� la escena. Puede haber c�maras m�s
		sofisticadas y m�s tipos de c�maras desde el punto de vista l�gico,
		ellas se encargar�n de mover esta instancia.
		*/
		CCamera *_camera;

		/**
		*/
		CCamera *_baseCamera;

		/** 
		Marco en la ventana de reenderizado donde se pinta lo captado por
		una c�mara. Solo puede haber una c�mara asociada a un viewport,
		sin embargo una ventana de reenderizado puede tener diferentes
		viewports al mismo tiempo.
		*/
		Ogre::Viewport *_viewport;
		
		/**
		Controla todos los elementos Ogre de una escena. Su equivalente
		en la l�gica del juego ser�a el mapa o nivel. 
		*/
		Ogre::SceneManager *_sceneMgr;

	


		/***************
		 SCENE ELEMENTS
		******************/
		
		/**
		Tipos para la lista de entidades.
		*/
		typedef std::list<CEntity*> TEntities;
	
		/**
		Tipos para la lista de luces.
		*/
		typedef std::list<CLight*> TLights;


		/**
		Lista de entidades din�micas.
		*/
		TEntities _dynamicEntities; // FRS Necesario para ejecutarles el tick

		/**
		Lista de entidades est�ticas.
		*/
		TEntities _staticEntities; // FRS Necesario para aglomerar geom est�tica
			
		
		/** 
		Lista de luces en la escena 
		*/
		TLights _lights;

		/**
		Geometr�a est�tica de la escena.
		*/
		Ogre::StaticGeometry *_staticGeometry;


		//---------------------------------------------------------------


		/**
		Constructor de la clase.
		*/
		CScene(const std::string& name);

		/**
		Destructor de la aplicaci�n.
		*/
		virtual ~CScene();

		/**
		Despierta la escena y crea un viewport que ocupa toda la
		pantalla.
		*/
		void activate();

		/**
		*/
		void activateBaseCam();

		/**
		Duerme la escena y destruye su viewport para que no se siga 
		reenderizando.
		*/
		void deactivate();
		
		/**
		Actualiza el estado de la escena cada ciclo. Llama a su vez a 
		todas las entidades.
		
		@param secs N�mero de segundos transcurridos desde la �ltima 
		llamada.
		*/
		void tick(float secs);

		/**
		A�ade las entidades est�ticas a la geometr�a est�tica del nivel
		y la construlle. Si la geometr�a est�tica ya ha sido construida
		no se vuelve a construir.
		<p>
		@remarks Una vez construida la geometr�a est�tica no se pueden 
		modificar los valores de las entidades est�ticas.
		*/
		void buildStaticGeometry(); 

		/**
		Devuelve el gestor de la escena de Ogre
		@return Puntero al gestor de la escena de Ogre.
		*/
		Ogre::SceneManager *getSceneMgr() { return _sceneMgr; }

		/**
		Devuelve la geometr�a est�tica de la escena de Ogre
		@return Puntero a la geometr�a est�tica de la escena de Ogre.
		*/
		Ogre::StaticGeometry *getStaticGeometry() { return _staticGeometry; }
		



	/*********************
		HELL HEAVENS FX
	*********************/

	public:		

		// WARNING : if != 1.0, This scale must be taken into account when setting and getting arbitrary particle attributes !
		static const float HHFX_WORLD_SCALE;

		IHHFXScene*	getHHFXScene() const { assert(_hhfxScene); return _hhfxScene; }
		
		//	FRAME LISTENER 
		bool frameStarted(const Ogre::FrameEvent& evt);
		bool frameRenderingQueued(const Ogre::FrameEvent& evt);
		
	private:	

		static const int _HHFX_UPDATE_TIME_MAX = 1000;
		Ogre::Real _hhfxTimeSinceUpdate;
		IHHFXScene *_hhfxScene;

		void _hhfxSceneInit();
		void _hhfxSceneDeinit();
		void _hhfxCompositorLoad();
		void _hhfxCompositorUnload();
		static bool _hhfxCollisionCheck(void *arg, const Ogre::Vector3 &start, const Ogre::Vector3 &direction, float length, SContactReport &contactReport);

	}; // class CScene

} // namespace Graphics

#endif // __Graphics_Scene_H
