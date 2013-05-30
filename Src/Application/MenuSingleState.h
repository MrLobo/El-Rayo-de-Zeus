//---------------------------------------------------------------------------
// MenuSingleState.h
//---------------------------------------------------------------------------

/**
@file MenuSingleState.h

Contiene la declaraci�n del estado de men� de Single Player (con las opciones para el Jugador).

@see Application::CApplicationState
@see Application::CMenuSingleState

@author Pablo Terrado
@date Mayo, 2013
*/

#ifndef __Application_MenuSingleState_H
#define __Application_MenuSingleState_H

#include "ApplicationState.h"


//PT
#include <cegui.h> //para que pueda crear combobox

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Application 
{
	class CBaseApplication;
}

namespace CEGUI
{
	class EventArgs;
	class Window;
}

namespace ScriptManager
{
	class Server;
}

//PT
namespace Ogre
{
	class Root;
}

namespace Application 
{
	/**
	Como su nombre indica, esta clase es la clase del men� Single Player del juego. 
	Es muy sencilla ysirve para que el jugador decida su nickname, modelo de jugador, y color.

	@ingroup applicationGroup

	@author Pablo Terrado
	@date Mayo, 2013
	*/
	class CMenuSingleState : public CApplicationState 
	{

	protected:

		typedef std::list<std::string> TMapNameList;
	
	public:
		/** 
		Constructor de la clase 
		*/
		CMenuSingleState(CBaseApplication *app) : CApplicationState(app)
				{}

		/** 
		Destructor 
		*/
		virtual ~CMenuSingleState();

		/**
		Funci�n llamada cuando se crea el estado (se "engancha" en la
		aplicaci�n, para que inicialice sus elementos.

		@return true si todo fue bien.
		*/
		virtual bool init();

		/**
		Funci�n llamada cuando se elimina ("desengancha") el
		estado de la aplicaci�n.
		*/
		virtual void release();

		/**
		Funci�n llamada por la aplicaci�n cuando se activa
		el estado.
		*/
		virtual void activate();

		/**
		Funci�n llamada por la aplicaci�n cuando se desactiva
		el estado.
		*/
		virtual void deactivate();

		/**
		Funci�n llamada por la aplicaci�n para que se ejecute
		la funcionalidad del estado.

		@param msecs N�mero de milisegundos transcurridos desde
		la �ltima llamada (o desde la �ctivaci�n del estado, en caso
		de ser la primera vez...).
		*/
		virtual void tick(unsigned int msecs);

		// M�todos de CKeyboardListener
		
		/**
		M�todo que ser� invocado siempre que se pulse una tecla. 
		Ser� la aplicaci�n qui�n llame a este m�todo cuando el 
		estado est� activo. Esta clase NO se registra en el 
		InputManager sino que es la aplicaci�n quien lo hace y 
		delega en los estados.

		@param key C�digo de la tecla pulsada.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		virtual bool keyPressed(GUI::TKey key);
		
		/**
		M�todo que ser� invocado siempre que se termine la pulsaci�n
		de una tecla. Ser� la aplicaci�n qui�n llame a este m�todo 
		cuando el estado est� activo. Esta clase NO se registra en
		el InputManager sino que es la aplicaci�n quien lo hace y 
		delega en los estados.

		@param key C�digo de la tecla pulsada.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		virtual bool keyReleased(GUI::TKey key);

		// M�todos de CMouseListener
		
		/**
		M�todo que ser� invocado siempre que se mueva el rat�n. La
		aplicaci�n avisa de este evento al estado actual.

		@param mouseState Estado del rat�n cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		virtual bool mouseMoved(const GUI::CMouseState &mouseState);
		
		/**
		M�todo que ser� invocado siempre que se pulse un bot�n. La
		aplicaci�n avisa de este evento al estado actual.

		@param mouseState Estado del rat�n cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		virtual bool mousePressed(const GUI::CMouseState &mouseState);

		/**
		M�todo que ser� invocado siempre que se termine la pulsaci�n
		de un bot�n. La aplicaci�n avisa de este evento al estado 
		actual.

		@param mouseState Estado del rat�n cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners. 
		*/
		virtual bool mouseReleased(const GUI::CMouseState &mouseState);

	private:

		/**
		Ventana CEGUI que muestra el men�.
		*/
		CEGUI::Window* _menuWindow;

		//PT combobox
		CEGUI::Combobox* _cbModel;
		CEGUI::Combobox* _cbColor;

		//PT
		//CEGUI::ProgressBar* _hbar;

		/**
		*/
		TMapNameList _mapsToLoad;

	
		/**
		Funci�n que se quiere realizar cuando se pulse el bot�n start.
		Simplemente cambia al estado de juego.
		*/
		bool startReleased(const CEGUI::EventArgs& e);

		/**
		Funci�n que se quiere realizar cuando se pulse el bot�n exit.
		Simplemente termina la aplicaci�n.
		*/
		bool backReleased(const CEGUI::EventArgs& e);

		/**
		Funci�n que se quiere realizar cuando el progreso avance/actualice
		para cambiar la barra de progreso
		*/
		bool CMenuSingleState::onProgressChanged(const CEGUI::EventArgs &e);

	}; // CMenuSingleState

} // namespace Application

#endif //  __Application_MenuSingleState_H
