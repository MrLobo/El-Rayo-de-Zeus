//---------------------------------------------------------------------------
// ServerENET.h
//---------------------------------------------------------------------------

/**
 * @file ServerENET.h
 *
 * Implementa un servidor de red con ENet
 *
 *
 * @author Juan A. Recio-Garc�a
 * @date Junio, 2006
 */

#ifndef __Net_ServerENET_H
#define __Net_ServerENET_H


#include "Server.h"

#include "Enet/enet.h"

namespace Net {

/**
 * Servidor de red implementado mediante la librer�a eNet
 */
class CServerENET: public CServer {

public:

	/**
	 * Constructor
	 */ 
	CServerENET();

	/**
	 * Destructor
	 */
	virtual ~CServerENET();

	/**
	 * Inicializa el servidor escuchando en un puerto determinado.
	 * @param port es el puerto donde se escucha
	 * @param clients es el n�mero m�ximo de clientes. El m�ximo ser� establecido por la implementaci�n.
	 * @param maxinbw Ancho de banda de entrada m�ximo en bytes/segundo (0 = ilimitado)
	 * @param maxoutbw Ancho de banda de salida m�ximo en bytes/segundo (0 = ilimitado)
	 */
	bool init(int port, int clients, unsigned int maxinbw = 0, unsigned int maxoutbw = 0);
	
	/**
	 * Refresca el servidor obteniendo todos los packets recibidos despu�s de la �ltima llamada a este m�todo
	 * IMPORTANTE: Es responsabilidad del invocador de service() borrar los packets recibidos
	 * @param packetsRecibidos Vector de CPacket* donde se almacenan los packets recibidos
	 */
	void service(std::vector<CPacket*>& packetsRecibidos);

	/**
	 * Libera los recursos ocupados por este cliente.
	 * Es responsabilidad de este m�todo desconectar todas las connectiones en este punto.
	 */ 
	void deInit();
	
	/**
	 * Devuelve una lista de las connectiones creadas 
	 */
	std::vector<CConnection*>::iterator listConnections();

	/**
	 * Env�a datos a trav�s de una conexi�n.
	 * @param connection por la que enviar los datos
	 * @param data son los datos a enviar
	 * @param longData tama�o de los datos a enviar
	 * @param channel canal l�gico por el que se enviar�n los datos
	 * @param reliable indica si el cliente debe comprobar que los datos han sido recibidos por la m�quina remota
	 */
	void sendData(void* data, size_t longData, int channel, bool reliable, CConnection* connection);

	/**
	 * Env�a el mismo dato a todos los clientes conectados
	 * @param data son los datos a enviar
	 * @param longData tama�o de los datos a enviar
	 * @param channel canal l�gico por el que se enviar�n los datos
	 * @param reliable indica si el cliente debe comprobar que los datos han sido recibidos por la m�quina remota
	 */
	void sendAll(void* data, size_t longData, int channel, bool reliable, CConnection* exception = 0);

	/**
	 * Desconecta la conexi�n indicada.
	 */
	void disconnect(CConnection * connection);

	/**
	 * Indica si el cliente tiene alguna conexi�n activa.
	 */
	bool isConnected();

	/**
	 * Indica si el cliente est� inicializado
	 */
	bool isInitialized();

private:

	enum EstadoServidorENet{
		NO_INIT,
		INIT_NOT_CONNECTED,
		INIT_AND_CONNECTED
	};

	void disconnectAll();

	void disconnectReceived(CConnection* connection);
	
	EstadoServidorENet estado;

	std::vector<CConnection*> _connectionsList;

	ENetHost* server;
};

} // namespace Net

#endif // __Net_ServerENET_H
