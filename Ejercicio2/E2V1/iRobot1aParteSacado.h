/*
 * File:   iRobot1aParteSacado.h
 * Author: Julian Maestri <serpi90@gmail.com>
 *
 * Created on June 19, 2014, 5:50 PM
 */

#ifndef IROBOT1APARTESACADO_H
#define	IROBOT1APARTESACADO_H

#include "includes.h"
#include "Queue.cpp"

class iRobot1aParteSacado {
	public:
		iRobot1aParteSacado(unsigned);
		struct dispositivo esperarDispositivo();
		void esperarSiArmando();
		struct dispositivo tomarDispositivo(struct dispositivo);
		void colocarDispositivo(struct dispositivo);
		void avisarSiEsperandoParaArmar();
	private:
		Queue<ColaActivado::message> * colaDeActivado;
		Queue<ColaSalida::message> * colaDeSalida;
		Queue<ColaPlataforma::message> * toPlataforma, *fromPlataforma;
		Queue<ColaExclusion::message> * toExclusion, *fromExclusion;
		long id;
		unsigned number;
};

#endif	/* IROBOT1APARTESACADO_H */

