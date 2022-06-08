MEMORY {
        MEM: start = $0000, size = $FFFF; 
}
SEGMENTS {
        ZERO_PAGE:    load = MEM, TYPE = rw, start = $0000;
        CODE:         load = MEM, TYPE = rw, start = $0100;
        RESET_VECTOR: load = MEM, TYPE = rw, start = $FFFC;
}