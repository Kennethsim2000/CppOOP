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

    bool isFilled()
    {
        return getRemainingQuantity() == 0;
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

using Trades = std::vector<Trade>; // used to store the list of trades that took place

class OrderBook
{
private:
    // Representation of an entry in the Orderbook, containing an iterator to its current location, and a shared_ptr to the Order
    struct OrderEntry
    {
        OrderPointer order_ = nullptr;
        OrderPointers::iterator location_;
    };

    std::map<Price, OrderPointers, std::greater<Price>> bids_; // sorted in descending order, bids need to be sorted based on the highest bid
    std::map<Price, OrderPointers, std::less<Price>> asks_;    // asks are sorted based on lowest
    std::unordered_map<OrderId, OrderEntry> orders_;

    bool canMatch(Side side, Price price) const
    {
        if (side == Side::BUY)
        {
            if (asks_.empty())
            {
                return false;
            }
            else
            {
                const auto &[bestSell, _] = *asks_.begin();
                return price >= bestSell;
            }
        }
        else
        {
            if (bids_.empty())
            {
                return false;
            }
            else
            {
                const auto &[bestBuy, _] = *bids_.begin();
                return price <= bestBuy;
            }
        }
    }

    Trades matchOrder()
    {
        Trades trades;
        trades.reserve(orders_.size());
        while (true)
        {
            if (bids_.empty() || asks_.empty())
            {
                break;
            }
            auto &[bidPrice, bidOrders] = *bids_.begin();
            auto &[askPrice, askOrders] = *asks_.begin();
            if (bidPrice < askPrice)
            {
                break;
            }
            while (bidOrders.size() && askOrders.size())
            {
                auto &firstBid = bidOrders.front();
                auto &firstAsk = askOrders.front();
                Quantity transactedQuantity = std::min(firstBid->getRemainingQuantity(), firstAsk->getRemainingQuantity());
                firstBid->fill(transactedQuantity);
                firstAsk->fill(transactedQuantity);

                // remove order from orderbook and bids and asks_ queue if it is filled
                if (firstBid->isFilled())
                {
                    bidOrders.pop_front();
                    orders_.erase(firstBid->getOrderId());
                }
                if (firstAsk->isFilled())
                {
                    askOrders.pop_front();
                    orders_.erase(firstAsk->getOrderId());
                }
                // now if our bidOrders or askOrders are empty, we want to remove the price key from the bids_ and asks_ map
                if (bidOrders.empty())
                {
                    bids_.erase(bidPrice);
                }
                if (askOrders.empty())
                {
                    asks_.erase(askPrice);
                }

                TradeInfo bidTradeInfo(firstBid->getOrderId(), firstBid->getPrice(), transactedQuantity);
                TradeInfo askTradeInfo(firstAsk->getOrderId(), firstAsk->getPrice(), transactedQuantity);
                Trade trade(bidTradeInfo, askTradeInfo);

                trades.push_back(trade);
            }
        }
        // now we want to check if the order is fill and kill, we want to cancel the order
        if (!bids_.empty())
        {
            auto &[_, bids] = *bids_.begin();
            auto &order = bids.front();
            if (order->getOrderType() == OrderType::FILLANDKILL)
            {
                cancelOrder(order->getOrderId());
            }
        }
        if (!asks_.empty())
        {
            auto &[_, asks] = *asks_.begin();
            auto &order = asks.front();
            if (order->getOrderType() == OrderType::FILLANDKILL)
            {
                cancelOrder(order->getOrderId());
            }
        }
        return trades;
    }

public:
    Trades addOrder(OrderPointer order) // if order book already contains the id, then reject it, because no two
    {
        if (orders_.contains(order->getOrderId()))
        {
            return {};
        }
        if (order->getOrderType() == OrderType::FILLANDKILL && !canMatch(order->getSide(), order->getPrice()))
        {
            return {};
        }
        OrderPointers::iterator iterator;
        // now we can add the order
        if (order->getSide() == Side::BUY)
        {
            auto &orders = bids_[order->getPrice()];
            orders.push_back(order);
            iterator = std::next(orders.begin(), orders.size() - 1); // assign the iterator to point to the last order
        }
        else
        {
            auto orders = asks_[order->getPrice()];
            orders.push_back(order);
            iterator = std::next(orders.begin(), orders.size() - 1);
        }
        orders_.insert({order->getOrderId(), OrderEntry(order, iterator)});
        matchOrder();
    }

    void cancelOrder(OrderId orderId)
    {
        if (!orders_.contains(orderId))
        {
            return;
        }
        // erase from orders_
        const auto &[order, orderIterator] = orders_.at(orderId);
        orders_.erase(orderId);
        if (order->getSide() == Side::BUY)
        {
            auto &orders = bids_.at(order->getPrice());
            orders.erase(orderIterator);
            if (orders.empty())
            {
                bids_.erase(order->getPrice());
            }
        }
        else
        {
            auto &orders = asks_.at(order->getPrice());
            orders.erase(orderIterator);
            if (orders.empty())
            {
                asks_.erase(order->getPrice());
            }
        }
    }

    Trades ModifyOrder(OrderModify order)
    {
        if (orders_.contains(order.getOrderId()))
        {
            return {};
        }
        cancelOrder(order.getOrderId());
        const auto &[existingOrder, _] = orders_.at(order.getOrderId());
        addOrder(order.toOrderPointer(existingOrder->getOrderType()));
    }
};

int main()
{
    return 0;
}