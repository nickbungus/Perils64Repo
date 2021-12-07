.import _noteAvailable, _noteColourLocations, _travelatorFrame, _travelatorCount, _travelatorLocations, _travelatorChars
.export _animamteNotes, _animateTravelators

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;   Animate Notes
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
_animamteNotes: 
    ldy #7
@notesLoop:
    lda _noteAvailable,y
    cmp #1
    bne @reloop

    ;double y and store in x
    tya 
    asl 
    tax 

    lda _noteColourLocations,x
    sta @screenRead+1
    sta @screenWrite+1

    lda _noteColourLocations+1,x
    sta @screenRead+2
    sta @screenWrite+2

@screenRead:
    ldx $ffff
    inx 
    cpx #8
    bne @screenWrite
    ldx #0

@screenWrite:
    stx $ffff
@reloop:
    dey 
    bpl @notesLoop
    rts
    
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;   Animate Travelators
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
_animateTravelators:
    lda _travelatorCount
    tax         ; x Contains travelCounter
    asl     
    tay         ; y contains travelCounter *2

    inc _travelatorFrame
    lda _travelatorFrame
    cmp #3
    bcc @travLoop
    lda #0
    sta _travelatorFrame

@travLoop:
    lda _travelatorLocations,y
    sta @screenWrite+1

    lda _travelatorLocations+1,y
    sta @screenWrite+2

    lda _travelatorChars,x
    clc 
    adc _travelatorFrame
@screenWrite:
    sta $ffff

    dex 
    bmi @exit

    txa 
    asl 
    tay 
    jmp @travLoop
@exit:
    rts 