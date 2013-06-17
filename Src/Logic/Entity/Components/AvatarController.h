/**
@file AvatarController.h

Contiene la declaraci�n del componente que controla el movimiento 
de la entidad.

@see Logic::CAvatarController
@see Logic::IComponent

@author David Llans�
@date Agosto, 2010
*/

#ifndef __Logic_AvatarController_H
#define __Logic_AvatarController_H

#pragma warning(disable: 4482)

#include "Logic/Entity/Component.h"
#include "Logic/Entity/LogicalPosition.h"

namespace Logic
{
	class CMessage;
}

//declaraci�n de la clase
namespace Logic 
{
/**
	Este componente es el encargado de mover a una entidad animada. Tiene
	diferentes m�todos que permiten avances o giros. El uso de este componente
	es un poco at�pico ya que se puede registrar en otro controlador externo
	(i.e. GUI::CPlayerController) que sea el que de las �rdenes que se deben
	llevar a cabo mediante llamadas a m�todos p�blicos del componente. Puede
	no obstante ampliarse este componente para aceptar mensajes tradicionales
	con las �rdenes, sin embargo de momento as� es suficiente.
	
    @ingroup logicGroup

	@author David Llans� Garc�a
	@date Agosto, 2010
*/

	class CAvatarController : public IComponent
	{
		DEC_FACTORY(CAvatarController);
	public:

		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		CAvatarController() : IComponent(GetAltTypeIdOf(CAvatarController)), _angularSpeed(0), _turnSpeedFactor(0), _totalYaw(0),
				_targetSense(Logic::LogicalPosition::UNDEFINED), _walkingRight(false), _walkingLeft(false), _acumRotation(0), _wander(false){}
		
		/**
		Inicializaci�n del componente, utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity). Toma del mapa el atributo
		speed que indica a la velocidad a la que se mover� el jugador.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del
			fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		M�todo que activa el componente; invocado cuando se activa
		el mapa donde est� la entidad a la que pertenece el componente.
		<p>
		Si el componente pertenece a la entidad del jugador, el componente
		se registra as� mismo en el controlador del GUI para que las ordenes 
		se reciban a partir de los eventos de teclado y rat�n.

		@return true si todo ha ido correctamente.
		*/
		virtual bool activate();
		
		/**
		M�todo que desactiva el componente; invocado cuando se
		desactiva el mapa donde est� la entidad a la que pertenece el
		componente. Se invocar� siempre, independientemente de si estamos
		activados o no.
		<p>
		Si el componente pertenece a la entidad del jugador, el componente
		se deregistra as� mismo en el controlador del GUI para dejar de
		recibir las ordenes dadas a partir de los eventos de teclado y rat�n
		(ver CEntity::deactivate() )
		*/
		virtual void deactivate();

		/**
		*/
		virtual void awake();

		/**
		*/
		virtual void sleep();

		/**
		M�todo llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la entidad en cada vuelta de ciclo cuando es
		necesario (cuando est� andando o desplaz�ndose lateralmente).

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		virtual void tick(unsigned int msecs);

		/**
		M�todo virtual que elige que mensajes son aceptados. Son v�lidos
		CONTROL.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(const CMessage *message);

		/**
		M�todo virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(CMessage *message);

		/**
		Provoca que la entidad avance a la RIGHT.
		*/
		void walkRight();

		/**
		Provoca que la entidad avance a la LEFT
		*/
		void walkLeft();

		/**
		Provoca que la entidad cese el desplazamiento.
		*/
		void stopMovement();

		/**
		*/
		bool isWalkingRight() {return _walkingRight;}

		bool isWalkingLeft() {return _walkingLeft;}

		void setWander(bool wander) {_wander=wander;}

	protected:

		/**
		Atributo para saber si la entidad est� andando a la RIGHT.
		*/
		bool _walkingRight;

		/**
		Atributo para saber si la entidad est� andando a la LEFT.
		*/
		bool _walkingLeft;

		/**
		*/
		Logic::Sense _targetSense;

		/**
		*/
		float _totalYaw;

		/**
		Atributo que indica la magnitud de velocidad de la entidad.
		*/
		float _angularSpeed;

		/**
		Factor que se aplica a la velocidad de giro por defecto
		*/
		float _turnSpeedFactor;
		
		/**
		*/
		float _acumRotation;
		
		/**
		*/
		void emitAngularSpeed(Logic::Sense sense);

		/**
		*/
		void estimateRotation(Logic::Sense sense);

		/**
		*/
		void rotate(Logic::Sense sense, unsigned int msecs);
		
		/**
		Variable que establece si tiene un comportamiento de estar vagando. Lo usa cancerbero, que tiene posibilidad de andar o correr.
		En estos casos cambia la velocidad
		*/
		bool _wander;

	}; // class CAvatarController

	REG_FACTORY(CAvatarController);

} // namespace Logic

#endif // __Logic_AvatarController_H */
