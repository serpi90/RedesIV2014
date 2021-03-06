/*
 * File:   iRobot1aParteArmado.h
 * Author: Julian Maestri <serpi90@gmail.com>
 *
 * Created on June 19, 2014, 5:49 PM
 */

#ifndef IROBOT1APARTEARMADO_H
#define	IROBOT1APARTEARMADO_H

#include "includes.h"
#include "Queue.cpp"

class iRobot1aParteArmado {
	public:
		iRobot1aParteArmado(unsigned);
		unsigned reservar();
		void esperarSiSacando();
		struct dispositivo esperarDispositivo();
		void colocarDispositivo(struct dispositivo, unsigned);
		void avisarSiEsperandoParaSacar();

	private:
		Queue<ColaArmado::message> * colaArmado;
		Queue<ColaPlataforma::message> * toPlataforma, *fromPlataforma;
		Queue<ColaExclusion::message> * toExclusion, *fromExclusion;
		Queue<Broker::message> * aBroker;
		long id;
		unsigned number;
};

#endif	/* IROBOT1APARTEARMADO_H */
