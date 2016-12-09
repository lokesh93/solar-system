#ifndef __IMAGELOADER_H__INCLUDED	//guard against cyclic dependancy
#define __IMAGELOADER_H__INCLUDED

class Image{
public:
	Image(char* ps, int w, int h);
	~Image();	//constructor
	char* pixels;
	int width;
	int height;
};

Image* loadBMP(const char* filename);

#endif