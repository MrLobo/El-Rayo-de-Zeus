/**
RECUERDOS DE MERCHE
@file PlayerController.cpp

Contiene la implementaci�n de la clase CPlayerController. Se encarga de
recibir eventos del teclado y del rat�n y de interpretarlos para
mover al jugador.

@see GUI::CPlayerController

@author David Llans�
@date Agosto, 2010
*/

#include "PlayerController.h"
#include "InputManager.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Messages/Message.h"
#include "Logic/Entity/Messages/MessageFloat.h"

#include <cassert>

#define TURN_FACTOR 0.001f

namespace GUI {

	CPlayerController::CPlayerController() : _controlledAvatar(0)
	{

	} // CPlayerController

	//--------------------------------------------------------

	CPlayerController::~CPlayerController()
	{
		deactivate();

	} // ~CPlayerController

	//--------------------------------------------------------

	void CPlayerController::activate()
	{		
		CInputManager::getSingletonPtr()->addKeyListener(this);
	//	CInputManager::getSingletonPtr()->addMouseListener(this);

	} // activate

	//--------------------------------------------------------

	void CPlayerController::deactivate()
	{
		CInputManager::getSingletonPtr()->removeKeyListener(this);
	//	CInputManager::getSingletonPtr()->removeMouseListener(this);

	} // deactivate

	//--------------------------------------------------------

	bool CPlayerController::keyPressed(TKey key)
	{
		// TODO Preguntar al CServer qui�n es el player si es null y dejarlo guardado
		if(_controlledAvatar)
		{
			Logic::CMessage *m = new Logic::CMessage();
				m->setType(Logic::Message::CONTROL);
			Logic::CMessageFloat *m2 = new Logic::CMessageFloat();
				m2->setType(Logic::Message::CONTROL);
			switch(key.keyId)
			{
			case GUI::Key::W: //subir anillo superior
				m->setAction(Logic::Message::GO_UP); // Pablo
				_controlledAvatar->emitMessage(m);
				break;
			
			case GUI::Key::S: //bajar anillo inferior
				m->setAction(Logic::Message::GO_DOWN); 
				_controlledAvatar->emitMessage(m);
				break;
			case GUI::Key::SPACE:
				m->setAction(Logic::Message::JUMP);  // Pablo
				_controlledAvatar->emitMessage(m);
				break;
			case GUI::Key::A:
				m->setAction(Logic::Message::WALK_LEFT);
				_controlledAvatar->emitMessage(m);
				break;
			case GUI::Key::D:
				m->setAction(Logic::Message::WALK_RIGHT);
				_controlledAvatar->emitMessage(m);
				break;
			case GUI::Key::NUMBER1:
				m2->setAction(Logic::Message::CHANGE_BASE);
				m2->setFloat(1);
				_controlledAvatar->emitMessage(m2);
				break;
			case GUI::Key::NUMBER2:
				m2->setAction(Logic::Message::CHANGE_BASE);
				m2->setFloat(2);
				_controlledAvatar->emitMessage(m2);
				break;			
			case GUI::Key::NUMBER3:
				m2->setAction(Logic::Message::CHANGE_BASE);
				m2->setFloat(3);
				_controlledAvatar->emitMessage(m2);
				break;
			case GUI::Key::NUMBER4:
				m2->setAction(Logic::Message::CHANGE_BASE);
				m2->setFloat(4);
				_controlledAvatar->emitMessage(m2);
				break;			
			case GUI::Key::NUMBER5:
				m2->setAction(Logic::Message::CHANGE_BASE);
				m2->setFloat(5);
				_controlledAvatar->emitMessage(m2);
				break;
			case GUI::Key::NUMBER6:
				m2->setAction(Logic::Message::CHANGE_BASE);
				m2->setFloat(6);
				_controlledAvatar->emitMessage(m2);
				break;			
			case GUI::Key::NUMBER7:
				m2->setAction(Logic::Message::CHANGE_BASE);
				m2->setFloat(7);
				_controlledAvatar->emitMessage(m2);
				break;
			case GUI::Key::NUMBER8:
				m2->setAction(Logic::Message::CHANGE_BASE);
				m2->setFloat(8);
				_controlledAvatar->emitMessage(m2);
				break;
			case GUI::Key::NUMBER0:
				m2->setAction(Logic::Message::CHANGE_BASE);
				m2->setFloat(0);
				_controlledAvatar->emitMessage(m2);
				break;
			case GUI::Key::E:
				//m._string = "specialAction";
				break;
			default:
				return false;
			}
			
			return true;
		}
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CPlayerController::keyReleased(TKey key)
	{
		if(_controlledAvatar)
		{
			Logic::CMessage *m = new Logic::CMessage();
			m->setType(Logic::Message::CONTROL);
			switch(key.keyId)
			{
			case GUI::Key::A:
			case GUI::Key::D:
				//m._string = "stopStrafe";
				m->setAction(Logic::Message::WALK_STOP);
				break;

			default:
				return false;
			}
			_controlledAvatar->emitMessage(m);
			return true;
		}
		return false;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CPlayerController::mouseMoved(const CMouseState &mouseState)
	{
		if(_controlledAvatar)
		{
			Logic::CMessageFloat *m = new Logic::CMessageFloat();
			m->setType(Logic::Message::CONTROL);
			m->setAction(Logic::Message::TURN);
			m->setFloat(-(float)mouseState.movX * TURN_FACTOR);
			_controlledAvatar->emitMessage(m);
			return true;
		}
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CPlayerController::mousePressed(const CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------

	bool CPlayerController::mouseReleased(const CMouseState &mouseState)
	{
		return false;

	} // mouseReleased

} // namespace GUI
