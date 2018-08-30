
#include "CarMap.h"

#include <iostream>
#include <cassert>
using namespace std;

int main() {
    CarMap m;
    
    assert(m.fleetSize() == 0);
    
    assert(m.addCar("ABC900") == true);
    assert(m.addCar("DEF700") == true);
    assert(m.addCar("ABC900") == false);
    
    assert(m.gas("ABC900") == 0 && m.gas("DEF700") == 0);
    assert(m.gas("XYZ100") == -1);
    
    assert(m.addGas("XYZ100", 40) == false && m.addGas("DEF700", -30) == false);
    assert(m.addGas("ABC900", 100) == true);
    assert(m.addGas("DEF700", 30) == true);
    assert(m.gas("ABC900") == 100 && m.gas("DEF700") == 30);
    
    assert(m.useGas("XYZ100", 20) == false && m.useGas("ABC900", -10) == false && m.useGas("DEF700", 40) == false);
    assert(m.useGas("ABC900", 30) == true && m.useGas("DEF700", 10) == true);
    assert(m.gas("ABC900") == 70 && m.gas("DEF700") == 20);
    
    assert(m.addGas("ABC900", 10) == true && m.addGas("DEF700", 5) == true);
    assert(m.useGas("DEF700", 10) == true);
    assert(m.gas("ABC900") == 80 && m.gas("DEF700") == 15);
    
    assert(m.fleetSize() == 2);
    
    m.print();
    
    cout << "Passed all tests" << endl;
}


