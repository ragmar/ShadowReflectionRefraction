#pragma once
#include "objeto.h"
#include <iostream>
using namespace std;


		objeto::objeto(){
				t=0.02f;
				estado=true;
		}

		objeto::~objeto(){
			individual_box.clear();
			obj.clear();
			vertex.clear();
			cara_vertice.clear();
			cara_normal.clear();
			cara_textura.clear();
			texturas.clear();
			origen.clear();
			norma_point.clear();
		}

		void objeto::agregarVertice(point vertice){
			vertex.push_back(vertice);
			if(vertex.size()==1){
				bounding_box.inicializar(vertice);
			}else{
				bounding_box.asignar_menor(vertice);
				bounding_box.asignar_mayor(vertice);
			}
		}

		void objeto::agregarCara(int *C,int *T,int *N){
			cara Aux;
			Aux.V[0]=C[0];
			Aux.V[1]=C[1];
			Aux.V[2]=C[2];
			cara_vertice.push_back(Aux);
			if(T[0]!=-1){
				Aux.V[0]=T[0];
				Aux.V[1]=T[1];
				Aux.V[2]=T[2];
				cara_textura.push_back(Aux);
			}

			if(N[0]!=-1){
				Aux.V[0]=N[0];
				Aux.V[1]=N[1];
				Aux.V[2]=N[2];
				cara_normal.push_back(Aux);
			}
		}
		
		void objeto::agregarNormal(point punto){
			norma_point.push_back(punto);
			
		}

		void objeto::terminoObjeto()
		{
			unsigned int inicial=0;
			obj.push_back(cara_vertice.size());
			if(obj.size()>1)
			{
				inicial=obj[obj.size()-2];
			}
			box auxiliar;
			for(unsigned int i=inicial;i<cara_vertice.size();i++)
			{
				if(i==inicial)
				{
					auxiliar.inicializar(vertex[cara_vertice[i].V[0]]);
					auxiliar.asignar_menor(vertex[cara_vertice[i].V[1]]);
					auxiliar.asignar_mayor(vertex[cara_vertice[i].V[1]]);
					auxiliar.asignar_menor(vertex[cara_vertice[i].V[2]]);
					auxiliar.asignar_mayor(vertex[cara_vertice[i].V[2]]);
				}else
				{
					auxiliar.asignar_menor(vertex[cara_vertice[i].V[0]]);
					auxiliar.asignar_mayor(vertex[cara_vertice[i].V[0]]);
					auxiliar.asignar_menor(vertex[cara_vertice[i].V[1]]);
					auxiliar.asignar_mayor(vertex[cara_vertice[i].V[1]]);
					auxiliar.asignar_menor(vertex[cara_vertice[i].V[2]]);
					auxiliar.asignar_mayor(vertex[cara_vertice[i].V[2]]);
				}
			
			}
			individual_box.push_back(auxiliar);
		}

		void objeto::agregarTextura(float x, float y){
			text pos_text;
			pos_text.T[0]=x;
			pos_text.T[1]=y;
			texturas.push_back(pos_text);
		}

		void objeto::normalizar(){ //poner todo entre 0 y 1
			float norm=bounding_box.normalizar();

			bounding_box.max=bounding_box.max/norm;
			bounding_box.min=bounding_box.min/norm;
			for(unsigned int i=0;i<vertex.size();i++){
				vertex[i]=vertex[i]/norm;
			}
			for(unsigned int i=0;i<individual_box.size();i++)
			{
				individual_box[i].max=individual_box[i].max/norm;
				individual_box[i].min=individual_box[i].min/norm;
			}
		}

		void objeto::sett(float t){
			this->t=t;
		}

		void objeto::CalcularNorma(){//sacar la norma
			if(cara_normal.size()==0){
				float unitario;
				origen.clear();
				norma_point.clear();
				cara_normal.clear();

				origen.resize(cara_vertice.size());
				cara_normal.resize(cara_vertice.size());
				norma_point.resize(vertex.size());
				
				for(unsigned int i=0;i<norma_point.size();i++){
					norma_point[i].x=0.0f;
					norma_point[i].y=0.0f;
					norma_point[i].z=0.0f;
				}

				point u,v,norma;
				for(unsigned int i=0;i<cara_vertice.size();i++){
					cara_normal[i]=cara_vertice[i];
					u=vertex[cara_vertice[i].V[1]]-vertex[cara_vertice[i].V[0]];
					v=vertex[cara_vertice[i].V[2]]-vertex[cara_vertice[i].V[0]];
					//sacar produto cruz
					norma=productoCruz(u,v);
					unitario=norma.norma2();
					norma=norma/unitario; 

					origen[i]=(vertex[cara_vertice[i].V[0]]+vertex[cara_vertice[i].V[1]]+vertex[cara_vertice[i].V[2]])/3.0f;

					norma_point[cara_vertice[i].V[0]]=norma_point[cara_vertice[i].V[0]]+norma;
					norma_point[cara_vertice[i].V[1]]=norma_point[cara_vertice[i].V[1]]+norma;
					norma_point[cara_vertice[i].V[2]]=norma_point[cara_vertice[i].V[2]]+norma;
				}
				for(unsigned int i=0;i<norma_point.size();i++){
					unitario=norma_point[i].norma2();
					norma_point[i]=norma_point[i]/unitario; 
				}
			}
		}

		point objeto::productoCruz(point u, point v){
			point A;
			A.x=u.y*v.z-u.z*v.y;
			A.y=u.z*v.x-u.x*v.z;
			A.z=u.x*v.y-u.y*v.x;
			return A;
		}

		void objeto::centrar(){
			point PM;
			PM=bounding_box.Medio();
			bounding_box.max=bounding_box.max-PM;
			bounding_box.min=bounding_box.min-PM;
			for(unsigned int i=0;i<vertex.size();i++){
				vertex[i]=vertex[i]-PM;
			}
			for(unsigned int i=0;i<individual_box.size();i++)
			{
				individual_box[i].max=individual_box[i].max-PM;
				individual_box[i].min=individual_box[i].min-PM;
			}
		}

		void objeto::Pintar(){
			for(unsigned int i=0;i<cara_vertice.size();i++){
				glBegin(GL_TRIANGLES);
					
					glTexCoord2f(texturas[cara_textura[i].V[0]].T[0], texturas[cara_textura[i].V[0]].T[1]);
					glNormal3f(norma_point[cara_normal[i].V[0]].x,norma_point[cara_normal[i].V[0]].y,norma_point[cara_normal[i].V[0]].z);
					vertex[cara_vertice[i].V[0]].Pintar();
					
					glTexCoord2f(texturas[cara_textura[i].V[1]].T[0], texturas[cara_textura[i].V[1]].T[1]);
					glNormal3f(norma_point[cara_normal[i].V[1]].x,norma_point[cara_normal[i].V[1]].y,norma_point[cara_normal[i].V[1]].z);
					vertex[cara_vertice[i].V[1]].Pintar();
					
					glTexCoord2f(texturas[cara_textura[i].V[2]].T[0], texturas[cara_textura[i].V[2]].T[1]);
					glNormal3f(norma_point[cara_normal[i].V[2]].x,norma_point[cara_normal[i].V[2]].y,norma_point[cara_normal[i].V[2]].z);
					vertex[cara_vertice[i].V[2]].Pintar();

				glEnd();
			}
		}

		GLvoid objeto::DibujarSombra(point pointLuzDir) {
			pointLuzDir = pointLuzDir * 50;
			point v1, v2;
			GLint a, b, c, d, p1, p2;
			for(a = 0; a < (int)(planos.size()); ++a) {
				if(planos[a].visible) {
					for(b = 0; b < 3; ++b) {
						c = planos[a].neigh[b];
						if (!c || !planos[c - 1].visible) {
							p1 = planos[a].puntos[b];
							d = (b + 1) % 3;
							p2 = planos[a].puntos[d];
							v1 = (vertex[p1] - pointLuzDir) * 100;
							v2 = (vertex[p2] - pointLuzDir) * 100;
							glBegin(GL_QUADS);
								glVertex3f(vertex[p1].x, vertex[p1].y, vertex[p1].z);
								glVertex3f(vertex[p2].x, vertex[p2].y, vertex[p2].z);
								glVertex4f(vertex[p2].x + v2.x, vertex[p2].y + v2.y, vertex[p2].z + v2.z, 0);
								glVertex4f(vertex[p1].x + v1.x, vertex[p1].y + v1.y, vertex[p1].z + v1.z, 0);
							glEnd();
						}
					}
					glBegin(GL_TRIANGLES);
						for(b = 0; b < 3; ++b) {
							v1 = (vertex[planos[a].puntos[b]] - pointLuzDir) * 100;
							glVertex4f(vertex[planos[a].puntos[b]].x + v1.x, vertex[planos[a].puntos[b]].y + v1.y, vertex[planos[a].puntos[b]].z + v1.z, 0);
						}
					glEnd();
				} else {
					glBegin(GL_TRIANGLES);
						for(b = 0; b < 3; ++b)
							glVertex3f(vertex[planos[a].puntos[b]].x, vertex[planos[a].puntos[b]].y, vertex[planos[a].puntos[b]].z);
					glEnd();
				}
			}
		}
		
		GLvoid objeto::VerificarVisibilidad(point pointLuzDir) {
			GLfloat side;
			pointLuzDir = pointLuzDir * 50;
			for (int a = 0; a < (GLint)(planos.size()); ++a) {
				side = planos[a].PlaneEq.x * pointLuzDir.x + planos[a].PlaneEq.y * pointLuzDir.y + planos[a].PlaneEq.z * pointLuzDir.z + planos[a].PlaneEq.w * pointLuzDir.w;
				if(side < 0)
					planos[a].visible = true;
				else
					planos[a].visible = false;
			}
		}
		
		void objeto::PintarNormales(bool norm,bool norm_p){
			point D;
			if(norm_p){
				for(unsigned int i=0;i<norma_point.size();i++){
					glBegin(GL_LINES);
						vertex[i].Pintar();
						D=vertex[i]+norma_point[i]*t;
						D.Pintar();
					glEnd();
				}
			}
		}

		void objeto::clear(){
			t=0.02f;
			individual_box.clear();
			obj.clear();
			vertex.clear();
			cara_vertice.clear();
			cara_normal.clear();
			cara_textura.clear();
			texturas.clear();
			origen.clear();
			norma_point.clear();
		}

		void objeto::Pintar_box() {
			for(unsigned int i=0;i<individual_box.size();i++)
			{
				individual_box[i].Pintar();
			}
			//bounding_box.Pintar();
		}

		GLvoid objeto::CalcularLosPlanos() {
			for(int i=0;i<((int)(cara_vertice.size()));++i){
				Plane aux;
				aux.puntos[0]=cara_vertice[i].V[0];
				aux.puntos[1]=cara_vertice[i].V[1];
				aux.puntos[2]=cara_vertice[i].V[2];
				aux.PlaneEq=EncontrarElPlano(vertex[cara_vertice[i].V[0]], vertex[cara_vertice[i].V[1]], vertex[cara_vertice[i].V[2]]);
				aux.visible=false;
				aux.normals=(vertex[cara_vertice[i].V[2]]-vertex[cara_vertice[i].V[1]]).producto_cruz(vertex[cara_vertice[i].V[0]]-vertex[cara_vertice[i].V[1]]),
				aux.neigh[0]=aux.neigh[1]=aux.neigh[2]=0;
				planos.push_back(aux);
			}
		}

		point objeto::EncontrarElPlano(point v0, point v1, point v2){
			point vec0, vec1, plane;
			vec0 = v1 - v0;
			vec1= v2 - v0;
			plane = vec0.producto_cruz(vec1);
			plane.w = -(plane.producto_punto(v0));
			return plane;
		}