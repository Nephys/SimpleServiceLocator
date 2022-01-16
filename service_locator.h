#ifndef _SERVICE_LOCATOR_H_
#define _SERVICE_LOCATOR_H_

#include <memory>
#include <type_traits>
#include <unordered_map>

class ServiceLocator {
private:
    static std::unordered_map<int, std::shared_ptr<void>> _instances;
public:
    static void Clear();

    template <typename T>
    static inline void Provide(T* p_instance = new T()) {
        int hash = static_cast<int>(typeid(T).hash_code());
	    if (_instances.find(hash) == _instances.end()) {
	    	_instances.emplace(hash, p_instance);
        }
    };

    template <typename T>
    static inline std::shared_ptr<T> Resolve() {
        int hash = static_cast<int>(typeid(T).hash_code());
	    auto instance = _instances.find(hash);
	    if (instance != _instances.end()) {
		    return std::static_pointer_cast<T>(instance->second);
        }

        // Return a NullService (Empty Service)
        return std::shared_ptr<T>(new T());
    };
};

#endif //_SERVICE_LOCATOR_H_