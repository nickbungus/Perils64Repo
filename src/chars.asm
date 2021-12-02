.export  _charAttributes
.segment    "GFXDATA"

_graphics:   .incbin     "Chars.bin"

_charAttributes:
        .incbin  "CharAttribs.bin"