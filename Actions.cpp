/*
    File: Actions.cpp
    Implementation of Actions
    Author: Paul peng
    Date: 2022.8.21
*/

#include "myheaders/Actions.h"

void initial(vector<vec3> &translation, vector<vec3> &rotation, vector<vec3> &scalar)
{
    for (int i = 0; i < BODY_PARTS; i++)
    {
        translation[i] = glm::vec3(0.0f);
        rotation[i] = glm::vec3(0.0f);
        scalar[i] = glm::vec3(1.0f);
    }
}

void combine(int main, int sub, vector<vec3> &translation, vector<vec3> &rotation, vector<vec3> &scalar)
{
    translation[sub] = translation[main] * translation[sub];
    rotation[sub] = rotation[sub] * rotation[main];
    scalar[sub] = scalar[sub] * scalar[main];
}

void setConnect(vector<vec3> &translation, vector<vec3> &rotation, vector<vec3> &scalar)
{
    // body conduct
    std::cout << rotation[BODY].x << " " << rotation[BODY].y << " " << rotation[BODY].z << std::endl;
    std::cout << rotation[HEAD].x << " " << rotation[HEAD].y << " " << rotation[HEAD].z << std::endl;
    combine(BODY, HEAD, translation, rotation, scalar);
    std::cout << rotation[BODY].x << " " << rotation[BODY].y << " " << rotation[BODY].z << std::endl;
    std::cout << rotation[HEAD].x << " " << rotation[HEAD].y << " " << rotation[HEAD].z << std::endl;
    combine(BODY, LOWER_BODY, translation, rotation, scalar);
    combine(BODY, LEFT_UPPER_ARM, translation, rotation, scalar);
    combine(BODY, RIGHT_UPPER_ARM, translation, rotation, scalar);

    // lower body conduct
    combine(LOWER_BODY, LEFT_UPPER_LEG, translation, rotation, scalar);
    combine(LOWER_BODY, RIGHT_UPPER_LEG, translation, rotation, scalar);

    // upper conduct
    combine(LEFT_UPPER_ARM, LEFT_LOWER_ARM, translation, rotation, scalar);
    combine(RIGHT_UPPER_ARM, RIGHT_LOWER_ARM, translation, rotation, scalar);
    combine(LEFT_UPPER_LEG, LEFT_LOWER_LEG, translation, rotation, scalar);
    combine(RIGHT_UPPER_LEG, RIGHT_LOWER_LEG, translation, rotation, scalar);

    // lower conduct
    combine(LEFT_LOWER_ARM, LEFT_HAND, translation, rotation, scalar);
    combine(RIGHT_LOWER_ARM, RIGHT_HAND, translation, rotation, scalar);
    combine(LEFT_LOWER_LEG, LEFT_FOOT, translation, rotation, scalar);
    combine(RIGHT_LOWER_LEG, RIGHT_FOOT, translation, rotation, scalar);
}

void stand(vector<vec3> &translation, vector<vec3> &rotation, vector<vec3> &scalar)
{
    translation[HEAD] = glm::vec3(0.0f, 1.0f, -0.04f);
    translation[BODY] = glm::vec3(0.0f, 1.3f, 0.0f);
    rotation[BODY] = glm::vec3(0.0f, 0.0f, 30.0f);
    // translation[LOWER_BODY] = glm::vec3(0.0f, -0.2f, 0.0f);
    // translation[LEFT_FOOT] = glm::vec3(0.5f, 0.0f, 0.0f);
    // translation[LEFT_HAND] = glm::vec3(0.0f, -0.8f, 0.0f);
    // translation[LEFT_LOWER_ARM] = glm::vec3(0.0f, -0.8f, 0.0f);
    // translation[LEFT_LOWER_LEG] = glm::vec3(0.0f, -0.8f, 0.0f);
    // translation[LEFT_UPPER_ARM] = glm::vec3(0.0f, -0.8f, 0.0f);
    // translation[LEFT_UPPER_LEG] = glm::vec3(0.0f, -0.8f, 0.0f);
    // translation[RIGHT_FOOT] = glm::vec3(-0.5f, 0.0f, 0.0f);
    // translation[RIGHT_HAND] = glm::vec3(0.0f, -0.8f, 0.0f);
    // translation[RIGHT_LOWER_ARM] = glm::vec3(0.0f, -0.8f, 0.0f);
    // translation[RIGHT_LOWER_LEG] = glm::vec3(0.0f, -0.8f, 0.0f);
    // translation[RIGHT_UPPER_ARM] = glm::vec3(0.0f, -0.8f, 0.0f);
    // translation[RIGHT_UPPER_LEG] = glm::vec3(0.0f, -0.8f, 0.0f);

    setConnect(translation, rotation, scalar);
}

void walk(vector<vec3> &translation, vector<vec3> &rotation, vector<vec3> &scalar)
{

}