// WindowsでJIS配列として認識しているときに、US配列として使うためのキーマップ
// @leopard_gecko さんがPlanck用に作成されたキーマップをかなり参考にしています。

#include QMK_KEYBOARD_H
#include "keymap_jp.h"       // qmk_firmware-master/quantum/keymap_extras/keymap_jp.h 日本語キーボード設定用
#include <sendstring_jis.h>  // macro sendstring for jis keyboard マクロ文字列送信時に日本語キーボード設定での文字化け回避

extern keymap_config_t keymap_config;

// レイヤー
#define _QWERTY 0
#define _LOWER 1
#define _RAISE 2
#define _GAME 3
#define _ARROW 4
#define _ADJUST 5

enum custom_keycodes {
  QWERTY = SAFE_RANGE, // QWERTYレイヤーへ
  MCR1,                // マクロ1
  MCR2,                // マクロ2
  MCR3,                // マクロ3
  DYNAMIC_MACRO_RANGE, // ダイナミックマクロ
  WN_SCLN,             // タップでJISの「:」  シフトでJISの「;」 (Windows)
  CMDCTL,  // ctrlをcmdと同じように扱うための試み
  DCTLHOME,  // ctrlキーをカウンターしてからHOMEをおす
  DCTLEND, // ctrlキーをカウンターしてからENDをおす
  NEO_LSFT,
  NEO_HOME,
  NEO_END,
  NEO_TAB
};


// Use Dynamic macro
#include "dynamic_macro.h"

#define KC_LOWR LT(_RAISE, KC_MHEN)    // タップで無変換     ホールドでRaise
#define KC_GAME LT(_GAME, KC_LCTL)     // タップで左Ctrl     ホールドでGame
#define KC_RASE LT(_RAISE, KC_HENK)    // タップで変換       ホールドでRaise
#define KC_ARROWCTL LT(CMDCTL, KC_MHEN)   // タップで無変換       ホールドでCMDCTL
#define KC_LSLB MT(MOD_LSFT, KC_LBRC)  // タップで[          ホールドで左Shift
#define KC_RSRB LT(_RAISE, KC_RBRC)    // タップで]          ホールドでRaise
#define KC_ALTB MT(MOD_LALT, KC_TAB)   // タップでTAB        ホールドで左Alt
#define CTL_ZH  CTL_T(KC_ZKHK)         // タップで半角/全角  ホールドで左Control     (Windows)
#define WN_CAPS S(KC_CAPS)             // Caps Lock                                  (Windows)
#define KC_ALPS LALT(KC_PSCR)          // Alt + PrintScreen
#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)
#define GAME MO(_GAME)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  /* QWERTY // UbuntuでJIS配列時のデフォルトキーマップ
   * ,-------------------------------------------------------.   ,-------------------------------------------------------.
   * |Tab    |   Q   |   W   |   E   |   R   |   T   |   -   |   |   =   |   Y   |   U   |   I   |   O   |   P   |   \   |
   * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
   * |  Ctrl |   A   |   S   |   D   |   F   |   G   |   '   |   |   `   |   H   |   J   |   K   |   L   |   :   | [/Sft |
   * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
   * |  Sift |   Z   |   X   |   C   |   V   |   B   |  BSPC |   |  ESC  |   N   |   M   |   ,   |   .   |   Up  |]/RAISE|
   * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
   * |無変換  |  GUI  |       |  Alt  | 無変換 | 無変換 | Space |   | Enter | 変換   | 変換   |  /   |  Left |  Down | Right |
   * |Lower  |       |       |       | RAISE | CMDCTL|       |   |       | RAISE | RAISE |       |       |       |       |
   * `-------------------------------------------------------'   `-------------------------------------------------------'
   */
  [_QWERTY] = LAYOUT(\
    KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_MINS,   KC_EQL, KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS, \
    KC_LCTL,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_QUOT,   KC_GRV, KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_LBRC, \
    KC_LSFT,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_BSPC,   _______ , KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_UP,    KC_RSRB, \
    KC_LOWR,  KC_LGUI, KC_ESC,  KC_LALT, CMDCTL,  CMDCTL, KC_SPC,   KC_ENT,  KC_RASE,  KC_RASE, KC_SLSH, KC_LEFT, KC_DOWN,   KC_RGHT \
  ),

  /* LOWER // 数字入力用レイヤー
   * ,-------------------------------------------------------.   ,-------------------------------------------------------.
   * |       |   1   |   2   |   3   |   4   |   5   |   6   |   |  Esc  |   7   |   8   |   9   |   .   |   =   |       |
   * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
   * |       |  F1   |  F2   |  F3   |  F4   |  F5   |  F6   |   |  F2   |   4   |   5   |   6   |   -   |   /   | Enter |
   * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
   * | Shift |  F7   |  F8   |  F9   |  F10  |  F11  |  F12  |   |       |   1   |   2   |   3   |   +   |   *   | Shift |
   * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
   * |       |       |XXXXXXX|       |       |       |       |   |       |   0   |       |       |       |       |       |
   * `-------------------------------------------------------'   `-------------------------------------------------------'
   */
  [_LOWER] = LAYOUT( \
    KC_ZKHK, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_MINS,   KC_EQL, KC_Y,    KC_U,    KC_I,    KC_O,     KC_P,    KC_BSLS, \
    _______,   KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_QUOT,   KC_GRV, KC_H,    KC_J,    KC_K,    KC_L,     _______,  _______, \
    KC_LSFT,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_BSPC,   KC_ESC, KC_N,    KC_M,    KC_COMM, KC_DOT,   KC_UP,  KC_RSFT, \
    _______,  _______, _______, _______, KC_LALT, KC_LCTL, _______, _______, _______, RAISE,    _______,  DCTLHOME, KC_DOWN,  DCTLEND \
  ),

  /* RAISE // 数字・記号入力用レイヤー
   * ,-------------------------------------------------------.   ,-------------------------------------------------------.
   * |       |   1   |   2   |   3   |   4   |   5   |       |   |       |   6   |   7   |   8   |   9   |   0   |   \   |
   * ,-------------------------------------------------------.   ,-------------------------------------------------------.
   * |       |   !   |   @   |   #   |   $   |   %   |       |   |       |   ^   |   &   |   *   |   (   |    )  |       |
   * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
   * | Shift |M-PLAY |M-MUTE |VOL_DWN|VOL_UP |       |       |   |       |       |       |   <   |   >   |   ?   | Shift |
   * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
   * |       |       |       |       |       |       |       |   |       |       |       |       |       |       |       |
   * `-------------------------------------------------------'   `-------------------------------------------------------'
   */
  [_RAISE] = LAYOUT( \
    _______, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,   _______,     _______ ,  KC_6,    KC_7,    KC_8,    KC_9, KC_0,  KC_BSLS, \
    _______, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, _______,    _______,  KC_CIRC, KC_AMPR, KC_PAST,  KC_LPRN,  KC_RPRN, _______, \
    KC_LSFT, KC_MPLY, KC_MUTE, KC_VOLD, KC_VOLU, _______, _______,    _______, XXXXXXX, XXXXXXX, KC_LT,   KC_GT,   KC_QUES, _______, \
    _______, _______, _______, _______, _______, _______, _______,    _______, _______, _______, _______, _______, _______, _______ \
  ),

  /* GAME // 左手はゲーム用レイヤー、右手はNumPad
   * ,-------------------------------------------------------.   ,-------------------------------------------------------.
   * |  Tab  |   Q   |   W   |   E   |   R   |   T   |PrntScr|   |  Esc  |   7   |   8   |   9   |   .   |   =   | BSPC  |
   * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
   * | Ctrl  |   A   |   S   |   D   |   F   |   G   |  F1   |   |  F2   |   4   |   5   |   6   |   -   |   /   | Enter |
   * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
   * | Shift |   Z   |   X   |   C   |   V   |   B   |  F2   |   |  Home |   1   |   2   |   3   |   +   |   Up  | Shift |
   * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
   * |  Del  |  GUI  |  Alt  |  Esc  | LOWER | Ctrl  | Enter |   |  End  |   0   | RAISE |        | HOME |  Down | END   |
   * `-------------------------------------------------------'   `-------------------------------------------------------'
   */
  [_GAME] = LAYOUT( \
    NEO_TAB, C(KC_Q),    C(KC_W),    C(KC_E),    C(KC_R),    C(KC_T),    C(KC_MINS),   C(KC_EQL), C(KC_Y),    C(KC_U),    C(KC_I),    C(KC_O),     C(KC_P),    C(KC_BSLS), \
    _______,   C(KC_A),    C(KC_S),    C(KC_D),    C(KC_F),    C(KC_G),    C(KC_QUOT),   C(KC_GRV), C(KC_H),    C(KC_J),    C(KC_K),    C(KC_L),     _______,  _______, \
    KC_LSFT,  C(KC_Z),    C(KC_X),    C(KC_C),    C(KC_V),    C(KC_B),    C(KC_BSPC),   C(KC_ESC), C(KC_N),    C(KC_M),    C(KC_COMM), C(KC_DOT),   C(KC_HOME),  C(KC_RSFT), \
    _______,  _______, _______, _______, C(KC_LALT), _______, C(KC_SPC), C(KC_ENT), _______, RAISE,    _______,  KC_HOME, C(KC_END),  KC_END \
  ),

  [_ARROW] = LAYOUT( \
    NEO_TAB, C(KC_Q),    C(KC_W),    C(KC_E),    C(KC_R),    C(KC_T),    C(KC_MINS),   C(KC_EQL), C(KC_Y),    C(KC_U),    C(KC_I),    C(KC_O),     C(KC_P),    C(KC_BSLS), \
    _______,   C(KC_A),    C(KC_S),    C(KC_D),    C(KC_F),    C(KC_G),    C(KC_QUOT),   C(KC_GRV), C(KC_H),    C(KC_J),    C(KC_K),    C(KC_L),     _______,  _______, \
    KC_LSFT,  C(KC_Z),    C(KC_X),    C(KC_C),    C(KC_V),    C(KC_B),    C(KC_BSPC),   C(KC_ESC), C(KC_N),    C(KC_M),    C(KC_COMM), C(KC_DOT),   C(KC_HOME),  C(KC_RSFT), \
    _______,  _______, _______, _______, C(KC_LALT), _______, _______, _______, _______, RAISE,    _______,  KC_LEFT, C(KC_END),  KC_RGHT \
  ),

  /* ADJUST // 設定用レイヤー (LOWER+RAISE)
   * ,-------------------------------------------------------.   ,-------------------------------------------------------.
   * |RGB_TOG| MCR1  | MCR2  | MCR3  |XXXXXXX|XXXXXXX|XXXXXXX|   |XXXXXXX|PLAY_M1|PLAY_M2|REC_M1 |REC_M2 |STP_REC| BSPC  |
   * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
   * | RESET |       |       |       |       |       |       |   |XXXXXXX|XXXXXXX|QWERTY | GAME  |XXXXXXX|XXXXXXX|XXXXXXX|
   * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
   * | Shift |       |       |       |       |       |       |   |XXXXXXX| M-PLAY|M-MUTE |VOL_DWN|VOL_UP |PREV_TR|NEXT_TR|
   * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
   * | DEBUG |XXXXXXX|XXXXXXX|       |       |XXXXXXX|XXXXXXX|   |XXXXXXX|XXXXXXX|       | Left  | Down  |   Up  | Right |
   * `-------------------------------------------------------'   `-------------------------------------------------------'
   */
  [_ADJUST] = LAYOUT( \
    RGB_TOG,  MCR1,    MCR2,    MCR3,    XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX, DYN_MACRO_PLAY1, DYN_MACRO_PLAY2, DYN_REC_START1, DYN_REC_START2, DYN_REC_STOP, KC_BSPC, \
    RESET,    RGB_MOD, RGB_M_P, RGB_M_B, RGB_HUI, RGB_SAI, RGB_VAI,   XXXXXXX, XXXXXXX, QWERTY,  GAME,   XXXXXXX, XXXXXXX, XXXXXXX, \
    KC_LSFT,  RGB_M_R, RGB_M_SN,RGB_M_G, RGB_HUD, RGB_SAD, RGB_VAD,   XXXXXXX, KC_MPLY, KC_MUTE, KC_VOLD, KC_VOLU, KC_MPRV, KC_MNXT, \
    DEBUG,    XXXXXXX, XXXXXXX, _______, _______, XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT \
  ),

};

// RGB Underglow使用時のレイヤー毎のカラー切り替え
uint32_t layer_state_set_keymap (uint32_t state) {
  return state;
}

void matrix_init_user(void) {
#ifdef RGBLIGHT_ENABLE
  rgblight_enable();
  rgblight_setrgb_teal();
#endif
}

uint32_t layer_state_set_user(uint32_t state) {
  state = update_tri_layer_state(state, _RAISE, _LOWER, _ADJUST);
#ifdef RGBLIGHT_ENABLE
    switch (biton32(state)) {
    case _RAISE:
      rgblight_setrgb_chartreuse(); // RAISE:シャルトリューズ
      break;
    case _LOWER:
      rgblight_setrgb_pink(); // LOWER:ピンク
      break;
    case _ADJUST:
      rgblight_setrgb_red(); // ADJUST:レッド
      break;
    default: //  for any other layers, or the default layer
      rgblight_setrgb_teal(); // 他:ティール
      break;
    }
#endif
return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    bool isNEO = false;
    static bool     lshift = false;
  if (!process_record_dynamic_macro(keycode, record)) {
      return false;
  }
  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
        set_single_persistent_default_layer(_QWERTY);
      }
      return false;
      break;
    case WN_SCLN: // コロン「;:」
      if (record->event.pressed) {
        lshift = keyboard_report->mods & MOD_BIT(KC_LSFT);
        if (lshift) {
          unregister_code(KC_LSFT);
          register_code(JP_SCLN);
          unregister_code(JP_SCLN);
        } else {
          register_code(JP_COLN);
          unregister_code(JP_COLN);
        }
      }
      return false;
      break;
    case MCR1:
      if (record->event.pressed) {
        SEND_STRING("hoge"); // 送信文字列
      }
      return false;
      break;
    case MCR2:
      if (record->event.pressed) {
        SEND_STRING("hogehoge"SS_TAP(X_ENTER)); // 送信文字列
      }
      return false;
      break;
    case MCR3:
      if (record->event.pressed) {
        SEND_STRING("hoge@hoge.hoge"); // 送信文字列
      }
      return false;
      break;
    case CMDCTL:
      if (record->event.pressed) {
        register_code(KC_MHEN); 
        unregister_code(KC_MHEN);
        layer_on(_GAME);
      } else {
        unregister_code(KC_LCTL);
        layer_off(_ARROW);
        layer_off(_GAME);
        isNEO = false;
      }
      return false;
      break;
    case NEO_TAB:
      if (record->event.pressed) {
        register_code(KC_LCTL); 
        register_code(KC_TAB);
        layer_on(_ARROW);
      } else {
        unregister_code(KC_TAB);
      }
      return false;
      break;          
    case NEO_LSFT:
      if (record->event.pressed) {
        register_code(KC_LSFT); 
        unregister_code(KC_LCTL);
        layer_on(_ARROW);
      } else {
        layer_off(_ARROW);
        unregister_code(KC_LSFT);
      }
      return false;
      break;    
    case NEO_HOME:
      if (record->event.pressed) {
        register_code(KC_LEFT); 
        register_code(KC_HOME); 
      } else {
        unregister_code(KC_LEFT);
        unregister_code(KC_HOME);
      }
      return false;
      break;
    case NEO_END:
      if (record->event.pressed) {
        register_code(KC_RGHT); 
        register_code(KC_END); 
      } else {
        unregister_code(KC_RGHT);
        unregister_code(KC_END);
      }
      return false;
      break;
    case DCTLHOME:
      if (record->event.pressed) {
        if (isNEO) {
          register_code(KC_LEFT);
        } else {
          register_code(KC_HOME);
        }
      } else {
        unregister_code(KC_HOME);
        unregister_code(KC_LEFT);
      }
      return false;
      break;
    case DCTLEND:
      if (record->event.pressed) {
        if (isNEO) {
          register_code(KC_RGHT);
        } else {
          register_code(KC_END);
        }
      } else {
        unregister_code(KC_END);
        unregister_code(KC_RGHT); 
      }
      return false;
      break;
  }
  return true;
}
