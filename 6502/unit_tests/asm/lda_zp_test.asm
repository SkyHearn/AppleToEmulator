.segment "CODE"
lda $00

.segment "ZERO_PAGE"
.byte $69

.segment "RESET_VECTOR"
.word $0100