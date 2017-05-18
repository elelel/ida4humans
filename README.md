# IDA disassembler API adapter to modern C++

Native IDA C++ API is a reflection of disassembler's long history and suffers from several problems.
It's inconsistent (same things are done/named/look different) and unstable (changes from version to version).
This library is a part of old projects and attempts to convert IDA API to a modern C++ STL-compatible interface.
Most of adaptation is done around iterator pattern, following C++ standard library concepts.

### Creating plugin
#### Header file
```c++
  struct plugin : public ::ida4humans::ida_plugin<plugin> {
    static plugin& instance();  // C++ singleton pattern
    // Function that correspond to IDA API interface
    int init();
    void run(int);
    void term();
  private:
    plugin();  // Singleton's constructor should be private
  };
```

#### Cpp file
```c++
// Initialize IDA global structure
::plugin_t PLUGIN = ruroida::plugin::instance().ida_plugin_info();

// Constructor with info required for IDA API
  plugin::plugin() :
    ida4humans::ida_plugin<plugin>(0,
                                   "My plugin comment",
                                   "My plugin help",
                                   "MyPlugin",
                                   "Alt-E") {
    // ...                               
  }

```
  


### Iterating
```c++
  ida4humans::instructions ins(startEA, endEA);  // Create instructions "container"
  auto ri = ins.rbegin();   // Get a reverse iterator to search backwards
  auto ri_end = ri + 100;   // Limit the search with 100 instructions maximum
  // Use <algorithm>'s find_if to find instruction "push 0"
  ri = std::find_if(ri, ri_end, [] (const insn_t* i) {              
    return (i->itype == NN_push) && (i->Operands[0].type == o_imm) && (i->Operands[0].value == 0);
  });
  if (ri != ri_end) {     // If found
    auto i = ri.base();   // Get forward iterator, instructions reverse iterator is compliant with STL concept
    i = std::find_if(i, ins.end(), [] (const insn_t* i) {
      return (i->itype == NN_call);    // Find call instruction
    });
  }
```

### UI

#### Plugin header file

```c++
// ...
   ida4humans::ui::action<plugin> menu_action_do_something_;    // Contains IDA action or a proxy for old IDAs that did not have actions
   static int menu_callback_do_something(plugin*);   // Callback to be called on activation
   void something(); // Local method

```
#### Cpp file
```c++
// in plugin constructor
  menu_action_set_files_(ida4humans::ui::action<plugin>("MyPluginActionSomething",
                                                        "My plugin: do something...",
                                                          "Alt-F",
                                                          this, menu_callback_do_something);
// ...
// Callback function
  int plugin::menu_callback_do_something(plugin* p) {
    p->something();
    return 0;
  }
// Local method
  void plugin::something() {
    msg("User requested something\n");
  }

```
