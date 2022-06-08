.segment "CODE"
lda #$69
sta $00

.segment "RESET_VECTOR"
.word $0100