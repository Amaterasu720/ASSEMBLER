; test3.asm
; Test SET
ldc 5
ldc 4
ldc 5
adj 4
adc 5
val3: data 16
val: SET 75
ldc   val
adc val3
adj val3

