#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include "Actor.h"
#include <vector>
#include <cmath>
#include <sstream>
#include <iostream>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
    return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(string assetDir): GameWorld(assetDir) {
    m_current = 0;
    m_dead = 0;
}

StudentWorld::~StudentWorld() {
    cleanUp();
}

void StudentWorld::addActor(Actor* act) {
    m_v.push_back(act);
}

int StudentWorld::shipsOnScreen() {
    return m_current;
}

void StudentWorld::shipDead() {
    m_current--;
}

int StudentWorld::destroyedShips() {
    return m_dead;
}

void StudentWorld::killShip() {
    m_dead++;
    shipDead();
}

bool StudentWorld::isNearNB(double ax, double ay) {
    if (ax > m_nb->getX() && ay >= m_nb->getY() - 4 && ay <= m_nb->getY() + 4)
        return true;
    return false;
}

double StudentWorld::distance(double x1, double y1, double x2, double y2) {
    double diffX = x1 - x2;
    double diffY = y1 - y2;
    return sqrt ((diffX * diffX) + (diffY * diffY));
}

bool StudentWorld::collisionNB(double x, double y, double radius) {
    if ( distance(x, y, m_nb->getX(), m_nb->getY()) < 0.75 * (radius + m_nb->getRadius()))
        return true;
    return false;
}

void StudentWorld::NBloseHits(int hits) {
    m_nb->loseHitPoints(hits);
}

void StudentWorld::NBgainHits(int hits) {
    m_nb->addHitPoints(hits);
}

void StudentWorld::NBgainTorpedoes(int torpedoes) {
    m_nb->addTorpedoes(torpedoes);
}


Actor* StudentWorld::collideWithAlien(Actor* act)
{
    int x = act->getX();
    int y = act->getY();
    int r = act->getRadius();
    int xAct, yAct, rAct;
    vector<Actor*>::iterator it;
    it = m_v.begin();
    while (it != m_v.end())
    {
        if ((*it)->getID() != IID_SMALLGON && (*it)->getID() != IID_SMOREGON && (*it)->getID() != IID_SNAGGLEGON)
        {
            it++;
            continue;
        }
        xAct = (*it)->getX();
        yAct = (*it)->getY();
        rAct = (*it)->getRadius();
        if (distance(x, y, xAct, yAct) <= 0.75 * (r + rAct))
            return *it;
        it++;
    }
    return nullptr;
}


bool StudentWorld::canActorMoveTo(Actor* act, double x, double y) {
    if (x < 0 || x >= VIEW_WIDTH || y < 0 || y >= VIEW_HEIGHT)
        return false;
    return true;
}

void StudentWorld::setDisplayText() {
    int lives = getLives();
    int health = m_nb->getHealth();
    int score = getScore();
    int level = getLevel();
    int cabbages = m_nb->getCabbagePercent();
    int torpedoes = m_nb->getTorpedoInventory();
    string s = formatDisplayText(lives, health, score, level, cabbages, torpedoes);
    setGameStatText(s);
}

string StudentWorld::formatDisplayText(int lives, int health, int score, int level, int cabbages, int torpedoes) {
    stringstream s;
    
    s << "Lives: " << lives;
    
    s << "  Health: " << health << '%';
    
    s << "  Score: " << score;
    
    s << "  Level: " << level;
    
    s << "  Cabbages: " << cabbages << '%';
    
    s << "  Torpedoes: " << torpedoes;
    
    return s.str();
}




/************** INIT *****************/
int StudentWorld::init()
{
    m_dead = 0;
    m_current = 0;
    
    double xPos, yPos;
    m_nb = new NachenBlaster(this);
    
    
    /************* ADD STARS **********/
    for (int i = 0; i < 30; i++) {
        xPos = rand() % 256;        // random int between 0 and 255
        yPos = rand() % 256;        // random int between 0 and 255
        m_v.push_back(new Star(xPos, yPos, this));
    }
    
    return GWSTATUS_CONTINUE_GAME;
}




/********** MOVE *********/
int StudentWorld::move()
{
    setDisplayText();
    
    
    /******* ADD STARS ******/
    double yPos = rand() % 256;
    if (rand() % 15 + 1 == 1)         // 1/15 chance
        m_v.push_back(new Star(VIEW_WIDTH - 1, yPos, this));
    
    
    
    
    
    /****** ADD NEW ALIEN SHIP ******/
    int T = 6 + (4 * getLevel());
    int D = destroyedShips();
    int R = T - D;
    int M = 4 + (0.5 * getLevel());
    double min = 0;
    
    if (M < R)
        min = M;
    else
        min = R;
    if (shipsOnScreen() < min) {
        int S1 = 60;
        int S2 = 20 + getLevel() * 5;
        int S3 = 5 + getLevel() * 10;
        int S = S1 + S2 + S3;
        int prob = (rand() % S + 1);       // random int between 1 and S1
        if (prob <= S1) {                               // S1/S chance
            m_v.push_back(new Smallgon(VIEW_WIDTH - 1, yPos, this));
            m_current++;
        }
        else if (prob <= S1 + S2 && prob > S1) {         // S2/S chance
            m_v.push_back(new Smoregon(VIEW_WIDTH - 1, yPos, this));
            m_current++;
        }
        else if (prob <= S && prob > S1 + S2) {         // S3/S chance
            m_v.push_back(new Snagglegon(VIEW_WIDTH - 1, yPos, this));
            m_current++;
        }
    }
    
    
    
    /*********** each actor does something or delete if dead/off screen *****************/
    for (vector<Actor*>::iterator it = m_v.begin(); it != m_v.end();) {
        if ( (*it)->getX() >= 0  && (*it)->isAlive() ) {
            (*it)->doSomething();
            *it++;
        }
        else {
            delete *it;
            *it = nullptr;
            it = m_v.erase(it);
        }
    }
    
    
    /********** delete dead actors *********/
     vector<Actor*>::iterator itD;
     itD = m_v.begin();
     while (itD != m_v.end())
     {
         if (!(*itD)->isAlive()) {
            delete *itD;
             *itD = nullptr;
             itD = m_v.erase(itD);
         }
         else
             itD++;
     }

    
    /***** if nachenblaster is dead, reduce lives *****/
    if (!m_nb->isAlive()) {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    
    
    
    /***** completed level!!! *****/
    if (R == 0) {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    
    m_nb->doSomething();
    m_nb->addCabbages();
    
    return GWSTATUS_CONTINUE_GAME;
    
}


/************ CLEAN UP **********/
void StudentWorld::cleanUp()
{
    
    vector<Actor*>::iterator it;
    it = m_v.begin();
    while (it != m_v.end()) {
        delete *it;
        *it = nullptr;
        it = m_v.erase(it);
    }
    
    delete m_nb;
    m_nb = nullptr;
}
