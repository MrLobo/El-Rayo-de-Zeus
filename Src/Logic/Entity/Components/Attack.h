/**
@file Attack.h

Contiene la declaraci�n del componente que controla el movimiento 
angular de entidades.

@see Logic::CAttack
@see Logic::IComponent

@author Jos� Luis L�pez
@date Febrero, 2013
*/
#ifndef __Logic_Attack_H
#define __Logic_Attack_H

#include "Logic/Entity/Component.h"
#include "BaseSubsystems/Math.h"
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
	class CAttack : public IComponent
	{
		DEC_FACTORY(CAttack);
	public:

		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		CAttack() : IComponent(GetAltTypeIdOf(CAttack)),_attackPower(0),_lightAttack(false),_heavyAttack(false){}

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
		bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		M�todo que activa el componente; invocado cuando se activa
		el mapa donde est� la entidad a la que pertenece el componente.
		<p>
		Si el componente pertenece a la entidad del jugador, el componente
		se registra as� mismo en el controlador del GUI para que las ordenes 
		se reciban a partir de los eventos de teclado y rat�n.

		@return true si todo ha ido correctamente.
		*/
		bool activate();
		
		/**
		M�todo que desactiva el componente; invocado cuando se
		desactiva el mapa donde est� la entidad a la que pertenece el
		componente. Se invocar� siempre, independientemente de si estamos
		activados o no.
		<p>
		Si el componente pertenece a la entidad del jugador, el componente
		se deregistra as� mismo en el controlador del GUI para dejar de
		recibir las ordenes dadas a partir de los eventos de teclado y rat�n.
		*/
		void deactivate();

		/**
		M�todo llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la entidad en cada vuelta de ciclo cuando es
		necesario (cuando est� andando o desplaz�ndose lateralmente).

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		void tick(unsigned int msecs);

		/**
		M�todo virtual que elige que mensajes son aceptados. Son v�lidos
		CONTROL.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		bool accept(const CMessage *message);

		/**
		M�todo virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		void process(CMessage *message);

		/**
		Provoca que la entidad ataque con su ataque ligero.
		*/
		void lightAttack();
	
		
		/**
		Provoca que la entidad ataque con su ataque ligero.
		*/
		void heavyAttack();
	
			//este metodo devuelve null si no se est� ocupando ese grado o la entidad que ocupa ese espacio
		bool attackPlace(float grado, short base, short ring,bool soloInfo);

		void resetAttackFlags();
	
	protected:

		float _attackPower;
		
		bool _lightAttack;
		
		bool _heavyAttack;

	}; // class CAttack

	REG_FACTORY(CAttack);

} // namespace Logic

#endif // __Logic_Attack_H
