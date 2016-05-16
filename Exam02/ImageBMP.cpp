//**********************************************************
// ITESO - Master's Degree in Computer Systems
// Computer Graphics
// Exam 02
//
// Mario Contreras (705080)
//
//*********************************************************
#include "stdafx.h"
#include "ImageBMP.h"
#include <fstream>
#include <Windows.h>

using namespace std;


CImageBMP::CImageBMP() {
  m_pBuffer = 0;
  m_ulSizeX = 0;
  m_ulSizeY = 0;
}

CImageBMP::~CImageBMP() {
}

CImageBMP* CImageBMP::CreateBitmapFromFile(char* pszFileName, PIXEL(*pFnAlpha)(PIXEL& P)) {
  CImageBMP* pNewImage = 0;
  fstream file;
  BITMAPFILEHEADER bfh;
  BITMAPINFOHEADER bih;

  file.open(pszFileName, ios::in | ios::binary);
  if (!file.is_open())
    return 0;

  memset(&bfh, 0, sizeof(bfh));
  memset(&bih, 0, sizeof(bih));
  file.read((char*)&bfh.bfType, sizeof(WORD));
  if (bfh.bfType != 'MB')
    return 0;

  file.read((char*)&bfh.bfSize, sizeof(bfh) - sizeof(WORD));
  file.read((char*)&bih.biSize, sizeof(DWORD));
  if (bih.biSize != sizeof(BITMAPINFOHEADER))
    return 0;

  file.read((char*)&bih.biWidth, sizeof(bih) - sizeof(DWORD));
  //Ya se tiene la información del bitmap en 
  //bih. Se procede con la carga de datos.
  unsigned long RowLength = 4 * ((bih.biBitCount*bih.biWidth + 31) / 32);
  switch (bih.biBitCount) {
  case 1: // TODO: 1bpp
    break;
  case 4: // TODO: 4bpp
    break;
  case 8: // 8bpp 256 colores
  {
    RGBQUAD Paleta[256];
    unsigned long Colors = bih.biClrUsed == 0 ? 256 : bih.biClrUsed;
    unsigned char *pRow = new unsigned char[RowLength];

    file.read((char*)Paleta, sizeof(RGBQUAD)*Colors);
    pNewImage = new CImageBMP();
    pNewImage->m_ulSizeX = bih.biWidth;
    pNewImage->m_ulSizeY = bih.biHeight;
    pNewImage->m_pBuffer = new PIXEL[bih.biWidth*bih.biHeight];

    for (unsigned long j = 0; j < bih.biHeight; j++) {
      file.read((char*)pRow, RowLength);
      for (unsigned long i = 0; i < bih.biWidth; i++) {
        PIXEL* p = &pNewImage->m_pBuffer[(bih.biHeight - j - 1) * bih.biWidth + i];
        p->b = Paleta[pRow[i]].rgbBlue;
        p->g = Paleta[pRow[i]].rgbGreen;
        p->r = Paleta[pRow[i]].rgbRed;
        p->a = Paleta[pRow[i]].rgbReserved;
      }
    }
    return pNewImage;
  }
  break;
  case 24: //TODO: 24bpp 16M Colores
    break;
  case 32: //TODO: 32bpp 16M Colores 256 alphas
    break;
  }
  return NULL;
}


GLuint CImageBMP::CreateTexture() {
  GLuint TexID = 0;

  glGenTextures(1, &TexID);
  glBindTexture(GL_TEXTURE_2D, TexID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_ulSizeX, m_ulSizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pBuffer);
  //GLenum  e = glGetError();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

  return TexID;
}

void CImageBMP::DestroyBitmap(CImageBMP* pBmp) {
  delete[] pBmp->m_pBuffer;
  delete pBmp;
}

// EOF
