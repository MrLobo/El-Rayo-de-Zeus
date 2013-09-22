//---------------------------------------------------------------------------
// Entity.cpp
//---------------------------------------------------------------------------

/**
@file Entity.cpp

Contiene la implementaci�n de la clase que representa una entidad gr�fica.

@see Graphics::CEntity

@author David Llans�
@date Julio, 2010
*/

#include "Entity.h"

#include <Graphics/Server.h>

#include <OgreMaterialManager.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>


#include <assert.h>

namespace Graphics 
{
	CEntity::TBoneDictionary CEntity::_BONE_DICTIONARY = CEntity::_initBoneDictionary();
		

	/***********************
		CONSTRUCTORES
	***********************/

	CEntity::CEntity(const std::string &name, const std::string &mesh, bool isStatic)
		: _name(name), _mesh(mesh), _entity(0), _matListLast(0), _matListCurrent(0)
	{
		// FRS Si no son din�micas son est�ticas.		
		_type = isStatic? TGraphicalType::STATIC : TGraphicalType::DYNAMIC;
	}

	//--------------------------------------------------------

	CEntity::~CEntity()
	{
		delete[] _matListLast;
		delete[] _matListCurrent;
	}
		
	

	/*******************
		SCENE ELEMENT
	********************/

	bool CEntity::load()
	{
		try{
			_entity = getSceneMgr()->createEntity(_name, _mesh);

			_node = getSceneMgr()->getRootSceneNode()
					->createChildSceneNode(_name + "_node");
				_node->setScale(_scale);
				_node->attachObject(_entity);
			
			_reloadAttachments();
			_reloadMaterials();

			_loaded = true;

		} catch(std::exception e){
			_loaded = false;
		}

		return _loaded;
	} // load
	
	//--------------------------------------------------------
		
	void CEntity::unload()
	{
		CSceneElement::unload();
		
		if(_entity){
			
			//ESC - cuando se hace unload queremos destruir tambien todos los objetos atachados a los huesos de la entidad.
			Ogre::Entity::ChildObjectListIterator it =_entity->getAttachedObjectIterator();
				while (it.hasMoreElements())				
					getSceneMgr()->destroyEntity(
						static_cast<Ogre::Entity*>( it.getNext() )
					);
				
			_loaded = false;
			getSceneMgr()->destroyEntity(_entity);
			_entity = 0;
			_boneObjectsTable.clear();
		}

	} // unload





	/***************
		ATTACHMENTS
	***************/

	void CEntity::attach(const std::string &toBone, const std::string &mesh, bool permanently) 
	{
		assert(_node && "La entidad no ha sido cargada en la escena");
		if(!_node) return;

		// PERMANENCIA: Algunos TAttachPoint son permanentes por definici�n; p.e FACE -> rasgos faciales est�ticos
		permanently |= toBone == _BONE_DICTIONARY[TAttachPoint::FACE];

		// AttachName = "parentName.bone.attachedMesh"
		std::string objectName = _name + '.';		
			objectName.append(		// FRS Si toBone tiene espacios (p.e "Bip01 LeftHand")
				toBone,				// nos quedamos con la ultima parte
				toBone.find_last_of(' ') == -1 ? 0 : toBone.find_last_of(' '), 
				toBone.length() - 1
			); 
			objectName.append(".");
			objectName.append( mesh, 0, mesh.find_last_of('.') ); // Suprimimos sufijo ".mesh"

		if( !getSceneMgr()->hasEntity(objectName) ) // No existe un objeto con el mismo nombre en la escena
		{	
			Ogre::Entity* newObject = getSceneMgr()->createEntity(objectName, mesh);			
				_entity->attachObjectToBone(toBone, newObject);	

			if(!permanently) { // FRS Apilamos solo attachables no permanentes, para poder gestionar despu�s el detach
				TAttachedMeshes& boneObjects = _boneObjectsTable[toBone];
					if( !boneObjects.empty() )					// Si ya hab�a objetos "atachados" a ese hueso
						boneObjects.top()->setVisible(false); // ocultamos el �ltimo attach que ten�a el hueso	
					boneObjects.push(newObject); 
			}
		}

		// FRS Solo atachamos si no existe una entidad con el mismo objectName ya
		// Un objectName en uso indica que ya ha sido atachado ese objeto al mismo hueso;
		// ya est� en la pila de AttachedMeshes de ese hueso y, por tanto, no se deber�a atachar de nuevo.

	} // reattach

	

	//--------------------------------------------------------

	void CEntity::detach(const std::string &fromBone) 
	{
		assert(_node && "La entidad no ha sido cargada en la escena");

		TAttachedMeshes& boneObjects = _boneObjectsTable[fromBone];
			if( boneObjects.empty() ) 
				return;				// Ning�n objeto "atachado" a ese hueso

		getSceneMgr()->destroyEntity(  boneObjects.top() ); // Eliminamos �ltimo attach		
		
		boneObjects.pop();						// lo desapilamos
		boneObjects.top()->setVisible(true);	//  y activamos el siguiente apilado
	} // detach

	//--------------------------------------------------------
	
	void CEntity::_reloadAttachments()
	{
		//se recorre el mapa de bicolas para volver a attachar todos los meshes
		TBoneObjectNamesTable::const_iterator boneDeque = _boneObjectsNameTable.cbegin();
		TBoneObjectNamesTable::const_iterator lastBoneDeque = _boneObjectsNameTable.cend();
			for (; boneDeque != lastBoneDeque; ++boneDeque)
			{
				TAttachedMeshNames::const_iterator attachedMeshName = boneDeque->second.cbegin();
				TAttachedMeshNames::const_iterator lastAttachedMeshName = boneDeque->second.cend();
					for (; attachedMeshName != lastAttachedMeshName; ++attachedMeshName)			
						attach(boneDeque->first, *attachedMeshName);			
			}	
	} //reattachAllMeshes()



	
	/***************
		MATERIALS
	***************/

	void CEntity::setMaterial(const std::string &materialName, unsigned int subIndex) 
	{	
		if( !materialName.length() ) return;

		if(!_matListLast)
			_matListLast	= new std::string[ _entity->getNumSubEntities() ];
		if(!_matListCurrent)
			_matListCurrent = new std::string[ _entity->getNumSubEntities() ]; 

		_matListLast[	subIndex ]	= _entity->getSubEntity(subIndex)->getMaterialName();
		_matListCurrent[subIndex ]	= materialName;
		_reloadMaterial(subIndex);
	} 

	//--------------------------------------------------------
	
	void CEntity::setColor(const Vector3& rgb, unsigned int subIndex) 
	{
		assert(_loaded && "La entidad no ha sido cargada");
		assert( rgb > -1e-10f * Vector3::UNIT_SCALE && 
				rgb.squaredLength() <= 3 && 
				"Vector RGB no v�lido");

		const Ogre::MaterialPtr& material = _entity->getSubEntity(subIndex)->getMaterial();
		
		std::stringstream ss;
			ss	<< material->getName() << "#"	
				<< std::setw(2) << std::setfill('0') << std::hex << int(rgb.x * 255) 
				<< std::setw(2) << std::setfill('0') << std::hex << int(rgb.y * 255) 
				<< std::setw(2) << std::setfill('0') << std::hex << int(rgb.z * 255);
			
		std::string colorMatName = ss.str(); // = "matName#RRGGBB"
			if( !Ogre::MaterialManager::getSingletonPtr()->resourceExists(colorMatName) ) {
				const Ogre::MaterialPtr& colorMat = material->clone( colorMatName );
					colorMat->setAmbient(rgb.x, rgb.y, rgb.z);
					colorMat->setDiffuse(rgb.x, rgb.y, rgb.z, 1);
			}
			setMaterial( colorMatName, subIndex);

	} // setColor

	void CEntity:: setColor(const std::string& color, unsigned int subIndex)
	{		
		setColor( colorToRGB(color), subIndex);
	}// setColor
	


} // namespace Graphics
