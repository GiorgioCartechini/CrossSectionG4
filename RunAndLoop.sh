export MAT="Y89" Z=39 A=89 D=8.9

ensvsubt '${MAT} ${Z} ${A} ${D}' < inelsatic.mac > inelastic_${MAT}.mac
./Hadrs03 inelastic_${MAT}.mac 1 100 10 10000
mkdir ${MAT}
mv *.csv ${MAT}
mv *.txt ${MAT}
