#include "imageloader.h"
#include <stdio.h> //needed for printf command
#include <iostream>
#include <fstream>
#ifdef __APPLE__
//#  include <OpenGL/gl.h>
//#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
//#  include <GL/gl.h>
//#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

using namespace std;

Image::Image(char* ps, int w, int h) : pixels(ps), width(w), height(h) {	//constructor

}
Image::~Image(){

}
namespace {
	int toInt(const char* bytes){
		return (int) (((unsigned char)bytes[3] << 24) |
			((unsigned char)bytes[2] << 16) |
			((unsigned char)bytes[1] << 8) |
			(unsigned char)bytes[0]);
	}
	int readInt(ifstream &input){
		char buffer[4];
		input.read(buffer, 4);
		return toInt(buffer);
	}
}

Image* loadBMP(const char* filename) {
	ifstream infile(filename, ifstream::binary);
	infile.seekg(10, std::ios::cur);
	int dataOffset = readInt(infile);

	int headerSize = readInt(infile);
	int width = readInt(infile);
	int height = readInt(infile);

	int bytesPerRow = ((width * 3 + 3)/4)*4 - (width * 3 % 4);
	int size = bytesPerRow * height;
	char* pixels = new char[size];
	infile.seekg(dataOffset, ios_base::beg);
	infile.read(pixels, size);
	infile.close();

	char* pixels2 = new char[width * height * 3];
	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){
			for(int c = 0; c < 3; c++){
				pixels2[3*(width*y + x) + c] = pixels[bytesPerRow * y + 3 * x + (2 - c)];
			}
		}
	}
	delete[] pixels;
	return new Image(pixels2, width, height);
}


