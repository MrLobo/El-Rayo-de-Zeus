/**
@file Attack.cpp

Contiene la declaraci�n del componente que controla el ataque 
angular de entidades.

@see Logic::CAttack
@see Logic::IComponent

@author Jos� Luis L�pez
@date Diciembre, 2012
*/

#include "Attack.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Logic/Maps/Map.h"
#include "Application/BaseApplication.h"

#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageFloat.h"
#include "Logic/Entity/Messages/MessageBoolString.h"
#include "Logic/Entity/Messages/MessageString.h"
#include "Logic/Entity/Messages/MessageInt.h"
#include "Logic/Entity/Messages/MessageAudio.h"

//declaraci�n de la clase
namespace Logic 
{
	IMP_FACTORY(CAttack);

	bool CAttack::spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo){

		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if(entityInfo->hasAttribute("attackPower"))
			_attackPower = entityInfo->getFloatAttribute("attackPower");
		if (_entity->getName()=="locoCubriendose")
			cover();
		if (entityInfo->hasAttribute("audioCubriendose") )
			_audioCubriendose = entityInfo->getStringAttribute("audioCubriendose");
		return true;
		}

	bool CAttack::activate()
	{				
		_lightAttack=_heavyAttack=false;
		return true;
	}		

	void CAttack::deactivate()
	{
	}
	
	bool CAttack::accept(const CMessage *message)
	{//aviso de que tanto accept como process son un poco hack, pero es es solo hasta tener un componente NPCCONTROLLER
	//	return false;
		return (message->getType() == Message::CONTROL && 
			(message->getAction() == Message::LIGHT_ATTACK||
			message->getAction() == Message::HEAVY_ATTACK) || message->getAction() == Message::COVER) || (message->getType()==Message::ANIMATION_FINISHED ||message->getType()==Message::ANIMATION_MOMENT);

	}
		
	 void CAttack::process(CMessage *message)
		 {
		switch(message->getType())
		{
		case Message::CONTROL:
			{			
				if(message->getAction() == Message::LIGHT_ATTACK)
					lightAttack();
				else if(message->getAction() == Message::HEAVY_ATTACK)
					 heavyAttack();
				else if(message->getAction() == Message::COVER)
					cover();
				break;
			}
		case Message::ANIMATION_FINISHED:
			{
				CMessageString* maux = static_cast<CMessageString*>(message);
				if (maux->getString().compare("FireKatana")==0)
				{
					
					_lightAttack=_heavyAttack=false;//stopMovement();
				}
				break;
			}
		
		case Message::ANIMATION_MOMENT:
			{
				//querr� saber cual animaci�n es, de momento se que solo puedo recibir un tipo de animaci�n
				float punto;
				if (_entity->getLogicalPosition()->getSense()==Logic::LogicalPosition::RIGHT)
					punto=_entity->getLogicalPosition()->getDegree()-10;
				else
					punto=_entity->getLogicalPosition()->getDegree()+10;
					//con este metodo vemos si con la espada le estamos dando
				
					unsigned short resultadoAtaque=attackPlace(punto,_entity->getLogicalPosition()->getRing(),_entity->getLogicalPosition()->getBase(),false);
					if (resultadoAtaque==2)
					{
						CMessageBoolString *message = new CMessageBoolString();
						message->setType(Message::REWIND_ANIMATION);
						message->setString("FireKatana");
						message->setAction(Message::UNDEF);
						message->setBool(false);
						_entity->emitMessage(message,this);	

					


					}
					
			}
		}
	 }

	 void CAttack::cover()
	 {
		_covering=true;
		CMessageBoolString *message = new CMessageBoolString();
		message->setType(Message::SET_ANIMATION);
		message->setString("CrouchKatana");
		message->setAction(Message::UNDEF);
		message->setBool(true);
		_entity->emitMessage(message,this);
	 }


	void CAttack::lightAttack() 
	{
		if (_lightAttack) return;
		_heavyAttack=false;
		_lightAttack=true;

	/*	CMessageBoolString *message = new CMessageBoolString();
		message->setType(Message::SET_ANIMATION);
		message->setAction(Message::LIGHT_ATTACK);
		message->setString("FireAK47");
		message->setBool(false);
		_entity->emitMessage(message,this);
		*/
		//envio mensaje para averiguar sin con este ataque le he arreado a alguien o no
		/*CMessageFloat *message2 = new CMessageFloat();
		message2->setType(Message::SPACE_REQUEST);
		if (_entity->getSense()==Logic::LogicalPosition::RIGHT)
		message2->setFloat(_entity->getDegree()-10);
		else
		message2->setFloat(_entity->getDegree()+10);		
		_entity->emitMessage(message2,this);*/
		/*float punto;
		if (_entity->getSense()==Logic::LogicalPosition::RIGHT)
			punto=_entity->getDegree()-10;
		else
			punto=_entity->getDegree()+10;*/
	//	attackPlace(punto,_entity->getRing(),_entity->getBase(),true);
	} // turn

	void CAttack::heavyAttack() 
	{		
		if (_heavyAttack) return;
		_heavyAttack=true;
		_lightAttack=false;

	} // turn

	//este metodo devuelve null si no se est� ocupando ese grado o la entidad que ocupa ese espacio
	unsigned short CAttack::attackPlace(float grado, short ring, short base,bool soloInfo)
	{//acotar
		//averiguo el espacio que ocupo:

		
		CMap::TEntityList::const_iterator it = _entity->getMap()->getEntities().begin();
		CMap::TEntityList::const_iterator end = _entity->getMap()->getEntities().end();

		for(; it != end; ++it)
		{			
			//Si la entidad que comparo no soy yo mismo y la distancia entre las posiciones
			//de las dos es menor de la distancia de colisi�n (o radio)


			if(_entity != (*it) )
			{
				if (((*it)->getType().compare("PowerUp")!=0)&&((*it)->getType().compare("Altar")!=0)&& ((*it)->getType().compare("World")!=0)&& ((*it)->getType().compare("SkyBox")!=0))
				{//lo que hay que  hacer es que no se itere sobre entidades que no tengan componente CCollider, de momento se hace esa comprobaci�n

			
					if (base==(*it)->getLogicalPosition()->getBase() && ring==(*it)->getLogicalPosition()->getRing())	
					{								
						//float limiteIzquierdo=(*it)->getDegree()-(*it)->getAngularBox();
						//float limiteDerecho=(*it)->getDegree()+(*it)->getAngularBox();
						float limiteIzquierdo=(*it)->getLogicalPosition()->getDegree()-5; //aqu� se est� asumiendo que es un angular box de 5
						float limiteDerecho=(*it)->getLogicalPosition()->getDegree()+5;
					//	if (_entity->getLogicalPosition()->getDegree()<grado) limiteIzquierdo =_entity->getLogicalPosition()->getDegree();
					//	else if (_entity->getLogicalPosition()->getDegree()>grado) limiteDerecho =_entity->getLogicalPosition()->getDegree();
						if ((*it)->getComponent<CAttack>()!=NULL)
						if (grado>limiteIzquierdo && grado<limiteDerecho 
							|| (grado>=limiteIzquierdo && grado>=limiteDerecho &&  _entity->getLogicalPosition()->getDegree()<=limiteIzquierdo && _entity->getLogicalPosition()->getDegree()<=limiteDerecho) 
							||  (grado<=limiteIzquierdo && grado<=limiteDerecho &&  _entity->getLogicalPosition()->getDegree()>=limiteIzquierdo && _entity->getLogicalPosition()->getDegree()>=limiteDerecho) 
							)
						{
							if (!soloInfo)
							{				
								if ((*it)->getComponent<CAttack>()!=NULL)
								if ((*it)->getComponent<CAttack>()->_covering==true && (*it)->getLogicalPosition()->getSense()!=_entity->getLogicalPosition()->getSense())
								{
									Logic::CMessage *m = new Logic::CMessage();
									m->setType(Logic::Message::CONTROL);
									m->setAction(Logic::Message::WALK_STOP);
									(*it)->emitMessage(m);

								
									Logic::CMessageAudio *cubiertoAudio=new Logic::CMessageAudio();		
									cubiertoAudio->setType(Message::AUDIO);			
									cubiertoAudio->setPath("media\\audio\\Sword_scrape2.wav");//�apa
									cubiertoAudio->setId("impactoCubriendose");
									cubiertoAudio->setPosition(_entity->getPosition());
									(*it)->emitMessage(cubiertoAudio);
										
									return 2; //Impacto en el que el objetivo est� cubriendose
								}
								else
								{
								CMessageInt *m2 = new CMessageInt();
								m2->setInt(-10);
								m2->setType(Message::LIFE_MODIFIER);						
								(*it)->emitMessage(m2,this);
								//veces++;
								//std::cout<<veces<<std::endl;
								Logic::CMessage *m = new Logic::CMessage();
								m->setType(Logic::Message::CONTROL);
								m->setAction(Logic::Message::WALK_STOP);
								(*it)->emitMessage(m);
								return 1;
								}
							}
							else
							{
							//a
							}
							 //Impacto con da�o
						}
					}
				}
			}

				//Logic::TMessage m;
				//m._type = Logic::Message::CONTACT;				
				//_entity->emitMessage(m,this);				
			}		
		return 0; //le has dado al aire
		}

	void CAttack::resetAttackFlags()
	{
		_heavyAttack=false;
		_lightAttack=false;

	}
	
	void CAttack::tick(unsigned int msecs)
	{
			IComponent::tick(msecs);
			
	} //fin de CAttack:tick


} // namespace Logic