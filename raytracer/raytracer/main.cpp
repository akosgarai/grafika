//=============================================================================================
// Szamitogepes grafika hazi feladat keret. Ervenyes 2012-tol.
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
// Nev    : Garai Akos
// Neptun : CQYOEH
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

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
// MsWindows-on ez is kell
#include <windows.h>
#endif // Win32 platform

#include <GL/gl.h>
#include <GL/glu.h>
// A GLUT-ot le kell tolteni: http://www.opengl.org/resources/libraries/glut/
#include <GL/glut.h>


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Innentol modosithatod...

//--------------------------------------------------------
// 3D Vektor
//--------------------------------------------------------
const float PI = atan(1.0)*4.0;
const float EPSILON = 0.00001;
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
   Vector operator/(float a) {
	return Vector(x / a, y / a, z / a);
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
   Vector& normalize() // egysegnyi hosszura allitja a vektort
	{
		return *this = *this * (1 / sqrt(x * x + y * y + z * z));
	}
   float Length() { return sqrt(x * x + y * y + z * z); }

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
class NVector {
public:
    float x,y,z,k;
    NVector(){ x = y = z = k = 0; }
    NVector(Vector v, float ki) { x = v.x; z = v.y; y = v.z; k = ki; }
    NVector operator*(const Matrix& mat) {
        NVector result;
        result.x = x*mat.m[0][0] + y*mat.m[0][1] + z*mat.m[0][2] + k*mat.m[0][3];
        result.y = x*mat.m[1][0] + y*mat.m[1][1] + z*mat.m[1][2] + k*mat.m[1][3];
        result.z = x*mat.m[2][0] + y*mat.m[2][1] + z*mat.m[2][2] + k*mat.m[2][3];
        result.k = x*mat.m[3][0] + y*mat.m[3][1] + z*mat.m[3][2] + k*mat.m[3][3];
        return result;
    }
    float operator*(const NVector& nv) {
        float result;
        result = x*nv.x + y*nv.y + z*nv.z + k*nv.k;
        return result;
    }
};
class Intersection {
public:
    float t;
    Vector norm;
    Intersection(float f, Vector v){ t = f; norm = v; }
};
class Sugar {
public:
    Vector start, irany;
    Sugar() {}
    Sugar(Vector s, Vector i){ start = s; irany = i; }
};
class Camera {
public:
    Vector pos, normal, up;
    Vector i, j, eye;
    float tavolsag;
    void init() { eye = pos + (normal * tavolsag); i = (normal % (up*(-1.0))).normalize();  j = (normal % this->i).normalize(); }
    Camera() { pos = Vector(0.0, -3.0, 0.0); normal = Vector(1.0, 0.0, 0.0); up = Vector(0.0, 0.0, 1.0); tavolsag = 3.0; init(); }
    Vector P(float x, float z){                                                                    // Az aktu?lis P pont ?tkonvert?l?sa
        x = x - 300;                                                                                            // leosztjuk 300-zal , hogy meg legyen hogy, a (-1,1)-es k?perny?n, hol van
        z = z -300;                                                                                                // a P pontunk.
        x /= 300;
        z /= 300;
        return pos + (this->i * x) + (this->j * z);
    }

};
Camera cam;
class Fenyforras {
public:
    Vector place;
    Color col;
};
class Object {
public:
    Color Fresn;	// Fresnel egyutthato
	Color n;		// toresmutato
	Color k;		// kioltasi tenyezo
	Color color;	// objektum sajat szine
	bool reflective; // true: tukrozo felulet, false: diffuz felulet
	bool refractive;
	Object() { reflective = false; refractive = false; }
	virtual Intersection Intersect(Sugar r) = 0;	// metszes sugarral, visszaadja a tavolsagot
	Color Fresnel(float costheta){
        float f0 = (pow(n.r-1, 2)+pow(k.r, 2))/(pow(n.r+1,2) + pow(k.r,2));
        float f1 = (pow(n.g-1, 2)+pow(k.g, 2))/(pow(n.g+1,2) + pow(k.g,2));
        float f2 = (pow(n.b-1, 2)+pow(k.b, 2))/(pow(n.b+1,2) + pow(k.b,2));
        return Color((f0+((1-f0)*pow(1-costheta, 5))), (f1+((1-f1)*pow(1-costheta, 5))), (f2+((1-f2)*pow(1-costheta, 5))));
	}
};
class Sphere : public Object {
public:
    float radius;
    Vector center;
    Matrix matr;

    Sphere() { radius = 1.0f; center = Vector(0.0, 0.0, 0.0); color = Color(1.0f, .0f, 0.0f); }
    Sphere(float r, Vector v, Color c) { radius = r; center = v; color = c; }

    Intersection Intersect(Sugar ray){
		float t;
		float A = ray.irany * ray.irany;
		float B = 2*((ray.start-center)*ray.irany);
		float C = ((ray.start-center)*(ray.start-center)) - (radius*radius);
		float D = B*B - 4*A*C; // diszkriminans
		if(D < 0){
		    return Intersection(-1,Vector(0,0,0));
		}
		float t1 = ((-B) + sqrt(D))/(2*A);
		float t2 = ((-B) - sqrt(D))/(2*A);
		if(t1 < 0.0) return Intersection(-1,Vector(0,0,0));
		if(t2 > 0.0) t = t2;
		else t = t1;
		return Intersection(t,((ray.start+ray.irany*t)-center)/radius);
	}

};
class Table : public Object{
public:
    float y;
    float wi, he, lo;
    Vector corner;
    Table() { y = 0.0f; }
    Table(float zv, Color c) {
        y = zv; color = c;
        wi = 800; he = 600; lo = 100;
        corner = Vector(-400,-400, y);
    }
    Intersection Intersect(Sugar ray){
        if(ray.irany.z == 0.0f){
            return Intersection(-1,Vector(0,0,0));
        }
        else {
            float t = (y-ray.start.z)/ray.irany.z;
            Vector mp = ray.start + ray.irany*t;
            if(((int)mp.x <= -40 && (int)mp.y <= -3) || ((int)mp.x > -40 && (int)mp.y > -3)) { color = Color(0,1,0); }
            else {
                color = Color(1,0,0);
                }
            return Intersection((y-ray.start.z)/ray.irany.z,Vector(0,0,1));
        }
    }
    Vector Norm(Vector p){ return Vector(0,1,0); }
};
class Henger : public Object {
public:
    Vector center;
    float rad;
    float h;
    Matrix quadr;
    void setMatrix(){
        quadr.Clear();
        quadr.m[0][0] = quadr.m[2][2] = (1/(rad*rad));
        quadr.m[3][3] = -1;
    }
    Henger(){ center = Vector(0,0,0); rad = 1.0f; h = 1.0f; color = Color(0.0f, 1.0f, 1.0f);}
    Henger(Vector cpos, float radh, float hh, Color c) { center = cpos; rad = radh; h = hh; color = c; setMatrix(); }
    Intersection Intersect(Sugar ray){
        float A, B, C, t;
        NVector o(ray.start-center, 1);
        NVector d(ray.irany, 0);
        A = ((d * quadr) * d);
        B = ((o * quadr) * d) + ((d * quadr) * o);
        C = ((o * quadr) * o);
        float D;
        D = B*B-4*A*C;
        if(D < 0){
		    return Intersection(-1,Vector(0,0,0));
		}
		float t1 = ((-B) + sqrt(D))/(2*A);
		float t2 = ((-B) - sqrt(D))/(2*A);
		if(t1 < 0.0) return Intersection(-1,Vector(0,0,0));
		if(t2 > 0.0) t = t2;
		else t = t1;
		if(A == 0 && B != 0 ){ t = -C/B; if(t < 0.0){ return Intersection(-1, Vector(0,0,0)); }}
		if(t < EPSILON) t = EPSILON;
		Vector result = ((ray.start-center+(ray.irany*t))-center); //.normalize();
		if ((result.z) > center.z && result.z < center.z+(h)) {
		    result = result.normalize();
            return Intersection(t, result);
		}else {
            return Intersection(-1,Vector(0,0,0));
		}
    }

};
Fenyforras feny;
Object *objs[4];
int nObjects = 3;//4;
Color Trace(Sugar r, int depth){
	Color c(0.3,0.5,1.0);
	Color c1, c2;
	Intersection t (2e30,Vector(0,0,0));
	int intersectObj = -1; // eltalalt objektum indexe
	if(depth == 0) {
        return Color(0,0,0);
	}
	for(int i = 0; i < nObjects; i++){
		Intersection temp = objs[i]->Intersect(r);
		if(temp.t >= 0 && temp.t < t.t){
			t = temp;
			intersectObj = i;
		}
	}

	if(intersectObj >= 0){
		Vector intersectP = r.start + r.irany*t.t;
		Vector norm = t.norm;
		Vector lightVec = (feny.place-intersectP).normalize();
		if(objs[intersectObj]->reflective || objs[intersectObj]->refractive){
			Color fresnel = objs[intersectObj]->Fresnel((intersectP-feny.place).normalize() * norm * (-1.0));
			float cosalpha = r.irany * norm * (-1.0);
			float n = objs[intersectObj]->n.r;
			Sugar r2;
			if(objs[intersectObj]->reflective){
			    c2.r=c2.g=c2.b=0;
				Vector vRefl = r.irany + norm*cosalpha*2;
				r2.start = intersectP + norm*0.01;
				r2.irany = vRefl;
				c2 = fresnel * Trace(r2,depth-1);
			}
			 if(objs[intersectObj]->refractive){
			     c1.r=c1.g=c1.b=0;
				Vector tempNorm = norm;
				if(cosalpha < 0){
					tempNorm = tempNorm * (-1.0);
					n = 1/n;
					cosalpha = r.irany * tempNorm * (-1.0);
				}
				float d = 1.0-((1.0-cosalpha*cosalpha)/(n*n));
				if(d >= 0){
					Vector vRefr = r.irany/n + tempNorm*(cosalpha/n-sqrt(d));
					r2.start = intersectP - tempNorm*0.1;
					r2.irany = vRefr;
					r2.irany.normalize();
					c1 = Trace(r2,depth-1)*n;
				}
			}
			if(intersectObj == 1){
                c = objs[intersectObj]->color * feny.col * (norm * lightVec)*c2*c1*600;
			}
			else{
                c = objs[intersectObj]->color * feny.col * (norm * lightVec) * c1 * c2*500;
			}
		}else{
			c = objs[intersectObj]->color * feny.col * (norm * lightVec);
		}
	}

	return c;
}
Vector fenypos(50,0,300);
Color fenycol(1,1,0);
const int screenWidth = 600;	// alkalmazás ablak felbontása
const int screenHeight = 600;
Sphere sph;

Color image[screenWidth*screenHeight];	// egy alkalmazás ablaknyi kép


// Inicializacio, a program futasanak kezdeten, az OpenGL kontextus letrehozasa utan hivodik meg (ld. main() fv.)
void onInitialization( ) {
	glViewport(0, 0, screenWidth, screenHeight);

    feny.col = Color(1,1,0);
    feny.place = Vector(10,-30,0);
/*    objs[0] = new Sphere(2, Vector(-50,0,-1), Color(1,0,0));
    objs[0]->n = Color(1.5,1.5,1.5);
    objs[0]->k = Color(0,0,0);
    //objs[0]->reflective = true;*/
    objs[0] = new Table(-6, Color(1,1,1));

    objs[1] = new Henger(Vector(-40.0,-3,-2), 1.9f, 9.0f, Color(1,1,0));
    objs[1]->n = Color(1.3,1.3,1.3);
    objs[1]->k = Color(1.7,3.1,25.0);
    objs[1]->reflective = true;
    objs[1]->refractive = true;

    objs[2] = new Henger(Vector(-40,-3,-2.1), 2.0f, 10.0f, Color(1,1,1));
    objs[2]->n = Color(1.5,1.5,1.5);
    objs[2]->k = Color(0,0,0);
    objs[2]->reflective = true;
    objs[2]->refractive = true;


    for(int y = 0; y <  screenHeight; y++) {
        for(int x = 0; x <  screenHeight; x++){

            Vector p = cam.P(x/1.0f,y/1.0f);
            Sugar s = Sugar(cam.eye, (p - cam.eye).normalize());
            Color c = Trace(s, 9);
            image[y*screenWidth + x]= c;
        }
    }

}

// Rajzolas, ha az alkalmazas ablak ervenytelenne valik, akkor ez a fuggveny hivodik meg
void onDisplay( ) {
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);		// torlesi szin beallitasa
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // kepernyo torles
    // Peldakent atmasoljuk a kepet a rasztertarba
    glDrawPixels(screenWidth, screenHeight, GL_RGB, GL_FLOAT, image);
    // ...
    glutSwapBuffers();     				// Buffercsere: rajzolas vege

}

// Billentyuzet esemenyeket lekezelo fuggveny
void onKeyboard(unsigned char key, int x, int y) {
    if (key == 'd') glutPostRedisplay( ); 		// d beture rajzold ujra a kepet

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

