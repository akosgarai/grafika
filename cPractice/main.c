#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
/*
//3-al oszthato szamok 100-200 kozott
    int szam = 100;
    while (szam <= 200) {
        if (szam % 3 == 0)
            printf("%d, ", szam);
        szam++;
    }
*/
/*
//szam primtenyezos felbontasa
    int szam = 91;
    int oszto = 1;
    while ( szam + 1 != oszto) {
        if (oszto > 1 && szam % oszto == 0) {
            printf("%d|%d\n", szam, oszto);
            szam = szam / oszto;
            oszto = 1;
        } else {
            //printf("nemoszto: %d\n", oszto);
            oszto++;
        }
    }
    printf("1|1\n");
*/
//szam primtenyezoinek osszege
int primtenyezokosszege(int input) {
    int oszto, sum;
    oszto = 1;
    sum = 0;
    while (input + 1 != oszto) {
        if (oszto > 1 && input % oszto == 0) {
            sum += oszto;
            input = input / oszto;
            oszto = 2;
        } else {
            oszto++;
        }
    }
    return sum;
}
/*
//valodi osztok szama + prim szam-e
    int szam = 7;
    int osztokszama = 0;
    int oszto = 2;
    while ( oszto < szam ) {
        if (szam % oszto == 0)
            osztokszama++;
        oszto++;
    }
    if (osztokszama == 0) { printf("A szam prim!\n"); }
    else { printf("Valodi osztok szama: %d\n", osztokszama); }
*/
/*
//legnagyobb kozos osztot kiszamolo fuggveny
int lnko(int s1, int s2){
    int kisebb;
    int oszto = 1;
    int lnko = 1;
    if (s1 < s2) kisebb = s1;
    else kisebb = s2;
    while ( kisebb + 1 > oszto) {
        if (s1 % oszto == 0 && s2 % oszto == 0) lnko = oszto;
        oszto++;
    }
    return lnko;
}
    int a = lnko(150, 60);
    printf("lnko: %d\n", a);
*/
/*
//masodfoku egyenlet megoldasa
int masodfoku(double a, double b, double c) {
    double disc, x1, x2;
    disc = b * b + 4 * a * c;
    if (disc > 0) {
        x1 = (-b + sqrt(disc)) / (2 * a);
        x2 = (-b - sqrt(disc)) / (2 * a);
        printf("A gyokok:\n\tx1 = %g\tx2 = %g\n", x1, x2);
    } else printf("Nincs valos gyok.\n");
    return 0;
}
int valtozoBeolvasas() {
    double a, b, c;
    printf("A = ");
    scanf("%lg", &a);
    printf("B = ");
    scanf("%lg", &b);
    printf("C = ");
    scanf("%lg", &c);
    masodfoku(a, b, c);
    return 0;
}
    valtozoBeolvasas();
*/
//sok szam atlaga
int atlagolo() {
    double input, osszeg;
    int num;
    osszeg = 0.0;
    num = 0;
    printf("Sok szam atlaga:\nElso szam:");
    scanf("%lg", &input);
    while (input != 0.0) {
        osszeg += input;
        num++;
        printf("Kovetkezo szam? (utolso 0)");
        scanf("%lg", &input);
    }
    printf("A(z) %d szam atlaga: %g\n", num, osszeg/num);
    return 0;
}
//haromjegyu palindromszamok
int palindromok() {
    int a, b;
    for (a = 0; a < 10; a++) {
        for (b = 1; b < 10; b++) {
            printf("%d%d%d, ", b, a, b);
        }
    }
    return 0;
}
//ketdimenzios vektor elforgatasa
int forgatas() {
    double x, y, alpha, x_rotated, y_rotated;
    printf("X = "); scanf("%lg", &x);
    printf("Y = "); scanf("%lg", &y);
    printf("Szog = "); scanf("%lg", &alpha);

    alpha = alpha * M_PI / 180.0;

    x_rotated = x * cos(alpha) - y * sin(alpha);
    y_rotated = x * sin(alpha) + y * cos(alpha);

    printf("Az elforgatott vektor V(%g, %g)\n", x_rotated, y_rotated);
    return 0;
}
//szam szamjegyeinek osszege
int szamjegyosszeg(int input) {
    int sum, modulo;
    sum = 0;
    modulo = 0;
    //printf("Adjon meg egy szamot!\nszam = "); scanf("%d", &input);
    while (input != 0) {
        modulo = input % 10;
        sum += modulo;
        input = (input - modulo) / 10;
    }
    //printf("A szamjegyek osszege: %d\n", sum);
    return sum;
}
//harom szamrol eldonti, lehet-e egy haromszog harom oldala
int haromszogoldal() {
    double a, b, c;
    printf("A = "); scanf("%lg", &a);
    printf("B = "); scanf("%lg", &b);
    printf("C = "); scanf("%lg", &c);
    if (a < 0 || b < 0 || c < 0)
        printf("Negativ szam nem lehet.\n");
    else if (a + b > c && a + c > b && b + c > a)
        printf("Ebbol lehet haromszog.\n");
    else
        printf("Nem lehet haromszog.\n");
    return 0;
}
//legnagyobb haromjegyu szam, mely szamjegyeinek osszege megegyezik a primtenyezoinek osszegevel
int primesosszeg() {
    int input;
    for(input = 999; input > 0; input--) {
        if (szamjegyosszeg(input) == primtenyezokosszege(input)){
            printf("A legnagyobb ilyen szam: %d\n", input);
            return 0;
        }
    }
    return 0;
}
//szam ket tizedesjegyet bepakolja egy valtozoba.
int doubletoint(double input){
    int e_input;
    double tizedek;
    if (input < 0) input *= (-1);
    e_input = input / 1;
    tizedek = input - e_input;
    printf("tizedek: %g\n", tizedek*100);
    e_input = tizedek * 1000 / 10;
    printf("egeszresze: %d\n", e_input);

    return e_input;
}
int dtoi(){
    double input;
    printf("Szam = "); scanf("%lg", &input);
    doubletoint(input);
    return 0;
}
//sok szam kozul a legnagyobb
int biggest() {
    double biggest, input;
    //biggest = 0;
    printf("Adjon meg egy szamot (0.0 vege): "); scanf("%lg", &input);
    if (input != 0.0) {
        biggest = input;
    } else {
        printf("Nincs legnagyobb:(\n");
        return 0;
    }
    while (input != 0.0) {
        if (input > biggest) biggest = input;
        printf("Adjon meg egy szamot (0.0 vege): "); scanf("%lg", &input);
    }
    printf("A legnagyobb beirt szam %g volt.\n", biggest);
    return 0;
}
//10-es szorzotabla
int szorzotabla() {
    int first, next;
    for (first = 0; first < 10; first++) {
        for (next = 0; next < 10; next++) {
            if (first == 0 && next == 0) printf("\t");
            if (next != 0 && first == 0) printf("%d\t", next);
            if (next == 0 && first != 0) printf("%d\t", first);
            if (next != 0 && first != 0) printf("%d\t", first*next);
        }
        printf("\n");
    }
    return 0;
}
//tokeletes szam-e
int osztoosszeg(int input) {
    int result, oszto;
    result = 0;
    for (oszto = 1; oszto <= input/2; oszto++) {
        if (input % oszto == 0) result += oszto;
    }
    //printf("osszeg: %d", result);
    //if (input == result) printf("tokeletes szam\n");
    return result;
}
//eldonti, hogy baratsagos szamparok-e
int baratsagosSzamparok() {
    int a, b;
    printf("Elso szam: "); scanf("%d", &a);
    printf("Masik szam: "); scanf("%d", &b);
    //tmp_a = osztoosszeg(a);
    //tmp_b = osztoosszeg(b);
    if (b == osztoosszeg(a) && a == osztoosszeg(b)) printf("A(z) %d es %d szampar baratsagos.\n", a, b);
    else printf("Nem baratsagos szampar.\n");
    return 0;
}
int main()
{
    baratsagosSzamparok();
    printf("Hello world!\n");
    return 0;
}
