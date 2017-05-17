/* This work is licensed under a Creative Commons CCZero 1.0 Universal License.
 * See http://creativecommons.org/publicdomain/zero/1.0/ for more information. */

#include <memory>

// Case 1: The host class is the owner of the object, i.e. responsible for destruction.
//
class Part {};

class Device
{
    std::unique_ptr<Part> part_;
public:
    Device(std::unique_ptr<Part> p)
    {
        part_ = std::move(p);
    }

    Part* get_ptr() const
    {
        return part_.get();
    }
};

// Case 2: The host class is *not* the owner of the object, i.e. not responsible for destruction.
//
class Color {};

class Car
{
    std::shared_ptr<Color> color_;
public:
    Car(const std::shared_ptr<Color>& c)
    {
        color_ = c;
    }

    std::shared_ptr<Color> get_sp() const
    {
        return color_;
    }
};

int main()
{
    std::unique_ptr<Part> bolt = std::make_unique<Part>();
    Device myDevice(std::move(bolt));
    Part* myPart = myDevice.get_ptr();

    std::shared_ptr<Color> red = std::make_shared<Color>();
    Car myCar(red);
    std::shared_ptr<Color> myColor = myCar.get_sp();
}
