#include "../Mailbox.hpp"
#include <iostream>

int main()
{
    MailBox<int> mailBox;
    int elem = 42;
    mailBox.push(elem);
    int num;
    auto res = mailBox.try_pop(num);
    std::cout << num << std::endl;
    return 0;
}