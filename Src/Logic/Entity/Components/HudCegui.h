///***
//
//@see Logic::CHudCegui
//@see Logic::IComponent
//
//@author Pablo Terrado
//@date Febrero, 2013
//*/
//#ifndef __Logic_HudCegui_H
//#define __Logic_HudCegui_H
//
//#include "Logic/Entity/Component.h"
//
//namespace Graphics 
//{
//	class CCegui;
//}
//
//
////declaraci�n de la clase
//namespace Logic 
//{
///**
//	Este componente controla el hud, mediante overlays
//    @ingroup logicGroup
//
//	@author Pablo Terrado
//	@date Febrero, 2013
//*/
//	class CHudCegui : public IComponent
//	{
//		DEC_FACTORY(CHudCegui);
//
//	public:
//
//		/**
//		Constructor por defecto; en la clase base no hace nada.
//		*/
//		CHudCegui() : IComponent(GetAltTypeIdOf(CHudCegui)), 
//			 _hudGame(0), _health(1000), _healthBase(0), _base(0),
//			 _playersInBase(0), _visibleHud(false) { }
//		
//		/** Destructor */
//		CHudCegui::~CHudCegui();
//
//		/**
//		Inicializaci�n del componente usando la descripci�n de la entidad que hay en 
//		el fichero de mapa.
//		*/
//		bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);
//
//		bool accept(const CMessage *message);
//
//		void process(CMessage *message);	
//
//		/**
//		M�todo que activa el componente; invocado cuando se activa
//		el mapa donde est� la entidad a la que pertenece el componente.
//		<p>
//
//		@return true si todo ha ido correctamente.
//		*/
//		bool activate();
//		
//		/**
//		M�todo que desactiva el componente; invocado cuando se
//		desactiva el mapa donde est� la entidad a la que pertenece el
//		componente. Se invocar� siempre, independientemente de si estamos
//		activados o no.
//		<p>
//		*/
//		void deactivate();
//
//		bool getVisibleHud(){return _visibleHud; }
//
//		void setVisibleHud(bool valor){_visibleHud = valor;}
//
//		
//
//	protected:
//
//		static enum eOverlayTextArea {HEALTH, HEALTHBASE, PLAYERS, DUMMY };
//		static enum ColorValue { WHITE,BLACK,RED,GREEN,BLUE };
//
//		//UNDONE FRS temporalmente
//		
//		void displayHud();
//		void hudSpawn(int spawn);
//		void hudPlayers(short int valor);
//		void hudLife(float health);
//
//		unsigned int _health;
//		unsigned int _healthBase; //la base tiene 3 vidas (3 rayos)
//		unsigned int _base; //numero de base
//		unsigned int _playersInBase; //num Players contrarios en la base del Player
//		bool _visibleHud;
//		
//
//		// TODO FRS de momento puntero, pero podr�a ser est�tico...
//		//Overlay de juego y overlay al morir el Player.
//		Graphics::CCegui* _hudGame;
//
//		 
//	}; // class CHudCegui
//
//	REG_FACTORY(CHudCegui);
//
//} // namespace Logic
//
//#endif // __Logic_HudCegui_H