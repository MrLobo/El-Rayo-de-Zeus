//---------------------------------------------------------------------------
// conexionENet.h
//---------------------------------------------------------------------------

/**
 * @file conexionENet.h
 *
 * Representa una conexi�n con un peer implementada con ENet.
 *
 *
 * @author Juan A. Recio-Garc�a
 * @date Octubre, 2006
 */


#include "conexionENet.h"

namespace Net {

	void CConexionENet::setENetPeer(ENetPeer* p)
	{
		_peer = p;
	};

	ENetPeer* CConexionENet::getENetPeer()
	{
		return _peer;
	};

	void CConexionENet::setId(const std::string& id)
	{
		_id = id;
	}

	std::string CConexionENet::getId()
	{
		return _id;
	}

	int CConexionENet::getAddress()
	{
		return _peer->address.host;
	}

	short CConexionENet::getPort()
	{
		return _peer->address.port;
	}

} //namespace