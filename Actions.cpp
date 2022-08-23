/*
    File: Actions.cpp
    Implementation of Actions
    Author: Paul peng
    Date: 2022.8.21
*/

#include "myheaders/Actions.h"

vector<mat4> produceModel(vector<vec3> &translation, vector<vec3> &rotation, vector<vec3> &scalar)
{
    vector<mat4> model(BODY_PARTS, mat4(1.0f));

    for (int i = 0; i < BODY_PARTS; i++)
    {
        model[i] = glm::translate(model[i], translation[i]);
        model[i] = glm::rotate(model[i], glm::radians(rotation[i].x), vec3(1.0f, 0.0f, 0.0f));
        model[i] = glm::rotate(model[i], glm::radians(rotation[i].y), vec3(0.0f, 1.0f, 0.0f));
        model[i] = glm::rotate(model[i], glm::radians(rotation[i].z), vec3(0.0f, 0.0f, 1.0f));
        model[i] = glm::scale(model[i], scalar[i]);
    }

    return model;
}

vector<mat4> initial(vector<vec3> &translation, vector<vec3> &rotation, vector<vec3> &scalar)
{
    for (int i = 0; i < BODY_PARTS; i++)
    {
        translation[i] = glm::vec3(0.0f);
        rotation[i] = glm::vec3(0.0f);
        scalar[i] = glm::vec3(1.0f);
    }

    return produceModel(translation, rotation, scalar);
}

void combine(int parent, int child, vector<mat4> &models)
{
    models[child] = models[parent] * models[child];
}

void setConnect(vector<mat4> &models)
{
    // body conduct
    combine(BODY, HEAD, models);
    combine(BODY, LOWER_BODY, models);
    combine(BODY, LEFT_UPPER_ARM, models);
    combine(BODY, RIGHT_UPPER_ARM, models);

    // lower body conduct
    combine(LOWER_BODY, LEFT_UPPER_LEG, models);
    combine(LOWER_BODY, RIGHT_UPPER_LEG, models);

    // upper conduct
    combine(LEFT_UPPER_ARM, LEFT_LOWER_ARM, models);
    combine(RIGHT_UPPER_ARM, RIGHT_LOWER_ARM, models);
    combine(LEFT_UPPER_LEG, LEFT_LOWER_LEG, models);
    combine(RIGHT_UPPER_LEG, RIGHT_LOWER_LEG, models);

    // lower conduct
    combine(LEFT_LOWER_ARM, LEFT_HAND, models);
    combine(RIGHT_LOWER_ARM, RIGHT_HAND, models);
    combine(LEFT_LOWER_LEG, LEFT_FOOT, models);
    combine(RIGHT_LOWER_LEG, RIGHT_FOOT, models);
}

vector<mat4> stand(vector<vec3> &translation, vector<vec3> &rotation, vector<vec3> &scalar)
{
    translation[BODY] = glm::vec3(0.0f, 1.3f, 0.0f);
    translation[LOWER_BODY] = glm::vec3(0.0f, -0.3f, 0.0f);

    translation[HEAD] = vec3(0.0f, 0.3f, -0.04f);
    translation[LEFT_UPPER_ARM] = vec3(0.23f, 0.15f, -0.08f);
    translation[RIGHT_UPPER_ARM] = vec3(-0.22f, 0.15f, -0.08f);

    translation[LEFT_UPPER_LEG] = vec3(0.15f, -0.08f, 0.025f);
    translation[RIGHT_UPPER_LEG] = vec3(-0.15f, -0.08f, 0.025f);

    translation[LEFT_LOWER_ARM] = vec3(0.07f, -0.2f, -0.06f);
    translation[RIGHT_LOWER_ARM] = vec3(-0.1f, -0.23f, -0.05f);
    translation[LEFT_LOWER_LEG] = vec3(0.0f, -0.45f, 0.0f);
    translation[RIGHT_LOWER_LEG] = vec3(0.0f, -0.45f, 0.0f);

    translation[LEFT_HAND] = vec3(0.05f, -0.2f, 0.08f);
    translation[RIGHT_HAND] = vec3(-0.05f, -0.2f, 0.08f);
    translation[LEFT_FOOT] = vec3(0.0f, -0.4f, -0.05f);
    translation[RIGHT_FOOT] = vec3(0.0f, -0.4f, -0.05f);

    translation[BODY] += glm::vec3(0.0f, 0.0f, -5.0f);
    rotation[LEFT_UPPER_ARM] = glm::vec3(-30.0f, 0.0f, 0.0f);
    rotation[RIGHT_UPPER_ARM] = glm::vec3(30.0f, 0.0f, 0.0f);
    rotation[LEFT_UPPER_LEG] = glm::vec3(-15.0f, 0.0f, 0.0f);
    rotation[RIGHT_UPPER_LEG] = glm::vec3(15.0f, 0.0f, 0.0f);

    vector<mat4> models = produceModel(translation, rotation, scalar);
    setConnect(models);

    return models;
}

vector<mat4> walk(vector<vec3> &translation, vector<vec3> &rotation, vector<vec3> &scalar)
{
    float time = glfwGetTime();
    float freq = 2.0f;
    float amp = 1.0f;
    float offset = 45.0f;
    float func = amp * cos(freq * (time + offset));

    translation[BODY] += vec3(0.0f, 0.0f, 0.004f);

    if (translation[BODY].z > 5.0f)
    {
        translation[BODY].z = -5.0f;
    }

    rotation[LEFT_UPPER_ARM] += vec3(func, 0.0f, 0.0f);
    rotation[RIGHT_UPPER_ARM] += vec3(-func, 0.0f, 0.0f);
    rotation[LEFT_UPPER_LEG] += vec3(-func * 0.5, 0.0f, 0.0f);
    rotation[RIGHT_UPPER_LEG] += vec3(func * 0.5, 0.0f, 0.0f);

    vector<mat4> models = produceModel(translation, rotation, scalar);
    setConnect(models);

    return models;
}
