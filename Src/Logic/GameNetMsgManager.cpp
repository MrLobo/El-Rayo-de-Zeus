//---------------------------------------------------------------------------
// GameNetMsgManager.cpp
//---------------------------------------------------------------------------

/**
@file GameNetMsgManager.cpp

Contiene la implementaci�n del gestor de los mensajes de red durante la partida.

@see Logic::Manager::IObserver
@see Logic::CGameNetMsgManager

@author David Llans�
@date Febrero, 2011
*/

#include "GameNetMsgManager.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Message.h"
#include "Logic/Maps/Map.h"
#include "Server.h"

#include "Net/paquete.h"
#include "Net/buffer.h"

#include "Application/BaseApplication.h"


namespace Logic {

	CGameNetMsgManager* CGameNetMsgManager::_instance = 0;

	//--------------------------------------------------------

	CGameNetMsgManager::CGameNetMsgManager()
	{
		_instance = this;

	} // CServer

	//--------------------------------------------------------

	CGameNetMsgManager::~CGameNetMsgManager()
	{
		_instance = 0;

	} // ~CServer
	
	//--------------------------------------------------------

	bool CGameNetMsgManager::Init()
	{
		assert(!_instance && "Segunda inicializaci�n de Logic::CGameNetMsgManager no permitida!");

		new CGameNetMsgManager();

		return true;

	} // Init

	//--------------------------------------------------------

	void CGameNetMsgManager::Release()
	{
		assert(_instance && "Logic::CGameNetMsgManager no est� inicializado!");

		if(_instance)
			delete _instance;

	} // Release

	//--------------------------------------------------------

	void CGameNetMsgManager::activate() 
	{
		// TODO Escuchamos los mensajes de red. Engancharnos a Net::CManager
		Net::CManager::getSingletonPtr()->addObserver(this);
	} // activate

	//--------------------------------------------------------

	void CGameNetMsgManager::deactivate() 
	{	
		// TODO Dejamos de escuchar los mensajes de red. 
		// Desengancharnos de Net::CManager
		Net::CManager::getSingletonPtr()->removeObserver(this);
	} // deactivate

	//---------------------------------------------------------
		
	void CGameNetMsgManager::sendEntityMessage(const TMessage &txMsg, TEntityID destID)
	{
		// TODO serializar el mensaje l�gico. Para la serializaci�n
		// apoyarse en Net::CBuffer. Se debe guardar primero el tipo
		// de mensaje de red (Net::ENTITY_MSG) y luego el id de la
		// entidad y el mensaje serializado. La funci�n de serializaci�n
		// de un mensaje l�gico est� en Message. Al final se debe hacer
		// el env�o usando el gestor de red.
		Net::NetMessageType msgType = Net::NetMessageType::ENTITY_MSG;

		Net::CBuffer serialMsg;		
			serialMsg.write(&msgType, sizeof(msgType));
			serialMsg.write(&destID, sizeof(destID));
			Message::Serialize(txMsg, serialMsg);

		Net::CManager::getSingletonPtr()->send(serialMsg.getbuffer(), serialMsg.getSize());

//#if _DEBUG
//		fprintf (stdout, "NET::TX>> ENTITY_MSG %d to EntityID %d.\n", txMsg._type, destID);
//#endif
	} // sendEntityMessage

	//---------------------------------------------------------
		
	void CGameNetMsgManager::processEntityMessage(Net::CPaquete* packet)
	{
		// TODO M�todo que debe de ser invocado desde el m�todo que
		// recibe todos los paquetes de red cuando el tipo de mensaje
		// de red es Net::ENTITY_MSG. Se debe sacar el ID de la entidad,
		// recuperarla, deserializar el mensaje y envi�rselo

		// Creamos un buffer con los datos para leer de manera m�s c�moda
		Net::CBuffer serialMsg;
			serialMsg.write(packet->getData(),packet->getDataLength());
			serialMsg.reset();

		// Ignoramos el tipo de mensaje de red. Ya lo hemos procesado		
		Net::NetMessageType msgType;
			serialMsg.read(&msgType,sizeof(msgType));
		TEntityID destID;
			serialMsg.read(&destID, sizeof(destID));
		TMessage rxMsg;
			Message::Deserialize(serialMsg, rxMsg);

		Logic::CServer::getSingletonPtr()->getMap()-> // Reenv�o del mensaje deserializado
			getEntityByID(destID)->emitMessage(rxMsg);

//#if _DEBUG
//		fprintf (stdout, "NET::RX>> ENTITY_MSG %d from EntityID %d.\n", rxMsg._type, destID);
//#endif
	} // processEntityMessage


	/*******************
		NET: IObserver
	*********************/
	// Aqu� es donde debemos recibir los mensajes de red
	void CGameNetMsgManager::dataPacketReceived(Net::CPaquete* packet)
	{
		Net::NetMessageType rxMsg;
			memcpy(&rxMsg, packet->getData(),sizeof(rxMsg));
			switch (rxMsg)
			{

			case Net::NetMessageType::ENTITY_MSG:	
				processEntityMessage(packet);
				break;	

			case Net::NetMessageType::END_GAME:	
				Application::CBaseApplication::getSingletonPtr()->setState("gameOver");
				break;
			}
	} // dataPacketReceived

	//--------------------------------------------------------

} // namespace Logic
