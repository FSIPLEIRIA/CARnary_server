//
// Created by carlostojal on 10-09-2023.
//

#include "Watcher.h"

using namespace carnary::server;

Watcher::Watcher(std::unique_ptr<carnary::server::Negotiation>& negotiation) : negotiation(negotiation) {
}
