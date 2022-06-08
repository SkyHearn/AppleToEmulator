FILES=$(ls *.asm)

for f in $FILES
do
    f=${f::-4}

    ca65 $f.asm
    ld65 $f.o -C linker.ld -o $f.out

    mv $f.out $1/${f::-5}.bin
done