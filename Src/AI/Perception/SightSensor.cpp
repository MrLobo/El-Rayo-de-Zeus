#include "SightSensor.h"

#include "PerceptionEntity.h"
#include "PerceptionSignal.h"

#include "BaseSubsystems/Math.h"
#include "Physics/Server.h"

#include "Logic/Entity/Entity.h"

namespace AI 
{

CSightSensor::~CSightSensor(void)
{
}

/**
 * Realiza todas las comprobaciones necesarias para averiguar si la se�al recibida
 * se encuentra dentro del cono de visi�n.
 *
 * Las comprobaciones se realizan de m�s "barata" a m�s "cara"
 * 1. Comprueba si el sensor y la se�al est�n activos
 * 2. Comprueba si el tipo de la se�al se corresponde con el tipo que percibe el sensor.
 * 3. Comprueba si la intensidad de la se�al est� por encima del threshold del sensor.
 * 4. Comprueba la distancia entre la se�al y el sensor. Se realiza en 2 pasos:
 * 4.1. Primero se comprueba si la distancia en cada dimensi�n es mayor que la distancia m�xima.
 * 4.2. A continuaci�n se comprueba si la distancia eucl�dea es mayor que la distancia m�xima.
 * 5. Comprobamos si la se�al se encuentra dentro del �ngulo de amplitud del cono de visi�n.
 * 6. Comprobamos si no existe ning�n objeto f�sico entre el sensor y la se�al. Para eso usamos un rayo f�sico.
 *
 * En el momento en que alguna de estas comprobaciones falle se detiene el proceso y se devuelve
 * NULL,  lo que significa que el sensor no es capaz de detectar la se�al. Por otro lado, si todas se
 * superan con �xito se devuelve una notificaci�n.
 *
 * @param perceptible Se�al cuya percepci�n queremos comprobar
 * @param time Instante en el que se realiza la comprobaci�n de percepci�n
 * @return NULL si no se ha percibido la se�al. Una instancia de CNotification en caso contrario.
 */
CNotification* CSightSensor::perceives(const CPerceptionSignal * signal, unsigned int time) 
{

	// Realizamos las comprobaciones
	// Si alguna de ellas falla tendremos que devolver NULL para indicar que no ha habido percepci�n
	// Si todas tienen �xito devolveremos una instancia de CNotification

	// Comprobaciones: 
	// 1. Comprueba si el sensor y la se�al est�n activos 
	// (m�todo isActive de la se�al - se recibe como par�metro- y del sensor)
	if (!(this->isActive()) || !(signal->isActive()))
	{
		//cout << "Fallo: inactivo" << endl;
		return NULL;
	}
	// 2. Comprueba si el tipo de la se�al se corresponde con el tipo que percibe el sensor
	// (m�todo getType)
	if (this->getType() != signal->getType())
	{
		//cout << "Fallo: tipo" << endl;
		return NULL;
	}
	// 3. Comprueba si la intensidad de la se�al est� por encima del threshold del sensor.
	// La intensidad que percibimos, F, es el resultado de usar una funci�n de atenuaci�n:
	// f(F0, dist) = F	donde F0 es la intensidad inicial y dist la distancia.
	// En nuestro caso vamos a simplificar y usaremos una funci�n constante:
	// f(F0, dist) = F0
	// Es decir, que s�lo tendremos que comprobar que el threshold sea mayor que la intensidad
	if (signal->getIntensity() < this->getThreshold())
	{
		//cout << "Fallo: intensidad" << endl;
		return NULL;
	}
	// 4. Comprueba la distancia entre la se�al y el sensor. Se realiza en 2 pasos:
	// 4.1. Primero se comprueba si la distancia en cada dimensi�n es mayor que la distancia m�xima.
	// Sacamos la matriz de transformaci�n de la entidad de percepci�n (getTransform) 
	// y con ella podemos obtener la posici�n del sensor (getTrans)
	// Hacemos lo mismo con la se�al. La diferencia entre estos vectores es el vector de distancia
	// Comparamos cada una de sus dimensiones con la distancia m�xima
	Matrix4 transform = this->getPerceptionEntity()->getTransform();
	Vector3 position = transform.getTrans();
	Vector3 signalPosition = signal->getPerceptionEntity()->getTransform().getTrans();
	Vector3 distance = signalPosition - position;
	if (abs(distance.x) > _maxDistance || abs(distance.y) > _maxDistance || abs(distance.z) > _maxDistance)
	{
		//cout << "Fallo: distancia 1" << endl;
		return NULL;
	}
	// 4.2. A continuaci�n se comprueba si la distancia eucl�dea es mayor que la distancia m�xima.
	// Usamos el vector distancia que hemos calculado antes
	float distMagnitude = distance.length();
	if (distMagnitude > _maxDistance)
	{
		//cout << "Fallo: distancia 2" << endl;
		return NULL;
	}
	// Comprobamos el cono de visi�n


	// 5. Comprobamos si la se�al se encuentra dentro del �ngulo de amplitud del cono de visi�n.
	// Tenemos que calcular el �ngulo que se forma entre la direcci�n hacia la que apunta la entidad (su orientaci�n) y
	// la direcci�n en la que se encuentra el sensor (con respecto a la entidad)
	// Sacamos la orientaci�n de la entidad (getYaw nos da el �ngulo de la entidad con respecto al eje Z)
	// Sacamos la orientaci�n de la se�al = orientaci�n del vector distancia
	// Y sacamos la diferencia 
	// Nos aseguramos de que el �ngulo es menor que PI (nos tenemos que quedar con la parte peque�a de la diferencia)
	// Sacamos la orientaci�n de la entidad 
	// (yaw nos da el �ngulo de la entidad con respecto al eje Z)
	float yaw = Math::getYaw(transform);
	// Hacemos lo mismo con el vector distancia 
	// (que va desde el sensor hasta la se�al)
	Ogre::Radian angle = Ogre::Math::ATan2(-distance.x, -distance.z);
	float signalAngle = angle.valueRadians();
	// El �ngulo de la se�al con respecto a la entidad es signalAngle
	float finalAngle = abs(yaw - signalAngle);
	// Nos aseguramos de que el �ngulo es menor que PI
	if (finalAngle > Ogre::Math::PI)
		finalAngle = Ogre::Math::TWO_PI - finalAngle;
	if (finalAngle > _alpha)
	{
		//cout << "Fallo: cono" << endl;
		return NULL;
	}
	
	// 6. Comprobamos si no existe ning�n objeto f�sico entre el sensor y la se�al. Para eso usamos un rayo f�sico.
	// Necesitamos
	// � la posici�n de origen del rayo = posici�n del sensor
	// � la direcci�n (normalizada) = vector distance normalizado
	// � la distancia m�xima = magnitud del vector distance
	// Con estos par�metros creamos una instancia de Ogre::Ray
	// Y lo usamos con raycastClosest. 
	// Si el rayo colisiona con alg�n objeto f�sico es que hay alg�n tipo de pared entre el sensor 
	// y la se�al, por lo que la entidad que ha emitido la se�al no se puede percibir.
	// Si hay alg�n obst�culo ==> raycastClosest nos devuelve la referencia ==> return NULL
	Vector3 origin, direction;
	float maxDistance;

	origin = position;
	direction = distance / distMagnitude;
	maxDistance = distMagnitude;

	// Dependiendo de la configuraci�n de la detecci�n de colisiones,
	// si lanz�ramos el rayo con estos par�metros, chocar�a contra la propia
	// c�psula del lanzador, por lo que tendr�amos que desplazarlo un poco (lo que mide su radio)
	// En concreto
	// � la nueva posici�n de origen se desplaza a lo largo de la direcci�n del vector
	//   lo que mide el radio: o' = o + r * <direcci�n normalizada>
	// � la direcci�n sigue siendo la misma
	// � la nueva distancia m�xima es la distancia anterior recort�ndole al principio y al 
	//   final los radios de las entidades del sensor y la se�al.
	//direction = distance / distMagnitude;
	//float sensorRadius = _pEntity->getRadius();
	//origin = position + sensorRadius * direction;
	//float signalRadius = signal->getPerceptionEntity()->getRadius();
	//maxDistance = distMagnitude - sensorRadius - signalRadius;

	Ray ray = Ray(position, direction);
	// TODO Si accedemos a la entidad se rompe la independencia entre esta clase y la l�gica
	Logic::CEntity* collisionedEntity =NULL;//PEP HACK// Physics::CServer::getSingletonPtr()->raycastClosest(ray, maxDistance);
	// Si el rayo colisiona con alg�n objeto f�sico es que hay alg�n tipo de pared entre el sensor 
	// y la se�al, por lo que la entidad que ha emitido la se�al no se puede percibir.
	// TODO habr�a que modificar la f�sica para permitir desactivar las colisiones del rayo con los triggers. En la versi�n actual los triggers contar�an como un objeto opaco.
	if (collisionedEntity != NULL)
	{
		//cout << "Fallo: rayo" << endl;
		return NULL;
	}

	// Si todos los chequeos han tenido �xito tenemos que devolver una nueva instancia de CNotification
	// Los par�metros que necesita son:
	// � El instante en el que hay que entregarla (ahora mismo ==> time)
	// � El sensor que lo ha percibido (this)
	// � La entidad de percepci�n que ha generado la se�al
	// Devolvemos la notificaci�n para que se env�e a la entidad

	return new CNotification(time, this, signal->getPerceptionEntity(), Vector3::ZERO);
}


} // namespace AI 
