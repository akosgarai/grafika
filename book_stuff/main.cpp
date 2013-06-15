#include <iostream>
#include <string>

using namespace std;
//const variables
const int bagcapacity = 10;

//Stuffs, has amount, name. the character can collet and use them.
class Stuff {
private:
    int stuffAmount;        //amount
    string stuffName;       //name
public:
    Stuff() { this->stuffAmount = 0; this->stuffName = '.'; }                       //default constructor
    Stuff(string name) { this->stuffAmount = 0; this->stuffName = name; }           //name constructor,
    Stuff(int num, string name) { this->stuffAmount = num; this->stuffName = name; }    //name and amount

    //get methods
    int getAmount() { return this->stuffAmount; }
    string getName() { return this->stuffName; }

    //add method increase the amount
    void add(int i) {
        this->stuffAmount +=i;
        cout << "You found " << i << " new " << this->getName() <<endl;
    }

    //use method, decrease the amount if possible
    void use(int i) {
        if(stuffAmount > i) {
            this->stuffAmount -= i;
            cout << "You lost " << i << " piece of " << this->getName() <<endl;
        }
        else { cout << "You have less than " << i <<" " << this->getName() <<" You have only " << this->getAmount() << endl; }
    }
};

//bag to store bagcapacity number of stuffs
class Bag {
private:
    Stuff myStuffs[bagcapacity];                    //how many different kind of stuffs could be stored
public:
    int stuffNum;                                   //the number of stuffs
    Bag() { stuffNum = 0; }                         //default constructor, in the beginning we have nothing

    //put a new stuff to the bag or rase the amount if the stuff isn't new
    void addStuff(Stuff s) {
        if(stuffNum < bagcapacity) {
            for(int i = 0; i < stuffNum; i++) {
                if(this->myStuffs[i].getName() == s.getName()) {
                    this->myStuffs[i].add(s.getAmount());
                    return;
                }
            }
            myStuffs[stuffNum] = s;
            stuffNum++;
            cout << "Your found your " << stuffNum <<". stuff: " << this->myStuffs[stuffNum-1].getName() << "\n";
        } else {
            cout << "Sorry, but you are full. You should drop something.\n";
        }
    }

    //returns true if there is a stuff in the bag which named s and has greater amount than 0
    bool getStuff(string s) {
        for(int i = 0; i < stuffNum; i++) {
            if(this->myStuffs[i].getName() == s && this->myStuffs[i].getAmount() > 0) {
                    return true;
            }
            return false;
        }
    }
};

//Stuffs like ammo for weapons, medikit, and other fun
Stuff normalGun(8, "Pisztoly");
Stuff shotGun("Shotgun");
Stuff machineGun("Geppuska");

//the player class
class Character {
private:
    int life;           //life points
    int IP;             //IP-s for doing things
    Bag myBag;          //collection of stuffs that player can use
public:
    Character() { this->life = 10; this->IP = 0; this->myBag.addStuff(normalGun); this->myBag.addStuff(shotGun); this->myBag.addStuff(machineGun); }
    int getLife() { return this->life; }        //returns the current lifepoints
    void writeLife() { cout << "Lara\'s life: " << this->life << endl;}     //prints the current lifepoints
    void writeIP() { cout << "Lara\'s IP number: " << this->IP << endl; }   //prints the current IP points
    void modifyLife(int i) { this->life -= i;                               //modify lifepoints (heal or injury)
        if (this->life <= 0)
            cout << "You died.\n";
        if (this->life > 10) {
            this->life = 10;
            cout << "Your life is full, you can\'t be healthier!\n";
        }
    }
    int getIP() { return this->IP; }
    void setIP(int i) { this->IP += i; }
    bool findInBag(string s) {
        return this->myBag.getStuff(s);
    }

};
//And the player is:
Character Lara;

class Chapter {
private:
    int chapterNumber;
    int nextChapters[5];
public:

};
int main()
{

    Lara.writeLife();
    Lara.modifyLife(13);
    Lara.writeLife();
    Lara.modifyLife(-103);
    Lara.writeLife();
    Lara.writeIP();
    return 0;
}
