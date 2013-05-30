//---------------------------------------------------------------------------
// AnimSet.cpp
//---------------------------------------------------------------------------

/**
@file AnimSet.cpp

Contiene la implementaci�n de la clase que reprenta una correspondencia entre nombr�s l�gicos de animaci�n y nombres f�sicos

@see Graphics::CAnimSet

@author Jose Luis L�pez
@date Mayo, 2013
*/
#include <assert.h>
#include "AnimSet.h"
//#include "Graphics\Entity.h"
//#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

namespace Graphics 
{
	
	/**
	Constructor de la clase.

	@param entityInfo informaci�n del Map para leer la informaci�n acerca de la animaci�n
	*/
	CAnimSet::CAnimSet()
	{
	}

	/**
	Destructor de la clase.
	*/
	CAnimSet::~CAnimSet()
	{
		_animationSet.clear();
	}

	//--------------------------------------------------------
		
	bool CAnimSet::addAnimation(const Logic::AnimationName animEnum, const std::string realName)
	{
		_animationSet[animEnum]=realName;

		return true; // TODO, hacer que devuelva false ante un error.
	} // addAnimation
	
	std::string CAnimSet::getAnimation(const Logic::AnimationName animEnum)
	{
		std::string animation;
		animation=_animationSet[animEnum];
		return animation;
	} // getAnimation

	bool CAnimSet::addEventTime(const Logic::AnimationName animEnum, const Logic::Tracks track, const float eventTime)
	{
		std::pair<Logic::Tracks,float> trackEvent(track,eventTime );
		_eventChainMap[animEnum].push_back(trackEvent);
		 // Using the constructor to declare and initialize a pair
		
		/*std::vector<TTrackEvent> trackEvent= _eventChain[animEnum];
		trackEvent.emplace_back(eventTime);
		_eventChain[animEnum]=trackEvent;
		*/	
		return true;
	}

	std::vector<std::pair<unsigned short,float>>* CAnimSet::getEventChain(const Logic::AnimationName animEnum)
	{
		return &_eventChainMap[animEnum];
	}

	//--------------------------------------------------------	

} // namespace Graphics
