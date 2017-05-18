#pragma once

namespace ida4humans {
  namespace ui {
    class action_anyver;
    
    class action_anyver {
    public:
      action_anyver(const std::string& action_name,
             const std::string& action_label,
             const std::string& hotkey) :
        action_name_(action_name),
        action_label_(action_label),
        hotkey_(hotkey) {};
    protected:
      const std::string action_name_;
      const std::string action_label_;
      const std::string hotkey_;
    };
    

  }
  
}
