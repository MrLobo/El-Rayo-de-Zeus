#include "RouteTo.h"

#include "Map/MapEntity.h"
#include "Logic/Entity/Entity.h"

#include "Logic/Maps/Map.h"

#include "AI/Server.h"
#include "AI/Movement.h"

namespace Logic 
{

	//---------------------------------------------------------

	IMP_FACTORY(CRouteTo);

	//---------------------------------------------------------
	/**
	Inicializaci�n del componente, utilizando la informaci�n extra�da de
	la entidad le�da del mapa (Maps::CEntity). Toma del mapa el atributo
	speed que indica a la velocidad m�xima a la que se mover� la entidad.

	@param entity Entidad a la que pertenece el componente.
	@param map Mapa L�gico en el que se registrar� el objeto.
	@param entityInfo Informaci�n de construcci�n del objeto le�do del
	fichero de disco.
	@return Cierto si la inicializaci�n ha sido satisfactoria.
	*/
	bool CRouteTo::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;
		if(entityInfo->hasAttribute("arriveTolerance"))
			_arriveTolerance = entityInfo->getFloatAttribute("arriveTolerance");
		if(entityInfo->hasAttribute("seekTolerance"))
			_seekTolerance = entityInfo->getFloatAttribute("seekTolerance");
		return true;

	} // spawn

	//---------------------------------------------------------
	/**
	M�todo llamado en cada frame que actualiza el estado del componente.
	<p>
	Se encarga de mover la entidad en cada vuelta de ciclo cuando es necesario.

	@param msecs Milisegundos transcurridos desde el �ltimo tick.
	*/
	void CRouteTo::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		// TODO PR�CTICA IA
		// En este m�todo se gestiona el seguimiento de la ruta.
		// Realmente no se ocupa de mover la entidad, sino de controlar cu�ndo se ha 
		// llegado a un waypoint y notificar a SteeringMovement (que es quien mueve) 
		// del siguiente punto al que moverse y el tipo de movimiento a hacer.
		// El componente almacena su estado en las variables:
		// _target: destino final
		// _currentRoute: ruta actual (vector de posiciones)
		// _currentNode: posici�n de destino actual (posici�n en el vector)
		// Lo primero que hay que hacer es recalcular la ruta si es necesario.
		// A continuaci�n hay que comprobar si la entidad ha llegado al siguiente 
		// punto de ruta. Si es as�, pasamos al siguiente y hacemos las comprobaciones
		// necesarias para saber si tenemos que parar o no.

		if (_recalculateRoute) {
			if (_currentRoute != 0) delete _currentRoute;
			_currentRoute = AI::CServer::getSingletonPtr()->getAStarRoute(_entity->getPosition(), _target);
			_recalculateRoute = false;

			// Si no se puede calcular la ruta enviamos un mensaje de fallo
			if (_currentRoute == 0) {
				_arrived = true;
				TMessage message;
				message._type = Message::FAILED_ROUTE;
				_entity->emitMessage(message, this);
			} else {
				_currentNode = 0;
				_arrived = false;
				// Si hay ruta hacemos que vaya al primer punto
				//sendMoveMessage((*_currentRoute)[0], AI::IMovement::MOVEMENT_KINEMATIC_SEEK);
				sendMoveMessage((*_currentRoute)[0], AI::IMovement::MOVEMENT_DYNAMIC_SEEK);
				_distanceTolerance = _seekTolerance;
			}
		}

		if (!_arrived) {
			// Si no hemos llegado al destino
			Vector3 currentTarget = (*_currentRoute)[_currentNode];
			// Si hemos llegado al siguiente punto
			if (_entity->getPosition().positionEquals(currentTarget, _distanceTolerance)) {
				// Pasamos al siguiente nodo
				_currentNode++;
				if (_currentNode >= _currentRoute->size()) {
					// Era el �ltimo nodo ==> parar
					_arrived = true;
					sendMoveMessage(_target, AI::IMovement::MOVEMENT_NONE);
					// Enviar un mensaje para notificar que hemos llegado la destino
					TMessage message;
					message._type = Message::FINISHED_ROUTE;
					_entity->emitMessage(message, this);
				} else if (_currentNode == _currentRoute->size() - 1) {
					// Es el pen�ltimo nodo. Nos acercamos con Arrive
					//sendMoveMessage((*_currentRoute)[_currentNode], AI::IMovement::MOVEMENT_KINEMATIC_ARRIVE);
					sendMoveMessage((*_currentRoute)[_currentNode], AI::IMovement::MOVEMENT_DYNAMIC_ARRIVE);
					_distanceTolerance = _arriveTolerance;
				} else {
					// Nos movemos al siguiente
					//sendMoveMessage((*_currentRoute)[_currentNode], AI::IMovement::MOVEMENT_KINEMATIC_SEEK);
					sendMoveMessage((*_currentRoute)[_currentNode], AI::IMovement::MOVEMENT_DYNAMIC_SEEK);
					_distanceTolerance = _seekTolerance;
				}
			}
		}
	} // tick

	//---------------------------------------------------------
	/** 
	Env�a un mensaje MOVE_TO.

	@param target Destino.
	@movementType Tipo de movimiento.
	*/
	void CRouteTo::sendMoveMessage(Vector3 target, int movementType)
	{
		TMessage message;
		message._type = Message::MOVE_TO;
		message._vector3 = target;
		message._vector3.y = 0.0;
		message._int = movementType;
		_entity->emitMessage(message, this);
	}

	//---------------------------------------------------------
	/**
	Este componente s�lo acepta mensajes de tipo ROUTE_TO.
	*/
	bool CRouteTo::accept(const TMessage &message)
	{
		return message._type == Message::ROUTE_TO;
	} // accept

	//---------------------------------------------------------
	/**
	Si se recibe un mensaje de tipo ROUTE_TO, almacena el vector como 
	destino del movimiento.
	*/
	void CRouteTo::process(const TMessage &message)
	{
		switch(message._type)
		{
		case Message::ROUTE_TO:
			// Comprobamos el _bool para saber si tenemos que calcular la ruta o detenernos
			if (message._bool) {
				// Anotamos el vector de desplazamiento para usarlo posteriormente en 
				// el m�todo tick. De esa forma, si recibimos varios mensajes AVATAR_MOVE
				// en el mismo ciclo s�lo tendremos en cuenta el �ltimo.
				_target = message._vector3;
				_recalculateRoute = true;
			} else {
				// Dejamos de avanzar por la ruta
				_recalculateRoute = false;
				_arrived = true;
				// Eliminamos la ruta
				if (_currentRoute != 0) {
					delete _currentRoute;
					_currentRoute = 0;
				}
				// Y enviamos un mensaje para parar
				sendMoveMessage(_target, AI::IMovement::MOVEMENT_NONE);
			}
			break;
		}
	} // process

} //namespace Logic 