/*
 * File:   iRobot2aParte.h
 * Author: Julian Maestri <serpi90@gmail.com>
 *
 * Created on June 19, 2014, 5:50 PM
 */

#ifndef IROBOT2APARTE_H
#define	IROBOT2APARTE_H

#include "includes.h"
#include "Queue.cpp"

class iRobot2aParte {
	public:
		iRobot2aParte(long);
		struct dispositivo tomarDispositivo();
	private:
		Queue<ColaSalida::message> * colaSalida;
		long type;
};

#endif	/* IROBOT2APARTE_H */

