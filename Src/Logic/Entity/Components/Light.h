/**
@file Light.h

Contiene la declaraci�n del componente que controla la representaci�n
de una luz.

@see Logic::CLight
@see Logic::IComponent

@author Emilio Santalla Comellas
@date Marzo, 2013
*/
#ifndef __Logic_Light_H
#define __Logic_Light_H

#include "Logic/Entity/Component.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Graphics 
{
	class CLight;
	class CScene;
};

namespace Logic
{
	class CMessage;
};

//declaraci�n de la clase
namespace Logic 
{

	class CLight : public IComponent
	{
		DEC_FACTORY(CLight);
	public:

		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		CLight() : IComponent(GetAltTypeIdOf(CLight)), _graphicalLight(0) {}

		CLight(altTypeId id) : IComponent(id), _graphicalLight(0) {}


		virtual ~CLight();
		
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		virtual bool accept(const CMessage *message);

		virtual void process(CMessage *message);

	protected:

		/**	Luz gr�fica.*/
		Graphics::CLight* _graphicalLight;

		/**
		Escena gr�fica donde se encontrar�n las representaciones gr�ficas de
		las entidades. La guardamos para la destrucci�n de la entidad gr�fica.
		*/
		Graphics::CScene* _scene;

		/**
		*/
		std::string _type;

	    /**
		@param entityInfo Informaci�n de construcci�n del objeto le�do del
			fichero de disco.
		*/
		virtual Graphics::CLight* createGraphicalLight(const Map::CEntity *entityInfo);
		

	}; // class CLight

	REG_FACTORY(CLight);

} // namespace Logic

#endif // __Logic_Light_H