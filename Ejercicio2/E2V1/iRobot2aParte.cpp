/*
 * File:   iRobot2aParte.cpp
 * Author: Julian Maestri <serpi90@gmail.com>
 *
 * Created on June 19, 2014, 5:50 PM
 */

#include "iRobot2aParte.h"

#include <string>

#include "net-idManagerProtocol.h"

iRobot2aParte::iRobot2aParte(long type) {
	std::string owner = "iRobot2aParte";

	Queue<IdManager::messageRequest> * toIdManager;
	IdManager::messageRequest idRequest;
	Queue<IdManager::messageReply> * fromIdManager;
	IdManager::messageReply idReply;

	toIdManager = new Queue<IdManager::messageRequest>(IPC::path, (int) IPC::QueueIdentifier::TO_ID_MANAGER, owner);
	toIdManager->get();
	fromIdManager = new Queue<IdManager::messageReply>(IPC::path, (int) IPC::QueueIdentifier::FROM_ID_MANAGER, owner);
	fromIdManager->get();

	idRequest.mtype = (long) IPC::MessageTypes::ROBOT_1_SACADO;
	idRequest.kind = IdManager::HostKind::ROBOT_1_SACADO;
	toIdManager->send(idRequest);
	idReply = fromIdManager->receive((long) IPC::MessageTypes::ROBOT_1_SACADO);

	this->id = idReply.id;
	this->type = type;
	aBroker = new Queue<Broker::message>(IPC::path, (int) IPC::QueueIdentifier::TO_BROKER_RECEIVER, owner);
	aBroker->get();
	colaSalida = new Queue<ColaSalida::message>(IPC::path, (int) IPC::QueueIdentifier::SALIDA_FROM_CTL_TO_INTERFACE, owner);
	colaSalida->get();
}

struct dispositivo iRobot2aParte::tomarDispositivo() {
	ColaSalida::message msg;
	Broker::message msgBrk;
	msgBrk.mtype = id;
	msgBrk.type = type;
	msgBrk.request = Broker::Request::DAME_DISPOSITIVO_PARA_SACAR_DE_CINTA_SALIDA;
	aBroker->send(msgBrk);
	msg = colaSalida->receive(type);
	return msg.dispositivo;
}
