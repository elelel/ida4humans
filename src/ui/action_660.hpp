#pragma once

#include <functional>
#include <vector>
#include <string>

/*
  Don't call attach_to_menu in init.
 */

namespace ida4humans {
  namespace ui {
    template <typename T>
    struct action {
      action(const std::string& action_name,
             const std::string& action_label,
             const std::string& hotkey,
             T* object, 
             std::function<int(T*)> handler) :
        object_(object),
        handler_(handler),
        action_name_(action_name),
        action_label_(action_label),
        hotkey_(hotkey) {
      }

      bool attach_to_menu(const std::string& menu_path,
                          const int menu_flags) {
        if (action_label_ == "") {
          if (action_name_ != "") {
            action_label_ = action_name_;
          } else {
            ::msg(("Error, called to set up action at " + menu_path + " but action name and label are empty\n").c_str());
            return false;
          }
        }
        const char* hotkey = nullptr;
        if (hotkey_.length() > 0) hotkey = hotkey_.c_str();
        menu_paths_.push_back(menu_path);
        if (add_menu_item(menu_path.c_str(), action_label_.c_str(), hotkey,
                          menu_flags, &action_callback, this)) {
          return true;
        } else {
          menu_paths_.pop_back();
          return false;
        }
      }

      void detach_from_all_menu() {
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
      std::string action_name_;
      std::string action_label_;
      std::string hotkey_;
      T* object_;
      std::function<int(T*)> handler_;

      std::vector<std::string> menu_paths_;
      bool attached_to_notification_point_;
      ::hook_type_t hook_type_;

      static bool idaapi action_callback(void * p_this) {
        IDA4H_LOG("action_callback entered");
        auto th = reinterpret_cast<action<T>*>(p_this);
        th->handler_(th->object_);
        return false;
      }

      static int idaapi hook_callback(void * p_this) {
        auto th = reinterpret_cast<action<T>*>(p_this);
        return th->handler_(th->object_);
      }

    };
  }
}
