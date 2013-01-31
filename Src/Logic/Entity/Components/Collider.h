/**
@file Collider.h

Contiene la declaraci�n del componente que controla la capacidad de colisionar 
de la entidad.

@see Logic::CCollider
@see Logic::IComponent

@author Jose Luis L�pez S�nchez
@date Diciembre, 2012
*/
#pragma once
#ifndef __Logic_Collider_H
#define __Logic_Collider_H

#include "Logic/Entity/Component.h"

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

	@author Jose Luis L�pez S�nchez
	@date Diciembre, 2012
*/
	class CCollider : public IComponent
	{
		DEC_FACTORY(CCollider);
	public:

		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		CCollider() : IComponent(),_sentidoColision(false),_hit(0) {}
		
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
		recibir las ordenes dadas a partir de los eventos de teclado y rat�n.
		*/
		virtual void deactivate();

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
		virtual bool accept(const TMessage &message);

		/**
		M�todo virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(const TMessage &message);
		
		/**
		M�todo virtual que define el comportamiento en caso de colisi�n.

		@return devuelve la direccion a la que tiene el obstaculo
		*/
		virtual bool contacto( CEntity* entidad1, CEntity* entidad2);

		/**
		M�todo virtual que determina en una colisi�n quien est� a la izquierda y quien a la derecha
		*/
		virtual bool sentidoColision(const CEntity* entidad1,const CEntity* entidad2);

		virtual bool contactoAngular( CEntity* entidad, CEntity* entidad2);

		virtual	bool CCollider::contactoExtremo( CEntity* entidad1, CEntity* entidad2);

	protected:
		/**
			false si en una colisi�n, est� a la izquierda
		*/
		bool _sentidoColision;
		
		/**
			false si en una colisi�n, est� a la izquierda
		*/
		short _hit;

		CEntity* _excluido;
		
	}; // class CCollider

	REG_FACTORY(CCollider);

} // namespace Logic

#endif // __Logic_Collider_H
