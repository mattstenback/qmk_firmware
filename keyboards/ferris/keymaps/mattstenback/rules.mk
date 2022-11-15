RGBLIGHT_ENABLE = no

# Enables combos to be defined cleanly in combos.def
COMBO_ENABLE = yes
VPATH += keyboards/gboards

# SRC += keymap.c
SRC += oneshot.c
SRC += swapper.c
