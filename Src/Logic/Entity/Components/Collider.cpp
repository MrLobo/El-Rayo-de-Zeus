/**
@file Collider.cpp

Contiene la implementaci�n del componente que controla el movimiento 
de la entidad.
 
@see Logic::CCollider
@see Logic::IComponent

@author David Llans�
@date Agosto, 2010
*/

#include "Collider.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Entity/Component.h"


#include "Map/MapParser.h"



namespace Logic 
{
	IMP_FACTORY(CCollider);
	
	//---------------------------------------------------------

	bool CCollider::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		_excluido=NULL;
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CCollider::activate()
	{
		return true;
	} // activate
	
	//---------------------------------------------------------

	void CCollider::deactivate()
	{

	} // deactivate
	
	
	//---------------------------------------------------------

	bool CCollider::accept(const TMessage &message)
	{
		
		return message._type == Message::CONTACTO;
	} // accept
	
	//---------------------------------------------------------

	void CCollider::process(const TMessage &message)
	{
		switch(message._type)
		{
		case Message::CONTACTO:
			//Contacto();
			break;
		 //PROCESAR EL CONTACTO, PARAR A LAS ENTIDADES IMPLICADAS
		
		}

	} // process
	
	//---------------------------------------------------------

	//---------------------------------------------------------

	bool CCollider::contactoAngular( CEntity* entidad1, CEntity* entidad2)
	{
		if (entidad1==entidad2)
			return false;
		if (entidad1->getBase()!=entidad2->getBase()) 
			return false;
		if (entidad1->getRing()!=entidad2->getRing()) 
			return false;
	

		if (entidad1->getJumping()==true || entidad2->getJumping()==true)
			return false;


		if (entidad2->getAngularBox()==0)
			return false;
		float logicalCenterDistance=abs(entidad1->getDegree()-entidad2->getDegree());//distancia entre los centros de las entidades
		if (logicalCenterDistance>180) //
			logicalCenterDistance=360-logicalCenterDistance;

		float angularBoxAmount=entidad1->getAngularBox()+entidad2->getAngularBox();
		//if (this->getType().compare("AnimatedEntity")==0)
		//	int i=0;
		if (logicalCenterDistance<=angularBoxAmount) //si la distancia de los centros es menor que la suma de los radios hay contacto
		{	
			return true;		
		}
			return false;
	}

	bool CCollider::contactoExtremo( CEntity* entidad1, CEntity* entidad2)
	{		
		if (entidad2->getAngularBox()==0)
			return false;
		float logicalCenterDistance=abs(entidad1->getDegree()-entidad2->getDegree());//distancia entre los centros de las entidades
		if (logicalCenterDistance>180) //
			logicalCenterDistance=360-logicalCenterDistance;

		if (logicalCenterDistance<((entidad1->getAngularBox()+entidad2->getAngularBox()/2)))
			return true;

		return false;

		float angularBoxAmount=entidad1->getAngularBox()+entidad2->getAngularBox();
		//if (this->getType().compare("AnimatedEntity")==0)
		//	int i=0;
		if (logicalCenterDistance<=angularBoxAmount) //si la distancia de los centros es menor que la suma de los radios hay contacto
		{	
			return true;		
		}
			return false;
	}


	bool CCollider::contacto( CEntity* entidad1, CEntity* entidad2)
	{
		Logic::TMessage m;

		//_Vida--; si tiene vida se le disminuye, si es un proyectil no tiene vida
		// Pablo 28-01-2013
		m._type = Logic::Message::CONTACTO;
		m._string="updateLife";
		m._float=10.0;
		entidad1->emitMessage(m,this);
		//entidad2->emitMessage(m,this);
		//fin emision mensaje de vida

		m._string="luminoso";
		m._type = Logic::Message::SET_SHADER;
		entidad1->emitMessage(m,this);
		entidad2->emitMessage(m,this);


		if (entidad1->getType().compare("Player")==0)
			m._type = Logic::Message::CONTROL;
		if (entidad1->getType().compare("AnimatedEntity")==0)
			m._type = Logic::Message::NPC_CONTROL;


	//	_sentidoColision=sentidoColision(entidad1,entidad2);
        m._bool=sentidoColision(entidad1,entidad2);
//		m._bool=_sentidoColision;

		//correcci�n para cuerpos superspuestos
		if (contactoExtremo(entidad1,entidad2))
		{
			m._string="walkBack";
			m._float=(((entidad1->getAngularBox()+entidad2->getAngularBox())/4));
			entidad1->emitMessage(m,this);	
			m._bool=!m._bool;
			if (entidad2->getType().compare("Player")==0)
				m._type = Logic::Message::CONTROL;
			if (entidad2->getType().compare("AnimatedEntity")==0)
				m._type = Logic::Message::NPC_CONTROL;


			entidad2->emitMessage(m,this);	
		}
		else
		{
			m._string="walkStop";
			entidad1->emitMessage(m,this);	
			m._bool=!m._bool;
			if (entidad2->getType().compare("Player")==0)
				m._type = Logic::Message::CONTROL;
			if (entidad2->getType().compare("AnimatedEntity")==0)
				m._type = Logic::Message::NPC_CONTROL;


			entidad2->emitMessage(m,this);	
		}
		m._string="changeDirection";
			
		m._bool=sentidoColision(entidad1,entidad2);
			if (entidad1->getType().compare("Player")==0)
				m._type = Logic::Message::CONTROL;
			if (entidad1->getType().compare("AnimatedEntity")==0)
				m._type = Logic::Message::NPC_CONTROL;

		entidad1->emitMessage(m,this);	
		m._bool=!m._bool;
			if (entidad2->getType().compare("Player")==0)
				m._type = Logic::Message::CONTROL;
			if (entidad2->getType().compare("AnimatedEntity")==0)
				m._type = Logic::Message::NPC_CONTROL;

		entidad2->emitMessage(m,this);	
			
		//}
		/*

		if (entidad2->getType().compare("Player")==0)
			m._type = Logic::Message::CONTROL;
		if (entidad2->getType().compare("AnimatedEntity")==0)
			m._type = Logic::Message::NPC_CONTROL;

		_sentidoColision=!_sentidoColision;
		m._bool=_sentidoColision;
		entidad2->emitMessage(m,this);

		*/		


		//_hit++;

		return false;
		}

		bool CCollider::sentidoColision(const CEntity* entidad1,const CEntity* entidad2)
	{
		float degree1=entidad1->getDegree();
		float degree2=entidad2->getDegree();
		
		if (abs(abs(degree1)-abs(degree2))>200)
		
			if (degree1<degree2) //esta comprobaci�n es para los casos en el extremo (grado 0 o 360) 
				degree1+=360;
			else
				degree2+=360;

		if (degree1<degree2)
			return false;
		else 
			return true;

	}

	void CCollider::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);		

		//me gustar�a que en esta lista tan s�lo estuvieran entidades con componente CColider
		//if (_entity->getType().compare("Player")==0)
		//{
		CMap::TEntityList::const_iterator it = _entity->getMap()->getEntities().begin();
		CMap::TEntityList::const_iterator end = _entity->getMap()->getEntities().end();
		_excluido=NULL;
		_comprobando=!_comprobando;
		//if (_comprobando)
		for(; it != end; it++)
		{
		
			//Si la entidad que comparo no soy yo mismo y la distancia entre las posiciones
			//de las dos es menor de la distancia de colisi�n (o radio)
			
			if(_entity != (*it) && contactoAngular(_entity,(*it)) )
			{
				if (_excluido==NULL && ((*it)->getType().compare("Altar")!=0)&& ((*it)->getType().compare("World")!=0))
				{//lo que hay que  hacer es que no se itere sobre entidades que no tengan componente CCollider, de momento se hace esa comprobaci�n
					contacto(_entity,(*it));
 					//_excluido=(*it);

					break;
				}
				//Logic::TMessage m;
				//m._type = Logic::Message::CONTACTO;				
				//_entity->emitMessage(m,this);				
			}
			} // tick
		}
//	}
		


} // namespace Logic
