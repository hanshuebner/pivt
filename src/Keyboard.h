// -*- C++ -*-

#pragma once

#include <set>
#include <map>
#include <string>

#include "Logging.h"

using namespace std;

class Terminal;
class CUSBKeyboardDevice;

class Keyboard
  : protected Logging
{
public:
  Keyboard(Terminal* terminal);

  Terminal* terminal() const { return _terminal; }

  void process();

private:
  struct KeyDefinition;
  class KeypressHandler;

  set<unsigned char> _keys_pressed;
  bool _error;
  map<unsigned char, KeyDefinition*> _map;
  Terminal* _terminal;
  CUSBKeyboardDevice* _usb_keyboard;

  static unsigned char _modifiers;
  static unsigned char _keys[6];

  static Keyboard* _this;

  void initialize_keymap();

  void key_pressed(unsigned char modifiers,
                   unsigned char key_code);

  void handle_report(unsigned char modifiers,
                     const unsigned char keys[6]);

  friend void handle_report_stub(unsigned char modifiers,
                                 const unsigned char key_code[6]);

  enum Modifiers {
                  LeftControl = 1,
                  LeftShift = 2,
                  Alt = 4,
                  LeftMeta = 8,
                  RightControl = 16,
                  RightShift = 32,
                  AltGr = 64,
                  RightMeta = 128
  };

  struct KeyDefinition
  {
    KeyDefinition(const char* const name, KeypressHandler* solo, KeypressHandler* shift, KeypressHandler* control)
      : _name(name), _solo(solo), _shift(shift), _control(control)
    {}

    const string _name;
    KeypressHandler* _solo;
    KeypressHandler* _shift;
    KeypressHandler* _control;
  };

  class KeypressHandler
  {
  public:
    virtual ~KeypressHandler() {};
    virtual const string operator()(Keyboard* keyboard) const = 0;
  };

  class String
    : public KeypressHandler
  {
  public:
    String(const char* s) : _s(s) {}
    virtual const string operator()(__unused Keyboard* keyboard) const { return _s; }

    const string _s;
  };

  class Char
    : public KeypressHandler
  {
  public:
    Char(const unsigned char c) : _c(c) {}
    virtual const string operator()(__unused Keyboard* keyboard) const { return string(1, _c); }

    const unsigned char _c;
  };

  class DeadKey
    : public KeypressHandler
  {
  public:
    virtual const string operator()(__unused Keyboard* keyboard) const { return ""; }
  };

  static DeadKey dead_key;

  class ToggleScreenSize
    : public KeypressHandler
  {
  public:
    virtual const string operator()(Keyboard* keyboard) const;
  };

  class CycleSerialSpeed
    : public KeypressHandler
  {
  public:
    virtual const string operator()(Keyboard* keyboard) const;
  };
};
