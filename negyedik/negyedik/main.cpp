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
class Object {
public:
Color col;
virtual void Draw() {}
};
class Sphere :public Object{
public:
Vector center;
float radius;
Sphere() {center = Vector(0,0,0); radius = 0.5f; col = Color(1,0,0); }
};
GLubyte  csempe[16][16][3];
GLubyte  fal[16][16][3];
GLubyte  fej[16][16][3];
GLubyte  kar[16][16][3];
void text(){
    for (int i = 0; i< 16; i++){
        for (int j = 0; j < 16; j++){
                csempe[i][j][0] = rand()%12;
                csempe[i][j][1] = rand()%72;
                csempe[i][j][2] = rand()%142;//0;
                int t  = rand()%20;

                fal[i][j][0] =30+t;
                fal[i][j][1] =30+t;
                fal[i][j][2] =30+t;

                t = rand()%70;
                fej[i][j][0] =80+t;
                fej[i][j][1] =50;
                fej[i][j][2] =0;

                t = rand()%50;
                kar[i][j][0] =200+rand()%60;
                kar[i][j][1] =140+rand()%60;
                kar[i][j][2] =255;
        }
    }
    /* ------ Textura "epites"------------*/
    glGenTextures(2, texname);
    glBindTexture(GL_TEXTURE_2D, texname[0]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 16, 16,0, GL_RGB, GL_UNSIGNED_BYTE, csempe);

    glBindTexture(GL_TEXTURE_2D, texname[1]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 16, 16,0, GL_RGB, GL_UNSIGNED_BYTE, fal);

    glBindTexture(GL_TEXTURE_2D, texname[2]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 16, 16,0, GL_RGB, GL_UNSIGNED_BYTE, fej);

    glBindTexture(GL_TEXTURE_2D, texname[3]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 16, 16,0, GL_RGB, GL_UNSIGNED_BYTE, kar);

}
class Plane : public Object{
public:
    Plane(){ }
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
                    glTexCoord2i(0,1);
                    glVertex3f((i+0)*1.0f,0.0,(j+1)*1.0f);
                glEnd();
            }
        }
    }
};
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
    }
/* Destruktor */
    virtual ~Gomb(){
        delete[] v;
    }
};
const int screenWidth = 600;	// alkalmazás ablak felbontása
const int screenHeight = 600;


Color image[screenWidth*screenHeight];	// egy alkalmazás ablaknyi kép
Camera cam;
//Brick tegla;
Gomb geza(100);
Plane padlo;

// Inicializacio, a program futasanak kezdeten, az OpenGL kontextus letrehozasa utan hivodik meg (ld. main() fv.)
void onInitialization( ) {
	glViewport(0, 0, screenWidth, screenHeight);
    text();
    cam.FunctionDirr();

}
// Rajzolas, ha az alkalmazas ablak ervenytelenne valik, akkor ez a fuggveny hivodik meg
void onDisplay( ) {
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);		// torlesi szin beallitasa
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // kepernyo torles
    glEnable(GL_TEXTURE_2D);
//*********Laszti
    glBindTexture(GL_TEXTURE_2D,texname[1]);
    //const float head[]= {1.0f,1.0f,1.0f,1.0f};
    glPushMatrix();
        glTranslatef(0.0f,5.0f,-30.0f);
        glScalef(3.0f,3.0f,3.0f);
        geza.rajzol();
    glPopMatrix();
//*********Padlo
    glBindTexture(GL_TEXTURE_2D,texname[0]);
    glPushMatrix();
        glTranslatef(0.0f,-5.0f,0.0f);
        padlo.rajzol();
        //glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Spec);
        //glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Null);
        //glMaterialfv(GL_FRONT, GL_DIFFUSE, Diffuz2);
            /*for (int i = -100;i<100;i++){
                for (int j = -100;j < 100;j++){
                    glBegin(GL_QUADS);
                        glNormal3f(0.0,1.0,0.0);
                        glTexCoord2i(0,0);
                        glVertex3f((i+0)*1.0f,-1.0,(j+0)*1.0f);
                        glTexCoord2i(1,0);
                        glVertex3f((i+1)*1.0f,-1.0,(j+0)*1.0f);
                        glTexCoord2i(1,1);
                        glVertex3f((i+1)*1.0f,-1.0,(j+1)*1.0f);
                        glTexCoord2i(0,1);
                        glVertex3f((i+0)*1.0f,-1.0,(j+1)*1.0f);
                    glEnd();
                }
            }*/
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glutSwapBuffers();     				// Buffercsere: rajzolas vege

}

// Billentyuzet esemenyeket lekezelo fuggveny
void onKeyboard(unsigned char key, int x, int y) {
    if (key == 'd') glutPostRedisplay( ); 		// d beture rajzold ujra a kepet
    if (key == 'f') { cam.eye.z-=1; cam.FunctionDirr(); }
    if (key == 'b') { cam.eye.z+=1; cam.FunctionDirr(); }
    glutPostRedisplay( );
}

// Eger esemenyeket lekezelo fuggveny
void onMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT && state == GLUT_DOWN)   // A GLUT_LEFT_BUTTON / GLUT_RIGHT_BUTTON illetve GLUT_DOWN / GLUT_UP
		glutPostRedisplay( ); 						 // Ilyenkor rajzold ujra a kepet
}

// `Idle' esemenykezelo, jelzi, hogy az ido telik, az Idle esemenyek frekvenciajara csak a 0 a garantalt minimalis ertek
void onIdle( ) {
     long time = glutGet(GLUT_ELAPSED_TIME);		// program inditasa ota eltelt ido

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
