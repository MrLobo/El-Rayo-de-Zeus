/**
@file PhysicEntity.h

Contiene la implementaci�n del componente encargado de representar entidades f�sicas simples,
que son aquellas representadas mediante un �nico actor de PhysX. Este componente no sirve
para representar character controllers.

@see Logic::CPhysicEntity
@see Logic::IComponent
@see Logic::CPhysicController

@author Antonio S�nchez Ruiz-Granados
@date Noviembre, 2012
*/

#include "PhysicEntity.h"





namespace Logic {

	IMP_FACTORY(CPhysicEntity);

	
	
	//---------------------------------------------------------

	//Physics::CActor* CPhysicEntity::createActor(const Map::CEntity *entityInfo)
	//{
	//	// Leer el tipo de entidad		
	//	assert( entityInfo->hasAttribute("physicType") );
	//		const std::string physicType= entityInfo->getStringAttribute("physicType");
	//		assert(physicType == "rigid");

	//	// Crear el tipo de entidad adecuada
	//	if(physicType == "rigid")
	//		return createRigid(entityInfo);
	//	//else if (physicType == "plane")  // TODO FRS Descomentar si queremos crear actores de tipo plano
	//	//	return createPlane(entityInfo);	// Esta funcion se implementa en la pr�ctica de f�sica
	//	else
	//		return 0;
	//}	
	
	// TODO Distinguir entre static, dynamic y kinematic
	// De momento, CPhysicEntity == rigid + static + square + trigger
	//Physics::CActor* CPhysicEntity::createRigid(const Map::CEntity *entityInfo)
	//{
	//	// Leer la posici�n de la entidad
	//	const TLogicalPosition logicPos = _entity->getLogicalPosition();
	//
	//	// RIGID TYPE: Leer el tipo de entidad: est�ticos, din�mico o cinem�tico
	//	assert(entityInfo->hasAttribute("physicRigidType"));
	//		const std::string physicRigidType = entityInfo->getStringAttribute("physicRigidType");
	//		assert( (physicRigidType == "static") );//|| (physicRigidType == "dynamic") || (physicRigidType == "kinematic"));

	//	// RIGID SHAPE: Leer la forma (shape)
	//	assert(entityInfo->hasAttribute("physicRigidShape"));
	//		const std::string physicRigidShape= entityInfo->getStringAttribute("physicRigidShape");
	//		assert(physicRigidShape== "square");

	//	// TRIGGER: Leer si es un trigger (por defecto no)
	//	bool isTrigger = false;
	//	if (entityInfo->hasAttribute("physicTrigger"))
	//		isTrigger = entityInfo->getBoolAttribute("physicTrigger");

	//	// CREATE STATIC
	//	if (physicRigidType == "static") 
	//		if (physicRigidShape== "square") {
	//						
	//			// Leer el ancho del angular box
	//			assert(entityInfo->hasAttribute("physicWidth")); 
	//				float physicWidth = entityInfo->getFloatAttribute("physicWidth");

	//			// Leer la altura del angular box
	//			assert(entityInfo->hasAttribute("physicHeight"));
	//				float physicHeight = entityInfo->getFloatAttribute("physicHeight");

	//			// Crear un �rea cuadrado est�tica
	//			return _server->createActor(logicPos, physicWidth, physicHeight, isTrigger, this);
	//		}

	//	return 0;

	//} // createRigid


}
