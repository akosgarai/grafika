//=============================================================================================
// Szamitogepes grafika hazi feladat keret. Ervenyes 2013-tol.
// A //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sorokon beluli reszben celszeru garazdalkodni, mert a tobbit ugyis toroljuk.
// A beadott program csak ebben a fajlban lehet, a fajl 1 byte-os ASCII karaktereket tartalmazhat.
// Tilos:
// - mast "beincludolni", illetve mas konyvtarat hasznalni
// - faljmuveleteket vegezni (printf is fajlmuvelet!)
// - new operatort hivni az onInitialization függvényt kivéve, a lefoglalt adat korrekt felszabadítása nélkül
// - felesleges programsorokat a beadott programban hagyni
// - tovabbi kommenteket a beadott programba irni a forrasmegjelolest kommentjeit kiveve
// ---------------------------------------------------------------------------------------------
// A feladatot ANSI C++ nyelvu forditoprogrammal ellenorizzuk, a Visual Studio-hoz kepesti elteresekrol
// es a leggyakoribb hibakrol (pl. ideiglenes objektumot nem lehet referencia tipusnak ertekul adni)
// a hazibeado portal ad egy osszefoglalot.
// ---------------------------------------------------------------------------------------------
// A feladatmegoldasokban csak olyan gl/glu/glut fuggvenyek hasznalhatok, amelyek
// 1. Az oran a feladatkiadasig elhangzottak ES (logikai AND muvelet)
// 2. Az alabbi listaban szerepelnek:
// Rendering pass: glBegin, glVertex[2|3]f, glColor3f, glNormal3f, glTexCoord2f, glEnd, glDrawPixels
// Transzformaciok: glViewport, glMatrixMode, glLoadIdentity, glMultMatrixf, gluOrtho2D,
// glTranslatef, glRotatef, glScalef, gluLookAt, gluPerspective, glPushMatrix, glPopMatrix,
// Illuminacio: glMaterialfv, glMaterialfv, glMaterialf, glLightfv
// Texturazas: glGenTextures, glBindTexture, glTexParameteri, glTexImage2D, glTexEnvi,
// Pipeline vezerles: glShadeModel, glEnable/Disable a kovetkezokre:
// GL_LIGHTING, GL_NORMALIZE, GL_DEPTH_TEST, GL_CULL_FACE, GL_TEXTURE_2D, GL_BLEND, GL_LIGHT[0..7]
//
// NYILATKOZAT
// ---------------------------------------------------------------------------------------------
// Nev    : <VEZETEKNEV(EK)> <KERESZTNEV(EK)>
// Neptun : <NEPTUN KOD>
// ---------------------------------------------------------------------------------------------
// ezennel kijelentem, hogy a feladatot magam keszitettem, es ha barmilyen segitseget igenybe vettem vagy
// mas szellemi termeket felhasznaltam, akkor a forrast es az atvett reszt kommentekben egyertelmuen jeloltem.
// A forrasmegjeloles kotelme vonatkozik az eloadas foliakat es a targy oktatoi, illetve a
// grafhazi doktor tanacsait kiveve barmilyen csatornan (szoban, irasban, Interneten, stb.) erkezo minden egyeb
// informaciora (keplet, program, algoritmus, stb.). Kijelentem, hogy a forrasmegjelolessel atvett reszeket is ertem,
// azok helyessegere matematikai bizonyitast tudok adni. Tisztaban vagyok azzal, hogy az atvett reszek nem szamitanak
// a sajat kontribucioba, igy a feladat elfogadasarol a tobbi resz mennyisege es minosege alapjan szuletik dontes.
// Tudomasul veszem, hogy a forrasmegjeloles kotelmenek megsertese eseten a hazifeladatra adhato pontokat
// negativ elojellel szamoljak el es ezzel parhuzamosan eljaras is indul velem szemben.
//=============================================================================================

#include <math.h>
#include <stdlib.h>

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Innentol modosithatod...
const float pi = atan(1.0)*4.0;
GLuint texname[2];
const float Spec[] = {1.0f, 1.0f, 1.0f, 1.0f};
const float Null[] = {0.0f, 0.0f, 0.0f, 1.0f};
const float Diffuz[] = {1.0f, 1.0f, 1.0f, 1.0f};
const float Diffuz2[] = {0.3f, 0.3f, 0.3f, 1.0f};
const float Ambiens[] = {0.1f, 0.1f, 0.1f, 1.0f};
float specnap[] = {1.0f, 1.0f, 0.0f, 1.0f};
float dcolor[4] = {0.1, 0.1, 0.1, 1.0};
float scolor[4] = {0.0, 0.0, 0.0, 1.0};
typedef enum mozgas{ALL, FEL, LE, BAL, JOBB, ELORE, HATRA} Allapot;
float ballheight;
bool balldirection;
//--------------------------------------------------------
// 3D Vektor
//--------------------------------------------------------
struct Vector {
   float x, y, z;

   Vector( ) {
	x = y = z = 0;
   }
   Vector(float x0, float y0, float z0 = 0) {
	x = x0; y = y0; z = z0;
   }
   Vector operator*(float a) {
	return Vector(x * a, y * a, z * a);
   }
   Vector operator+(const Vector& v) {
 	return Vector(x + v.x, y + v.y, z + v.z);
   }
   Vector operator-(const Vector& v) {
 	return Vector(x - v.x, y - v.y, z - v.z);
   }
   Vector operator/(float a) {
	return Vector(x / a, y / a, z / a);
   }
   float operator*(const Vector& v) { 	// dot product
	return (x * v.x + y * v.y + z * v.z);
   }
   Vector operator%(const Vector& v) { 	// cross product
	return Vector(y*v.z-z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
   }
   float Length() { return sqrt(x * x + y * y + z * z); }
   Vector& normalize() // egysegnyi hosszura allitja a vektort
   {
        return *this = *this * (1 / Length());
   }
};
class Matrix {
public:
    float m[4][4];
    void Clear() {
        for(int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                m[i][j] = 0;
    }
    void LoadIdentity() {
        Clear();
        m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1;
    }
    Matrix operator+(const Matrix& mat) {
        Matrix result;
        for(int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                result.m[i][j] = m[i][j] + mat.m[i][j];
        return result;
    }
    Matrix operator*(const Matrix& mat) {
        Matrix result;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++){
                result.m[i][j] = 0;
                for (int k = 0; k < 4; k++) result.m[i][j] += m[i][k] * mat.m[k][j];
            }
        return result;
    }
    Vector operator*(const Vector& v) {
        float xh = m[0][0] * v.x + m[0][1] * v.z + m[0][2] * v.y + m[0][3];
        float yh = m[1][0] * v.x + m[1][1] * v.z + m[1][2] * v.y + m[1][3];
        float zh = m[2][0] * v.x + m[2][1] * v.z + m[2][2] * v.y + m[2][3];
        float h = m[3][0] * v.x + m[3][1] * v.z + m[3][2] * v.y + m[3][3];
        return Vector(xh/h, zh/h, yh/h);
    }
    Vector multip0(const Vector& v) {
        float xh = m[0][0] * v.x + m[0][1] * v.z + m[0][2] * v.y;
        float yh = m[1][0] * v.x + m[1][1] * v.z + m[1][2] * v.y;
        float zh = m[2][0] * v.x + m[2][1] * v.z + m[2][2] * v.y;
        float h = m[3][0] * v.x + m[3][1] * v.z + m[3][2] * v.y;
        return Vector(xh/h, zh/h, yh/h);
    }
};
//--------------------------------------------------------
// Spektrum illetve szin
//--------------------------------------------------------
struct Color {
   float r, g, b;

   Color( ) {
	r = g = b = 0;
   }
   Color(float r0, float g0, float b0) {
	r = r0; g = g0; b = b0;
   }
   Color operator*(float a) {
	return Color(r * a, g * a, b * a);
   }
   Color operator*(const Color& c) {
	return Color(r * c.r, g * c.g, b * c.b);
   }
   Color operator+(const Color& c) {
 	return Color(r + c.r, g + c.g, b + c.b);
   }
};
class Camera {
public:
Vector eye, up, look;
Camera() { eye = Vector(0, 0, 0); look = Vector(0,0,-1); up = Vector(0,1,0); }
void FunctionDirr(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1, 1, 10000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    /* ----------------A szem pozicioja-----------*/
    gluLookAt(eye.x,eye.y,eye.z,look.x,look.y,look.z,up.x,up.y,up.z);
}
};
Camera cam;
float tmp = ((int)cam.eye.z % 3) * 1.0f;
class Object {
public:
Color col;
virtual void Draw() {}
};

GLubyte  csempe[16][16][3];
GLubyte  ball[16][16][3];
GLubyte  csempe2[16][16][3];
Vector fejkozep(19, 0, 1.5);

//********texturak
void text(){
    for (int i = 0; i< 16; i++){
        for (int j = 0; j < 16; j++){
                csempe[i][j][0] = rand()%12;
                csempe[i][j][1] = rand()%72;
                csempe[i][j][2] = rand()%142;//0;

                int t;
                if(i>8) t=255;
                else t=0;
                ball[i][j][0] =255;
                ball[i][j][1] =255-t;
                ball[i][j][2] =0;

                t = 255;
                if (i > 3 && i < 13 && j > 3 && j < 13) t = 66;
                csempe2[i][j][0] =255;
                csempe2[i][j][1] =255;
                csempe2[i][j][2] =t;

        }
    }
    /* ------ Textura "epites"------------*/
    glGenTextures(2, texname);
    glBindTexture(GL_TEXTURE_2D, texname[0]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 16, 16,0, GL_RGB, GL_UNSIGNED_BYTE, csempe2);

    glBindTexture(GL_TEXTURE_2D, texname[1]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 16, 16,0, GL_RGB, GL_UNSIGNED_BYTE, ball);

}
class Plane2 : public Object{
public:
    Plane2(){ }
    void rajzol(){
        for (int i = -100;i<100;i++){
            for (int j = -100;j < 100;j++){
                glBegin(GL_QUADS);
                    glNormal3f(0.0,1.0,0.0);
                    glTexCoord2i(0,0);
                    glVertex3f((i+0)*1.0f,0.0,(j+0)*1.0f);
                    glTexCoord2i(1,0);
                    glVertex3f((i+1)*1.0f,0.0,(j+0)*1.0f);
                    glTexCoord2i(1,1);
                    glVertex3f((i+1)*1.0f,0.0,(j+1)*1.0f);
                    if(i % 10 == 0 && j % 10 == 9) glTexCoord2i(0,1);
                    glVertex3f((i+0)*1.0f,0.0,(j+1)*1.0f);
                glEnd();
            }
        }
    }
};
class Plane : public Object{
public:
    Plane() {}
    void rajzol(){
        glPushMatrix();
        for(int i = -10; i < 10; i++){
            for (int j = -10; j < 10; j++){
                glBegin(GL_QUADS);
                glNormal3f(0.0,1.0,0.0);
                glTexCoord2i(0,0);
                glVertex3f((i*10)*1.0f,0.0,(j*10)*1.0f);
                glTexCoord2i(1,0);
                glVertex3f((i*10)*1.0f+10.0f,0.0,(j*10)*1.0f);
                glTexCoord2i(1,1);
                glVertex3f((i*10)*1.0f+10.0f,0.0,(j*10)*1.0f+10.0f);
                glTexCoord2i(0,1);
                glVertex3f((i*10)*1.0f,0.0,(j*10)*1.0f+10.0f);
                glEnd();
            }
        }
        glPopMatrix();
    }
};
void lampa(){
float position2[] = {-10.0f, 10.0f, 10.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, Ambiens);                // Az Ambiens tag
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Diffuz);
    glLightfv(GL_LIGHT0, GL_SPECULAR, Spec);
    glLightfv(GL_LIGHT0, GL_POSITION, position2);
    glEnable(GL_LIGHT0);
}
class Gomb : public Object{
public:
    Vector *v;
    int felbontas;
/* Default konstruktor */
    Gomb(){}
/* Egy parameteres konstruktor, ami a parameterben a felbontast veszi at */
    Gomb(int f){
        felbontas = f;
        v = new Vector[200*felbontas];

        float szog = 0.0f;
        int db = 0;
        float alfa = pi/2.0f;
        float valami = 0.0f;

        /* -------------- Also felgomb--------------*/
        for ( float r = 0.0f; r < 1.0f;)
        {
            for ( int i = 0; i < felbontas; i++)
            {
                v[db*felbontas+i] = Vector(r*cos(szog),-1.0f*valami,r*sin(szog));

                szog += 2.0f*pi / float(felbontas);
            }
            szog = 0.0f;
            alfa -= (pi/2.0f)/100.0f;
            valami = sin(alfa);
            r=cos(alfa);
            db++;
        }
        /* -------------- Felso felgomb--------------*/
        alfa = 0.0f;
        for ( float r = 1.0f; r > 0.0f; )
        {
            for ( int i = 0; i < felbontas; i++)
            {
                v[db*felbontas+i] = Vector(r*cos(szog),valami,r*sin(szog));

                szog += 2.0f*pi / float(felbontas);
            }
            szog = 0.0f;
            alfa += (pi/2.0f)/100.0f;
            valami = sin(alfa);
            r=cos(alfa);
            db++;
        }
    }
/* A gomb kirajzolasa es a normalok beallitasa */
    virtual void rajzol(){
        int i;
        int db = 0;
        //glColor3f(1,0,0);
        glPushMatrix();
        glBegin(GL_TRIANGLES);
        for ( db = 0; db < 199 ; db++)
        {
            for (i = 0; i<felbontas-1; i++)
            {
                glTexCoord2i(0,0);
                glNormal3f(v[db*felbontas+i].x, v[db*felbontas+i].y,v[db*felbontas+i].z);
                glVertex3f(v[db*felbontas+i].x, v[db*felbontas+i].y,v[db*felbontas+i].z);

                glTexCoord2i(1,0);
                glNormal3f(v[db*felbontas+i+1].x, v[db*felbontas+i+1].y,v[db*felbontas+i+1].z);
                glVertex3f(v[db*felbontas+i+1].x, v[db*felbontas+i+1].y,v[db*felbontas+i+1].z);

                glTexCoord2i(1,1);
                glNormal3f(v[db*felbontas+i+felbontas].x, v[db*felbontas+i+felbontas].y,v[db*felbontas+i+felbontas].z);
                glVertex3f(v[db*felbontas+i+felbontas].x, v[db*felbontas+i+felbontas].y,v[db*felbontas+i+felbontas].z);

                glTexCoord2i(1,1);
                glNormal3f(v[db*felbontas+i+felbontas].x, v[db*felbontas+i+felbontas].y,v[db*felbontas+i+felbontas].z);
                glVertex3f(v[db*felbontas+i+felbontas].x, v[db*felbontas+i+felbontas].y,v[db*felbontas+i+felbontas].z);

                glTexCoord2i(1,0);
                glNormal3f(v[db*felbontas+i+1].x, v[db*felbontas+i+1].y,v[db*felbontas+i+1].z);
                glVertex3f(v[db*felbontas+i+1].x, v[db*felbontas+i+1].y,v[db*felbontas+i+1].z);

                glTexCoord2i(0,1);
                glNormal3f(v[db*felbontas+i+1+felbontas].x, v[db*felbontas+i+1+felbontas].y,v[db*felbontas+i+1+felbontas].z);
                glVertex3f(v[db*felbontas+i+1+felbontas].x, v[db*felbontas+i+1+felbontas].y,v[db*felbontas+i+1+felbontas].z);
            }
            glTexCoord2i(0,0);
            glNormal3f(v[db*felbontas+i].x, v[db*felbontas+i].y,v[db*felbontas+i].z);
            glVertex3f(v[db*felbontas+i].x, v[db*felbontas+i].y,v[db*felbontas+i].z);

            glTexCoord2i(0,1);
            glNormal3f(v[db*felbontas+0].x, v[db*felbontas+0].y,v[db*felbontas+0].z);
            glVertex3f(v[db*felbontas+0].x, v[db*felbontas+0].y,v[db*felbontas+0].z);

            glTexCoord2i(1,1);
            glNormal3f(v[db*felbontas+i+felbontas].x, v[db*felbontas+i+felbontas].y,v[db*felbontas+i+felbontas].z);
            glVertex3f(v[db*felbontas+i+felbontas].x, v[db*felbontas+i+felbontas].y,v[db*felbontas+i+felbontas].z);

            glTexCoord2i(1,1);
            glNormal3f(v[db*felbontas+i+felbontas].x, v[db*felbontas+i+felbontas].y,v[db*felbontas+i+felbontas].z);
            glVertex3f(v[db*felbontas+i+felbontas].x, v[db*felbontas+i+felbontas].y,v[db*felbontas+i+felbontas].z);

            glTexCoord2i(0,1);
            glNormal3f(v[db*felbontas+0].x, v[db*felbontas+0].y,v[db*felbontas+0].z);
            glVertex3f(v[db*felbontas+0].x, v[db*felbontas+0].y,v[db*felbontas+0].z);

            glTexCoord2i(1,0);
            glNormal3f(v[db*felbontas+felbontas].x, v[db*felbontas+felbontas].y,v[db*felbontas+felbontas].z);
            glVertex3f(v[db*felbontas+felbontas].x, v[db*felbontas+felbontas].y,v[db*felbontas+felbontas].z);
        }
        glEnd();
        glPopMatrix();
    }
/* Destruktor */
    virtual ~Gomb(){
        delete[] v;
    }
};
Vector ep(2,2,3);
class Fokkha : public Object {
public:
//a masodik hazibol a gorbe
    Vector controlpoints[20];
    Allapot state;
    Vector headcontrolpoints[10];
    float knots[20];
    Vector bodypoints[180];
    Vector headpoints[90];
    Vector bodynormals[180];
    Vector headnormals[90];
    Vector rotate;
    float cntr;
    bool dir;
    Fokkha() { state = ALL; rotate = Vector(0, 0, 0); cntr = 0.0f;  dir = true; }
    void setKnots(){
        for(int i = 0; i < 20; i++){
            knots[i] = i+sin(i/(5.0f))*0.4f;
        }
    }

    void setRotateVector() {
        switch (state) {
            case ALL:
                rotate.x = rotate.y = rotate.z = 0;
                break;
            case FEL:
                rotate.x -= 0.1;
                break;
            case LE:
                rotate.x += 0.1;
                break;
            case BAL:
                rotate.y -= 0.1;
                break;
            case JOBB:
                rotate.y += 0.1;
                break;
            case ELORE:
                break;
            case HATRA:
                break;
        }
        if(rotate.x != 0 && rotate.y != 0 && rotate.z != 0)
        rotate.normalize();
        if(dir == true) cntr = cntr + 1.0f;
        else cntr = cntr - 1.0f;
    }
    Vector calcVi(Vector v0, Vector v1, Vector v2, float f0, float f1, float f2){
            Vector result;
            result = (((v1 - v0) / (f1 - f0)) + ((v2 - v1) / (f2 - f1))) * 0.5f;
            return result;
    }

    Vector calcPoint(int i, float j, float n, int r, int imax, Vector controlpoints[]){
            Vector d;
            Vector c;
            Vector b;
            Vector a;
            Vector ci, ci1;
            if(i == 0) {
                ci = Vector(0.0f, 0.0f, 0.0f);
                ci1 = calcVi(controlpoints[0], controlpoints[1], controlpoints[2], knots[0], knots[1], knots[2]);
            } else if(i == imax) {
                ci = calcVi(controlpoints[i-1], controlpoints[i], controlpoints[i+1], knots[i-1], knots[i], knots[i+1]);
                ci1 = Vector(0.0f, 0.0f, 0.0f);
            } else{
                ci = calcVi(controlpoints[i-1], controlpoints[i], controlpoints[i+1], knots[i-1], knots[i], knots[i+1]);
                ci1 = calcVi(controlpoints[i], controlpoints[i+1], controlpoints[i+2], knots[i], knots[i+1], knots[i+2]);
            }
            d = controlpoints[i];
            c = ci;
            b = (((controlpoints[i+1] - controlpoints[i])*3/((knots[i+1] - knots[i])*(knots[i+1] - knots[i]))) - ((ci1 + (ci*2.0f))/(knots[i+1] - knots[i])));
            a = (((controlpoints[i] - controlpoints[i+1])*(2.0f)/((knots[i+1] - knots[i])*(knots[i+1] - knots[i])*(knots[i+1] - knots[i]))) + ((ci1 + ci)/((knots[i+1] - knots[i]) * (knots[i+1] - knots[i]))));
            Vector result;
            float dt = (knots[i+1] - knots[i])/n*j;
            if(r == 0) { result = (a * dt * dt * dt + b * dt * dt + c * dt + d); }
            else { result = (a * dt * dt * 3 + b * dt * 2 + c); }
            return result;
    }

    void setCurve(){
        float n = 10.0f;
        for(int i = 0; i < 18; i++) {
            for (int k = 0; k < n; k++) {
                Vector tmp;
                tmp = calcPoint(i, k, n, 0, 18, controlpoints);
                bodypoints[i * (int)n + (int)k] = tmp;
                tmp = calcPoint(i, k, n, 1, 18, controlpoints);
                bodynormals[i * (int)n + (int)k] = tmp;
            }
        }
    }

    void setHead(){
        float n = 10.0f;
        for(int i = 0; i < 9; i++) {
            for(int k = 0; k < n; k++) {
                Vector tmp;
                tmp = calcPoint(i, k, n, 0, 9, headcontrolpoints);
                headpoints[i * (int)n +  (int)k] = tmp;
                tmp = calcPoint(i, k, n, 1, 9, headcontrolpoints);
                headnormals[i * (int)n +  (int)k] = tmp;
            }
        }
    }

 /*   void gorbe(){
        glBegin(GL_LINE_STRIP);
        for (int a = 0; a < 179; a++){
            glVertex3f(points[a].x, points[a].y, points[a].z);
        }
        glEnd();
    }*/
    void felulet(Vector points[], Vector normals[], int imax){

        glPushMatrix();
        if (state == ELORE){
            glTranslatef(cntr * -0.2,0,0);
        }else if (state == HATRA){
            glTranslatef(cntr * 0.2f,0,0);
        }

        for (int i = 0; i < imax; i++)
        for (int j = 0; j < 100; j++){
            glBegin(GL_QUADS);
            Vector side1;
            side1.x = points[i+1].z*(cos(3.6*j*pi/180)) - points[i].z*(cos((3.6*j*pi/180)));
            side1.y = points[i+1].z*(sin(3.6*j*pi/180)) - points[i].z*(sin(3.6*j*pi/180));
            side1.z = points[i+1].x - points[i].x;
            Vector side2;
            side2.x = points[i+1].z*(cos(3.6*(j+1)*pi/180)) - points[i+1].z*(cos(3.6*j*pi/180));
            side2.y = points[i+1].z*(sin(3.6*(j+1)*pi/180)) - points[i+1].z*(sin(3.6*j*pi/180));
            side2.z = 0;
            Vector normal;
            normal = (side1 % side2).normalize()*(-1.0f);
            glNormal3f(normal.x, normal.y, normal.z);
            glVertex3f(points[i].z*(cos((3.6*j*pi/180))), points[i].z*(sin(3.6*j*pi/180)), points[i].x);
            //glNormal3f(normal.x, normal.y, normal.z);
            glVertex3f(points[i+1].z*(cos(3.6*j*pi/180)), points[i+1].z*(sin(3.6*j*pi/180)), points[i+1].x);
            //glNormal3f(normal.x, normal.y, normal.z);
            glVertex3f(points[i+1].z*(cos(3.6*(j+1)*pi/180)), points[i+1].z*(sin(3.6*(j+1)*pi/180)), points[i+1].x);
            //glNormal3f(normal.x, normal.y, normal.z);
            glVertex3f(points[i].z*(cos(3.6*(j+1)*pi/180)), points[i].z*(sin(3.6*(j+1)*pi/180)), points[i].x);
            glEnd();
            /*glBegin(GL_LINES);
            glVertex3f(points[i+1].z*(cos(3.6*j*pi/180)), points[i+1].z*(sin(3.6*j*pi/180)), points[i+1].x);
            glVertex3f(normal.x*10.0f, normal.y*10.0f, normal.z*10.0f);
            glEnd();*/
        }
        glPopMatrix();
    }

    void egyikszem(){
        float tmpdifcol[] = {0.1,0.1,0.1,1.0};
        float wh[] = {1,1,1,1};
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, wh);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, tmpdifcol);
        Gomb feher(10), fekete(10);
        glPushMatrix();
        if (state == ELORE){
            glTranslatef(cntr * -0.2,0,0);
        }else if (state == HATRA){
            glTranslatef(cntr * 0.2f,0,0);
        }
            glTranslatef(ep.x, ep.y,ep.z);
            glRotatef(cntr, rotate.x, rotate.y, rotate.z);
            feher.rajzol();
            float tmpspeccol[] = {0.0,0.0,0.0,1.0};
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, tmpspeccol);
            glRotatef(-40+ballheight*12, -1,1,-1);
            glTranslatef(0, 0.5,0.5);
            glScalef(0.5,0.5,0.5);
            fekete.rajzol();
        glPopMatrix();
    }

    void masikszem(){
        float tmpdifcol[] = {0.1,0.1,0.1,1.0};
        float wh[] = {1,1,1,1};
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, wh);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, tmpdifcol);
        Gomb feher(10), fekete(10);
        glPushMatrix();
        if (state == ELORE){
            glTranslatef(cntr * -0.2,0,0);
        }else if (state == HATRA){
            glTranslatef(cntr * 0.2f,0,0);
        }
            glTranslatef(ep.x-3, ep.y,ep.z);
            glRotatef(cntr, rotate.x, rotate.y, rotate.z);
            feher.rajzol();
            float tmpspeccol[] = {0.0,0.0,0.0,1.0};
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, tmpspeccol);
            glRotatef(-40+ballheight*12, -1,-1,-1);
            glTranslatef(0, 0.5,0.5);
            glScalef(0.5,0.5,0.5);
            fekete.rajzol();
        glPopMatrix();
    }

    void fej(){
        glPushMatrix();
        if (state == ELORE){
            glTranslatef(cntr * -0.,0,0);
        }else if (state == HATRA){
            glTranslatef(cntr * 0.2f,0,0);
        }
            glRotatef(90,-1,-1,1);
            glRotatef(cntr, rotate.x, rotate.y, rotate.z);
            felulet(headpoints, headnormals, 90);
            egyikszem();
            masikszem();
        glPopMatrix();
    }


    void rajzol(){
        float tmpdiffcol[] = {0.2,0.2,0.2,1.0};
        float tmpspeccol[] = {0.0,0.0,0.0,1.0};
        //glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, tmpspeccol);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, tmpspeccol);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 30.0f);
        felulet(bodypoints, bodynormals, 179);
        fej();
        //felulet(headpoints, headnormals, 89);

       // szemek();

        //glRotatef((2 +tmp) * 6, 0,0,1);
        //szemfekete();
    }

    void setControlPoints(){
        controlpoints[0] = Vector(0,0,0);
        controlpoints[1] = Vector(2,0,3);
        controlpoints[2] = Vector(3,0,3);
        controlpoints[3] = Vector(4,0,3);
        controlpoints[4] = Vector(5,0,3);
        controlpoints[5] = Vector(6,0,3);
        controlpoints[6] = Vector(7,0,3);
        controlpoints[7] = Vector(8,0,3);
        controlpoints[8] = Vector(9,0,3);
        controlpoints[9] = Vector(10,0,5);
        controlpoints[10] = Vector(11,0,5);
        controlpoints[11] = Vector(12,0,5);
        controlpoints[12] = Vector(13,0,5);
        controlpoints[13] = Vector(14,0,5);
        controlpoints[14] = Vector(15,0,5);
        controlpoints[15] = Vector(16,0,5);
        controlpoints[16] = Vector(17,0,3);
        controlpoints[17] = Vector(18,0,3);
        controlpoints[18] = Vector(19,0,3);
        controlpoints[19] = Vector(20,0,0);
    }

    void setHeadPoints(){
        headcontrolpoints[0] = Vector(0,0,0);
        headcontrolpoints[1] = Vector(1,0,2);
        headcontrolpoints[2] = Vector(2,0,2.3);
        headcontrolpoints[3] = Vector(3,0,2.6);
        headcontrolpoints[4] = Vector(4,0,3);
        headcontrolpoints[5] = Vector(5,0,3);
        headcontrolpoints[6] = Vector(6,0,3);
        headcontrolpoints[7] = Vector(7,0,3);
        headcontrolpoints[8] = Vector(8,0,3);
        headcontrolpoints[9] = Vector(9,0,0);
    }
};

const int screenWidth = 600;	// alkalmazás ablak felbontása
const int screenHeight = 600;


Color image[screenWidth*screenHeight];	// egy alkalmazás ablaknyi kép
Gomb geza(100), bela(100), pepe(100);
Plane padlo;
Plane plafon;
Plane fal1, fal2, fal3;
Fokkha idomitott;

// Inicializacio, a program futasanak kezdeten, az OpenGL kontextus letrehozasa utan hivodik meg (ld. main() fv.)
void onInitialization( ) {
	glViewport(0, 0, screenWidth, screenHeight);
	glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glEnable(GL_NORMALIZE);
    text();
    lampa();
    cam.FunctionDirr();
    idomitott.setKnots();
    idomitott.setControlPoints();
    idomitott.setHeadPoints();
    idomitott.setCurve();
    idomitott.setHead();
    ballheight = 6.2f;
    balldirection = false;
}
// Rajzolas, ha az alkalmazas ablak ervenytelenne valik, akkor ez a fuggveny hivodik meg
void onDisplay( ) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);		// torlesi szin beallitasa
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // kepernyo torles
    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    tmp = -1 + (int)cam.eye.z % 3;
//*********Padlo
    glBindTexture(GL_TEXTURE_2D,texname[0]);
    glPushMatrix();
    glLoadIdentity();
        glTranslatef(cam.eye.x*(-1)-0.0f,cam.eye.y-10.0f,cam.eye.z*(-1)-50.0f);
        glScalef(0.25f,1.0f,0.5f);
        padlo.rajzol();
    glPopMatrix();
//*********Plafon
    glPushMatrix();
        glLoadIdentity();
        glTranslatef(0.0f,cam.eye.y +10.0f,cam.eye.z*(-1)-50.0f);
        glRotatef(180.0f,0.0f,0.0f,0.0f);
        glScalef(0.25f,1.0f,0.5f);
        plafon.rajzol();
    glPopMatrix();
//**********BalFal
    glPushMatrix();
        glLoadIdentity();
        glTranslatef(-25.0f,cam.eye.y+0.0f,cam.eye.z*(-1)-50.0f);
        glRotatef(90.0f,0.0f,0.0f,-1.0f);
        glScalef(0.1f,1.0f,0.5f);
        fal1.rajzol();
    glPopMatrix();
//*********Jobbfal
    glPushMatrix();
        glLoadIdentity();
        glTranslatef(25.0f,cam.eye.y+0.0f,cam.eye.z*(-1)-50.0f);
        glRotatef(90.0f,0.0f,0.0f,-1.0f);
        glScalef(0.1f,0.1f,0.5f);
        fal2.rajzol();
    glPopMatrix();
//*********Hatsofal
    glPushMatrix();
        glLoadIdentity();
        glTranslatef(0.0f,cam.eye.y+0.0f,cam.eye.z*(-1)-100.0f);
        glRotatef(90.0f,1.0f,0.0f,0.0f);
        glScalef(0.25f,0.1f,0.1f);
        fal3.rajzol();
    glPopMatrix();
//*********Laszti
    glBindTexture(GL_TEXTURE_2D,texname[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    float tmpdifcol[] = {0.1,0.1,0.1,1.0};
    float by[] = {0.5,0.5,0.5,1};
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, by);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, tmpdifcol);
    glPushMatrix();
        glLoadIdentity();
        glTranslatef(0.0f,cam.eye.y+ballheight,cam.eye.z*(-1)-44.0f);
        glScalef(1.0f,1.0f,1.0f);
        pepe.rajzol();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
//********fokkha
    glPushMatrix();
        glLoadIdentity();
        glTranslatef(+3.0, cam.eye.y-0.0f, cam.eye.z*(-1)-45.0f);
        glRotatef(90.0f,1.0f,0.0f,0.0f);
        glScalef(0.5f,0.2f,0.5f);
        idomitott.rajzol();
    glPopMatrix();
    glutSwapBuffers();     				// Buffercsere: rajzolas vege

}

// Billentyuzet esemenyeket lekezelo fuggveny
void onKeyboard(unsigned char key, int x, int y) {
    if (key == 'd') glutPostRedisplay( ); 		// d beture rajzold ujra a kepet
    if (key == 'f') { cam.eye.z-=1; cam.FunctionDirr(); }
    if (key == 'b') { cam.eye.z+=1; cam.FunctionDirr(); }
    if (key == 'u') { cam.eye.y-=1; cam.FunctionDirr(); }
    //if (key == 'd') { cam.eye.y+=1; cam.FunctionDirr(); }
    if (key == 's') { idomitott.state = BAL; }
    if (key == 'd') { idomitott.state = JOBB; }
    if (key == 'e') { idomitott.state = FEL; }
    if (key == 'x') { idomitott.state = LE; }
    if (key == 'a') { idomitott.state = ELORE; }
    if (key == 'y') { idomitott.state = HATRA; }
    glutPostRedisplay( );
}

// Eger esemenyeket lekezelo fuggveny
void onMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT && state == GLUT_DOWN)   // A GLUT_LEFT_BUTTON / GLUT_RIGHT_BUTTON illetve GLUT_DOWN / GLUT_UP
		glutPostRedisplay( ); 						 // Ilyenkor rajzold ujra a kepet
}

long oldTime = 0;
long epsilon = 100;
long epsilon2 = 20;
// `Idle' esemenykezelo, jelzi, hogy az ido telik, az Idle esemenyek frekvenciajara csak a 0 a garantalt minimalis ertek
void onIdle( ) {
    long time = glutGet(GLUT_ELAPSED_TIME);		// program inditasa ota eltelt ido
    long dt = time - oldTime;

    if (dt > epsilon){
        idomitott.setRotateVector();
    }
    if (dt > epsilon2){
        if (!balldirection){ ballheight -= 0.6; }
        else { ballheight += 0.6; }
        if (ballheight >= 7.2) balldirection = false;
        if (idomitott.state == FEL) {
            if (ballheight <= 2.5f) { balldirection = true; ballheight = 1.7f + idomitott.cntr*0.05f; }
        }
        else if (idomitott.state == LE){
            if (ballheight <= 1.4f) { balldirection = true; ballheight = 2.0f - idomitott.cntr*0.15f; }
        }
        else{
            if (ballheight <= (2.0f )) { balldirection = true; ballheight = 1.7f; }
        }
    }
    if (idomitott.cntr == 9){
        idomitott.dir = false;
    }
    if (idomitott.cntr == 0){
        idomitott.dir = true;
        idomitott.state = ALL;
        idomitott.cntr = 0.0f;
    }
     glutPostRedisplay();
}

// ...Idaig modosithatod
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// A C++ program belepesi pontja, a main fuggvenyt mar nem szabad bantani
int main(int argc, char **argv) {
    glutInit(&argc, argv); 				// GLUT inicializalasa
    glutInitWindowSize(600, 600);			// Alkalmazas ablak kezdeti merete 600x600 pixel
    glutInitWindowPosition(100, 100);			// Az elozo alkalmazas ablakhoz kepest hol tunik fel
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);	// 8 bites R,G,B,A + dupla buffer + melyseg buffer

    glutCreateWindow("Grafika hazi feladat");		// Alkalmazas ablak megszuletik es megjelenik a kepernyon

    glMatrixMode(GL_MODELVIEW);				// A MODELVIEW transzformaciot egysegmatrixra inicializaljuk
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);			// A PROJECTION transzformaciot egysegmatrixra inicializaljuk
    glLoadIdentity();

    onInitialization();					// Az altalad irt inicializalast lefuttatjuk

    glutDisplayFunc(onDisplay);				// Esemenykezelok regisztralasa
    glutMouseFunc(onMouse);
    glutIdleFunc(onIdle);
    glutKeyboardFunc(onKeyboard);

    glutMainLoop();					// Esemenykezelo hurok

    return 0;
}
