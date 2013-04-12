//---------------------------------------------------------------------------
// Entity.h
//---------------------------------------------------------------------------

/**
@file Entity.h

Contiene la declaraci�n de la clase que representa una entidad gr�fica.

@see Graphics::CEntity

@author David Llans�
@date Julio, 2010
*/

#ifndef __Graphics_Entity_H
#define __Graphics_Entity_H

#include "Graphics\SceneElement.h"

#include <stack>

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Ogre 
{
	class Entity;
};

namespace Graphics 
{

	enum TAttachPoint {
			ARM_L,
			ARM_R,
			HAND_L,
			HAND_R,
			HEAD
	};

	/**
	Clase que representa una entidad gr�fica. Contiene una referencia a
	una entidad de Ogre y al Nodo que la contiene.
	Oculta los detalles escabrosos del motor gr�fico.
	<p>
	Al construir la clase solo se almacenan los atributos necesarios para
	la carga de las entidades de Ogre. Para cargar la entidad en Ogre y que 
	sea reenderizada, �sta debe a�adirse a una escenada de Ogre mediante el 
	m�todo attachToScene(). Solo la clase CScene puede invocar los m�todos 
	para a�adir o eliminar entidades de una escena y por tanto forzar su 
	carga o descarga en el motor de Ogre. Esto deriva de que en Ogre la carga
	de entidades se hace por las escenas, por lo que en nuestra arquitectura
	no permitimos entidades cargadas que no pertenezcan a ninguna escena.
	Al a�adir una entidad a una escena se fuerza su carga en Ogre invocando a 
	load.
	<p>
	Todas las modificaciones que se deseen hacer sobre los atributos de la
	entidad gr�fica (posici�n, orientaci�n, escala...) se deber�n hacer una 
	vez que las estructuras de Ogre han sido cargadas. De la misma manera 
	la entidad debe estar cargada para poder acceder a ellos.
	<p>
	Pueden implementarse clases derivadas de �stas, que permitan
	modelos con distintas caracter�sticas. Por ejemplo, �sta clase
	no permite especificar la animaci�n utilizada. Si queremos que podamos
	guardar un modelo gr�fico, y poder cambiar la animaci�n actual,
	se utilizar� la clase derivada correspondiente.
	
	@ingroup graphicsGroup

	@author David Llans�
	@date Julio, 2010
	*/
	class CEntity : public CSceneElement
	{
	public:

		/**
		Constructor de la clase.

		@param name Nombre de la entidad.
		@param mesh Nombre del modelo que debe cargarse.
		*/
		CEntity::CEntity(const std::string &name, const std::string &mesh, bool isStatic = false)
		: _name(name), _mesh(mesh), _entity(0), _isStatic(isStatic) {} 


		/************
			ATTACH
		**************/
/*		void attachToHead(const std::string &mesh) { 
			attach(TBoneName::HEAD, mesh); 
		}
		void attachToHand(const std::string &mesh){ 
			attach(TBoneName::HAND, mesh); 
		}	*/	
		
		/*void detachFromHead() {	detach( _name.append("_head") ); }
		void detachFromHand() {	detach( _name.append("_hand") ); }*/

		/**
			Unir el model mesh al hueso toBone
			FRS De momento, no permite "atachar" el mismo mesh m�s de una vez en un �nico cuerpo.
		*/
		void attach(TAttachPoint attachPoint, const std::string &mesh) {
			attach( BONE_DICTIONARY[attachPoint], mesh);
			_boneObjectsNameTable[ BONE_DICTIONARY[attachPoint] ].push_back(mesh); 
		}		
		void detach(TAttachPoint detachPoint) {
			detach( BONE_DICTIONARY[detachPoint] );
			_boneObjectsNameTable[ BONE_DICTIONARY[detachPoint] ].pop_back();
		}
		void attach(const std::string &toBone, const std::string &mesh);
		void detach(const std::string &fromBone);	

		void reattach(const std::string &toBone, const std::string &mesh);

		void reattachAllMeshes();
		

		/******************
			GET's & SET's
		********************/
			
		/**
		 Devuelve el valor de la propiedad visible.
		 La propiedad indica si la entidad debe dibujarse o no,
		 es decir, no est� relacionada con si la entidad est�
		 dentro del campo de visi�n de la c�mara o no.

		 @return Cierto si la entidad es visible (est� activa 
		 para ser reenderizada).
		*/
		bool isVisible() const;

		bool isStatic() const { return _isStatic; }		

		/**
		*/
		void setMaterial(const std::string &materialName);

		/**
		*/
		void setSubEntityMaterial(const std::string &materialName, unsigned int subEntityIndex); 



	protected:

		// CScene es la �nica que puede a�adir o eliminar entidades de una 
		// escena y por tanto cargar o descargar entidades.
		// Por otro lado cada entidad debe pertenecer a una escena. Solo 
		// permitimos a la escena actualizar el estado.
		friend class CScene;
	
		/**
		Entidad de Ogre.
		*/
		Ogre::Entity *_entity;

		
		/**
		Carga la entidad gr�fica correspondiente al nombre _mesh. No hace 
		comprobaci�nes de si la entidad est� ya cargada o de si pertenece a
		otra escena. Esto se debe hacer de manera externa.

		@return true si la entidad pudo crear los objetos necesarios en Ogre
		o si la entidad ya estaba cargada.
		*/
		virtual bool load();

		/**
		Elimina las estructuras creadas en Ogre mediante load(). No hace 
		comprobaci�nes de si la entidad est� o no cargada o de si pertenece
		a una escena. Esto se debe hacer de manera externa.
		*/
		virtual void unload();
		
		
	private:

		 // DICCIONARIO TAttachPoint -> BoneName
		typedef std::map<TAttachPoint, std::string> TBoneDictionary;

			static TBoneDictionary BONE_DICTIONARY;

				static TBoneDictionary initBoneDictionary() {
					TBoneDictionary dictionary;
//*
						dictionary[TAttachPoint::ARM_L] =	"LeftForeArm";		// SPARTAN
						dictionary[TAttachPoint::ARM_R] =	"RightForeArm";
						dictionary[TAttachPoint::HAND_L] =	"joint20";
						dictionary[TAttachPoint::HAND_R] =	"joint49";
						dictionary[TAttachPoint::HEAD] =	"paracascos";
/*/
						dictionary[TAttachPoint::ARM_L] =	"Bip01 L Forearm";	// MARINE
						dictionary[TAttachPoint::ARM_R] =	"Bip01 R Forearm";
						dictionary[TAttachPoint::HAND_L] =	"Bip01 L Hand";
						dictionary[TAttachPoint::HAND_R] =	"Bip01 R Hand";
						dictionary[TAttachPoint::HEAD] =	"Bip01 Head";
/**/
					// TODO a�adir on demand...
					return dictionary;
				}

		/**
		Nombre de la entidad.
		*/
		std::string _name;

		/**
		Nombre del modelo de la entidad.
		*/
		std::string _mesh;


		typedef std::stack<Ogre::Entity*> TAttachedMeshes;
		typedef std::map<std::string, TAttachedMeshes> TBoneObjectsTable;
			TBoneObjectsTable _boneObjectsTable;

		typedef std::deque<std::string> TAttachedMeshNames;
		typedef std::map<std::string, TAttachedMeshNames> TBoneObjectNamesTable;
			TBoneObjectNamesTable _boneObjectsNameTable;
		
		bool _isStatic;

		


	}; // class CEntity

} // namespace Graphics

#endif // __Graphics_Entity_H
