/**
@file Arrow.h

Contiene la declaraci�n del componente que se utiliza para representar flechas en el m�ndo f�sico
@see Logic::Arrow

@author Jose Luis L�pez S�nchez
@date September, 2013
*/

#ifndef __Logic_Arrow_H
#define __Logic_Arrow_H


#include "Logic/Entity/Components/Physics.h"
#include "Logic/Entity/LogicalPosition.h"

namespace Physics
{
	class CActor;
	class CLife;
}

// Los componentes se definen dentro del namespace Logica
// TODO corregir comentarios
namespace Logic 
{
	/**
	Componente que se utiliza para representar jugadores y enemigos en el mundo f�sico usando 
	character controllers.
	<p>
	Este componente recibe mensajes de tipo AVATAR_MOVE indicando el movimiento que se quiere 
	realizar. A continuaci�n se le indica al motor de f�sica el movimiento deseado, y en el
	siguiente tick se actualiza la posici�n de la entidad l�gica. Debemos tener en cuenta que 
	la posici�n final la determina el motor de f�sica, ya que durante el movimiento podemos 
	colisionar con otra entidades, resbalar sobre paredes, etc.
	<p>
	Este componente tambi�n simula el efecto de la gravedad sobre el jugador (los controllers
	de PhysX no se ven afectados por la gravedad).
	
    @ingroup logicGroup

	@author Jose Luis L�pez S�nchez & ���
	@date September, 2013
	*/
	class CArrow : public CPhysics
	{
		DEC_FACTORY(CArrow);
	
	public:

		CArrow() : CPhysics(GetAltTypeIdOf(CArrow)), _diffDegrees(0), _diffHeight(0), _diffBase(0){}

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

		/**Este componente s�lo acepta mensajes de tipo AVATAR_WALK.*/
		virtual bool accept(const CMessage *message);
		
		/**
		Cuando se recibe un mensaje de tipo AVATAR_WALK, se almacena su vector de 
		desplazamiento para mover el character controller en el pr�ximo tick.
		De esta forma, si en un ciclo se reciben varios mensaje de tipo AVATAR_WALK 
		s�lo tendr� efecto el �ltimo.
		*/
		virtual void process(CMessage *message);

		/**
		Este m�todo se invoca en cada ciclo de la simulaci�n y hace lo siguiente:
		<ul>
		<li> Actualiza la posici�n de la entidad l�gica usando la informaci�n porporcionada por
		     el motor de f�sica. </li>
		<li> Mueve el character controller de acuerdo al �ltimo mensaje AVATAR_WALK recibido. </li>
		</ul>
		<p>
		Los character controllers no tienen orientaci�n, s�lo posici�n
		*/
		virtual void tick(unsigned int msecs);

		/**************
			IOBSERVER
		***************/
		//Se invoca cuando se produce una colisi�n entre una entidad f�sica y un trigger.
		virtual void onTrigger(IObserver*, bool);
		virtual void onCollision(IObserver* other);

		void DestroyArrow();
	protected:

		float _diffDegrees;
		float _diffHeight;
		char _diffBase;

	}; // class CArrow

	REG_FACTORY(CArrow);

} // namespace Logic

#endif // __Logic_Arrow_H
