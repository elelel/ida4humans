#pragma once

#include "action_anyver.hpp"

#include <loader.hpp>
#include <kernwin.hpp>

#include <set>
#include <string>

#include "logging.hpp"

namespace ida4humans {
  namespace ui {
    template <typename action_t>
    class action;
    
    template <typename action_t>
    class action : public action_anyver {
      typedef action<action_t> type;
    public:
      action(const std::string& action_name,
             const std::string& action_label,
             const std::string& hotkey) :
        action_anyver(action_name, action_label,  hotkey) {};
      
      bool attach_to_menu(const std::string& menu_path, const int menu_flags) {
        LOG("ida4humans::action::attach_to_menu");
        const char* hotkey = nullptr;
        if (hotkey_.length() > 0) hotkey = hotkey_.c_str();
        if (add_menu_item(menu_path.c_str(), action_label_.c_str(), hotkey,
                          menu_flags, &action_callback, this)) {
          menu_paths_.insert(menu_path);
          LOG("ida4humans::action::attach_to_menu attached");
          return true;
        } else {
          LOG(std::string("ida4humans::action::attach_to_menu failed: '"
                          + action_label_ + "', '" + menu_path + "', '" + hotkey_ + "'"));
          return false;
        }
      }

      void detach_all_from_menu() {
        std::set<std::string> new_menu_paths;
        for (auto & path : menu_paths_) {
          if (!(del_menu_item(path.c_str()))) new_menu_paths.insert(path);
        };
        menu_paths_ = new_menu_paths;
      }

      bool attach_to_notification_point(::hook_type_t hook_type) {
        if (attached_to_notification_point_) {
          return false;
        } else {
          hook_type_ = hook_type;
          if (hook_to_notification_point(hook_type_, &hook_callback, this)) {
            attached_to_notification_point_ = true;
          }
        }
        return attached_to_notification_point_;
      }

      void detach_from_notification_point() {
        if (attached_to_notification_point_) {
          unhook_from_notification_point(hook_type_, &hook_callback, this);
        }
      }
      
    private:
      std::set<std::string> menu_paths_;
      bool attached_to_notification_point_;
      ::hook_type_t hook_type_;

      static bool idaapi action_callback(void * p_this) {
        reinterpret_cast<action_t*>(p_this)->execute();
        return false;
      }

      static int idaapi hook_callback(void * p_this) {
        return reinterpret_cast<action_t>(p_this)->execute();
      }
    };

  }
}
