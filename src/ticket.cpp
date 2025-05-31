#include "ticket.hpp"

void TicketManagement::initialize() {
    bpt8->initialize("user_order");
    bpt9->initialize("order_queue");
}
void TicketManagement::clear() {
    bpt8->clear();
    bpt9->clear();
}