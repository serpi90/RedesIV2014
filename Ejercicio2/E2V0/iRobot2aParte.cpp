/*
 * File:   iRobot2aParte.cpp
 * Author: Julian Maestri <serpi90@gmail.com>
 *
 * Created on June 19, 2014, 5:50 PM
 */

#include "iRobot2aParte.h"

iRobot2aParte::iRobot2aParte(long type) {
	this->type = type;
	colaSalida = new Queue<ColaSalida::message>(IPC::path, (long) IPC::QueueIdentifier::SALIDA, "iRobot2aParte");
	colaSalida->get();
}

struct dispositivo iRobot2aParte::tomarDispositivo() {
	ColaSalida::message msg;
	msg = colaSalida->receive(type);
	return msg.dispositivo;
}
