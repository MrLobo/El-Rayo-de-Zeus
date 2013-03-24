//---------------------------------------------------------------------------
// ParticleSystem.h
//---------------------------------------------------------------------------

/**
@file ParticleSystem.h

Contiene la declaraci�n de la clase que maneja el ParticleSystem.

@see Graphics::CParticleSystem

@author Pablo Terrado
@date Enero, 2013
*/

#ifndef __Graphics_ParticleSystem_H
#define __Graphics_ParticleSystem_H

#include "Graphics\SceneElement.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Ogre 
{	
	class ParticleSystem;
};


namespace Graphics 
{
	/**
	Clase de ParticleSystem extendida basada en Ogre.
	
	@ingroup graphicsGroup

	@author Pablo Terrado
	@date Enero, 2013
	*/
	class CParticleSystem : public CSceneElement
	{
	public:

		/**
		Constructor de la clase.

		@param name Nombre del ParticleSystem.
		*/
		CParticleSystem(const std::string &name, const std::string &templateName) : 
		  _name(name), _templateName(templateName){}

	protected:

		// CScene es la �nica que puede a�adir o eliminar part�culas de una 
		// escena y por tanto cargar o descargar sistemas de part�culases.
		// Por otro lado cada sistema de part�culas debe pertenecer a una escena. Solo 
		// permitimos a la escena actualizar el estado.
		friend class CScene;

		/** 
		ParticleSystem 
		*/
		Ogre::ParticleSystem* _particleSystem;


	private:

		/**
		Nombre del ParticleSystem.
		*/
		std::string _name;
		std::string _templateName; 

		
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

	}; // class CParticleSystem



} // namespace Graphics

#endif // __Graphics_ParticleSystem_H
