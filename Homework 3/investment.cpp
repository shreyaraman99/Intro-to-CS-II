class Investment {
public:
    Investment(string name, int value){
        m_name = name;
        m_value = value;
    }
    virtual ~Investment() {}
    virtual string name() const {
        return m_name;
    }
    virtual int purchasePrice() const {
        return m_value;
    }
    virtual string description() const = 0;
    virtual bool fungible() const = 0;
private:
    string m_name;
    int m_value;
};



class Painting: public Investment {
public:
    Painting(string name, int value): Investment(name, purchasePrice()) {
        m_value = value;
    }
    virtual ~Painting(){
        cout << "Destroying " << name() << ", a painting" << endl;
    }
    virtual string description() const{
        return "painting";
    }
    virtual bool fungible() const {
        return false;
    }
    virtual int purchasePrice() const {
        return m_value;
    }
private:
    int m_value;
};




class Stock: public Investment {
public:
    Stock(string name, int value, string ticker): Investment(name, purchasePrice()) {
        m_value = value;
        m_ticker = ticker;
    }
    virtual ~Stock() {
        cout << "Destroying " << name() << ", a stock holding" << endl;
    }
    virtual string description() const{
        return "stock trading as " + m_ticker;
    }
    virtual int purchasePrice() const {
        return m_value;
    }
    virtual bool fungible() const {
        return true;
    }
private:
    int m_value;
    string m_ticker;
};





class House: public Investment {
public:
    House(string name, int value): Investment(name, purchasePrice()) {
        m_value = value;
    }
    virtual ~House() {
        cout << "Destroying the house " << name() << endl;
    }
    virtual string description() const{
        return "house";
    }
    virtual int purchasePrice() const {
        return m_value;
    }
    virtual bool fungible() const {
        return false;
    }
private:
    int m_value;
};
