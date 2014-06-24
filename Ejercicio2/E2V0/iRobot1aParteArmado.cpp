/*
 * File:   iRobot1aParteArmado.cpp
 * Author: Julian Maestri <serpi90@gmail.com>
 *
 * Created on June 19, 2014, 5:49 PM
 */

#include "iRobot1aParteArmado.h"
#include "Helper.h"

iRobot1aParteArmado::iRobot1aParteArmado(unsigned number) {
	this->id = number * 2 + 1;
	this->number = number;
	colaArmado = new Queue<ColaArmado::message>(IPC::path, (int) IPC::QueueIdentifier::ARMADO, "iRobot1aParteArmado");
	colaArmado->get();
	toPlataforma = new Queue<ColaPlataforma::message>(IPC::path, (int) IPC::QueueIdentifier::PLATAFORMA_FROM_INTERFACE, "iRobot1aParteArmado");
	toPlataforma->get();
	fromPlataforma = new Queue<ColaPlataforma::message>(IPC::path, (int) IPC::QueueIdentifier::PLATAFORMA_TO_INTERFACE, "iRobot1aParteArmado");
	fromPlataforma->get();
	toExclusion = new Queue<ColaExclusion::message>(IPC::path, (int) IPC::QueueIdentifier::EXCLUSION_FROM_INTERFACE, "iRobot1aParteArmado");
	toExclusion->get();
	fromExclusion = new Queue<ColaExclusion::message>(IPC::path, (int) IPC::QueueIdentifier::EXCLUSION_TO_INTERFACE, "iRobot1aParteArmado");
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

struct dispositivo iRobot1aParteArmado::tomarDispositivo() {
	ColaArmado::message msg;
	colaArmado->receive((long) IPC::MessageTypes::M_ANY);
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
