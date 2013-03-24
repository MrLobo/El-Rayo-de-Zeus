//---------------------------------------------------------------------------
// Scene.cpp
//---------------------------------------------------------------------------

/**
@file Scene.cpp

Contiene la implementaci�n de la clase contenedora de los elementos
de una escena.

@see Graphics::CScene

@author David Llans�
@date Julio, 2010
*/

#include "Scene.h"

#include "BaseSubsystems/Server.h"

#include "Graphics/Billboard.h"
#include "Graphics/Camera.h"
#include "Graphics/Entity.h"
#include "Graphics/GlowMaterialListener.h"
#include "Graphics/Server.h"
#include "Graphics/StaticEntity.h"

#include "Logic/Server.h"

#include <assert.h>

#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreViewport.h>
#include <OgreStaticGeometry.h>
#include <OgreColourValue.h>

//PT
#include <OgreParticleSystem.h> // TODO FRS Por desvincular (al igual que billboardSet)
#include <OgreCompositorManager.h>


namespace Graphics 
{

	CScene::CScene(const std::string& name) : _name(name), _viewport(0), 
			_staticGeometry(0), _directionalLight1(0), _directionalLight2(0), counterParticles(0)
	{
		_root = BaseSubsystems::CServer::getSingletonPtr()->getOgreRoot();
		_sceneMgr = _root->createSceneManager(Ogre::ST_INTERIOR, name);
		_camera = new CCamera(name,this);
	} // CScene

	//--------------------------------------------------------

	CScene::~CScene() 
	{
		deactivate();
		_sceneMgr->destroyStaticGeometry(_staticGeometry);
		delete _camera;
		_root->destroySceneManager(_sceneMgr);

	} // ~CScene

	//--------------------------------------------------------

	
	void CScene::activate()
	{
		buildStaticGeometry(); // FRS Se debe construir en cada activaci�n?

		// HACK en pruebas
		_viewport = BaseSubsystems::CServer::getSingletonPtr()->getRenderWindow()
						->addViewport(_camera->getCamera());
		_viewport->setBackgroundColour(Ogre::ColourValue::Black);
		
		// FRS Lo suyo ser�a introducirlas mediante un CShadows o algo asin + attachToScene 
		//Sombras Chulas - Consumen mucho*/
		//_sceneMgr->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_STENCIL_ADDITIVE);

		_sceneMgr->setAmbientLight(Ogre::ColourValue(0.9f,0.9f,0.9f));

		// Adem�s de la luz ambiente creamos una luz direccional que 
		// hace que se vean mejor los vol�menes de las entidades.
		_directionalLight1 = _sceneMgr->createLight("directional light1");
		//_directionalLight2 = _sceneMgr->createLight("directional light2");

		_directionalLight1->setDiffuseColour(.5f,.5f,.5f);
		//_directionalLight2->setDiffuseColour(.5f,.5f,.5f);

		_directionalLight1->setSpecularColour(.5f,.5f,.5f);
		//_directionalLight2->setSpecularColour(.5f,.5f,.5f);

		_directionalLight1->setType(Ogre::Light::LT_DIRECTIONAL);
		//_directionalLight2->setType(Ogre::Light::LT_DIRECTIONAL);

		_directionalLight1->setPosition(150, 150, 0);
		//_directionalLight2->setPosition(-150, 150, 0);

		_directionalLight1->setDirection(-150, -150, 0);
		//_directionalLight2->setDirection(150, -150, 0);

		Ogre::CompositorManager::getSingletonPtr()->addCompositor(_camera->getViewport(), "Glow");
		Ogre::CompositorManager::getSingletonPtr()->setCompositorEnabled(_camera->getViewport(), "Glow", true);

		GlowMaterialListener *gml = new GlowMaterialListener();
		Ogre::MaterialManager::getSingletonPtr()->addListener(gml);

		//_directionalLight->setType(Ogre::Light::LT_POINT);
		//_directionalLight->setPosition(0, 100, 0);
		

	} // activate

	//--------------------------------------------------------

	void CScene::deactivate()
	{
		if(_directionalLight1)
		{
			_sceneMgr->destroyLight(_directionalLight1);
			_directionalLight1 = 0;
		}
		if(_directionalLight2)
		{
			_sceneMgr->destroyLight(_directionalLight2);
			_directionalLight2 = 0;
		}
		if(_viewport)
		{
			BaseSubsystems::CServer::getSingletonPtr()->getRenderWindow()->
					removeViewport(_viewport->getZOrder());
			_viewport = 0;
		}

	} // deactivate
	
	//--------------------------------------------------------

	// FRS S�lo se actualizan las entidades din�micas
	// Y en concreto, s�lo implementa el tick el CAnimatedEntity : CEntity
	void CScene::tick(float secs)
	{	
		TEntities::const_iterator it = _dynamicEntities.begin();
		TEntities::const_iterator end = _dynamicEntities.end();
		for(; it != end; it++)
			(*it)->tick(secs);

	} // tick



	/************************
		SCENE ELEMENTS
	************************/


	//---------- ENTITIES -------------------------

	bool CScene::add(CEntity* entity)
	{
		if(!entity->attachToScene(this))
			return false;
		_dynamicEntities.push_back(entity);
		return true;

	} // addEntity

	void CScene::remove(CEntity* entity)
	{
		entity->deattachFromScene();
		_dynamicEntities.remove(entity);
	} // addEntity



	//----------STATIC ENTITIES-------------------------

	bool CScene::add(CStaticEntity* entity)
	{
		if(!entity->attachToScene(this))
			return false;
		_staticEntities.push_back(entity);
		return true;

	} // addStaticEntity


	void CScene::remove(CStaticEntity* entity)
	{
		entity->deattachFromScene();
		_staticEntities.remove(entity);

	} // addStaticEntity


	void CScene::buildStaticGeometry()
	{
		if(!_staticGeometry && !_staticEntities.empty())
		{
			_staticGeometry = 
					_sceneMgr->createStaticGeometry("static");

			TStaticEntities::const_iterator it = _staticEntities.begin();
			TStaticEntities::const_iterator end = _staticEntities.end();
			for(; it != end; it++)
				(*it)->addToStaticGeometry();

			_staticGeometry->build();
		}

	} // buildStaticGeometry



	

} // namespace Graphics
