#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include "GameConstants.h"
#include "Actor.h"
#include <vector>
using namespace std;

class Star;
class Actor;
class NachenBlaster;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    ~StudentWorld();
    
    void addActor(Actor* act);
    
    int shipsOnScreen();
    void shipDead();
    int destroyedShips();
    void killShip();
    
    bool isNearNB(double ax, double ay);
    double distance (double x1, double y1, double x2, double y2);
    bool collisionNB (double x, double y, double radius);
    
    void NBloseHits(int hits);
    void NBgainHits(int hits);
    void NBgainTorpedoes(int torpedoes);
    
    Actor* collideWithAlien(Actor* act);
    
    bool canActorMoveTo(Actor* act, double x, double y);
    
    void setDisplayText();
    string formatDisplayText(int lives, int health, int score, int level, int cabbages, int torpedoes);
    
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    
private:
    vector<Actor*> m_v;
    NachenBlaster* m_nb;
    int m_current;
    int m_dead;
    
};

#endif // STUDENTWORLD_H_
