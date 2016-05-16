//**********************************************************
// ITESO - Master's Degree in Computer Systems
// Computer Graphics
// Final Project - Basic Map Editor
//
// Mario Contreras (705080)
//
//*********************************************************
// OBJ loader based on:
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
//*********************************************************
#include "stdafx.h"
#include "objloader.h"

#include <stdio.h>
#include <string>
#include <cstring>

#include "Mesh.h"


bool loadOBJ(const char * path, std::vector<CMesh::VERTEX>& out_vertices, vector<unsigned long>& out_indices, VECTOR4D c) {
  out_vertices.clear();
  out_indices.clear();

  FILE* file = fopen(path, "r");
  if (file == NULL) {
    return false;
  }

  while (1) {
    char lineHeader[128];
    // read the first word of the line
    int res = fscanf(file, "%s", lineHeader);
    if (res == EOF)
      break; // EOF = End Of File. Quit the loop.
    // else : parse lineHeader
    if (strcmp(lineHeader, "v") == 0) {
      CMesh::VERTEX vertex;
      float x, y, z;
      
      fscanf(file, "%f %f %f\n", &x, &y, &z);
      vertex.Position.x = x;
      vertex.Position.y = y;
      vertex.Position.z = z;
      vertex.Position.w = 1.0f;
      vertex.Color = c;

      out_vertices.push_back(vertex);
    }
    else if (strcmp(lineHeader, "f") == 0) {
      //std::string vertex1, vertex2, vertex3;
      unsigned int vertexIndex[3];
      int matches = fscanf(file, "%d %d %d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
      if (matches != 3) {
        return false;
      }
      // out_indices instead of vertexIndices
      out_indices.push_back(vertexIndex[0] - 1);
      out_indices.push_back(vertexIndex[1] - 1);
      out_indices.push_back(vertexIndex[2] - 1);
    }
    else {
      // Probably a comment, eat up the rest of the line
      char stupidBuffer[1000];
      fgets(stupidBuffer, 1000, file);
    }
  }

  return true;
}

// EOF
