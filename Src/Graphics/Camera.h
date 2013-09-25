//---------------------------------------------------------------------------
// Camera.h
//---------------------------------------------------------------------------

/**
@file Camera.h

Contiene la declaraci�n de la clase que maneja la c�mara.

@see Graphics::CCamera

@author David Llans�
@date Julio, 2010
*/

#ifndef __Graphics_Camera_H
#define __Graphics_Camera_H

#include "BaseSubsystems/Math.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Ogre 
{
	class Camera;
	class SceneNode;
	class Viewport;
}
namespace Graphics 
{
	class CScene;
	class CServer;
}

namespace Graphics 
{
	/**
	Clase de la c�mara extendida basada en Ogre. Para simplificar su uso
	La c�mara extendida consta de dos nodos, uno es el propio de la 
	c�mara, que contiene la c�mara real de Ogre y otro es el nodo 
	objetivo, que representa la posici�n a la que la c�mara debe mirar.
	
	@ingroup graphicsGroup

	@author David Llans�
	@date Julio, 2010
	*/
	class CCamera 
	{
	public:

		/**
		Constructor de la clase.

		@param name Nombre de la c�mara.
		@param sceneMgr Gestor de la escena de Ogre a la que pertenece.
		*/
		CCamera(const std::string &name, CScene *scene);

		/**
		Destructor de la aplicaci�n.
		*/
		virtual ~CCamera();
		

		/*******************
			GET's & SET's
		*********************/

		/**
		Devuelve la posici�n de la c�mara.

		@return Referencia a la posici�n del nodo que contiene la c�mara de Ogre.
		*/
		const Vector3 &getPosition() const;

		/**
		Devuelve la orientaci�n de la c�mara.

		@return Referencia al quaternion del nodo que contiene la c�mara de Ogre.
		*/
		const Quaternion &getOrientation() const;
		
		/**
		Devuelve la posici�n a la que apunta la c�mara.

		@return Referencia a la posici�n del nodo _targetCamera.
		*/
		const Vector3 &getTargetPosition() const;


		/**
		*/
		Ogre::Viewport* getViewport() const;

		
		/**
		Cambia la posici�n de la c�mara.

		@param newPosition Nueva posici�n para el nodo que contiene la c�mara 
		de Ogre.
		*/
		void setPosition(const Vector3 &newPosition);

		/**
		Cambia la posici�n de la posici�n a la que apunta la c�mara.

		@param newPosition Nueva posici�n para el _targetNode.
		*/
		void setTargetPosition(const Vector3 &newPosition);


	protected:
		
		/**
		Clase amiga. Solo la escena tiene acceso a la c�mara de Ogre para
		poder crear el viewport.
		*/
		friend class CScene;
		friend class CServer;


		/**
		Nodo que representa el punto a donde debe mirar la c�mara.
		*/
		Ogre::SceneNode *_targetNode;
		
		/**
		La c�mara de Ogre.
		*/
		Ogre::Camera *_camera;

		/**
		Controla todos los elementos de una escena. Su equivalente
		en la l�gica del juego ser�a el mapa o nivel. 
		*/
		CScene *_scene;

		/**
		Nombre de la c�mara.
		*/
		std::string _name;


		/**
		Devuelve la c�mara de Ogre.

		@return puntero a la c�mara de Ogre.
		*/
		Ogre::Camera *getCamera() {return _camera;}

	}; // class CCamera

} // namespace Graphics

#endif // __Graphics_Camera_H
