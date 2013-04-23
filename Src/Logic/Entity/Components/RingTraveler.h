/**
@file RingTraveler.h

Contiene la declaraci�n del componente que controla la capacidad de cambiar de anillo
de los elementos del juego

@see Logic::CRingTraveler
@see Logic::IComponent

@author Jose Luis L�pez S�nchez
@date Febrero, 2013
*/
#ifndef __Logic_RingTraveler_H
#define __Logic_RingTraveler_H

#include "Logic/Entity/Component.h"

namespace Logic
{
	class CMessage;
}

namespace Graphics
{
	class CScene;
}

//declaraci�n de la clase
namespace Logic 
{
/**
	Componente que se encarga de los cambios de anillo. Con este componente no puede cambiar de Base.
	
    @ingroup logicGroup

	@author Jose Luis L�pez S�nchez
	@date Febrero, 2013
*/
	class CRingTraveler : public IComponent
	{
		DEC_FACTORY(CRingTraveler);
	public:

		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		CRingTraveler() : IComponent(GetAltTypeIdOf(CRingTraveler)),_changingRing(false),_changingRingTime(0),_toUp(1),_desencogiendo(false) {}

		CRingTraveler(altTypeId id) : IComponent(id),_changingRing(false),_changingRingTime(0)
		{}

		/**
		Destructor (virtual); Quita de la escena y destruye la entidad gr�fica.
		*/
		virtual ~CRingTraveler();
		
		
		virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

		/**
		M�todo virtual que elige que mensajes son aceptados. Son v�lidos
		CHANGE_PLANE.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(const CMessage *message);

		/**
		M�todo virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(CMessage *message);

		 bool activate();
		
		///** UNDONE ���
		//Provoca que la entidad baje de anillo. Conlleva un cambio del eje de giro en su coordenada y
		//*/
		//void goDown();

		///**
		//Provoca que la entidad suba de anillo. Conlleva un cambio del eje de giro en su coordenada y
		//*/
		//void goUp();

		/**
		M�todo llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la entidad en cada vuelta de ciclo cuando es
		necesario (cuando est� andando o desplaz�ndose lateralmente).

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		void tick(unsigned int msecs);

		void resetChangingRing(){_changingRing=false;}

		bool isChangingRing(){return _changingRing;}

		/**
		M�todo que ser� invocado siempre que se termine una el reloj 
		llegue al evento solicitado desde la m�quina de estados.		
		*/
		void timeArrived();

	protected:

		bool _changingRing;	

		char _toUp;

		float _changingRingTime;

		bool _desencogiendo;


	}; // class CRingTraveler

	REG_FACTORY(CRingTraveler);

} // namespace Logic

#endif // __Logic_CRingTraveler_H
