#ifndef GROUP_H
#define GROUP_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <string>
#include <vector>
#include <cmath>

#include "model.h"
#include "../../utils/ponto.h"
#include "../../lib/Matrix.tpp"

#define CATMULL_TESSELATION 0.01

using namespace std;

class Transformation {
    public:
        virtual void smt(){};
};

class Translate : public Transformation {
    private:
        float x, y, z;
    public:
        float getX() {return this->x;};
        float getY() {return this->y;};
        float getZ() {return this->z;};
        Translate();
        Translate(float x, float y, float z) {
            this->x = x;
            this->y = y;
            this->z = z;
        };
};

class DynamicTranslate : public Transformation {
    private:
        float total_time;  // time to run the whole curve
        float segment_time;  // time to run each segment
        float timebase;  // last time measured
        float elapsed_time;  // time elapsed since beginning of the curve
        vector<Ponto> points;
        vector<Ponto> render_points;

        void generateRenderPoints();
    public:
        void applyTransformations();
        void renderCatmullRomCurve();

        DynamicTranslate();
        DynamicTranslate(float total_time, vector<Ponto> points);    
};

class Rotate : public Transformation {
    private:
        float angle, axisX, axisY, axisZ;
    public:
        float getAngle() {return this->angle;};
        float getAxisX() {return this->axisX;};
        float getAxisY() {return this->axisY;};
        float getAxisZ() {return this->axisZ;};
        Rotate();
        Rotate(float angle, float axisX, float axisY, float axisZ) {
            this->angle = angle;
            this->axisX = axisX;
            this->axisY = axisY;
            this->axisZ = axisZ;
        };
};

class DynamicRotate : public Transformation {
    private:
        float total_time;  // time to perform 360 degrees rotation
        float timebase;  // time at the start of the rotation
        float axisX, axisY, axisZ;  // axis of rotation
    public:
        void applyTransformation();

        DynamicRotate();
        DynamicRotate(float total_time, float axisX, float axisY, float axisZ);    
};

class Scale : public Transformation {
    private:
        float x, y, z;
    public:
        float getX() {return this->x;};
        float getY() {return this->y;};
        float getZ() {return this->z;};
        Scale();
        Scale(float x, float y, float z) {
            this->x = x;
            this->y = y;
            this->z = z;
        };
};

class Color {
    private:
        float r, g, b;
    public:
        float getR() {return this->r;};
        float getG() {return this->g;};
        float getB() {return this->b;};
        Color() {
            this->r = 1.0f;
            this->g = 1.0f;
            this->b = 1.0f;
        };
        Color(float r, float g, float b) {
            this->r = r;
            this->g = g;
            this->b = b;
        };
};

class Group {
    private:
        vector<Transformation*> transformations;
        Color* color;
        vector<Model> models;
        vector<Group> groups;
    public:
        Group();
        void addTranslate(float x, float y, float z);
        void addDynamicTranslate(float time, vector<Ponto> points);
        void addRotate(float angle, float axisX, float axisY, float axisZ);
        void addDynamicRotate(float time, float axisX, float axisY, float axisZ);
        void addScale(float x, float y, float z);
        vector<Transformation*> getTransformations();

        void setColor(float r, float g, float b);
        Color* getColor();

        void addModel(Model model);
        vector<Model> getModels();
        void addGroup(Group group);
        vector<Group> getGroups();
};

#endif //GROUP_H
