#include "../include/keyboard.h"

static void print_letter (uint8_t scancode);

static void keyboard_callback (registers_t *regs)
{
  uint8_t scancode = port_byte_in(KEYBOARD_SCANCODE);

  char sc_ascii[10];
  itoa (scancode, sc_ascii);
  kprint_debug ("scan_code: ", WHITE);
  kprint_debug (sc_ascii, WHITE);
  kprint_debug (" (", WHITE);
  print_letter (scancode);
  kprint_debug (")", WHITE);
  kprint("\n");
}

void keyboard_init ()
{
  register_interrupt_handler(IRQ1, keyboard_callback); 
}

static void print_letter (uint8_t scancode)
{
  switch (scancode) {
    case 0x0:
      kprint_debug ("ERROR", WHITE);
      break;
    case 0x1:
      kprint_debug ("esc", WHITE);
      break;
    case 0x2:
      kprint_debug ("1", WHITE);
      break;
    case 0x3:
      kprint_debug ("2", WHITE);
			change_state ();
      break;
    case 0x4:
      kprint_debug ("3", WHITE);
			to_nr ();
      break;
    case 0x5:
      kprint_debug ("4", WHITE);
      break;
    case 0x6:
      kprint_debug ("5", WHITE);
      break;
    case 0x7:
      kprint_debug ("6", WHITE);
      break;
    case 0x8:
      kprint_debug ("7", WHITE);
      break;
    case 0x9:
      kprint_debug ("8", WHITE);
      break;
    case 0x0A:
      kprint_debug ("9", WHITE);
      break;
    case 0x0B:
      kprint_debug ("0", WHITE);
      break;
    case 0x0C:
      kprint_debug ("-", WHITE);
      break;
    case 0x0D:
      kprint_debug ("+", WHITE);
      break;
    case 0x0E:
      kprint_debug ("backspace", WHITE);
      break;
    case 0x0F:
      kprint_debug ("tab", WHITE);
      break;
    case 0x10:
      kprint_debug ("q", WHITE);
      break;
    case 0x11:
      kprint_debug ("w", WHITE);
      break;
    case 0x12:
      kprint_debug ("e", WHITE);
      break;
    case 0x13:
      kprint_debug ("r", WHITE);
      break;
    case 0x14:
      kprint_debug ("t", WHITE);
      break;
    case 0x15:
      kprint_debug ("y", WHITE);
      break;
    case 0x16:
      kprint_debug ("u", WHITE);
      break;
    case 0x17:
      kprint_debug ("i", WHITE);
      break;
    case 0x18:
      kprint_debug ("o", WHITE);
      break;
    case 0x19:
      kprint_debug ("p", WHITE);
      break;
    case 0x1A:
      kprint_debug ("[", WHITE);
      break;
    case 0x1B:
      kprint_debug ("]", WHITE);
      break;
    case 0x1C:
      kprint_debug ("enter", WHITE);
      break;
    case 0x1D:
      kprint_debug ("left_ctrl", WHITE);
      break;
    case 0x1E:
      kprint_debug ("a", WHITE);
      break;
    case 0x1F:
      kprint_debug ("s", WHITE);
      break;
    case 0x20:
      kprint_debug ("d", WHITE);
      break;
    case 0x21:
      kprint_debug ("f", WHITE);
      break;
    case 0x22:
      kprint_debug ("g", WHITE);
      break;
    case 0x23:
      kprint_debug ("h", WHITE);
      break;
    case 0x24:
      kprint_debug ("j", WHITE);
      break;
    case 0x25:
      kprint_debug ("k", WHITE);
      break;
    case 0x26:
      kprint_debug ("l", WHITE);
      break;
    case 0x27:
      kprint_debug (";", WHITE);
      break;
    case 0x28:
      kprint_debug ("'", WHITE);
      break;
    case 0x29:
      kprint_debug ("`", WHITE);
      break;
    case 0x2A:
      kprint_debug ("left_shift", WHITE);
      break;
    case 0x2B:
      kprint_debug ("\\", WHITE);
      break;
    case 0x2C:
      kprint_debug ("z", WHITE);
      break;
    case 0x2D:
      kprint_debug ("x", WHITE);
      break;
    case 0x2E:
      kprint_debug ("c", WHITE);
      break;
    case 0x2F:
      kprint_debug ("v", WHITE);
      break;
    case 0x30:
      kprint_debug ("b", WHITE);
      break;
    case 0x31:
      kprint_debug ("n", WHITE);
      break;
    case 0x32:
      kprint_debug ("m", WHITE);
      break;
    case 0x33:
      kprint_debug (",", WHITE);
      break;
    case 0x34:
      kprint_debug (".", WHITE);
      break;
    case 0x35:
      kprint_debug ("/", WHITE);
      break;
    case 0x36:
      kprint_debug ("right_shift", WHITE);
      break;
    case 0x37:
      kprint_debug ("*", WHITE);
      break;
    case 0x38:
      kprint_debug ("left_alt", WHITE);
      break;
    case 0x39:
      kprint_debug ("space", WHITE);
      break;
    default:
      // release key event corresponds to the push key + 0x80 
      if (scancode <= 0x7f)
        kprint_debug ("unknown", WHITE);
      else if (scancode <= 0x39 + 0x80)
      {
        //print_letter(scancode - 0x80);
        //kprint_debug (" released", WHITE);
      } 
      else
        kprint_debug ("unknown released", WHITE);
      break;
  }
}
