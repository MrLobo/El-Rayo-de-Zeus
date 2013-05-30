//---------------------------------------------------------------------------
// cliente.h
//---------------------------------------------------------------------------

/**
 * @file cliente.h
 *
 * Especifica un cliente de red
 *
 *
 * @author Juan A. Recio-Garc�a
 * @date Junio, 2006
 */

#ifndef __Net_Client_H
#define __Net_Client_H

#include <vector>

#include "Connection.h"
#include "packet.h"


namespace Net {

/**
 * Especifica un cliente de red.
 * Esta interfaz podr� ser implementada de distintas maneras o con distintas librer�as. 
 * Con ella se pretende representar el comportamiento de un cliente de red gen�rico 
 * e independiente de la implementaci�n.
 */
class CClient {

public:

	/**
	 * Destructor virtual.
	 */
	virtual ~CClient(){}

	/**
	 * Inicializa el cliente de red.
	 * @param maxConnections M�ximo de connectiones permitidas
	 * @param maxinbw Ancho de banda de entrada m�ximo en bytes/segundo (0 = ilimitado)
	 * @param maxoutbw Ancho de banda de salida m�ximo en bytes/segundo (0 = ilimitado)
	 * @param true si todo ok
	 */
	virtual bool init(unsigned int maxConnections = 1, unsigned int maxinbw = 0, unsigned int maxoutbw = 0)=0;

	/**
	 * Devuelve la conexi�n (peer) creada para la direcci�n y puerto indicados
	 * @param address Direcci�n IP o nombre de la m�quina remota a conectar
	 * @param puerto Puerto remoto al que conectar. (El puerto local utilizado no se especifica en este m�tdo ya que normalmente es asignado por el S.O.)
     * @param channels N�mero de canales l�gicos a crear sobre la conexi�n
	 * @param timeout Tiempo m�ximo de espera para conectar con la m�quina remota.
	 * @return La conexi�n creada. Devuelve NULL si la conexi�n no ha podido ser establecida o si no se llam� a init() anteriormente.
	 */
	virtual CConnection* connect(char* address, int port, int channels, unsigned int timeout = 5000)=0;

	/**
	 * Devuelve una lista de las connectiones creadas 
	 */
	virtual std::vector<CConnection*>::iterator listConnections()=0;

	/**
	 * Env�a datos a trav�s de una conexi�n.
	 * @param connection por la que enviar los datos
	 * @param data son los datos a enviar
	 * @param longData tama�o de los datos a enviar
	 * @param channel canal l�gico por el que se enviar�n los datos
	 * @param reliable indica si el cliente debe comprobar que los datos han sido recibidos por la m�quina remota
	 */
	virtual void sendData(void* data, int longData, int channel, bool reliable, CConnection* connection)=0;
	
	/**
	 * Refresca el cliente obteniendo todos los packets recibidos despu�s de la �ltima llamada a este m�todo
	 * IMPORTANTE: Es responsabilidad del invocador de service() borrar los packets recibidos
	 * @param packetsRecibidos Vector de CPacket* donde se almacenan los packets recibidos
	 */
	virtual void service(std::vector<CPacket*>& packetsRecibidos)=0;

	/**
	 * Obtiene el siguiente packet recibido
	 * IMPORTANTE: Es responsabilidad del invocador de readPacket() borrar el packet recibido
	 * @return �ltimo packet recibido
	 */
	virtual CPacket* readPacket() = 0;

	/**
	 * Desconecta la conexi�n indicada.
	 */
	virtual void disconnect(CConnection * connection)=0;

	/**
	 * Libera los recursos ocupados por este cliente.
	 * Es responsabilidad de este m�todo desconectar todas las connectiones en este punto.
	 */ 
	virtual void deInit()=0;

	/**
	 * Indica si el cliente tiene alguna conexi�n activa.
	 */
	virtual bool isConnected()=0;

	/**
	 * Indica si el cliente est� inicializado
	 */
	virtual bool isInitialized()=0;

};

} // namespace Net

#endif // __CLIENTE_H
