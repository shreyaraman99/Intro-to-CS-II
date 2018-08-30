#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameWorld.h"
#include "StudentWorld.h"


class StudentWorld;

/***************** ACTOR CLASS ********************/
class Actor: public GraphObject {    // base class
public:
    Actor(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* world);
    virtual ~Actor();
    virtual void doSomething() = 0;
    virtual bool isAlive() = 0;
    
    StudentWorld* getStudentWorld() const;
    void moveToIfCan(double x, double y);
    
    virtual void setAlive();
    virtual void setDead();
    
    virtual int getID();
    
    
private:
    bool m_alive;
    int m_ID;
    StudentWorld* m_world;
    
};


/******************* STAR CLASS *****************/
class Star: public Actor {
public:
    Star(double startX, double startY, StudentWorld* world);
    virtual void doSomething();
    virtual bool isAlive();
private:
    bool m_alive;
};



/******************* EXPLOSION CLASS *********************/
class Explosion: public Actor {
public:
    Explosion(double startX, double startY, StudentWorld* world);
    virtual void doSomething();
    virtual bool isAlive();
    virtual void setDead();
private:
    bool m_alive;
    StudentWorld* m_world;
};




/******************* NACHENBLASTER CLASS *******************/
class NachenBlaster: public Actor {
public:
    NachenBlaster(StudentWorld* world);
    virtual void doSomething();
    void loseHitPoints(int hits);
    void addHitPoints(int hits);
    void addTorpedoes(int torpedoes);
    void addCabbages();
    int getHealth();
    int getCabbagePercent();
    int getTorpedoInventory();
    virtual bool isAlive();
    
private:
    int m_hitPoints;
    int m_cabbagePoints;
    int m_torpedoes;
    bool m_alive;
};

/****************** ALIEN CLASS *****************/
class Alien: public Actor {
public:
    Alien(int imageID, double startX, double startY, StudentWorld* world, int flight_plan, double speed);
    
    virtual void doSomething() = 0;
    virtual int flightPlan() = 0;
    virtual int travelSpeed() = 0;
    virtual void sufferDamage(int hits) = 0;
    virtual void possiblyDropGoodie() = 0;
    virtual void whenAlienDies() = 0;
    
    virtual int getID();
    
    void travelInDirection();
    void moveDownAndLeft();
    void moveUpAndLeft();
    void moveDueLeft();
    
private:
    int m_ID;
    int m_speed;
    int m_flightPlan;
    int m_hitPoints;
    int m_travelDirection;
};



/**************** SMALLGON CLASS ****************/
class Smallgon: public Alien {
public:
    Smallgon(double startX, double startY, StudentWorld* world);
    virtual void doSomething();
    virtual bool isAlive();
    virtual int flightPlan();
    virtual int travelSpeed();
    virtual void possiblyDropGoodie();
    virtual void whenAlienDies();
    virtual void sufferDamage(int hits);
    
private:
    int m_ID;
    int m_flightPlan;
    int m_speed;
    int m_alive;
    int m_hitPoints;
};


/**************** SMOREGON CLASS ****************/
class Smoregon: public Alien {
public:
    Smoregon(double startX, double startY, StudentWorld* world);
    virtual void doSomething();
    virtual bool isAlive();
    virtual int flightPlan();
    virtual int travelSpeed();
    virtual void possiblyDropGoodie();
    virtual void whenAlienDies();
    virtual void sufferDamage(int hits);
    
private:
    int m_ID;
    int m_flightPlan;
    int m_speed;
    int m_alive;
    int m_hitPoints;
};


/**************** SNAGGLEGON CLASS ****************/
class Snagglegon: public Alien {
public:
    Snagglegon(double startX, double startY, StudentWorld* world);
    virtual void doSomething();
    virtual bool isAlive();
    virtual int flightPlan();
    virtual int travelSpeed();
    virtual void possiblyDropGoodie();
    virtual void whenAlienDies();
    virtual void sufferDamage(int hits);
    
private:
    int m_ID;
    int m_flightPlan;
    int m_speed;
    int m_alive;
    int m_hitPoints;
};





/**************** CABBAGE CLASS *****************/
class Cabbage: public Actor {
public:
    Cabbage(double startX, double startY, StudentWorld* world);
    virtual void doSomething();
    virtual bool isAlive();
private:
    bool m_alive;
    StudentWorld* m_world;
};


/**************** TURNIP CLASS *****************/
class Turnip: public Actor {
public:
    Turnip(double startX, double startY, StudentWorld* world);
    virtual void doSomething();
    virtual bool isAlive();
private:
    bool m_alive;
    StudentWorld* m_world;
};

/**************** TORPEDO CLASS *****************/
class Torpedo: public Actor {
public:
    Torpedo(double startX, double startY, int dir, StudentWorld* world);
    virtual void doSomething();
    virtual bool isAlive();
private:
    bool m_alive;
    StudentWorld* m_world;
};




/*************** GOODIE CLASS *****************/
class Goodie: public Actor {
public:
    Goodie(int imageID, double startX, double startY, StudentWorld* world);
    virtual void doSomething();
    virtual void grantSpecificReward() = 0;
    virtual bool isAlive();
private:
    bool m_alive;
};


/**************** REPAIR GOODIE CLASS ************/
class RepairGoodie: public Goodie {
public:
    RepairGoodie(double startX, double startY, StudentWorld* world);
    virtual void grantSpecificReward();
};


/**************** EXTRA LIFE GOODIE CLASS ************/
class ExtraLifeGoodie: public Goodie {
public:
    ExtraLifeGoodie(double startX, double startY, StudentWorld* world);
    virtual void grantSpecificReward();
};


/**************** TORPEDO GOODIE CLASS ************/
class TorpedoGoodie: public Goodie {
public:
    TorpedoGoodie(double startX, double startY, StudentWorld* world);
    virtual void grantSpecificReward();
};





#endif // ACTOR_H_
