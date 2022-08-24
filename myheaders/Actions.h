/*
    File: Actions.cpp
    Definition of Actions
    Author: Paul peng
    Date: 2022.8.21
*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define BODY_PARTS 15
#define HEAD 0
#define LEFT_FOOT 1
#define LEFT_HAND 2
#define LEFT_LOWER_ARM 3
#define LEFT_LOWER_LEG 4
#define LEFT_UPPER_ARM 5
#define LEFT_UPPER_LEG 6
#define LOWER_BODY 7
#define RIGHT_FOOT 8
#define RIGHT_HAND 9
#define RIGHT_LOWER_ARM 10
#define RIGHT_LOWER_LEG 11
#define RIGHT_UPPER_ARM 12
#define RIGHT_UPPER_LEG 13
#define BODY 14

using namespace std;
using namespace glm;

extern float startTime;
extern float actionFreq;

vector<mat4> produceModel(vector<vec3> &translation, vector<vec3> &rotation, vector<vec3> &scalar);
vector<mat4> initial(vector<vec3> &translation, vector<vec3> &rotation, vector<vec3> &scalar);
void combine(int parent, int child, vector<mat4> &models);
void setConnect(vector<mat4> &models);
vector<mat4> stand(vector<vec3> &translation, vector<vec3> &rotation, vector<vec3> &scalar);
vector<mat4> walk(vector<vec3> &translation, vector<vec3> &rotation, vector<vec3> &scalar);
