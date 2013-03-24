//---------------------------------------------------------------------------
// SceneElement.h
//---------------------------------------------------------------------------

/**
@file SceneElement.h

Contiene la declaraci�n de la clase base de cualquier elemento de escena.

@see Graphics::CSceneElement

@author FRS
@date Marzo, 2013
*/

#ifndef __Graphics_SceneElement_H
#define __Graphics_SceneElement_H

#include "BaseSubsystems/Math.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Ogre 
{
	class SceneNode;
};

namespace Graphics 
{
	class CScene;
};

namespace Graphics 
{
	/**
	Contiene la declaraci�n de la clase base de cualquier elemento de escena.

	
	@ingroup graphicsGroup

	@author FRS
	@date Marzo, 2013
	*/
	class CSceneElement 
	{
	public:

		/**
		Constructor de la clase.

		@param name Nombre de la entidad.
		@param mesh Nombre del modelo que debe cargarse.
		*/
		CSceneElement::CSceneElement() : _node(0), _scene(0), _loaded(false) {} 

		/**
		Destructor de la aplicaci�n.
		*/
		virtual ~CSceneElement();


		/******************
			GET's & SET's
		********************/
	
		/**
		 Devuelve el valor de la propiedad visible.
		 La propiedad indica si el elemento debe dibujarse o no,
		 es decir, no est� est� dentro del campo de visi�n de la c�mara o no.

		 @return Cierto si el elemento es visible (est� activo para ser reenderizado).*/
		virtual bool isVisible() const = 0; // FRS virtual porque el nodo no ofrece getter de visibilidad

		/**
		 Establece la propiedad visible del elemento.
		 La propiedad indica si el elemento debe dibujarse o no,
		 es decir, no est� relacionada con si est�
		 dentro del campo de visi�n de la c�mara o no.
		 <p>
		 El m�todo cambia la propiedad visible a todas las 
		 subentidades o nodos que contenga _graphicalNode.

		 @param visible Nuevo valor de la propiedad visible.
		 */
		void setVisible(bool visible); 


		/**
		Cambia la posici�n y orientaci�n del elemento gr�fico.
		@param transform Referencia a la matriz de transformaci�n con la 
		que debe posicionarse y orientarse el elemento.
		*/
		void setTransform(const Matrix4 &transform);

		/**
		Cambia la posici�n del elemento.
		@param position Nueva posici�n para el nodo de escena contenedor del elemento.
		*/
		void setPosition(const Vector3& position);

		/**
		Cambia la orientaci�n del elemento.
		@param orientation Referencia a la matriz de rotaci�n con la que debe orientarse el elemento.
		*/
		void setOrientation(const Matrix3 &orientation);


		// UNDONE FRS Analizar usos y si es necesario devolver ref
		///**
		// Devuelve el valor de la escala del elemento gr�fico
		// @return Valores de la escala en los diferentes ejes.
		//*/
		//const Vector3 &getScale() const;

		/**
		 Escala el elemento
		 @param scale Valores de la escala en los diferentes ejes.
		 */
		void setScale(const Vector3 &scale);

		/**
		 Escala el elemento
		 @param scale Valor de la escala para los 3 ejes.
		 */
		void setScale(float scale);
		


	protected:

		// CScene es la �nica que puede a�adir o eliminar elementos de una 
		// escena y por tanto cargarlos o descargarlos.
		// Por otro lado cada elemento debe pertenecer a una escena. 
		// Solo permitimos a la escena actualizar el estado.
		friend class CScene;
	
		/**	
		Controla todos los elementos Ogre de una escena.
		*/
		CScene *_scene;

		/**
		Nodo que contiene el elemento de escena
		*/
		Ogre::SceneNode *_node;


		/**
		A�ade el elemento al SceneManager pasado por par�metro. 
		Si el elemento no est� cargado se fuerza su carga.

		@param scene escena de Ogre a la que se quiere a�adir el elemento.
		@return true si el elemento se pudo cargar y a�adir a la escena.
		*/
		bool attachToScene(CScene *scene);

		/**
		Descarga un elemento de la escena en la que se encuentra cargado.

		@return true si el elemento se descargo y elimin� de la escena
		correctamente. Si el elemento no estaba cargado se devuelve false.
		*/
		bool deattachFromScene();
		
		/**
		Carga el elemento gr�fico. No hace comprobaci�nes de si ya estaba cargado
		o de si pertenece a otra escena. Esto se debe hacer de manera externa.

		@return true si el elemento pudo crear los objetos necesarios en Ogre
		o si ya estaba cargado.
		*/
		virtual bool load() = 0; 

		/**
		Elimina las estructuras creadas en Ogre mediante load(). No hace 
		comprobaci�nes de si el elemento est� o no cargado o de si pertenece
		a una escena. Esto se debe hacer de manera externa.
		*/
		virtual void unload();
		
		/**
		Actualiza el estado del elemento en cada ciclo. 

		@param secs N�mero de mili segundos transcurridos desde la �ltima llamada.
		*/
		virtual void tick(float msecs) {}


		
	private:
		
		/**
		Indica si el elemento ha sido cargada en el motor gr�fico.
		*/
		bool _loaded;

	}; // class CSceneElement

} // namespace Graphics

#endif // __Graphics_SceneElement_H
