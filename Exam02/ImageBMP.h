//**********************************************************
// ITESO - Master's Degree in Computer Systems
// Computer Graphics
// Exam 02
//
// Mario Contreras (705080)
//
//*********************************************************
#pragma once
#include "GL\glew.h"

class CImageBMP {
  unsigned long m_ulSizeX; //Pixeles horizontales
  unsigned long m_ulSizeY; //Pixeles verticales
  struct PIXEL {
    unsigned char r, g, b, a;
  };
  PIXEL* m_pBuffer; //Buffer de pixeles.
public:
  static CImageBMP* CreateBitmapFromFile(char* pszFileName, PIXEL(*pFnAlpha)(PIXEL& P));
  GLuint            CreateTexture();
  static void       DestroyBitmap(CImageBMP* pBmp);
protected:
  CImageBMP();
  ~CImageBMP();
};

// EOF
