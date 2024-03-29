#ifndef KEY_H
#define KEY_H

#include <GLFW/glfw3.h>
#include "SDK/STL/STL.h"

struct Key
{
	bool pressing = false;
	bool pressDown = false;
	bool liftUp = false;
	float pressDur = 0;
	float downTime = 0;
	float downDeltaTime = 10000;
};

enum KEY
{
    // Mouse buttons
    MOUSE_LAST = GLFW_MOUSE_BUTTON_8,
    MOUSE_LEFT = GLFW_MOUSE_BUTTON_1,
    MOUSE_RIGHT = GLFW_MOUSE_BUTTON_2,
    MOUSE_MIDDLE = GLFW_MOUSE_BUTTON_3,
    MOUSE_1 = 0,
    MOUSE_2 = 1,
    MOUSE_3 = 2,
    MOUSE_4 = 3,
    MOUSE_5 = 4,
    MOUSE_6 = 5,
    MOUSE_7 = 6,
    MOUSE_8 = 7,

    // Keyboard buttons
    SPACE = 32, 
    APOSTROPHE = 39,   /* ' */
    COMMA = 44,   /* , */
    MINUS = 45,   /* - */
    PERIOD = 46,   /* . */
    SLASH = 47,   /* / */
    NUM_0 = 48, 
    NUM_1 = 49, 
    NUM_2 = 50, 
    NUM_3 = 51, 
    NUM_4 = 52, 
    NUM_5 = 53, 
    NUM_6 = 54, 
    NUM_7 = 55, 
    NUM_8 = 56, 
    NUM_9 = 57, 
    SEMICOLON = 59,   /* ; */
    EQUAL = 61,   /* = */
    A = 65, 
    B = 66, 
    C = 67, 
    D = 68, 
    E = 69, 
    F = 70, 
    G = 71, 
    H = 72, 
    I = 73, 
    J = 74, 
    K = 75, 
    L = 76, 
    M = 77, 
    N = 78, 
    O = 79, 
    P = 80, 
    Q = 81, 
    R = 82, 
    S = 83, 
    T = 84, 
    U = 85, 
    V = 86, 
    W = 87, 
    X = 88, 
    Y = 89, 
    Z = 90, 
    LEFT_BRACKET = 91,   /* [ */
    BACKSLASH = 92,   /* \ */
    RIGHT_BRACKET = 93,   /* ] */
    GRAVE_ACCENT = 96,   /* ` */
    WORLD_1 = 161,  /* non-US #1 */
    WORLD_2 = 162,  /* non-US #2 */
    /* Function keys */
    ESCAPE = 256,
    ENTER = 257,
    TAB = 258,
    BACKSPACE = 259,
    INSERT = 260,
    DELETE = 261,
    RIGHT = 262,
    LEFT = 263,
    DOWN = 264,
    UP = 265,
    PAGE_UP = 266,
    PAGE_DOWN = 267,
    HOME = 268,
    END = 269,
    CAPS_LOCK = 280,
    SCROLL_LOCK = 281,
    NUM_LOCK = 282,
    PRINT_SCREEN = 283,
    PAUSE = 284,
    F1 = 290,
    F2 = 291,
    F3 = 292,
    F4 = 293,
    F5 = 294,
    F6 = 295,
    F7 = 296,
    F8 = 297,
    F9 = 298,
    F10 = 299,
    F11 = 300,
    F12 = 301,
    F13 = 302,
    F14 = 303,
    F15 = 304,
    F16 = 305,
    F17 = 306,
    F18 = 307,
    F19 = 308,
    F20 = 309,
    F21 = 310,
    F22 = 311,
    F23 = 312,
    F24 = 313,
    F25 = 314,
    KP_0 = 320,
    KP_1 = 321,
    KP_2 = 322,
    KP_3 = 323,
    KP_4 = 324,
    KP_5 = 325,
    KP_6 = 326,
    KP_7 = 327,
    KP_8 = 328,
    KP_9 = 329,
    KP_DECIMAL = 330,
    KP_DIVIDE = 331,
    KP_MULTIPLY = 332,
    KP_SUBTRACT = 333,
    KP_ADD = 334,
    KP_ENTER = 335,
    KP_EQUAL = 336,
    LEFT_SHIFT = 340,
    LEFT_CONTROL = 341,
    LEFT_ALT = 342,
    LEFT_SUPER = 343,
    RIGHT_SHIFT = 344,
    RIGHT_CONTROL = 345,
    RIGHT_ALT = 346,
    RIGHT_SUPER = 347,
    MENU = 348
};

map<std::string, KEY> keyMap
{
    // Custom
    {"ml", MOUSE_LEFT},
    {"mr", MOUSE_RIGHT},
    {"space", SPACE},
    {"shift", LEFT_SHIFT},
    {"q", Q},
    {"e", E},
    {"f", F},

    // Default
    // {"MOUSE_1", MOUSE_1},
    // {"MOUSE_2", MOUSE_2},
    // {"MOUSE_3", MOUSE_3},
    // {"MOUSE_4", MOUSE_4},
    // {"MOUSE_5", MOUSE_5},
    // {"MOUSE_6", MOUSE_6},
    // {"MOUSE_7", MOUSE_7},
    // {"MOUSE_8", MOUSE_8},
    // {"MOUSE_LAST", MOUSE_LAST},
    // {"MOUSE_LEFT", MOUSE_LEFT},
    // {"MOUSE_RIGHT", MOUSE_RIGHT},
    // {"MOUSE_MIDDLE", MOUSE_MIDDLE},
    // {"SPACE", SPACE},
    // {"APOSTROPHE", APOSTROPHE},
    // {"COMMA", COMMA},
    // {"MINUS", MINUS},
    // {"PERIOD", PERIOD},
    // {"SLASH", SLASH},
    // {"NUM_0", NUM_0},
    // {"NUM_1", NUM_1},
    // {"NUM_2", NUM_2},
    // {"NUM_3", NUM_3},
    // {"NUM_4", NUM_4},
    // {"NUM_5", NUM_5},
    // {"NUM_6", NUM_6},
    // {"NUM_7", NUM_7},
    // {"NUM_8", NUM_8},
    // {"NUM_9", NUM_9},
    // {"SEMICOLON", SEMICOLON},
    // {"EQUAL", EQUAL},
    // {"A", A},
    // {"B", B},
    // {"C", C},
    // {"D", D},
    // {"E", E},
    // {"F", F},
    // {"G", G},
    // {"H", H},
    // {"I", I},
    // {"J", J},
    // {"K", K},
    // {"L", L},
    // {"M", M},
    // {"N", N},
    // {"O", O},
    // {"P", P},
    // {"Q", Q},
    // {"R", R},
    // {"S", S},
    // {"T", T},
    // {"U", U},
    // {"V", V},
    // {"W", W},
    // {"X", X},
    // {"Y", Y},
    // {"Z", Z},
    // {"LEFT_BRACKET", LEFT_BRACKET},
    // {"BACKSLASH", BACKSLASH},
    // {"RIGHT_BRACKET", RIGHT_BRACKET},
    // {"GRAVE_ACCENT", GRAVE_ACCENT},
    // {"WORLD_1", WORLD_1},
    // {"WORLD_2", WORLD_2},
    // {"ESCAPE", ESCAPE},
    // {"ENTER", ENTER},
    // {"TAB", TAB},
    // {"BACKSPACE", BACKSPACE},
    // {"INSERT", INSERT},
    // {"DELETE", DELETE},
    // {"RIGHT", RIGHT},
    // {"LEFT", LEFT},
    // {"DOWN", DOWN},
    // {"UP", UP},
    // {"PAGE_UP", PAGE_UP},
    // {"PAGE_DOWN", PAGE_DOWN},
    // {"HOME", HOME},
    // {"END", END},
    // {"CAPS_LOCK", CAPS_LOCK},
    // {"SCROLL_LOCK", SCROLL_LOCK},
    // {"NUM_LOCK", NUM_LOCK},
    // {"PRINT_SCREEN", PRINT_SCREEN},
    // {"PAUSE", PAUSE},
    // {"F1", F1},
    // {"F2", F2},
    // {"F3", F3},
    // {"F4", F4},
    // {"F5", F5},
    // {"F6", F6},
    // {"F7", F7},
    // {"F8", F8},
    // {"F9", F9},
    // {"F10", F10},
    // {"F11", F11},
    // {"F12", F12},
    // {"F13", F13},
    // {"F14", F14},
    // {"F15", F15},
    // {"F16", F16},
    // {"F17", F17},
    // {"F18", F18},
    // {"F19", F19},
    // {"F20", F20},
    // {"F21", F21},
    // {"F22", F22},
    // {"F23", F23},
    // {"F24", F24},
    // {"F25", F25},
    // {"KP_0", KP_0},
    // {"KP_1", KP_1},
    // {"KP_2", KP_2},
    // {"KP_3", KP_3},
    // {"KP_4", KP_4},
    // {"KP_5", KP_5},
    // {"KP_6", KP_6},
    // {"KP_7", KP_7},
    // {"KP_8", KP_8},
    // {"KP_9", KP_9},
    // {"KP_DECIMAL", KP_DECIMAL},
    // {"KP_DIVIDE", KP_DIVIDE},
    // {"KP_MULTIPLY", KP_MULTIPLY},
    // {"KP_SUBTRACT", KP_SUBTRACT},
    // {"KP_ADD", KP_ADD},
    // {"KP_ENTER", KP_ENTER},
    // {"KP_EQUAL", KP_EQUAL},
    // {"LEFT_SHIFT", LEFT_SHIFT},
    // {"LEFT_CONTROL", LEFT_CONTROL},
    // {"LEFT_ALT", LEFT_ALT},
    // {"LEFT_SUPER", LEFT_SUPER},
    // {"RIGHT_SHIFT", RIGHT_SHIFT},
    // {"RIGHT_CONTROL", RIGHT_CONTROL},
    // {"RIGHT_ALT", RIGHT_ALT},
    // {"RIGHT_SUPER", RIGHT_SUPER},
    // {"MENU", MENU}
};

#endif