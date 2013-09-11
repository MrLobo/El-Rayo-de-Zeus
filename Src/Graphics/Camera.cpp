//---------------------------------------------------------------------------
// Camera.cpp
//---------------------------------------------------------------------------

/**
@file Camera.cpp

Contiene la implementaci�n de la clase que maneja la c�mara.

@see Graphics::CCamera

@author David Llans�
@date Julio, 2010
*/

#include "Camera.h"
#include "Scene.h"

#include "BaseSubsystems/Server.h"
#include "BaseSubsystems/Math.h"

#include <assert.h>

#include <OgreCamera.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreViewport.h>

namespace Graphics 
{
	CCamera::CCamera(const std::string &name, CScene *scene)
		: _scene(0)
	{
		_name = name;
		_scene = scene;

		_camera = scene->getSceneMgr()->createCamera(name + "_camera");
			_camera->setNearClipDistance(20);
			_camera->setFarClipDistance(30000);
		//HACK: Valores cableados de las distancias para reenderizar. 
		// Deber�an poder configurarse.

		// Creamos la estructura de nodos de la c�mara. Los nodos cuelgan
		// de la raiz, son globales.		
		_targetNode = scene->getSceneMgr()->getRootSceneNode()->createChildSceneNode(name + "_target_node");
		_cameraNode = _scene->getSceneMgr()->getRootSceneNode()->createChildSceneNode(name + "_camera_node");
			_cameraNode->setAutoTracking(true, _targetNode);// Hacemos que el nodo de la c�mara siempre est� mirando al nodo objetivo.
			_cameraNode->setFixedYawAxis(true); // Fijamos el viraje de la c�mara para se mantenga paralelo al suelo.
			_cameraNode->attachObject (_camera); // Finalmente adjuntamos la c�mara a su nodo.

	} // CCamera

	//--------------------------------------------------------

	CCamera::~CCamera() 
	{
		// desacoplamos la c�mara de su nodo
		_cameraNode->detachAllObjects();
		_scene->getSceneMgr()->destroyCamera(_camera);
		_scene->getSceneMgr()->destroySceneNode(_cameraNode);
		_scene->getSceneMgr()->destroySceneNode(_targetNode);

	} // ~CCamera


	/********************
		GET's & SET's
	*******************/
	
	const Vector3 &CCamera::getPosition() const
	{
		return _cameraNode->getPosition();
	}
	
	//--------------------------------------------------------
	
	const Quaternion &CCamera::getOrientation() const
	{
		return _cameraNode->getOrientation();
	}

	//--------------------------------------------------------

	const Vector3 &CCamera::getTargetPosition() const
	{
		return _targetNode->getPosition();
	}

	//--------------------------------------------------------

	Ogre::Viewport* CCamera::getViewport() const
	{
		return _camera->getViewport();
	}


	//--------------------------------------------------------

	void CCamera::setPosition(const Vector3 &newPosition)
	{
		_cameraNode->setPosition(newPosition);
	}

	//--------------------------------------------------------

	void CCamera::setTargetPosition(const Vector3 &newPosition)
	{
		_targetNode->setPosition(newPosition);
	}

	

} // namespace Graphics
