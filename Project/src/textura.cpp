#pragma once
#include "textura.h"

	
	textura::textura(){
		activa=true;
	}
	textura::~textura(){
	
	}

	void textura::Init(string fname){
		archivo=fname;
	}

	void textura::cargarMapaCubico(char *px,char *nx,char *py,char *ny,char *pz,char *nz){
		FREE_IMAGE_FORMAT fifmt;

		fifmt = FreeImage_GetFileType(px, 0);
		FIBITMAP *dibpx = FreeImage_ConvertTo24Bits(FreeImage_Load(fifmt, px,0));
		fifmt = FreeImage_GetFileType(nx, 0);
		FIBITMAP *dibnx = FreeImage_ConvertTo24Bits(FreeImage_Load(fifmt, nx,0));
		fifmt = FreeImage_GetFileType(py, 0);
		FIBITMAP *dibpy = FreeImage_ConvertTo24Bits(FreeImage_Load(fifmt, ny,0));
		fifmt = FreeImage_GetFileType(ny, 0);
		FIBITMAP *dibny = FreeImage_ConvertTo24Bits(FreeImage_Load(fifmt, py,0));
		fifmt = FreeImage_GetFileType(pz, 0);
		FIBITMAP *dibpz = FreeImage_ConvertTo24Bits(FreeImage_Load(fifmt, pz,0));
		fifmt = FreeImage_GetFileType(nz, 0);
		FIBITMAP *dibnz = FreeImage_ConvertTo24Bits(FreeImage_Load(fifmt, nz,0));



		unsigned char *bitspx = new unsigned char[FreeImage_GetWidth(dibpx) * FreeImage_GetHeight(dibpx) * 3];
		unsigned char *bitsnx = new unsigned char[FreeImage_GetWidth(dibnx) * FreeImage_GetHeight(dibnx) * 3];
		unsigned char *bitspy = new unsigned char[FreeImage_GetWidth(dibpy) * FreeImage_GetHeight(dibpy) * 3];
		unsigned char *bitsny = new unsigned char[FreeImage_GetWidth(dibny) * FreeImage_GetHeight(dibny) * 3];
		unsigned char *bitspz = new unsigned char[FreeImage_GetWidth(dibpz) * FreeImage_GetHeight(dibpz) * 3];
		unsigned char *bitsnz = new unsigned char[FreeImage_GetWidth(dibnz) * FreeImage_GetHeight(dibnz) * 3];

		unsigned char *pixelspx = (unsigned char*)FreeImage_GetBits(dibpx);
		unsigned char *pixelsnx = (unsigned char*)FreeImage_GetBits(dibnx);
		unsigned char *pixelspy = (unsigned char*)FreeImage_GetBits(dibpy);
		unsigned char *pixelsny = (unsigned char*)FreeImage_GetBits(dibny);
		unsigned char *pixelspz = (unsigned char*)FreeImage_GetBits(dibpz);
		unsigned char *pixelsnz = (unsigned char*)FreeImage_GetBits(dibnz);


		for(unsigned int pix=0; pix < FreeImage_GetWidth(dibpx) * FreeImage_GetHeight(dibpx); pix++){
			bitspx[pix*3+0]=pixelspx[pix*3+2];
			bitspx[pix*3+1]=pixelspx[pix*3+1];
			bitspx[pix*3+2]=pixelspx[pix*3+0];
		}
		for(unsigned int pix=0; pix < FreeImage_GetWidth(dibnx) * FreeImage_GetHeight(dibnx); pix++){
			bitsnx[pix*3+0]=pixelsnx[pix*3+2];
			bitsnx[pix*3+1]=pixelsnx[pix*3+1];
			bitsnx[pix*3+2]=pixelsnx[pix*3+0];
		}
		for(unsigned int pix=0; pix < FreeImage_GetWidth(dibpy) * FreeImage_GetHeight(dibpy); pix++){
			bitspy[pix*3+0]=pixelspy[pix*3+2];
			bitspy[pix*3+1]=pixelspy[pix*3+1];
			bitspy[pix*3+2]=pixelspy[pix*3+0];
		}
		for(unsigned int pix=0; pix < FreeImage_GetWidth(dibny) * FreeImage_GetHeight(dibny); pix++){
			bitsny[pix*3+0]=pixelsny[pix*3+2];
			bitsny[pix*3+1]=pixelsny[pix*3+1];
			bitsny[pix*3+2]=pixelsny[pix*3+0];
		}
		for(unsigned int pix=0; pix < FreeImage_GetWidth(dibpz) * FreeImage_GetHeight(dibpz); pix++){
			bitspz[pix*3+0]=pixelspz[pix*3+2];
			bitspz[pix*3+1]=pixelspz[pix*3+1];
			bitspz[pix*3+2]=pixelspz[pix*3+0];
		}
		for(unsigned int pix=0; pix < FreeImage_GetWidth(dibnz) * FreeImage_GetHeight(dibnz); pix++){
			bitsnz[pix*3+0]=pixelsnz[pix*3+2];
			bitsnz[pix*3+1]=pixelsnz[pix*3+1];
			bitsnz[pix*3+2]=pixelsnz[pix*3+0];
		}

		/** Aqui viene la parte importante. **/
		// Generar la textura.
		glEnable(GL_TEXTURE_CUBE_MAP_ARB);
		glGenTextures( 1, &tex );
		// Adjuntar la unidad de textura a un mapa cúbico.
		glActiveTexture(GL_TEXTURE0);
		glBindTexture( GL_TEXTURE_CUBE_MAP_ARB, tex );
		// Poner los parámetros para el mapa cúbico en general.
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Si no pones los filtros en GL_NEAREST salen bordes blancos en las zonas donde
		// se encuentran las caras del cubo.
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		// Cargar las 6 caras del cubo en OpenGL.
		gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB, 3, FreeImage_GetWidth(dibpx), FreeImage_GetHeight(dibpx), GL_RGB, GL_UNSIGNED_BYTE, bitspx);
		gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB, 3, FreeImage_GetWidth(dibpx), FreeImage_GetHeight(dibpx), GL_RGB, GL_UNSIGNED_BYTE, bitsnx);
		gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB, 3, FreeImage_GetWidth(dibpx), FreeImage_GetHeight(dibpx), GL_RGB, GL_UNSIGNED_BYTE, bitspy);
		gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB, 3, FreeImage_GetWidth(dibpx), FreeImage_GetHeight(dibpx), GL_RGB, GL_UNSIGNED_BYTE, bitsny);
		gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB, 3, FreeImage_GetWidth(dibpx), FreeImage_GetHeight(dibpx), GL_RGB, GL_UNSIGNED_BYTE, bitspz);
		gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB, 3, FreeImage_GetWidth(dibpx), FreeImage_GetHeight(dibpx), GL_RGB, GL_UNSIGNED_BYTE, bitsnz);
		/** Aqui terminó la parte importante. **/

		FreeImage_Unload(dibpx);
		FreeImage_Unload(dibnx);
		FreeImage_Unload(dibpy);
		FreeImage_Unload(dibny);
		FreeImage_Unload(dibpz);
		FreeImage_Unload(dibnz);
		delete bitspx;
		delete bitsnx;
		delete bitspy;
		delete bitsny;
		delete bitspz;
		delete bitsnz;
		glDisable(GL_TEXTURE_CUBE_MAP_ARB);
	}

	void textura::CargarTextura(){
		GLint enteroAux;
		FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(archivo.c_str(), 0);

		FIBITMAP *dib = FreeImage_Load(fifmt, archivo.c_str(),0);

		dib = FreeImage_ConvertTo24Bits(dib);
		
		if( dib != NULL )
		{
			glGenTextures( 1, &tex );
			glBindTexture(GL_TEXTURE_2D, tex );
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

			BYTE *bits = new BYTE[FreeImage_GetWidth(dib) * FreeImage_GetHeight(dib) * 3];

			BYTE *pixels = (BYTE*)FreeImage_GetBits(dib);
			enteroAux = FreeImage_GetWidth(dib) * FreeImage_GetHeight(dib);


				for(int pix=0; pix < enteroAux; pix++)
				{
					bits[pix*3+0]=pixels[pix*3+2];
					bits[pix*3+1]=pixels[pix*3+1];
					bits[pix*3+2]=pixels[pix*3+0];

				}
			     
				glTexImage2D( GL_TEXTURE_2D, 0, 3, FreeImage_GetWidth(dib), FreeImage_GetHeight(dib), 0,
						GL_RGB, GL_UNSIGNED_BYTE, bits );

				
				FreeImage_Unload(dib);
				delete bits;
		}
	}

	void textura::Bind(){
		glBindTexture( GL_TEXTURE_2D, tex );
	}

	void textura::BindCube(GLuint texind) {
		//glActiveTexture(texind);
		//if(glIsTexture(tex))
		glEnable(GL_TEXTURE_CUBE_MAP_ARB);
		glBindTexture( GL_TEXTURE_CUBE_MAP_ARB, tex );
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //modulate the texture// You should know what this is;)
		/*glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_REFLECTION_MAP_ARB);
		glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_REFLECTION_MAP_ARB);
		glTexGeni(GL_R,GL_TEXTURE_GEN_MODE,GL_REFLECTION_MAP_ARB);
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glEnable(GL_TEXTURE_GEN_R);*/
	}

	void textura::UnBindCube() {
	/*	glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_GEN_R)*/;

		glDisable(GL_TEXTURE_CUBE_MAP_ARB);
		glBindTexture(GL_TEXTURE_2D,0);
	}
