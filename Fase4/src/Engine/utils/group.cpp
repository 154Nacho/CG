#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <string>
#include <cmath>
#include <sstream>
#include <vector>

#include "../../utils/float_vector.h"
#include "../../lib/Matrix.tpp"

#include "group.h"

using namespace std;

// * Dynamic Translate * //

// Given a t from a segment an its 4 points, returns the corresponding points in the curve
Ponto getCatmullRomPoint(float t, Ponto p0, Ponto p1, Ponto p2, Ponto p3) {
    // Generate matrices
    Matrix<float> m_t = {{t*t*t, t*t, t, 1.0f}};

    Matrix<float> m = {{-0.5f, 1.5f,-1.5f, 0.5f},
                       { 1.0f,-2.5f, 2.0f,-0.5f},
                       {-0.5f, 0.0f, 0.5f, 0.0f},
                       { 0.0f, 1.0f, 0.0f, 0.0f}};

    Matrix<float> m_px = {{p0.getX()},
                          {p1.getX()},
                          {p2.getX()},
                          {p3.getX()}};

    Matrix<float> m_py = {{p0.getY()},
                          {p1.getY()},
                          {p2.getY()},
                          {p3.getY()}};

    Matrix<float> m_pz = {{p0.getZ()},
                          {p1.getZ()},
                          {p2.getZ()},
                          {p3.getZ()}};

    // Calculate curve point
    float x_pos = (m_t * m * m_px)[0][0];
    float y_pos = (m_t * m * m_py)[0][0];
    float z_pos = (m_t * m * m_pz)[0][0];

    return Ponto(x_pos, y_pos, z_pos);
}

Ponto getDerivativeCatmullRomPoint(float t, Ponto p0, Ponto p1, Ponto p2, Ponto p3) {
    Matrix<float> m_t = {{3*t*t, 2*t, 1.0f, 0.0f}};

    Matrix<float> m = {{-0.5f, 1.5f,-1.5f, 0.5f},
                       { 1.0f,-2.5f, 2.0f,-0.5f},
                       {-0.5f, 0.0f, 0.5f, 0.0f},
                       { 0.0f, 1.0f, 0.0f, 0.0f}};

    Matrix<float> m_px = {{p0.getX()},
                          {p1.getX()},
                          {p2.getX()},
                          {p3.getX()}};

    Matrix<float> m_py = {{p0.getY()},
                          {p1.getY()},
                          {p2.getY()},
                          {p3.getY()}};

    Matrix<float> m_pz = {{p0.getZ()},
                          {p1.getZ()},
                          {p2.getZ()},
                          {p3.getZ()}};

    Matrix<float> aux = m_t * m;
    float x_der = (aux * m_px)[0][0];
    float y_der = (aux * m_py)[0][0];
    float z_der = (aux * m_pz)[0][0];

    return Ponto(x_der, y_der, z_der);
}

void DynamicTranslate::generateRenderPoints() {
    int segment_count = points.size() - 3;

    for (int i = 0; i < segment_count; i++) {
        Ponto p0 = points[i];
        Ponto p1 = points[i+1];
        Ponto p2 = points[i+2];
        Ponto p3 = points[i+3];

        for (float t = 0; t < 1.0; t += CATMULL_TESSELATION) {
            Ponto curve_point = getCatmullRomPoint(t, p0, p1, p2, p3);
            render_points.push_back(curve_point);
        }
    }

}

DynamicTranslate::DynamicTranslate(float total_time, vector<Ponto> points) {
    this->total_time = total_time;
    this->segment_time = this->total_time / (points.size() - 3);
    this->timebase = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    this->elapsed_time = 0.0;
    this->points = points;
    
    generateRenderPoints();  // iniciates render_points with the points to render the curve
}

void buildRotMatrix(float *x, float *y, float *z, float *m) {

	m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0;
	m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0;
	m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
	m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}

void DynamicTranslate::applyTransformations() {
    // Calculate passage of time
    float current_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    elapsed_time = current_time - timebase;

    if (elapsed_time > total_time) elapsed_time = total_time; // TODO: melhorar isto

    float t = fmod(elapsed_time, segment_time) / segment_time;

    // Calculate transformations
    int segment = elapsed_time / segment_time;
    Ponto p0 = points[segment];
    Ponto p1 = points[segment+1];
    Ponto p2 = points[segment+2];
    Ponto p3 = points[segment+3];

    Ponto curve_point = getCatmullRomPoint(t, p0, p1, p2, p3);
    Ponto deriv_point = getDerivativeCatmullRomPoint(t, p0, p1, p2, p3);

    float XX[3] = {deriv_point.getX(), deriv_point.getY(), deriv_point.getZ()},
          YY[3] = {0, 1, 0},
          ZZ[3];

    float m[4][4];

    vector_cross(XX, YY, ZZ);
    vector_cross(ZZ, XX, YY);

    vector_normalize(XX);
    vector_normalize(YY);
    vector_normalize(ZZ);

    buildRotMatrix(XX, YY, ZZ, (float*) m);

    // Apply transformations
    // position teapot along the curve
    glTranslatef(curve_point.getX(), curve_point.getY(), curve_point.getZ());

    // align teapot with the curve
    glMultMatrixf((float*) m);


    if (elapsed_time == total_time) {
        elapsed_time = 0;
        timebase = current_time;
    }
}

void DynamicTranslate::renderCatmullRomCurve() {
    GLfloat grey[4] = {0.1, 0.1, 0.1, 1.0};
    GLfloat zero[4] = {0.0, 0.0, 0.0, 0.0};

    glBegin(GL_LINE_LOOP);
	
    glMaterialfv(GL_FRONT, GL_AMBIENT, grey);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, grey);
	glMaterialfv(GL_FRONT, GL_SPECULAR, zero);
	glMaterialfv(GL_FRONT, GL_EMISSION, zero);
	glMaterialf(GL_FRONT, GL_SHININESS, 0);
    
    //for (int i = 1; i < points.size()-2; i++) glVertex3f(points[i].getX(), points[i].getY(), points[i].getZ());
    for (Ponto p : render_points) glVertex3f(p.getX(), p.getY(), p.getZ());
    
    glEnd();
}

// * Dynamic Rotate * //

DynamicRotate::DynamicRotate(float total_time, float axisX, float axisY, float axisZ) {
    this->total_time = total_time;
    this->timebase = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    this->axisX = axisX;
    this->axisY = axisY;
    this->axisZ = axisZ;
}

void DynamicRotate::applyTransformation() {
    // Get the time elapsed since the beginning of this full rotation
    float elapsed_time = fmod(glutGet(GLUT_ELAPSED_TIME)/1000.0 - timebase, total_time);

    // Get the current angle of the object
    float angle = 360 * elapsed_time / total_time;

    // Apply the rotation
    glRotatef(angle, axisX, axisY, axisZ);
}

// * Group * //

Group::Group() {
    transformations = {};
    color = nullptr;
    models = {};
    groups = {};
}

void Group::addTranslate(float x, float y, float z) {
    Translate* tr = new Translate(x,y,z);
    this->transformations.push_back(tr);
}

void Group::addDynamicTranslate(float time, vector<Ponto> points) {
    DynamicTranslate* dtr = new DynamicTranslate(time, points);
    this->transformations.push_back(dtr);
}

void Group::addRotate(float angle, float axisX, float axisY, float axisZ) {
    Rotate* rt = new Rotate(angle,axisX,axisY,axisZ);
    this->transformations.push_back(rt);
}

void Group::addDynamicRotate(float time, float axisX, float axisY, float axisZ) {
    DynamicRotate* drt = new DynamicRotate(time, axisX, axisY, axisZ);
    this->transformations.push_back(drt);
}

void Group::addScale(float x, float y, float z) {
    Scale* sc = new Scale(x,y,z);
    this->transformations.push_back(sc);
}

vector<Transformation*> Group::getTransformations() {
    return this->transformations;
}

void Group::setColor(float r, float g, float b) {
    Color* cl = new Color(r,g,b);
    this->color = cl;
}

Color* Group::getColor() {
    return this->color;
}

void Group::addModel(Model model) {
    this->models.push_back(model);
}

vector<Model> Group::getModels() {
    return this->models;
}

void Group::addGroup(Group group) {
    this->groups.push_back(group);
}

vector<Group> Group::getGroups() {
    return this->groups;
}
