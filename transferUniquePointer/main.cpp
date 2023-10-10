#include <memory>
#include <iostream>
#include <qdebug.h>
#include <qapplication.h>

struct Animal {
    virtual ~Animal() {}
    virtual void speak() = 0;
};

struct Cat : Animal {
    Cat() {
        speak();
    }
    void speak() override { 
        std::cout << "Meow!\n";
        qDebug() << "Meow!\n";
    }
};

struct Dog : Animal {
    Dog() {
        speak();
    }
    void speak() override { 
        std::cout << "Woof!\n"; 
        qDebug() << "Woof!\n";
    }
};

std::unique_ptr<Animal> createPet(double obedience) {
    if (obedience > 5.0)
        return std::make_unique<Dog>();
    return std::make_unique<Cat>();
}

class House {
private:
    std::unique_ptr<Animal> pet_;
public:
    //House(std::unique_ptr<Animal> pet) : pet_(std::move(pet)) {}
    House(std::unique_ptr<Animal>&& pet) : pet_(std::move(pet)) {}

};

int main(int argc, char* argv[]) 
{
    auto pet = createPet(6.0);
    House house(std::move(pet));

    QApplication app(argc, argv);
    app.exec();
}
