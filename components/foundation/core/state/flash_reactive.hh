#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include <core/structures/ui_queue.h>
#include "components/vnode.h"
#include <algorithm>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include "components/registry.h"
#include "core/state/reactive.h"
#include "core/structures/delegate.h"
#include "nvs_flash.h"
#include "nvs.h"

namespace foundation {

class FlashReactive : public IReactive {
 public:
    using MapType = std::unordered_map<std::string, std::string>;

 private:
    struct Binding {
        std::string key;
        uintptr_t component_id;
        Delegate<void(void*, std::shared_ptr<const MapType>), 64> updater;
    };

    std::shared_ptr<MapType> value_store;
    std::vector<Binding> bindings;
    std::string _ns_name;
    SemaphoreHandle_t _mutex;

    struct Lock {
        SemaphoreHandle_t m;
        explicit Lock(SemaphoreHandle_t m) : m(m) {
            if (m) xSemaphoreTakeRecursive(m, portMAX_DELAY);
        }
        ~Lock() {
            if (m) xSemaphoreGiveRecursive(m);
        }
    };

 public:
    explicit FlashReactive(std::string ns_name) 
        : value_store(std::make_shared<MapType>()), _ns_name(std::move(ns_name)) {
        
        _mutex = xSemaphoreCreateRecursiveMutex();

        esp_err_t err = nvs_flash_init();
        if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
            nvs_flash_erase();
            nvs_flash_init();
        }
    }

    virtual ~FlashReactive() override {
        if (_mutex) vSemaphoreDelete(_mutex);
    }

    void set_str(const std::string& key, const char* value) {
        if (!_mutex) return;
        
        std::shared_ptr<MapType> new_map;
        std::vector<Binding> snapshot;

        {
            Lock lock(_mutex);
            auto updated_map = std::make_shared<MapType>(*value_store);
            (*updated_map)[key] = std::string(value);
            value_store = updated_map;
            
            new_map = value_store;
            snapshot = bindings;

            nvs_handle_t h;
            if (nvs_open(_ns_name.c_str(), NVS_READWRITE, &h) == ESP_OK) {
                nvs_set_str(h, key.c_str(), value);
                nvs_commit(h);
                nvs_close(h);
            }
        }
        _notify(snapshot, new_map);
    }

    void load_key(const std::string& key) {
        Lock lock(_mutex);
        nvs_handle_t h;
        if (nvs_open(_ns_name.c_str(), NVS_READONLY, &h) == ESP_OK) {
            size_t required_size;
            if (nvs_get_str(h, key.c_str(), nullptr, &required_size) == ESP_OK) {
                char* buf = new char[required_size];
                nvs_get_str(h, key.c_str(), buf, &required_size);
                
                auto updated_map = std::make_shared<MapType>(*value_store);
                (*updated_map)[key] = std::string(buf);
                value_store = updated_map;
                
                delete[] buf;
            }
            nvs_close(h);
        }
    }

    template <typename TComp>
    void attach(TComp* component, Delegate<void(TComp*, std::shared_ptr<const MapType>), 64> updater) {
        if (!_mutex || !component) return;
        Lock lock(_mutex);

        Binding b;
        b.component_id = component->get_component_id();
        b.updater = [updater, id = b.component_id](void*, std::shared_ptr<const MapType> val) {
            auto* node = foundation::ComponentRegistry::instance().get(id);
            if (node && node->alive()) {
                if (auto* typed = dynamic_cast<TComp*>(node)) {
                    updater(typed, std::move(val));
                }
            }
        };
        bindings.push_back(std::move(b));
    }

    void detach(const void* component) override {
        if (!_mutex || !component) return;
        Lock lock(_mutex);
        auto* node = static_cast<const VNode*>(component);
        const auto target_id = node->get_component_id();
        std::erase_if(bindings, [target_id](const Binding& b) {
          return b.component_id == target_id;
        });
    }

    std::shared_ptr<const MapType> get() {
        Lock lock(_mutex);
        return value_store;
    }

 private:
    void _notify(const std::vector<Binding>& snapshot, std::shared_ptr<const MapType> val) const {
        for (auto& b : snapshot) {
            if (b.updater) {
                UIQueue::get_instance().push([upd = b.updater, val]() {
                    if (upd) upd(nullptr, val);
                });
            }
        }
    }
};

}