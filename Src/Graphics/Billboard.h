//---------------------------------------------------------------------------
// Billboard.h
//---------------------------------------------------------------------------

/**
@file Billboard.h

Contiene la declaraci�n de la clase que maneja el billboard.

@see Graphics::CBillboard

@author Pablo Terrado
@date Enero, 2013
*/

#ifndef __Graphics_Billboard_H
#define __Graphics_Billboard_H

#include "Graphics\SceneElement.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Ogre 
{	
	class BillboardSet;
	class Billboard;
};

namespace Graphics 
{
	/**
	Clase de billboard extendida basada en Ogre.
	
	@ingroup graphicsGroup

	@author Pablo Terrado
	@date Enero, 2013
	*/
	class CBillboard : public CSceneElement
	{
	public:

		/**
		Constructor de la clase.

		@param name Nombre del billboard.
		*/		
		CBillboard(const std::string &parentName, const Vector3& relativePos, 
			const float width, const float height, const std::string material) :
			_parentName(parentName), _relativePos(relativePos), 
			_width(width), _height(height), _material(material), _u0(0), _v0(0), _u1(0.5), _v1(1) {}


		/*****************
			GET's & SET'S
		*******************/

		bool isVisible() const;

		//Poner coordenadas
		void setTextureCoords(const float u0,const float v0,const float u1,const float v1);

		//Poner dimensiones
		void setDimensions(const float,const float);

		void setMaterial(const std::string &name);



	private:

		std::string _parentName;
		Vector3 _relativePos;
		float _width;
		float _height;
		std::string _material;

		float _u0, _v0, _u1, _v1;

		/** 
		BillboardSet _bbSet(conjunto de Billboards)
		*/
		Ogre::BillboardSet* _bbSet;
	
		/**
		Carga la entidad gr�fica correspondiente al nombre _mesh. No hace 
		comprobaci�nes de si la entidad est� ya cargada o de si pertenece a
		otra escena. Esto se debe hacer de manera externa.

		@return true si la entidad pudo crear los objetos necesarios en Ogre
		o si la entidad ya estaba cargada.
		*/
		bool load(); 
		void unload();

	}; // class CBillboard

} // namespace Graphics

#endif // __Graphics_Billboard_H
