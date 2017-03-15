#pragma once

#include <Commdlg.h>

string openfilenameOBJ(HWND owner = NULL) {
	OPENFILENAMEA ofn;

	char fileName[MAX_PATH] = "";
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = owner;
	ofn.lpstrFilter = "OBJ files (*.obj)\0*.obj\0";
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "obj";
	ofn.lpstrTitle = "Abrir";

	if (GetOpenFileNameA(&ofn) )
		return ofn.lpstrFile;
	else 
		return "";
}

string openfilenameImg(HWND owner = NULL) {
	OPENFILENAMEA ofn;

	char fileName[MAX_PATH] = "";
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = owner;
	ofn.lpstrFilter = "Todos los formatos de imagenes\0*.bmp;*.dds;*.tiff;*.jpg;*.jpeg;*.gif;*.png;*.tga\0BMP files (*.bmp)\0*.bmp\0DDS files (*.dds)\0*.dds\0TIFF files (*.tiff)\0*.tiff\0PNG files (*.png)\0*.png\0JPG files (*.jpg)\0*.jpg;*.jpeg\0";
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "Imagen";
	ofn.lpstrTitle = "Abrir";

	if (GetOpenFileNameA(&ofn) )
		return ofn.lpstrFile;
	else 
		return "";
}
