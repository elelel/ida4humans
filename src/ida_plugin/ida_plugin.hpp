#pragma once

#include <ida.hpp>
#include <idp.hpp>
#include <loader.hpp>


#include <string>

#include "../logging.hpp"

namespace ida4humans {
  template <typename plugin_t>
  class ida_plugin;

  template <typename plugin_t>
  class ida_plugin {
  protected:
    ida_plugin(const ida_plugin&) = delete;
    ida_plugin& operator=(const ida_plugin&) = delete;
    ida_plugin(const int plugin_flags,
               const std::string& comment,
               const std::string& help,
               const std::string& wanted_name,
               const std::string& wanted_hotkey)  : 
      plugin_flags_(plugin_flags),
      comment_(comment),
      help_(help),
      wanted_name_(wanted_name),
      wanted_hotkey_(wanted_hotkey) {
      IDA4H_LOG("ida4humans::plugin::plugin");
    };
    
  public:
    plugin_t& instance() {
      static plugin_t inst;
      return inst;
    }

    ::plugin_t ida_plugin_info() {
      IDA4H_LOG("ida4humans::plugin::ida_plugin_info");
      return ::plugin_t{IDP_INTERFACE_VERSION,
          plugin_flags_,
          ida_init,
          ida_term,
          ida_run,
          comment_.c_str(),
          help_.c_str(),
          wanted_name_.c_str(),
          wanted_hotkey_.c_str()
          };      
    }
    
    static int idaapi ida_init() {
      IDA4H_LOG("ida4humans::plugin::ida_init");
      return plugin_t::instance().init();
    }

    static void idaapi ida_run(int x) {
      IDA4H_LOG("ida4humans::plugin::ida_run");
      plugin_t::instance().run(x);
    }
    
    static void idaapi ida_term() {
      IDA4H_LOG("ida4humans::plugin::ida_term");
      plugin_t::instance().term();
    }

  private:
    const int plugin_flags_;
    const std::string comment_;
    const std::string help_;
    const std::string wanted_name_;
    const std::string wanted_hotkey_;

  };
    
}
