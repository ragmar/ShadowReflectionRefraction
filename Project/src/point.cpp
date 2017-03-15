#pragma once
#include "point.h"

		point::point(float x,float y,float z){
			this->x=x;
			this->y=y;
			this->z=z;

		}

		point::point(){
			x=0;
			y=0;
			z=0;
		}
		point::~point(){
		}
		void point::operator=(point punto){//igualo puntos
			x=punto.x;
			y=punto.y;
			z=punto.z;
		}

		point point::operator*(point punto){
			point A;
			A.x=x*punto.x;
			A.y=y*punto.y;
			A.z=z*punto.z;
			return A;
		}

		point point::operator*(float cantidad){
			point A;
			A.x=x*cantidad;
			A.y=y*cantidad;
			A.z=z*cantidad;
			return A;
		}

		point point::operator/(float cantidad){
			point A;
			A.x=x/cantidad;
			A.y=y/cantidad;
			A.z=z/cantidad;
			return A;
		}

		point point::operator+(point punto){
			point A;
			A.x=x+punto.x;
			A.y=y+punto.y;
			A.z=z+punto.z;
			return A;
		}
		point point::operator-(point punto){
			point A;
			A.x=x-punto.x;
			A.y=y-punto.y;
			A.z=z-punto.z;
			return A;
		}

		float point::norma2(){
			return sqrt(x*x+y*y+z*z);
		
		}


		point point::MedioFigura(){
			point A(x,y,z);
			return A;
		}

		void point::Pintar(){
			glVertex3f(x,y,z);
		}

		float point::producto_punto(point aux){
			//Funcion que realiza el producto punto entre dos vectores
			return (this->x*aux.x+this->y*aux.y+this->z*aux.z);
		}

		point point::producto_cruz(point au2) {
			//Funcion que realiza el producto cruz entre dos vectores
			point auxiliar;
			//auxiliar.initPunto(this->y*au2.z-this->z*au2.y,this->z*au2.x-this->x*au2.z,this->x*au2.y-this->y*au2.x);
			auxiliar.x = this->y*au2.z-this->z*au2.y;
			auxiliar.y = this->z*au2.x-this->x*au2.z;
			auxiliar.z = this->x*au2.y-this->y*au2.x;
			return auxiliar;
		}
