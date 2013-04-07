/**
@file BaseTraveler.cpp

Contiene la implementaci�n del componente que controla la representaci�n
gr�fica de la entidad.
 
@see Logic::CBaseTraveler
@see Logic::IComponent

@author Jose Luis L�pez S�nchez
@date Febrero, 2013
*/

#include "Logic/Entity/Entity.h"
#include "../../../Application/OgreClock.h" 
#include "../../../Application/BaseApplication.h" 

#include "Logic/Entity/Components/BaseTraveler.h"

#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageChar.h"
#include "Logic/Entity/Messages/MessageUShort.h"
#include "Logic/Entity/Messages/MessageString.h"

#include "Map/MapEntity.h"
#include "Logic/Maps/Map.h"

/*para tener un acceso directo al gamestatus*/
#include "Logic/GameStatus.h"
#include "Logic/RingInfo.h"
#include "Logic/BaseInfo.h"
#include "Logic/PlayerInfo.h"

#define DEBUG 1
#if DEBUG
#	include <iostream>
#	define LOG(msg) std::cout << "LOGIC::BASE_TRAVELER>> " << msg << std::endl;
#else
#	define LOG(msg)
#endif

namespace Logic 
{
	IMP_FACTORY(CBaseTraveler);
	
	//---------------------------------------------------------

	CBaseTraveler::~CBaseTraveler() 
	{	} // ~CBaseTraveler
	
	void CBaseTraveler::timeArrived()
	{
		if (_changingBase && !this->isChangingRing())
		{
			LOG("EXITO");
			CMessageChar *m0 = new CMessageChar();	
			m0->setType(Message::AVATAR_MOVE);
			m0->setAction(Message::CHANGE_BASE);
			m0->setChar( _destiny - (int) _entity->getLogicalPosition()->getBase() ); // ��� Enviamos diferencial de base (AVATAR_MOVE es movimiento diferencial)
			_entity->emitMessage(m0,this);

			LOG("Change Base from " << _entity->getLogicalPosition()->getBase() << " to " << _destiny );

			CMessageString *m2 = new CMessageString();	
			m2->setType(Message::SET_MATERIAL);
			m2->setString("marine");
			_entity->emitMessage(m2,this);
		}
		else
				CRingTraveler::timeArrived();

		_changingBase=false;
		_changingBaseTime=0;
			
	
	}
	//---------------------------------------------------------

	bool CBaseTraveler::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;		
		
		//lo hago aqu� mismo, en alg�n componente hay que hacerlo y en principio solo los personajes
		//player  (ya sea humano o bot) pueden viajar entre bases.
		
		//_gameStatus=Application::CBaseApplication::getSingletonPtr()->getGameState()->getGameStatus();
			
		_gameStatus=CGameStatus::getSingletonPtr();
		//creamos un altar pasandole la entidad propietaria del presente compontente.
		//IMPORTANTE: Hay que tener en cuenta que una unidad pertenece SIEMPRE a la base en la que nace.
		//tambi�n es imporante tener en cuenta, que los arrays _base y _players tienen una corresponendencia de �ndice
		//es decir, _bases[1] es del jugador 1 que est� guardado en _players[1]
		_gameStatus->getPlayer(entity->getLogicalPosition()->getBase())->attachCEntity(_entity);
		
		if(entityInfo->hasAttribute("hero"))
			_gameStatus->getPlayer(entity->getLogicalPosition()->getBase())->setHeroName( entityInfo->getStringAttribute("hero"));		
		
		return true;
	} // spawn
	
	//---------------------------------------------------------

	
	bool CBaseTraveler::accept(const CMessage *message)
	{//que no os confunda el nombre de mensaje CHANGE_PLANE es tanto para cambiar de base como de anillo dentro de la base. Apreciad que en cualquier caso siempre es un cambio de anillo, de ah� el nombre
		return (isAwake() && !_changingBase && (CRingTraveler::accept(message) || 
					(message->getType() == Message::CONTROL &&
					message->getAction() == Message::CHANGE_BASE)));

	} // accept
	
	//---------------------------------------------------------

	void CBaseTraveler::process(CMessage *message)
	{
		CRingTraveler::process(message);
		CMessageUShort *maux = static_cast<CMessageUShort*>(message); // TODO FRS esto podr�a ser char...
		
		switch(message->getType())
		{
		case Message::CONTROL:
			if(message->getAction() == Message::CHANGE_BASE)
			{
				_destiny=maux->getUShort();
				_changingBase=true;
			}
		}

	} // process

			
	void CBaseTraveler::tick(unsigned int msecs)
	{
			CRingTraveler::tick(msecs);
			IComponent::tick(msecs);
			
			if (_changingBase)
			{
				if(_entity->isPlayer())
				{
				/*	CMessageString *m = new CMessageString();	
					m->setType(Message::SET_MATERIAL);
					m->setString("transito");
					_entity->emitMessage(m,this);
				*/}
			}
		}

} // namespace Logic

