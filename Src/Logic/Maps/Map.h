/**
@file Map.h

Contiene la declaraci�n de la clase CMap, Un mapa l�gico.

@see Logic::Map

@author David Llans�
@date Agosto, 2010
*/
#ifndef __Logic_Map_H
#define __Logic_Map_H

#include "EntityID.h"


#include <Logic/PlayerSettings.h>
#include <map/Entity.h>

#include <map>
#include <list>




// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Logic 
{
	class CEntity;
	class CLogicalPosition;
}

namespace Graphics 
{
	class CScene;
}
namespace Physics 
{
	class CScene;
}

// Declaraci�n de la clase
namespace Logic
{

	

	/**
	Clase que representa un mapa l�gico.
	<p>
	Esta clase se encarga de almacenar todas las entidades del mapa, tanto 
	aquellas que tienen representaci�n gr�fica y se ven como entidades
	puramente l�gicas. Gestiona la activaci�n y desactivaci�n de �stas y
	tiene tambi�n m�todos para buscar entidades, tanto por su nombre como 
	por su tipo y por su identificador.

	@ingroup logicGroup
	@ingroup mapGroup

	@author David Llans�
	@date Agosto, 2010
	*/
	class CMap
	{
	public:
		/**
		M�todo factor�a que carga un mapa de fichero. Tras el parseo de
		todas las entidades del mapa mediante CMapParser, genera todas las
		entidades con CEntityFactory.

		@param filename Nombre del archivo a cargar.
		@return Mapa generado.
		*/
		static CMap* createMap(CPlayerSettings& settings, int mapNumber);
		

		/**
		Constructor.

		@param name Nombre que se le da a este mapa.
		*/
		CMap (const std::string &name);

		CMap (const std::string &name, int mapNumber);

		/**
		Destructor.
		*/
		~CMap();
		
		/**
		Activa el mapa. Invocar� al m�todo activate() de todas las 
		entidades para que se den por enterados y hagan lo que necesiten.
		 
		@return Devuelve true al invocador si todas las entidades se
		activaron sin problemas.
		*/
		bool activate();

		/**
		Desactiva el mapa. Invocar� al m�todo deactivate() de todas las 
		entidades para que se den por enterados y hagan lo que necesiten.
		*/
		void deactivate();


		/**
		*/
		void activatePlayerCam();

		/**
		*/
		void activateBaseCam();

		/**
		*/
		void switchDebugDraw();

		/**
		Funci�n llamada en cada frame para que se realicen las funciones
		de actualizaci�n adecuadas.
		<p>
		Llamar� a los m�todos tick() de todas las entidades.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		void tick(unsigned int msecs);

		/**
		A�ade una nueva entidad al mapa. Si la entidad ya estaba incluida
		no se hace nada.

		@param entity Entidad a a�adir.
		*/
		void addEntity(CEntity *entity);

		/*
		*/
		void insertEntity(CEntity *entity);
		void insertIntoGraphics(CEntity *entity);
		void insertIntoPhysics(CEntity *entity);

		/**
		Elimina una entidad del mapa. Si la entidad no estaba incluida
		no se hace nada. La funci�n desactiva previamente la entidad si
		�sta se encontraba activa.

		@note El mapa no se hace responsable de eliminar (con delete) la
		entidad.

		@param entity Entidad a eliminar.
		*/
		void removeEntity(CEntity *entity);
		void removeFromGraphics(CEntity *entity);
		void removeFromPhysics(CEntity *entity);


		/**
		Elimina y destruye todas las entidades del mapa dejando la lista 
		de entidades vac�a.
		*/
		void destroyAllEntities();

		
		/**
		Recupera una entidad del mapa a partir de su ID.

		@param entityID ID de la entidad a recuperar.
		@return Entidad con el ID pedido, NULL si no se encuentra.
		*/
		CEntity *getEntityByID(TEntityID entityID){	return _entityMap[entityID]; }

		/**
		Recupera una entidad del mapa a partir de su nombre.

		@param name nombre de la entidad a recuperar.
		@param start Entidad desde la que se debe empezar a buscar
		en la lista. �til si se tienen varias entidades con el mismo
		nombre y desde fuera se quiere ir accediendo a ellas una a una.
		Si no se especifica se empieza desde el principio.
		@return Entidad con el nombre pedido, NULL si no se encuentra.
		*/
		CEntity *getEntityByName(const std::string &name, CEntity *start = 0);

		/**
		Recupera una entidad del mapa a partir de su tipo.

		@param type nombre del tipo de la entidad a recuperar.
		@param start Entidad desde la que se debe empezar a buscar
		en la lista. �til si se tienen varias entidades del mismo tipo
		y desde fuera se quiere ir accediendo a ellas una a una.
		Si no se especifica se empieza desde el principio.
		@return Entidad con el nombre pedido, NULL si no se encuentra.
		*/
		CEntity *getEntityByType(const std::string &type, CEntity *start = 0);

		/**
		Tipo lista de entidades de mapa.
		*/ 
		typedef std::list<CEntity*> TEntityList;

		/**
		Devuelve la lista con todas las entidades del mapa.

		@return Lista con todas las entidades.
		*/
		const TEntityList& getEntities() {return _entityList; }

		/**
		Devuelve la escena gr�fica correspondiente a este mapa.

		@return Escena con las entidades gr�ficas.
		*/
		Graphics::CScene *getGraphicScene() {return _graphicScene;}

		/**
		Devuelve la escena f�sica correspondiente a este mapa.

		@return Escena con los actores f�sicos.
		*/
		Physics::CScene *getPhysicScene() {return _physicsScene;}
		
		/**
		Crea un nuevo jugador y le porporciona un nombre determinado.

		@param name Nombre del jugador.
		*/


		/*******************
			CREATORS
		****************/

		//FRS DEPRECATED
		// Ya no es necesario ejecutar esta funci�n; la creaci�n del player va ligada a loadMap
		void createPlayer(bool isLocalPlayer, const std::string& nickname = "", const std::string& color = "", const std::string& model = "");
		

		//PT
		void createAlly(std::string entityName, const std::string& type="", const unsigned short base=0U, const unsigned short ring=0U, const unsigned short degrees=0U, const unsigned short sense=0);

		//PeP
		void createProjectile(const std::string entityName, const CLogicalPosition pos, const CEntity* father=NULL);

		/*******************
			GET's & SET's
		******************/		
		const Map::TAttrKeywords& getProperties() const { return _properties; }
		void setProperty(const std::string& keyword, const std::string& value) 
			{ _properties[keyword] = value; }
		void setProperties(const Map::TAttrKeywords& properties) 
			{ _properties.insert(properties.cbegin(), properties.cend() ); }

		bool isActive() const { return _isActive; }
		int getMapNumber() const { return _mapNumber; }

		std::string getName() { return _name; }

	private:

		/**
		Escena gr�fica donde se encontrar�n las representaciones gr�ficas de las entidades.
		*/
		Graphics::CScene* _graphicScene;
		/**
		Escena f�sica donde se encontrar�n los actores f�sicos de las entidades.
		*/
		Physics::CScene* _physicsScene;

		/**
		Nombre del mapa.
		*/
		std::string _name;

		/**
		*/
		int _mapNumber;

		//PeP Contador para crear flechas y que no se repitan sus nombres al crearlos
		unsigned short int bullet;
		/**
		Tipo tabla de entidades de mapa.
		*/
		typedef std::map<TEntityID,CEntity*> TEntityMap;

		/**
		tabla con las entidades del mapa localizadas por su ID.
		*/
		TEntityMap _entityMap;

		/**
		lista con las entidades del mapa.
		*/
		TEntityList _entityList;

			

		bool _isActive;

		//PT Contador para crear aliados y que no se repitan sus nombres al crearlos
		unsigned short int _nAllies;
		
		// FRS Propiedades (Keyword - value) a ser sustituidas sobre el mapa patron.
		Map::TAttrKeywords _properties;
		
	}; // class CMap

} // namespace Logic

#endif // __Logic_Map_H
