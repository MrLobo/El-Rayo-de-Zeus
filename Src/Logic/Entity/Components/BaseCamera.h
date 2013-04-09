/**
@file Camera.h

Contiene la declaraci�n del componente que controla la c�mara gr�fica
de una escena.

@see Logic::CCamera
@see Logic::IComponent

@author David Llans�
@date Septiembre, 2010
*/
#ifndef __Logic_BaseCamera_H
#define __Logic_BaseCamera_H

#include "Logic/Entity/Component.h"
#include "Logic/Entity/LogicalPosition.h"
// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Graphics 
{
	class CCamera;
	class CScene;
}

namespace Logic
{
	class CMessage;
}
//declaraci�n de la clase
namespace Logic 
{
/**
*/
	class CBaseCamera : public IComponent
	{
		DEC_FACTORY(CBaseCamera);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CBaseCamera() : IComponent(GetAltTypeIdOf(CBaseCamera)), _graphicsCamera(0), _currentPos(Vector3::ZERO), _degree(0), _radius(0), 
			_height(0), _angularSpeed(0) {}
		
		/**
		Inicializaci�n del componente, utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity). Se accede a los atributos 
		necesarios como la c�mara gr�fica y se leen atributos del mapa.

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
		Se coge el jugador del mapa, la entidad que se quiere "seguir".

		@return true si todo ha ido correctamente.
		*/
		bool activate();
		
		/**
		M�todo que desactiva el componente; invocado cuando se
		desactiva el mapa donde est� la entidad a la que pertenece el
		componente. Se invocar� siempre, independientemente de si estamos
		activados o no.
		<p>
		Se pone el objetivo a seguir a NULL.
		*/
		void deactivate();

		/**
		M�todo llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la c�mara siguiendo al jugador.

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


	protected:
		
		/**
		C�mara gr�fica.
		*/
		Graphics::CCamera *_graphicsCamera;
		
		/**
		*/
		Vector3 _currentPos;		
	
		/**
		*/
		float _degree;
		/**
		*/
		float _radius;

		/**
		*/
		float _height;

		/**
		*/
		float _angularSpeed;



	}; // class CBaseCamera

	REG_FACTORY(CBaseCamera);

} // namespace Logic

#endif // __Logic_Camera_H
