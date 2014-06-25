/*
 * File:   iRobot1aParteArmado.cpp
 * Author: Julian Maestri <serpi90@gmail.com>
 *
 * Created on June 19, 2014, 5:49 PM
 */

#include "iRobot1aParteArmado.h"

#include <cstdio>
#include <string>

#include "Helper.h"
#include "net-idManagerProtocol.h"

iRobot1aParteArmado::iRobot1aParteArmado(unsigned number) {

	std::string owner = "iRobot1aParteArmado";
	Queue<IdManager::messageRequest> * toIdManager;
	IdManager::messageRequest idRequest;
	Queue<IdManager::messageReply> * fromIdManager;
	IdManager::messageReply idReply;

	toIdManager = new Queue<IdManager::messageRequest>(IPC::path, (int) IPC::QueueIdentifier::TO_ID_MANAGER, owner);
	toIdManager->get();
	fromIdManager = new Queue<IdManager::messageReply>(IPC::path, (int) IPC::QueueIdentifier::FROM_ID_MANAGER, owner);
	fromIdManager->get();

	idRequest.mtype = (long) IPC::MessageTypes::ROBOT_1_ARMADO;
	idRequest.kind = IdManager::HostKind::ROBOT_1_ARMADO;
	toIdManager->send(idRequest);
	idReply = fromIdManager->receive((long) IPC::MessageTypes::ROBOT_1_ARMADO);

	this->id = idReply.id;
	this->number = number;
	colaArmado = new Queue<ColaArmado::message>(IPC::path, (int) IPC::QueueIdentifier::ARMADO_FROM_CTL_TO_INTERFACE, owner);
	colaArmado->get();
	toPlataforma = new Queue<ColaPlataforma::message>(IPC::path, (int) IPC::QueueIdentifier::FROM_INTERFACE_TO_PLATAFORMA, owner);
	toPlataforma->get();
	fromPlataforma = new Queue<ColaPlataforma::message>(IPC::path, (int) IPC::QueueIdentifier::FROM_PLATAFORMA_TO_INTERFACE, owner);
	fromPlataforma->get();
	toExclusion = new Queue<ColaExclusion::message>(IPC::path, (int) IPC::QueueIdentifier::FROM_INTERFACE_TO_EXCLUSION, owner);
	toExclusion->get();
	fromExclusion = new Queue<ColaExclusion::message>(IPC::path, (int) IPC::QueueIdentifier::FROM_EXCLUSION_TO_INTERFACE, owner);
	fromExclusion->get();
}

unsigned iRobot1aParteArmado::reservar() {
	ColaPlataforma::message msg;
	msg.mtype = id;
	msg.operation = ColaPlataforma::RESERVAR;
	msg.numero = number;

	toPlataforma->send(msg);
	msg = fromPlataforma->receive(id);
	if (msg.operation != ColaPlataforma::RESERVAR) {
		Helper::output(stderr, "iRobot1aParteArmado esperaba RESERVAR\n", Helper::Colours::RED);
	}
	return msg.lugar;
}

void iRobot1aParteArmado::esperarSiSacando() {
	ColaExclusion::message msg;
	msg.mtype = id;
	msg.operation = ColaExclusion::ESPERAR_SI_SACANDO;
	msg.number = number;

	toExclusion->send(msg);
	msg = fromExclusion->receive(id);
	if (msg.operation != ColaExclusion::ESPERAR_SI_SACANDO) {
		Helper::output(stderr, "iRobot1aParteArmado esperaba ESPERAR_SI_SACANDO\n", Helper::Colours::RED);
	}
}

struct dispositivo iRobot1aParteArmado::esperarDispositivo() {
	ColaArmado::message msg;
	colaArmado->receive((long) IPC::MessageTypes::ANY);
	return msg.dispositivo;
}

void iRobot1aParteArmado::colocarDispositivo(struct dispositivo d, unsigned l) {
	ColaPlataforma::message msg;
	msg.mtype = id;
	msg.operation = ColaPlataforma::COLOCAR_DISPOSITIVO;
	msg.dispositivo = d;
	msg.lugar = l;

	toPlataforma->send(msg);
	msg = fromPlataforma->receive(id);
	if (msg.operation != ColaPlataforma::COLOCAR_DISPOSITIVO) {
		Helper::output(stderr, "iRobot1aParteArmado esperaba COLOCAR_DISPOSITIVO\n", Helper::Colours::RED);
	}
}

void iRobot1aParteArmado::avisarSiEsperandoParaSacar() {
	ColaExclusion::message msg;
	msg.mtype = id;
	msg.operation = ColaExclusion::AVISAR_SI_ESPERANDO_PARA_SACAR;
	msg.number = number;

	toExclusion->send(msg);
}
