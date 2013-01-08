/**
@file Manager.h

Contiene la declaraci�n de la clase CManager, Singleton que se encarga de
la gesti�n de la red del juego.

@see Net::CManager

@author David Llans�
@date Diciembre, 2010
*/
#ifndef __Net_Manager_H
#define __Net_Manager_H

#include <vector>

// Predeclaracion de clases
namespace Net {
	class CBuffer;
	class CServidor;
	class CCliente;
	class CConexion;
	class CFactoriaRed;
	class CFactoriaRedEnet;
	class CPaquete;
};

/**
Namespace que engloba lo relacionado con la parte de red.
(para m�s informaci�n ver @ref NetGroup).

@author David Llans�
@date Diciembre, 2010
*/
namespace Net
{
	enum NetMessageType {
		COMMAND,
		START_GAME,
		END_GAME,
		LOAD_MAP,
		MAP_LOADED,
		ENTITY_MSG
	};



	/**
	Gestor de la red. Sirve como interfaz para que el resto de los
	proyectos interact�en con la red y no tengan que preocuparse de 
	la tecnolog�a espec�fica usada para su implementaci�n.
	<p>
	Esta clase debe ser espec�ficamente inicializada o configurada
	como cliente <em>o</em> como servidor. Sin embargo, desde fuera
	<em>puede usarse sin conocer su configuraci�n</em> y as� invocar 
	m�todos como CManager::tick() para que compruebe si se ha recibido 
	alg�n mensaje o invocar a CManager::send() cuando se quiera enviar 
	un mensaje al otro extremo de la conexi�n.

	@ingroup NetGroup

	@author David Llans�
	@date Diciembre, 2010
	*/
	class CManager
	{
	public:

		/**
		Interfaz que deben implementar las clases que quieran ser 
		notificadas de eventos de red.
		*/
		class IObserver{
		public:
			virtual void dataPacketReceived(Net::CPaquete* packet)=0;
			virtual void connexionPacketReceived(Net::CPaquete* packet)=0;
			virtual void disconnexionPacketReceived(Net::CPaquete* packet)=0;
		};

		/**
		Devuelve la �nica instancia de la clase CManager.
		
		@return �nica instancia de la clase CManager.
		*/
		static CManager* getSingletonPtr() {return _instance;}

		/**
		Inicializa la instancia

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera la instancia de CManager. Debe llamarse al finalizar la 
		aplicaci�n.
		*/
		static void Release();

		/**
		Funci�n llamada en cada frame para que se realicen las funciones
		de actualizaci�n adecuadas.
		<p>
		Ser� aqu� donde consultaremos si han llegado mensajes y de ser as� 
		los procesaremos o delegaremos su proceso al m�dulo correspondiente.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		void tick(unsigned int msecs);

		/**
		Funci�n que sirve para enviar datos al otro lado de la conexi�n.
		Si se est� en modo cliente los datos se enviar�n al servidor
		mientras que si se encuentra en modo servidor la informaci�n se
		enviar� a todos los clientes registrados.

		@param data Datos a enviar.
		*/
		void send(void* data, size_t longdata);

		void activateAsServer(int port, int clients = 16, unsigned int maxinbw = 0, unsigned int maxoutbw = 0);

		void activateAsClient(unsigned int maxConnections = 1, unsigned int maxinbw = 0, unsigned int maxoutbw = 0);

		void connectTo(char* address, int port, int channels = 1, unsigned int timeout = 5000);

		void disconnect();

		void deactivateNetwork();


		void addObserver(IObserver*);

		void removeObserver(IObserver*);

	protected:
		/**
		Constructor.
		*/
		CManager ();

		/**
		Destructor.
		*/
		~CManager();

		/**
		Segunda fase de la construcci�n del objeto. Sirve para hacer
		inicializaciones de la propia instancia en vez de inicializaciones 
		est�ticas.

		@return true si todo fue correctamente.
		*/
		bool open();

		/**
		Segunda fase de la destrucci�n del objeto. Sirve para hacer liberar 
		los recursos de la propia instancia, la liberaci�n de los recursos 
		est�ticos se hace en Release().
		*/
		void close();

		void getPackets(std::vector<Net::CPaquete*>& _paquetes);

	private:
		/**
		�nica instancia de la clase.
		*/
		static CManager* _instance;

		/**
		Factor�a de objetos de red
		*/
		Net::CFactoriaRed* _factoriaRed;

		/**
			 Servidor de red
		*/
		Net::CServidor* _servidorRed;

		/**
			Cliente de red
		*/
		Net::CCliente* _clienteRed;

		/**
			Conexion cliente de red. Es decir, el servidor visto desde el cliente.
			En un juego avanzado habr�a m�s de una si estamos en p2p.
		*/
		Net::CConexion* _conexion;

		std::vector<IObserver*> _observers;

		std::vector<Net::CPaquete*> _paquetes;

		}; // class CManager

} // namespace Net

#endif // __Net_Manager_H
