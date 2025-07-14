#include <iostream>
#include <vector>
#include <string>
#include <format>
#include <memory>
#include <list>

enum class OrderType
{
    GOODTILLFILL,
    FILLANDKILL
};
// enum does not allow for different enums to have the same enumType, and does not allow for variables to have the same name as the enum type, but enum class
// allows for this

enum class Side
{
    BUY,
    SELL
};

// Type aliasing
using Quantity = std::uint32_t; // unsigned
using OrderId = std::uint32_t;
using Price = std::int32_t;

struct LevelInfo
{
    Price price_;
    Quantity quantity_;
};

using LevelInfos = std::vector<LevelInfo>;

// This class is used to displaying the orderbook details
class OrderBookLevelInfos
{
public:
    OrderBookLevelInfos(LevelInfos &ask, LevelInfos &bid) : ask_(ask), bid_(bid) {}

    const LevelInfos &getAsk()
    {
        return ask_;
    }

    const LevelInfos &getBid()
    {
        return bid_;
    }

private:
    LevelInfos &ask_;
    LevelInfos &bid_;
};

// Representation of an Order
class Order
{
public:
    Order(OrderType ordertype, OrderId orderId, Side side, Quantity quantity, Price price) : ordertype_(ordertype), orderid_(orderId), side_(side),
                                                                                             initialQuantity_(quantity), remainingQuantity_(quantity), price_(price) {}
    const OrderType getOrderType()
    {
        return ordertype_;
    }

    const OrderId getOrderId()
    {
        return orderid_;
    }

    const Side getSide()
    {
        return side_;
    }

    const Price getPrice()
    {
        return price_;
    }

    const Quantity getInitialQuantity()
    {
        return initialQuantity_;
    }

    const Quantity getRemainingQuantity()
    {
        return remainingQuantity_;
    }

    const Quantity getFilledQuantity()
    {
        return getInitialQuantity() - getRemainingQuantity();
    }

    void fill(Quantity quantity)
    {
        if (quantity > getRemainingQuantity())
        {
            throw std::logic_error(std::format("Order{} cannot be filled with a quantity higher than its remaining quantity", getOrderId()));
        }
        remainingQuantity_ -= quantity;
    }

private:
    OrderType ordertype_;
    OrderId orderid_;
    Side side_;
    Quantity initialQuantity_;
    Quantity remainingQuantity_;
    Price price_;
};

using OrderPointer = std::shared_ptr<Order>; // we use a shared pointer because it can be referenced from both the bid and ask maps
using OrderPointers = std::list<OrderPointer>;

int main()
{
    return 0;
}