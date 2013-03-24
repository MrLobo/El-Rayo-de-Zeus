//---------------------------------------------------------------------------
// Particle.h
//---------------------------------------------------------------------------

/**
@file Particle.h

Contiene la declaraci�n de la clase que maneja el Particle.

@see Graphics::CParticle

@author Pablo Terrado
@date Enero, 2013
*/

#ifndef __Graphics_Particle_H
#define __Graphics_Particle_H

#include "BaseSubsystems/Math.h"
#include "Logic\Entity\Entity.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n

namespace Graphics 
{
	class CScene;
};

namespace Ogre 
{
	class SceneNode;
	class ParticleSystem;
};


namespace Graphics 
{
	/**
	Clase de Particle extendida basada en Ogre.
	
	@ingroup graphicsGroup

	@author Pablo Terrado
	@date Enero, 2013
	*/
	class CParticle
	{
	public:

		/**
		Constructor de la clase.

		@param name Nombre del Particle.
		*/
		CParticle(const std::string &name, const std::string &templateName);

		/**
		Destructor de la part�cula.
		*/
		~CParticle();
		

		Ogre::ParticleSystem* getParticleSystem() {return _particleSystem;}


	protected:

		// CScene es la �nica que puede a�adir o eliminar part�culas de una 
		// escena y por tanto cargar o descargar sistemas de part�culases.
		// Por otro lado cada sistema de part�culas debe pertenecer a una escena. Solo 
		// permitimos a la escena actualizar el estado.
		friend class CScene;

		/** 
		ParticleSystem _particleSystem(sistema de Particulas)
		*/
		Ogre::ParticleSystem* _particleSystem;

		/**		
		Controla todos los elementos Ogre de una escena. Su equivalente
		en la l�gica del juego ser�a el mapa o nivel. 
		*/
		CScene *_scene;

			/**
		Nodo que contiene la sistema de part�culas de Ogre.
		*/
		Ogre::SceneNode *_entityNode;
		


		/**
		A�ade la sistema de part�culas al SceneManager pasado por par�metro. Si la sistema de part�culas
		no est� cargada se fuerza su carga.

		@param sceneMgr Gestor de la escena de Ogre a la que se quiere a�adir
		la sistema de part�culas.
		@return true si la sistema de part�culas se pudo cargar y a�adir a la escena.
		*/
		bool attachToScene(CScene *scene);

		/**
		Descarga una sistema de part�culas de la escena en la que se encuentra cargada.

		@return true si la sistema de part�culas se descargo y elimin� de la escena
		correctamente. Si la sistema de part�culas no estaba cargada se devuelve false.
		*/
		bool deattachFromScene();
		
		/**
		Carga la sistema de part�culas gr�fica correspondiente al nombre _mesh. No hace 
		comprobaci�nes de si la sistema de part�culas est� ya cargada o de si pertenece a
		otra escena. Esto se debe hacer de manera externa.

		@return true si la sistema de part�culas pudo crear los objetos necesarios en Ogre
		o si la sistema de part�culas ya estaba cargada.
		*/
		bool load();

		/**
		Elimina las estructuras creadas en Ogre mediante load(). No hace 
		comprobaci�nes de si la sistema de part�culas est� o no cargada o de si pertenece
		a una escena. Esto se debe hacer de manera externa.
		*/
		void unload();
		
		/**
		Actualiza el estado de la sistema de part�culas cada ciclo. En esta clase no se
		necesita actualizar el estado cada ciclo, pero en las hijas puede que
		si.
		
		@param secs N�mero de segundos transcurridos desde la �ltima llamada.
		*/
		void tick(float secs);


	private:

		/**
		Nombre del ParticleSystem.
		*/
		std::string _name;

		/**
		Indica si las part�culas han sido cargada en el motor gr�fico.
		*/
		bool _loaded;

	}; // class CParticle

} // namespace Graphics

#endif // __Graphics_Particle_H
