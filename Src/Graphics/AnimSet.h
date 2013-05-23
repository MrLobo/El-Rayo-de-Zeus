//---------------------------------------------------------------------------
// AnimSet.h
//---------------------------------------------------------------------------

/**
@file AnimSet.h

Contiene la declaraci�n de la clase que reprenta una correspondencia entre nombr�s l�gicos de animaci�n y nombres f�sicos

@see Graphics::CAnimSet

@author Jose Luis L�pez	
@date Mayo, 2013
*/

#ifndef __Graphics_AnimSet_H
#define __Graphics_AnimSet_H

#include <map>
#include <vector>
#include <string>
#include "Logic\Entity\Components\AnimatedGraphics.h"

namespace Graphics 
{
	
	/**
	Clase que representa una entidad gr�fica con animaciones. Especializa la clase
	Graphics::CEntity por lo que contiene una referencia a una entidad de 
	Ogre y al Nodo que la contiene. A�ade la posibilidad de activar una animaci�n 
	y reproducirla.
	<p>
	Esta clase es bastante sencilla y solo controlamos una animaci�n activa.
	Podr�a ser m�s sofisticada permitiendo interpolaci�n de animaciones o avisando
	mediante observers cuando una animaci�n termina de reproducirse.
	<p>
	Oculta los detalles escabrosos del motor gr�fico.
	
	@ingroup graphicsGroup

	@author Jose Luis L�pez	
	@date Mayo, 2013
	*/
	class CAnimSet
	{
	public:

		/**
		Constructor de la clase.

		@param entityInfo informaci�n del Map para leer la informaci�n acerca de la animaci�n
		*/
		CAnimSet();



		/**
		Destructor de la clase.

		@param entityInfo informaci�n del Map para leer la informaci�n acerca de la animaci�n
		*/
		~CAnimSet();

		/**
		Asigna un nombre de animaci�n l�gico a un string correcpondiente a la animaci�n f�sica.

		@param animEnum enumerado que estamos seteando
		@param realName nombre f�sico leido del map
		@return true si el metodo termina de forma satisfactoria
		*/
		bool addAnimation(const Logic::AnimationName animEnum, const std::string realName);
		
		/**
		Obtiene el nombre f�sico de la animaci�n a partir de su nombre l�gico.

		@param anim Nombre de la animaci�n a activar.
		@return std::string devuelve el nombre f�sico de la animaci�n
		*/
		std::string getAnimation(const Logic::AnimationName animEnum);

		/**
		A�ade un evento de tiempo  leido desde el Map.

		@param animEnum enumerado que estamos seteando
		@param eventTime tiempo que queremos almacenar
		@return true si el metodo termina de forma satisfactoria
		*/
		bool addEventTime(const Logic::AnimationName animEnum, const Logic::Tracks, const float eventTime);

		/**
		Obtiene la cadena de eventos

		@param anim Nombre de la animaci�n a la que acceder.
		@return std::list<float> devuelve la lista de eventos asociada a la animaci�n.
		*/
		std::vector<std::pair<unsigned short,float>>* getEventChain(const Logic::AnimationName animEnum);
	
	private:

		/**
		Map de nombres de animaciones correspondientes a los enum L�gicos
		*/
		std::map<Logic::AnimationName,std::string> _animationSet; 
		


		/**
		Map que guarda una lista de eventos para una animaci�n L�gica.
		*/
		std::map<Logic::AnimationName,std::vector<std::pair<unsigned short,float>>> _eventChain;

	}; // class CAnimSet

} // namespace Graphics

#endif // __Graphics_AnimSetEntity_H
