
#include "CarMap.h"
//#include "Map.h"
#include <iostream>
using namespace std;


CarMap::CarMap() {
    Map m_car;  
}

bool CarMap::addCar(std::string license) {
    if (!m_car.contains(license) && m_car.size() < DEFAULT_MAX_ITEMS) {
        m_car.insert(license, 0);
        return true;
    }
    else
        return false;
}

double CarMap::gas(std::string license) const {
    ValueType value;
    if (m_car.get(license, value))
        return value;
    else
        return -1;
}

bool CarMap::addGas(std::string license, double gallons) {
    if (!m_car.contains(license) || gallons < 0)
        return false;
    else {
        m_car.update(license, gas(license) + gallons);
        return true;
    }
}

bool CarMap::useGas(std::string license, double gallons) {
    if (!m_car.contains(license) || gallons < 0 || gallons > gas(license))
        return false;
    else {
        m_car.update(license, gas(license) - gallons);
        return true;
    }
}

int CarMap::fleetSize() const {
    return m_car.size();

}
void CarMap::print() const {
    KeyType license;
    ValueType gallons;
    for (int i = 0; i < m_car.size(); i++) {
        m_car.get(i, license, gallons);
        cout << license << " " << gallons << endl;
    }
}


