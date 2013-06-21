//---------------------------------------------------------------------------
// ParticleSystem.cpp
//---------------------------------------------------------------------------

/**
@file ParticleSystem.cpp

Contiene la implementaci�n de la clase que maneja el ParticleSystem.

@see Graphics::CParticleSystem

@author FRS
@date Mayo, 2013
*/

#include "ParticleSystem.h"


#include <OgreCommon.h>
#include <OgreMovableObject.h>
#include <OgreSceneManager.h>

#include "Camera.h"
#include "Scene.h"
#include "Server.h"

#define DEBUG 1
#if DEBUG
#	include <iostream>
#	define LOG(msg) std::cout << "GRAPHICS::PARTICLE_SYSTEM>> " << msg << std::endl;
#else
#	define LOG(msg)
#endif

namespace Graphics 
{	

	CParticleSystem::CParticleSystem(const std::string& hfx, const std::string& parentName, const Vector3& relativePos) :
			_hfx(hfx), _parentName(parentName), _relativePos(relativePos), _movObj(0), _hhfxScene(0), _fxLight(0)
	{
		_hhfxParams["pack"] =  Graphics::CServer::getSingletonPtr()->getHHFXBase()->GetHHFXPackExplorer().GetPack();
		_hhfxParams["fx"]	=  Graphics::CServer::getSingletonPtr()->getHFXLongName(hfx);
		_hhfxParams["run"]	=  "yes";	// Por defecto: ejecuci�n inmediata al crear el MO
	}

	//--------------------------------------------------------
	
	void CParticleSystem::start()
	{	
		if(_movObj) { // MO tiene que ser NULL; lo contrario significa que ya hay un FX en curso
			_movObj->RunFX(); // TODO Verificar que se liberan bien... queremos acumular N veces el mismo efecto?

		} else {
			// spawn a new effect at this location
			Ogre::MovableObject	*mo = getSceneMgr()->createMovableObject("HHFX", &_hhfxParams);
				assert(mo && "Error al crear ParticleSystem");	

			// set this class to listen to the ps, to be notified when it is destroyed.
			_movObj = static_cast<IHHFXOgre*>(mo);
				_movObj->SetFXListener(this);
				_node->attachObject(_movObj);
		}
	}

	//--------------------------------------------------------

	// UNDONE FRS: No se puede usar el m�todo StopFX => Inconsistencias y pescaillas que se muerden la cola con evento StoppedFX
	void CParticleSystem::stop()
	{
		if(_movObj) {

			// TODO FRS

			//_movObj->setVisible(false);
			//_movObj->detachFromParent();
			//LOG("isAttached = "			<< _movObj->isAttached()			)
			//LOG("IsFXActive = "			<< _movObj->IsFXActive()			)
			//LOG("isInScene= "			<< _movObj->isInScene()			)
			//LOG("isParentTagPoint = "	<< _movObj->isParentTagPoint()	)
			//LOG("isVisible = "			<< _movObj->isVisible()			)

			//_movObj->SetFXListener(0);

			//_movObj->StopFX();
			//static_cast<Ogre::MovableObject*>(_movObj)->detachFromParent();		
			//getSceneMgr()->destroyMovableObject( static_cast<Ogre::MovableObject*>(_movObj) ); 
			//getSceneMgr()->destroySceneNode(_node);		
			//_movObj = 0;	
			//_node = 0;
		}

			// FRS Opciones fallutas
			//	_node->setVisible(false); 
			//	_movObj->StopFX();		
		
	
	}
		
	




	/**********************
		HHFX LISTENER
	***********************/

	// TODO revisar
	void CParticleSystem::OnFXStarted(IHHFX* obj)
	{
		assert( _movObj == static_cast<IHHFXOgre*>(obj)  
			&& "Evento recibido para un MO distinto del wrappeado en este ParticleSystem");

		if(_fxLight) 
			_fxLight->setVisible(true);
	}

	//--------------------------------------------------------

	// called when an effect stopped by itself or when the hhfx scene is cleared
	void CParticleSystem::OnFXStopped(IHHFX* obj)
	{		
		assert( _movObj == static_cast<IHHFXOgre*>(obj)  
			&& "Evento recibido para un MO distinto del wrappeado en este ParticleSystem");
		
		// destroy the light created under ElectricOrb
// UNDONE FRS -> ESC es es el apa�o
		//if(_fxLight) 
		//	_fxLight->setVisible(false);		

		if(_movObj) {
			getSceneMgr()->destroyMovableObject(_movObj); 		
			_movObj = 0;			
		}
	}

	


	/**********************
		SCENE ELEMENT
	***********************/

	bool CParticleSystem::load()
	{
		// Orientacion initial rotada 180 hacia -X (cara frontal de nuestras entidades)
		static const Quaternion ORIENTATION(Ogre::Radian( Math::PI ), Vector3::UNIT_Y);

		assert( getSceneMgr()->hasSceneNode( _parentName + "_node") );

		try{		
			_hhfxScene = _scene->getHHFXScene();

			_node = getSceneMgr()->getSceneNode( _parentName + "_node") 
				 ->createChildSceneNode(_relativePos, ORIENTATION);

			// create a light under the ElectricOrb effect
			if (_hfx == "ElectricOrb")  {			
				_fxLight = getSceneMgr()->createLight();
				_fxLight->setVisible(false);
				_fxLight->setType(Ogre::Light::LT_POINT);	
				_fxLight->setDiffuseColour(0.1f, 0.1f, 1.0f);
				_fxLight->setSpecularColour(0.8f, 0.8f, 1.0f);
				_fxLight->setAttenuation(160, 1.0, 0.027, 0.0028);		
				_fxLight->setPosition(Vector3::UNIT_Y * 0.8f); 	// Segun HHFX sample para centrar la luz en la esfera (si worldScale = 1)
				_node->attachObject(_fxLight);					
			}

			_loaded = true;
			
		} catch(std::exception e){
			_loaded = false;
		}

		return _loaded;		
	} // load
	
	//--------------------------------------------------------
		
	void CParticleSystem::unload()
	{	
		CSceneElement::unload();	

		if (_fxLight) {
			getSceneMgr()->destroyLight(_fxLight);
			_fxLight = 0;
		}
		if(_movObj)	{		
			getSceneMgr()->destroyMovableObject(_movObj); 
			_movObj = 0;
		}
	} // unload

	


	
	
	/**********************
		GET's & SET's
	***********************/

	bool CParticleSystem::isVisible() const {
		assert(_loaded && "Imprescindible haber cargado el sistema de particulas en escena primero");
		return _movObj->isVisible();
	}

	
} // namespace Graphics

