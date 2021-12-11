.export _initInterrupt

_initInterrupt:
        SEI                  ; set interrupt bit, make the CPU ignore interrupt requests
           LDA #%01111111       ; switch off interrupt signals from CIA-1
           STA $DC0D

           AND $D011            ; clear most significant bit of VIC's raster register
           STA $D011

           LDA $DC0D            ; acknowledge pending interrupts from CIA-1
           LDA $DD0D            ; acknowledge pending interrupts from CIA-2

           LDA #120             ; set rasterline where interrupt shall occur
           STA $D012

           LDA #<_handleInterrupt            ; set interrupt vectors, pointing to interrupt service routine below
           STA $0314
           LDA #>_handleInterrupt
           STA $0315

           LDA #%00000001       ; enable raster interrupt signals from VIC
           STA $D01A

           CLI                  ; clear interrupt flag, allowing the CPU to respond to interrupt requests
           RTS

_handleInterrupt:
        LDA #$7
        STA $D020            ; change border colour to yellow

        jsr $8003

        LDA #$0
        STA $D020            ; change border back to black

          
        ASL $D019            ; acknowledge the interrupt by clearing the VIC's interrupt flag

        JMP $EA31            ; jump into KERNAL's standard interrupt service routine to handle keyboard scan, cursor display etc.