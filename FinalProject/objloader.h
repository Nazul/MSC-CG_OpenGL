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
#pragma once

#include <vector>
#include "Mesh.h"

#ifndef OBJLOADER_H
#define OBJLOADER_H

bool loadOBJ(const char * path, std::vector<CMesh::VERTEX>& out_vertices, vector<unsigned long>& out_indices, VECTOR4D c);

#endif

// EOF
