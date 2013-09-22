/**
@file AnimatedGraphics.h

Contiene la declaraci�n del componente que controla la representaci�n
gr�fica de una entidad est�tica.

@see Logic::CAnimatedGraphics
@see Logic::CGraphics

@author David Llans�
@date Agosto, 2010
*/
#ifndef __Logic_AnimatedGraphics_H
#define __Logic_AnimatedGraphics_H

#include "Graphics.h"
#include "Graphics/AnimatedEntity.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Graphics 
{
	class CAnimatedEntity;
	class CAnimSet;
}
namespace Logic
{
	class CMessage;
}

//declaraci�n de la clase
namespace Logic 
{
/**
	Componente que se encarga de la representaci�n gr�fica animada de una 
	entidad. Especializa a la clase CGraphics para que la entidad gr�fica
	creada sea animada.
	<p>
	Adem�s de los mensajes de cambio de posici�n y de cambio de orientaci�n
	que acepta CGraphics, acepta mensajes para poner o para una animaci�n
	(SET_ANIMATION y STOP_ANIMATION).
	
    @ingroup logicGroup

	@author David Llans� Garc�a
	@date Agosto, 2010
*/
	
	enum AnimationName : unsigned short //Esto pertenece a la l�gica, son nombre l�gicos que tendr�n su traducci�n a strings de animaci�n
	{
		NONE,
		IDLE,
		WALK,
		RUN,
		DEATH,
		JUMP,
		JUMP_DOWN,
		ALERT,
		DAMAGE,
		SWITCH_ALTAR,
		COVER_WITH_WEAPON,
		COVER_WITH_SHIELD,
		ATTACK1,
		ATTACK2,
		ATTACK3,
		COMBO1,
		COMBO2,
		COMBO3
	};

	enum Tracks : unsigned short //Posibles tracks de animaciones.
	{
		ANIMATION_BEGIN=0,
		ANIMATION_END=1,
		COMBO_TRACK=2,
		DAMAGE_TRACK=3,
		COVER_MOMENT=4,
		SWITCH_MOMENT=5
	};
	




	class CAnimatedGraphics : public CGraphics, public Graphics::CAnimatedEntityListener
	{
		DEC_FACTORY(CAnimatedGraphics);
	public:
				/**
		Tipo de elemento que contiene el vector de tiempos, formado por un identificador de track y un tiempo float
		*/
		//std::map<Logic::AnimationName,std::vector<float>> _eventChain;

		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		CAnimatedGraphics() : CGraphics(GetAltTypeIdOf(CAnimatedGraphics)), _graphicalEntity(0), _animSet(0), _deathAllocation(false),
				_defaultAnimation("") {}

		/**
		M�todo virtual que elige que mensajes son aceptados. Son v�lidos
		SET_ANIMATION y STOP_ANIMATION.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		bool accept(const CMessage *message);

		/**
		M�todo virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		void process(CMessage *message);

		virtual void detachFromMap();
		virtual void attachToMap(CMap* map);
		
		////////////////////////////////////////
		// M�todos de CAnimatedEntityListener //
		////////////////////////////////////////
		/**
		M�todo que ser� invocado siempre que se termine una animaci�n.
		Las animaciones en c�clicas no invocar�n nunca este m�todo.

		@param animation Nombre de la animaci�n terminada.
		*/
		void animationFinished(const std::pair<unsigned short,float> track);

		void animationMomentReached(const std::pair<unsigned short,float> track);
	
	protected:
		/**
		Entidad gr�fica.
		*/
		Graphics::CAnimatedEntity *_graphicalEntity;

		/**	puntero a la clase de animaciones. */
		Graphics::CAnimSet *_animSet;
				
		/**
		Animaci�n l�gica que tiene la entidad activada.
		*/
		Logic::AnimationName _currentLogicAnimation;

		/**
		Animaci�n por defecto de una entidad gr�fica animada.
		*/
		std::string _defaultAnimation;  //cambiar por animaci�n l�gica

		/**
		M�todo virtual que construye la entidad gr�fica animada de la entidad. 
		Sobreescribe el m�todo de la clase CGraphics.
		
		@param entityInfo Informaci�n de construcci�n del objeto le�do del
			fichero de disco.
		@return Entidad gr�fica creada, NULL si hubo alg�n problema.
		*/
		Graphics::CEntity* createGraphicalEntity(const Map::CEntity *entityInfo);

		/**
		M�todo que construye el animSet de la entidad. 
		
		@param entityInfo Informaci�n de construcci�n del objeto le�do del
			fichero de disco.
		@return puntero al conjuto de animaciones de la entidad, NULL si hubo problemas.
		*/
		bool initializeAnimSet(const Map::CEntity *entityInfo);

		/**
		Variable que dice si una entidad ha muerto y ya se ha puesto, si es necesario, una posici�n nueva del cadaver (para que por ejemplo no se quede flotando en el aire)
		*/
		bool _deathAllocation;
	
	
	}; // class CAnimatedGraphics

	REG_FACTORY(CAnimatedGraphics);

} // namespace Logic

#endif // __Logic_AnimatedGraphics_H
