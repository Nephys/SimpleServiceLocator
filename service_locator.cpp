#include "service_locator.h"

std::unordered_map<int, std::shared_ptr<void>> ServiceLocator::_instances = std::unordered_map<int, std::shared_ptr<void>>();

void ServiceLocator::Clear() {
    _instances.clear();
}