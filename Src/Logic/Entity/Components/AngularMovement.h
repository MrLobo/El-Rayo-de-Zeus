/**
@file AngularMovement.h

Contiene la declaraci�n del componente que controla el movimiento 
angular de entidades.

@see Logic::CAngularMovement
@see Logic::IComponent

@author Jos� Luis L�pez
@date Diciembre, 2012
*/
#ifndef __Logic_AngularMovement_H
#define __Logic_AngularMovement_H

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
	class CAngularMovement : public IComponent
	{
		DEC_FACTORY(CAngularMovement);
	public:

		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		// Pablo. A�adido la inicializaci�n de _jumping y _timeJumping
		CAngularMovement() : IComponent(), _sentidoColision(false),_changingRing(false),_changingBase(false), _walkBack(false), _walkingRight(false), _walkingLeft(false), _goingUp(false), _goingDown(false),
			_angularSpeed(0.00625f), _actualRadius(-55), _actualDegree(-90), _jumpingDown(false), _initialJump(false), _timeJumping(0), _potenciaSaltoInicial(5)/*5*/, _correccionGrados(0){}
		// divididio entre 32
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
		bool accept(CMessage *message);

		/**
		M�todo virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		void process(CMessage *message);

			/**
		Provoca que la entidad retroceda al chocarse con otra entidad en lugar de simplemente pararse, 
		as� ya no est� colisionando y se puede mover.
		*/	
		void walkBack();

		/**
		Provoca que la entidad avance a la derecha.
		*/
		void walkRight();

		/**
		Provoca que la entidad avance a la izquierda
		*/
		void walkLeft();

		//Pablo
		/**
		Provoca que la entidad salte.
		*/
		void jump();
		
		/**
		Provoca que la entidad baje de anillo. Conlleva un cambio del eje de giro en su coordenada y
		*/
		void goDown();

		/**
		Provoca que la entidad suba de anillo. Conlleva un cambio del eje de giro en su coordenada y
		*/
		void goUp();

		/**
		Provoca que la entidad cese el desplazamiento.
		*/
		void stopMovement();
		
		/**
		Provoca que la entidad gire. N�meros Positivos para	giro a 
		derechas, negativos para giro izquierdas.

		@param amount Cantidad de giro. Positivos giro a derechas,
		negativos a izquierdas.
		*/
		void turn(float amount);

		/**
		Provoca que la entidad cambie de direcci�n.
		@param amount Cantidad de giro. Positivos giro a derechas,
		negativos a izquierdas.
		*/
		void changeDirection(const bool newDirection);

				
		void changeBase(int base);



	
	protected:

		/**
		Atributo para hacer que retroceda ante una colisi�n.
		*/
		bool _walkBack;

		/**
		Atributo para saber si la entidad est� avanzando a la derecha.
		*/
		bool _walkingRight;

		/**
		Atributo para saber si la entidad est� avanzando a la izquierda
		*/
		bool _walkingLeft;


		/**
		Atributo para saber si la entidad est� bajando en el salto dentro de un anillo
		*/
		bool _jumpingDown;

		/**
			Atributo para saber si la entidad est� bajando de anillo
		*/
		bool _goingUp;
		/**
		Atributo para saber si la entidad est� subiendo de anillo
		*/
		bool _goingDown;

		/**
		Atributo que indica la magnitud de velocidad de la entidad.
		*/
		float _angularSpeed;


		float _actualRadius;

		float _actualDegree;

		float _correccionGrados;

		bool _sentidoDerecha;

		bool _sentidoColision;

		// Pablo. Atributo que indica la velocidad de salto de la entidad
		float jumpSpeed;

		// Pablo. Atributo que indica que acaba de empezar a saltar
		bool _initialJump;

		float _timeJumping;

		// Pablo. Atributo que indica la velocidad de la fuerza de gravedad
		float _gravity;

		//Pablo. Altura maxima del salto
		float _Hmax;

		//Pablo. Tiempo m�ximo en el aire
		float _Tmax;

		//Pablo. Tiempo hasta alcanzar la m�xima altura
		float _Tmaxaltura;

		//Pablo. Y inicial en el momento de saltar
		float _inicialY;

		//Pablo. posicion Vector3 (x,y,z) en el momento de saltar
		Vector3 _posicionSalto;

		bool _changingRing;

		bool _changingBase;
	
		float _potenciaSalto;

		float _potenciaSaltoInicial;


	}; // class CAngularMovement

	REG_FACTORY(CAngularMovement);

} // namespace Logic

#endif // __Logic_AngularMovement_H
