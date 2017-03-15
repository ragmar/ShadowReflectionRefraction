
#pragma comment (lib,"lib/freeglut.lib")
#pragma comment (lib,"lib/AntTweakBar.lib")
#pragma comment (lib,"lib/glew32.lib")
#include "../tools/GL/glew.h"
#include "../tools/GL/freeglut.h"
#include "../tools/AntTweakBar/AntTweakBar.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "Quaternion.h"
#include "objeto.h"
#include "textura.h"
#include "carga_archivo.h"
#include "MapaAltura.h"
#include "Shader.h"


#define SQRT3 1.7320508f
#define OBJECT 23
#define SOMVOL 7

using namespace std;

GLint w;
GLint h;
GLfloat xCol;
GLfloat zCol;
GLfloat xMir;
GLfloat yMir;
GLfloat zMir;
GLuint Escenario;

float X=0.0f,Y=0.0f,Z=0.0f;

textura texture[OBJECT];
objeto figura[OBJECT];
int activo[OBJECT];

int time=0;
int tiempo=0;
float rotl=0.0f;

bool iluminacion;
int Luz=0,Modelo=0;

/*material*/
float materialAmbiental[] = {0.5f,0.5f,0.5f};
float materialDifuso[] = {0.5f,0.5f,0.5f};
float materialEspecular[] = {0.5f,0.5f,0.5f};
float materialBrillo = 20.0f;

/*luz*/
float luzAmbiental[3];
float luzDifusa[3];
float luzEspecular[3];
float luzDireccion[4];
GLfloat global[4];

MapaAltura Colision;
Shader sh;
float shadowMatrix[16];

point EncontrarPlano(point v0, point v1, point v2){
    point vec0, vec1, plane;
	vec0 = v1 - v0;
    vec1= v2 - v0;
	plane = vec0.producto_cruz(vec1);
	plane.w = -(plane.producto_punto(v0));
	return plane;
}

void rotarFigura(int i,bool eje)
{
	glPushMatrix();
		point Medio = figura[i].bounding_box.Medio();
		float Matriz[16];
		Quaternion aux;
		aux.CreateFromAxisAngle(0.0f,1.0f,0.0f,-rotl*3.0f);
		aux.CreateMatrix(Matriz);
		if (eje)
			glTranslatef(Medio.x,Medio.y, Medio.z);	
		glMultMatrixf(Matriz);
		if (eje)
			glTranslatef(-Medio.x,-Medio.y, -Medio.z);	
		glCallList(Escenario+i);

	glPopMatrix();
}

void ShadowMatrix(GLfloat fMatrix[16], point fLightPos, point fPlane) {
    GLfloat dotp;
	dotp = fPlane.producto_punto(fLightPos);
	fMatrix[0]  = dotp - fLightPos.x * fPlane.x;
    fMatrix[4]  = - fLightPos.x * fPlane.y;
    fMatrix[8]  = - fLightPos.x * fPlane.z;
    fMatrix[12] = - fLightPos.x * fPlane.w;
    fMatrix[1]  = - fLightPos.y * fPlane.x;
    fMatrix[5]  = dotp - fLightPos.y * fPlane.y;
    fMatrix[9]  = - fLightPos.y * fPlane.z;
    fMatrix[13] = - fLightPos.y * fPlane.w;
    fMatrix[2]  = - fLightPos.z * fPlane.x;
    fMatrix[6]  = - fLightPos.z * fPlane.y;
    fMatrix[10] = dotp - fLightPos.z * fPlane.z;
    fMatrix[14] = - fLightPos.z * fPlane.w;
    fMatrix[3]  = - fLightPos.w * fPlane.x;
    fMatrix[7]  = - fLightPos.w * fPlane.y;
    fMatrix[11] = - fLightPos.w * fPlane.z;
    fMatrix[15] = dotp - fLightPos.w * fPlane.w;
}

void PlanarShadow() {

	point pointLuzDir;
	pointLuzDir.x = luzDireccion[0];
	pointLuzDir.y = luzDireccion[1];
	pointLuzDir.z = luzDireccion[2];
	pointLuzDir.w = luzDireccion[3];
	point v[3];

	glClear(GL_STENCIL_BUFFER_BIT);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDepthMask(GL_FALSE);
	glEnable(GL_CULL_FACE);
	glEnable( GL_STENCIL_TEST );
	glStencilFunc(GL_ALWAYS, 1, 0xFFFFFFFF); 
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

	texture[0].Bind();
	glCallList(Escenario);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	glStencilFunc(GL_EQUAL, 1, 0xFFFFFFFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
	glDisable(GL_CULL_FACE);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(-1.0f, 1.0f);
	glDepthMask(GL_TRUE);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glColor3f(0.0, 0.0, 0.0);

	for(int i=0;i<3;++i)
		v[i] = figura[0].vertex[i+1];

	ShadowMatrix(shadowMatrix, pointLuzDir, EncontrarPlano(v[0], v[1], v[2]));
	glPushMatrix();
		glMultMatrixf((GLfloat*)shadowMatrix);
		for(int i=6;i<OBJECT-1;i++)
			if(i==12||i==21||i==18)
				if(i==12||i==18)
					rotarFigura(i,true);
				else
					rotarFigura(i,false);
			else
				glCallList(Escenario+i);
	glPopMatrix();


	glClear(GL_STENCIL_BUFFER_BIT);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDepthMask(GL_FALSE);
	glEnable(GL_CULL_FACE);
	glEnable( GL_STENCIL_TEST );
	glStencilFunc(GL_ALWAYS, 1, 0xFFFFFFFF); 
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

	texture[20].Bind();
	glCallList(Escenario+20);

	glBindTexture(GL_TEXTURE_2D, 0);
	glStencilFunc(GL_EQUAL, 1, 0xFFFFFFFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
	glDisable(GL_CULL_FACE);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(-1.0f, 1.0f);
	glDepthMask(GL_TRUE);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glColor3f(0.0, 0.0, 0.0);

	for(int i=0;i<3;++i)
	v[i] = figura[20].vertex[i+3];
	ShadowMatrix(shadowMatrix, pointLuzDir, EncontrarPlano(v[0], v[1], v[2]));
	glPushMatrix();
		glMultMatrixf((GLfloat*)shadowMatrix);
		for(int i=6;i<OBJECT-1;i++)
			if(i!=20&&i!=9)
				if(i==12||i==21||i==18)
					if(i==12||i==18)
						rotarFigura(i,true);
					else
						rotarFigura(i,false);
				else
					glCallList(Escenario+i);
	glPopMatrix();

	glClear(GL_STENCIL_BUFFER_BIT);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDepthMask(GL_FALSE);
	glEnable(GL_CULL_FACE);
	glEnable( GL_STENCIL_TEST );
	glStencilFunc(GL_ALWAYS, 1, 0xFFFFFFFF); 
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

	texture[1].Bind();
	glCallList(Escenario+1);

	glBindTexture(GL_TEXTURE_2D, 0);
	glStencilFunc(GL_EQUAL, 1, 0xFFFFFFFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
	glDisable(GL_CULL_FACE);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(-1.0f, 1.0f);
	glDepthMask(GL_TRUE);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glColor3f(0.0, 0.0, 0.0);
	
	for(int i=0;i<3;++i)
		v[i] = figura[1].vertex[i+1];
		ShadowMatrix(shadowMatrix, pointLuzDir, EncontrarPlano(v[0], v[1], v[2]));
		glPushMatrix();
		glMultMatrixf((GLfloat*)shadowMatrix);
		for(int i=6;i<OBJECT-1;i++)
				if(i==12||i==21||i==18)
					if(i==12||i==18)
						rotarFigura(i,true);
					else
						rotarFigura(i,false);
				else
					glCallList(Escenario+i);
	glPopMatrix();

	glDisable(GL_POLYGON_OFFSET_FILL);
	glEnable(GL_CULL_FACE);
	glDisable(GL_STENCIL_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
}

void Reflection() {
	glClear(GL_STENCIL_BUFFER_BIT);
	double eqr[] = {0.0f, 0.0f, 0.0f, 0.0f};
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 0xFFFFFFFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glDisable(GL_DEPTH_TEST);
		texture[0].Bind();
		glCallList(Escenario);
	glEnable(GL_DEPTH_TEST);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glStencilFunc(GL_EQUAL, 1, 0xFFFFFFFF);				
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glEnable(GL_CLIP_PLANE0);
	glClipPlane(GL_CLIP_PLANE0, eqr);
	glCullFace(GL_FRONT);
	glPushMatrix();
		glScalef(1.0f, -1.0f, 1.0f);
		glLightfv(GL_LIGHT0, GL_POSITION, luzDireccion);
		for(int i = 1; i < OBJECT-1; i++) {
			texture[i].Bind();
			if(i==12||i==21||i==18)
				if(i==12||i==18)
					rotarFigura(i,true);
				else
					rotarFigura(i,false);
			else
				glCallList(Escenario+i);
		}
		texture[OBJECT-1].BindCube(GL_TEXTURE0);
		sh.useShader(true);
		point m_vPosition;
		m_vPosition.x = -xCol;
		m_vPosition.y = -5.0f;
		m_vPosition.z = -zCol;
		sh.setEyePos(m_vPosition);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPushMatrix();
			glCallList(Escenario+OBJECT-1);
		glPopMatrix();
		glDisable(GL_BLEND);
		texture[OBJECT-1].UnBindCube();
		sh.useShader(false);	
	glPopMatrix();
	glCullFace(GL_BACK);
	glLightfv(GL_LIGHT0, GL_POSITION, luzDireccion);
	glDisable(GL_CLIP_PLANE0);
	glDisable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glColor4f(1.0f, 1.0f, 1.0f, 0.2f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		texture[0].Bind();
		glCallList(Escenario);
	glEnable(GL_LIGHTING);								
	glDisable(GL_BLEND);
}


GLvoid SombrasVolumetricas() {


	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	for(GLint i = 0; i < OBJECT-1; ++i) {
			texture[i].Bind();
			if(i==12||i==21||i==18)
				if(i==12||i==18)
					rotarFigura(i,true);
				else
					rotarFigura(i,false);
			else
				glCallList(Escenario+i);
	}
	glDepthMask(GL_FALSE);
	glClear(GL_STENCIL_BUFFER_BIT);
	glEnable(GL_STENCIL_TEST); 
	glStencilFunc(GL_ALWAYS, 0, ~0);
	point pointLuzDir;
	pointLuzDir.x = -luzDireccion[0];
	pointLuzDir.y = -luzDireccion[1];
	pointLuzDir.z = -luzDireccion[2];
	pointLuzDir.w = -luzDireccion[3];

	figura[SOMVOL].VerificarVisibilidad(pointLuzDir);
	glCullFace(GL_FRONT); 
	glStencilOp(GL_KEEP, GL_INCR, GL_KEEP);
	figura[SOMVOL].DibujarSombra(pointLuzDir);
	glCullFace(GL_BACK); 
	glStencilOp(GL_KEEP, GL_DECR, GL_KEEP);
	figura[SOMVOL].DibujarSombra(pointLuzDir);
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glStencilFunc(GL_NOTEQUAL, 0, ~0); 
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
	glDepthFunc(GL_EQUAL);
	glColorMask(1, 1, 1, 1);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	for(GLint i = 0; i < OBJECT-1; ++i) {
			texture[i].Bind();
			if(i==12||i==21||i==18)
				if(i==12||i==18)
					rotarFigura(i,true);
				else
					rotarFigura(i,false);
			else
				glCallList(Escenario+i);
	}
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glDepthFunc(GL_LESS);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_BLEND);
	glDepthMask(1);
}

GLvoid mirar() {
	if(xMir > 45.0)
		xMir = 45.0;
	else if(xMir < -45.0)
		xMir = -45.0;
	while(yMir < 0.0)
		yMir += 360.0;
	while(yMir >= 360.0)
		yMir -= 360.0;
}

GLvoid camara() {
	glLoadIdentity();
	glRotatef(xMir, 1.0, 0.0, 0.0);
	glRotatef(yMir, 0.0, 1.0, 0.0);
		/*gluLookAt(xCol,5.0f,zCol,
		xCol,5.0f,-zCol,
		0.0f,1.0f,0.0f);*/
	glTranslated(-xCol, -5.0f, -zCol);
}

void lecturaOBJ(string OBJ,int indice){
	figura[indice].clear();
	ifstream entrada(OBJ.c_str(),ios::in);
	point XYZ;
	string letra,aux_letra;
	string slash ("/");
	size_t found;

	int T,C,N;
	int cara[3];
	int c_normal[3];
	int textura[3];
	int objetos =0;
	if(entrada.is_open()){
		entrada>>letra;
		while(!entrada.eof()){
			if(letra=="v"){
				entrada>>XYZ.x>>XYZ.y>>XYZ.z;
				figura[indice].agregarVertice(XYZ);
			}
			if(letra=="vt"){
				entrada>>XYZ.x>>XYZ.y;
				figura[indice].agregarTextura(XYZ.x,XYZ.y);
			}
			if(letra=="vn"){
				entrada>>XYZ.x>>XYZ.y>>XYZ.z;
				figura[indice].agregarNormal(XYZ);
			}
			if(letra=="o")
			{
				entrada>>letra;
				if(objetos!=0){
					figura[indice].terminoObjeto();
					objetos++;
				}
			}
			if(letra=="f"){
				if(objetos==0) objetos++;
				int i=0;
				while((entrada>>letra)&&(letra[0]>=48&&letra[0]<=57)){
					C=0;T=0;N=0;

					found=letra.find_first_of(slash);
					if (found!=string::npos){
						aux_letra=letra.substr(0,found);
						C=atoi(aux_letra.c_str());

						letra=letra.substr(found+1);

						found=letra.find_first_of(slash);
						if(letra[0]=='/'){
							//vertice y norma
							letra=letra.substr(1);
							N=atoi(letra.c_str());

						}else if (found!=string::npos){
							//vertice textura y norma
							aux_letra=letra.substr(0,found);
							T=atoi(aux_letra.c_str());
							letra=letra.substr(found+1);
							N=atoi(letra.c_str());

						}else{
							//vertice y textura
							aux_letra=letra.substr(found+1,letra.size());
							T=atoi(aux_letra.c_str());
						}
					}else{
						//solo el vertice
						C=atoi(letra.c_str());
					}
					//agregar cara
					if(i==0){
						cara[0]=C-1;
						textura[0]=T-1;
						c_normal[0]=N-1;
						i++;
					}else if(i==1){
						cara[1]=C-1;
						textura[1]=T-1;
						c_normal[1]=N-1;
						i++;
					}else{
						cara[2]=C-1;
						textura[2]=T-1;
						c_normal[2]=N-1;
						figura[indice].agregarCara(cara,textura,c_normal);
						cara[1]=cara[2];
						textura[1]=textura[2];
						c_normal[1]=c_normal[2];
						
					}
				}
				continue;
			}
			entrada>>letra;
		}

	}else{
		cout<<"no se pudo abrir el archivo"<<endl;	
	}
	figura[indice].terminoObjeto();
	figura[indice].CalcularNorma();

	for(unsigned int i=0;i<figura[indice].individual_box.size();i++)
	{
		Colision.AgregarObjeto(figura[indice].individual_box[i].min,figura[indice].individual_box[i].max);
	}
	figura[indice].CalcularLosPlanos();

}

void CargarModelo(int Modelo,string archivo){
	if(archivo!=""){
		lecturaOBJ(archivo, Modelo);
	}
}

void FilesToLoad(int Modelo){
	string archivo="";
	string archivo_textura="";
	if(Modelo==0){
		archivo="./Project/files/Modelos/abajo.obj";
		archivo_textura="./Project/files/Texturas/Skybox/Bottom.jpg";
	}else if(Modelo==1){
		archivo="./Project/files/Modelos/adelante.obj";
		archivo_textura="./Project/files/Texturas/Skybox/Front.jpg";
	}else if(Modelo==2){
		archivo="./Project/files/Modelos/atras.obj";
		archivo_textura="./Project/files/Texturas/Skybox/Back.jpg";
	}else if(Modelo==3){
		archivo="./Project/files/Modelos/derecha.obj";
		archivo_textura="./Project/files/Texturas/Skybox/Right.jpg";
	}else if(Modelo==4){
		archivo="./Project/files/Modelos/izquierda.obj";
		archivo_textura="./Project/files/Texturas/Skybox/Left.jpg";
	}else if(Modelo==5){
		archivo="./Project/files/Modelos/arriba.obj";
		archivo_textura="./Project/files/Texturas/Skybox/Up.jpg";
	}else if(Modelo==6){
		archivo="./Project/files/Modelos/techo.obj";
		archivo_textura="./Project/files/Texturas/techo.jpg";
	}else if(Modelo==7){
		archivo="./Project/files/Modelos/truck.obj";
		archivo_textura="./Project/files/Texturas/watertruck.bmp";
	}else if(Modelo==8){
		archivo="./Project/files/Modelos/columnas.obj";
		archivo_textura="./Project/files/Texturas/verde.jpg";
	}else if(Modelo==9){
		archivo="./Project/files/Modelos/borde.obj";
		archivo_textura="./Project/files/Texturas/borde.jpg";
	}else if(Modelo==10){
		archivo="./Project/files/Modelos/jeep.obj";
		archivo_textura="./Project/files/Texturas/jeep.jpg";
	}else if(Modelo==11){
		archivo="./Project/files/Modelos/thor.obj";
		archivo_textura="./Project/files/Texturas/thor.jpg";
	}else if(Modelo==12){
		archivo="./Project/files/Modelos/ironman.obj";
		archivo_textura="./Project/files/Texturas/ironman.jpg";
	}else if(Modelo==13){
		archivo="./Project/files/Modelos/oiltank.obj";
		archivo_textura="./Project/files/Texturas/oiltank.png";
	}else if(Modelo==14){
		archivo="./Project/files/Modelos/basura.obj";
		archivo_textura="./Project/files/Texturas/basura.jpg";
	}else if(Modelo==15){
		archivo="./Project/files/Modelos/barrel.obj";
		archivo_textura="./Project/files/Texturas/barrel.png";
	}else if(Modelo==16){
		archivo="./Project/files/Modelos/cow.obj";
		archivo_textura="./Project/files/Texturas/cow.jpg";
	}else if(Modelo==17){
		archivo="./Project/files/Modelos/bench.obj";
		archivo_textura="./Project/files/Texturas/bench.jpg";
	}else if(Modelo==18){
		archivo="./Project/files/Modelos/r2d2.obj";
		archivo_textura="./Project/files/Texturas/r2d2.tga";
	}else if(Modelo==19){
		archivo="./Project/files/Modelos/box.obj";
		archivo_textura="./Project/files/Texturas/box.jpg";
	}else if(Modelo==20){
		archivo="./Project/files/Modelos/tabla.obj";
		archivo_textura="./Project/files/Texturas/tabla.jpg";
	}else if(Modelo==21){
		archivo="./Project/files/Modelos/Wheatley.obj";
		archivo_textura="./Project/files/Texturas/Wheatley.jpg";
	}else if(Modelo==22) {
		archivo="./Project/files/Modelos/refraccion2.obj";
		texture[Modelo].Init("./Project/files/Texturas/Skybox/");
		texture[Modelo].cargarMapaCubico("./Project/files/Texturas/Skybox/right.jpg","./Project/files/Texturas/Skybox/left.jpg",
				"./Project/files/Texturas/Skybox/top.jpg","./Project/files/Texturas/Skybox/bottom.jpg",
				"./Project/files/Texturas/Skybox/front.jpg","./Project/files/Texturas/Skybox/back.jpg");
	}
	if(Modelo<=OBJECT-1) {
		CargarModelo(Modelo,archivo);
		texture[Modelo].Init(archivo_textura);
		texture[Modelo].CargarTextura();
	}
}

void TW_CALL CargarTextura(void *clientData){
	int *x=(int*)clientData;

	string archivo=openfilenameImg();
	if(archivo!=""){
		texture[*x].Init(archivo);
		texture[*x].CargarTextura();
	}
}


void TW_CALL pressExit(void *clientData)
{ 
	TwTerminate();
	exit(0);
}


void init(){
	Escenario = glGenLists(OBJECT);
	xCol=0, zCol=0;
	sh.initShader();
	sh.createShader("./Project/files/Shader/refraction.vert","./Project/files/Shader/refraction.frag","Refraction");
	glEnable(GL_TEXTURE_CUBE_MAP_ARB);

	iluminacion = true;
	Colision.limpiar();
	for(int i=0;i<OBJECT;i++){
		activo[i]=i;
		FilesToLoad(i);
		glNewList(Escenario+i, GL_COMPILE);
			figura[i].Pintar();
		glEndList();
	}
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);

	TwInit(TW_OPENGL, NULL);
	TwWindowSize(w,h);
	glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
	TwGLUTModifiersFunc(glutGetModifiers);
	TwBar *myBar;
	myBar = TwNewBar("Opciones");
	
	TwAddVarRW(myBar, "LightDir0", TW_TYPE_DIR3F, &luzDireccion[0], " label='Direccion Luz 0'  group='Luz 0'");

	TwAddButton(myBar,"Salir", pressExit,NULL,"label='Salir' group=Archivo");
	

}

void Init_Light(){

	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHT0);

	luzDireccion[0] = 8000.0f, luzDireccion[1] = -3000.0f,luzDireccion[2] = 4500.0f,luzDireccion[3] = 0.0f;
	luzAmbiental[0] =1.0f,luzAmbiental[1] =1.0f,luzAmbiental[2] =1.0f;
	luzDifusa[0] = 1.0f, luzDifusa[1] = 0.5f,luzDifusa[2] = 0.0f;
	luzEspecular[0] = 1.0f, luzEspecular[1] = 0.5f,luzEspecular[2] = 0.0f;
	global[0] = 1.0f, global[1] = 0.5f,global[2] = 0.0f,global[3]= 0.0f;

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global);
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiental);    
	glLightfv(GL_LIGHT0,GL_DIFFUSE, luzDifusa); 
	glLightfv(GL_LIGHT0,GL_SPECULAR, luzEspecular);  

	glMaterialfv(GL_FRONT,GL_AMBIENT, materialAmbiental);
	glMaterialfv(GL_FRONT,GL_DIFFUSE, materialDifuso);
	glMaterialfv(GL_FRONT,GL_SPECULAR, materialEspecular);
	glMaterialf(GL_FRONT,GL_SHININESS, materialBrillo);




}

void LucesDir(){
	glLightfv(GL_LIGHT0,GL_DIFFUSE, luzDifusa); 
	glPushMatrix();
		glTranslatef(-luzDireccion[0],-luzDireccion[1],-luzDireccion[2]);	
		glLightfv(GL_LIGHT0, GL_POSITION, luzDireccion);
		gluSphere(gluNewQuadric(), 100, 10, 10);
		glEnable(GL_LIGHTING);
	glPopMatrix();


}

void movimiento(int x,int y){
	if(!TwEventMouseMotionGLUT(x,y) ){


	}

	glutPostRedisplay();

}

void Draw(){
    glPushMatrix();//salvo la traslacion y el escalamiento

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);//rellena la figura

	for(int i=1;i<OBJECT-1;i++){
		if(texture[i].activa){
			texture[i].Bind();
		}else{
			glBindTexture( GL_TEXTURE_2D, 0 );
		}
		if(i==12||i==21||i==18)
			if(i==12||i==18)
				rotarFigura(i,true);
			else
				rotarFigura(i,false);
		else
			glCallList(Escenario+i);
	}

	Reflection();
	PlanarShadow();
	SombrasVolumetricas();

	/**********/
	texture[OBJECT-1].BindCube(GL_TEXTURE0);
	sh.useShader(true);
	point m_vPosition;
	m_vPosition.x = -xCol;
	m_vPosition.y = -5.0f;
	m_vPosition.z = -zCol;
	sh.setEyePos(m_vPosition);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCallList(Escenario+OBJECT-1);
	glDisable(GL_BLEND);
	texture[OBJECT-1].UnBindCube();
	sh.useShader(false);
	glPopMatrix();
	glLoadIdentity();
}

void teclado(unsigned char key, int x, int y){
		float auxX = xCol, auxZ = zCol;
		if(key == 'w') {
			auxX += GLfloat(sin(yMir / 180 * PI));
			auxZ -= GLfloat(cos(yMir / 180 * PI));
			if(Colision.Colision(auxX,auxZ))//al frente
			{
				xCol=auxX;
				zCol=auxZ;
			}else if(Colision.Colision(xCol,auxZ))//eje z
				zCol=auxZ;
			
			else if(Colision.Colision(auxX,zCol))//eje <
				xCol=auxX;
		}
		if(key == 'd') {
			yMir +=  2.5;
			if(yMir >360)
	
			yMir -= 360;
		}
		if(key == 's') {
			auxX -= GLfloat(sin(yMir / 180 * PI));
			auxZ += GLfloat(cos(yMir / 180 * PI));
			if(Colision.Colision(auxX,auxZ))//al frente
			{
				xCol=auxX;
				zCol=auxZ;
			}else if(Colision.Colision(xCol,auxZ))//eje z
				zCol=auxZ;
			else if(Colision.Colision(auxX,zCol))//eje x
				xCol=auxX;
		}
		if(key == 'a') {
			yMir -=  2.5;
			if(yMir < -360)
				yMir += 360;
		}
		if(key == 27)
			exit(0);
	glutPostRedisplay();
}

void teclado_especiales(int key, int x, int y){
		if(key == GLUT_KEY_UP) {
			xMir -= 2.5;
			mirar();
		}
		if(key == GLUT_KEY_DOWN) {
			xMir += 2.5;
			mirar();
		}
	glutPostRedisplay();
}

GLvoid idle() {

	int tiempo = glutGet(GLUT_ELAPSED_TIME);
	if(time == 0 || tiempo >= time + 20) {
		rotl=rotl+1;
		time = tiempo;
	}

	glutPostRedisplay();
}

void renderScene(void) {//renderizar

	glClearColor(0.0, 0.0, 0.0, 0.0);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);

	glEnable(GL_NORMALIZE);
	camara();
	LucesDir();
	Draw();

	TwDraw();
	glutSwapBuffers();
}


void click_izquierdo(int boton, int estado, int x, int y) 
{
	float x1 =2.0f*(float)x/(float)w-1.0f;//redimenciono los puntos al tamaño de mi ventana
	float y1= - (2.0f*(float)y/h - 1.0f);
	if( !TwEventMouseButtonGLUT(boton,estado,x,y) )  // send event to AntTweakBar
	{
		if(estado==GLUT_DOWN){
			X=(float)x;
			Y=(float)y;
			Z=(float)y;
		}
	}
	glutPostRedisplay();
}

void reshape(int x, int y){//redimensiono la pantalla
	
	w = x;
	h = y;
	glViewport(0, 0, w, h);	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, ((GLfloat) w) / ((GLfloat) h), 1.0f, 500000.0f);
	glMatrixMode(GL_MODELVIEW);
	TwWindowSize(w, h);
	glLoadIdentity();
}


//Main Principal
int main(int argc, char *argv[])
{	
	w = 800;
	h = 600;
	xCol  = zCol = xMir = yMir = zMir = 0.0f;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(w,h);
	glutCreateWindow("Grafica II - Proyecto 2 - Alejandro Sans y Victor Felipe");


	glutDisplayFunc(renderScene);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(teclado);//para usar el teclado con AntTweakBar fluidamente
	glutSpecialFunc(teclado_especiales);
	glutMouseFunc(click_izquierdo);
	glutMotionFunc(movimiento);//para mover el mouse con AntTweakBar fluidamente
	init();
	Init_Light();

	glutMainLoop();
	return 0;
}
