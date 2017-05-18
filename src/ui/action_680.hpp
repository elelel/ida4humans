#pragma once

#include "action_anyver.hpp"

namespace ida4humans {
  namespace ui {
    template <typename action_t>
    class action;
    
    template <typename action_t>
    class action : public action_anyver {
      typedef action_680<action_t> type;

    public:
      action_680() {
        action_ = ACTION_DESC_LITERAL_OWNER(action_name, action_label,
                                            &handler_, &PLUGIN, hot_key, tool_tip, -1);

      }
      bool attach_to_menu() {
        if (attach_action_to_menu(menu_path_.c_str(), action_name_, menu_flags_)) {
          return true;
        } else {
          return false;
        }
      }
    protected:
      ::action_desc_t action_;
    };
  }
}
