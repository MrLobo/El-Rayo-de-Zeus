//---------------------------------------------------------------------------
// connection.h
//---------------------------------------------------------------------------

/**
 * @file connection.h
 *
 * Representa una conexi�n con un peer.
 *
 *
 * @author Juan A. Recio-Garc�a
 * @date Octubre, 2006
 */

#ifndef __Net_Connection_H
#define __Net_Connection_H

#include <string>

namespace Net {

	/** 
	ID de identificaci�n en la red.
	*/
	typedef unsigned int NetID;

class CConnection {

public:
	virtual int getAddress()=0;
	virtual short getPort()=0;
	virtual void setId(NetID id)=0;
	virtual NetID getId()=0;
};


} // namespace Net

#endif // __Net_Connection_H