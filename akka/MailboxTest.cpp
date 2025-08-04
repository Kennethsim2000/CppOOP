#include "Mailbox.hpp"

int main()
{
    MailBox<int> mailBox;
    int elem = 42;
    mailBox.push(elem);
    auto res = mailBox.try_pop();
    return 0;
}