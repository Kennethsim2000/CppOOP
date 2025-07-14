#include <iostream>
#include <vector>
#include <string>
#include <format>
#include <memory>
#include <list>
#include <map>

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

// Representation of an order to be modified
class OrderModify
{
public:
    OrderModify(OrderId orderId, Side side, Price price, Quantity quantity) : orderId_(orderId), side_(side), price_(price), quantity_(quantity) {}
    const OrderId getOrderId()
    {
        return orderId_;
    }
    const Side getSide()
    {
        return side_;
    }
    const Price getPrice()
    {
        return price_;
    }
    const Quantity getQuantity()
    {
        return quantity_;
    }

    OrderPointer toOrderPointer(OrderType orderType)
    {
        return std::make_shared<Order>(orderType, getOrderId(), getSide(), getQuantity(), getPrice());
    }

private:
    OrderId orderId_;
    Side side_;
    Price price_;
    Quantity quantity_;
};

struct TradeInfo
{
    OrderId orderId_;
    Price price_;
    Quantity quantity_;
};

// Representation of aggregation of buy side and ask side trades
// create a class Trade that consist of TradeInfo& bidTrade and askTrade

class Trade
{
public:
    Trade(TradeInfo &bidTrade, TradeInfo &askTrade) : bidTrade_(bidTrade), askTrade_(askTrade) {}

private:
    TradeInfo bidTrade_;
    TradeInfo askTrade_;
};

using Trades = std::vector<Trade>;

class OrderBook
{
private:
    // Representation of an entry in the Orderbook, containing an iterator to its current location, and a shared_ptr to the Order
    struct OrderEntry
    {
        OrderPointer order_ = nullptr;
        OrderPointers::iterator location_;
    };

    std::map<Price, OrderPointers, std::greater<Price>> bids_;
};

int main()
{
    return 0;
}