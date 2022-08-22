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

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define BODY_PARTS      15

#define HEAD            0
#define LEFT_FOOT       1
#define LEFT_HAND       2
#define LEFT_LOWER_ARM  3
#define LEFT_LOWER_LEG  4
#define LEFT_UPPER_ARM  5
#define LEFT_UPPER_LEG  6
#define LOWER_BODY      7
#define RIGHT_FOOT      8
#define RIGHT_HAND      9
#define RIGHT_LOWER_ARM 10
#define RIGHT_LOWER_LEG 11
#define RIGHT_UPPER_ARM 12
#define RIGHT_UPPER_LEG 13
#define BODY            14

using namespace std;
using namespace glm;

void initial(vector<vec3> &translation, vector<vec3> &rotation, vector<vec3> &scalar);
void combine(int main, int sub, vector<vec3> &translation, vector<vec3> &rotation, vector<vec3> &scalar);
void setConnect(vector<vec3> &translation, vector<vec3> &rotation, vector<vec3> &scalar);
void stand(vector<vec3> &translation, vector<vec3> &rotation, vector<vec3> &scalar);
void walk(vector<vec3> &translation, vector<vec3> &rotation, vector<vec3> &scalar);