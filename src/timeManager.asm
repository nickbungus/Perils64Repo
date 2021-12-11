.export _initTime
.export _updateTime

MAXTIMECLICKER = 30
NUMBER_OF_TIME_CHARS = 21
_initTime:
    ldx #22
@initLoop:
    lda #90
    sta $4347,x
    dex
    bne @initLoop

    lda #MAXTIMECLICKER
    sta timeClicker
    lda #0
    sta timeCharIndex
    lda #NUMBER_OF_TIME_CHARS
    sta timeCharCountdown


    rts


timeChars:   
    .byte   90, 91, 92, 93, 94, $ff

timeClicker:    .byte   MAXTIMECLICKER
timeCharIndex:  .byte   0
timeCharCountdown:   .byte   22

_updateTime:
    dec timeClicker
    bne @exit

    lda #MAXTIMECLICKER
    sta timeClicker

    inc timeCharIndex
    ldx timeCharIndex
    lda timeChars,x
    cmp #$ff
    bne @charLoaded

    ldx #1
    stx timeClicker
    ldx #0
    stx timeCharIndex
    lda timeChars,x     ;reload the initial char
    dec timeCharCountdown
    bmi @exit           ; I think we are out of time


@charLoaded:
    ldx timeCharCountdown
    sta $4348,x
@exit:
    rts

