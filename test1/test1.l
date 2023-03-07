00000000                ; test1.asm
00000000                label: 
00000000 00000500       ldc 5 	; an unused label
00000001 00000000       	ldc 0
00000002 FFFFFB00       	ldc -5
00000003 00000500       	ldc +5
00000004 FFFFFB11       loop: br label ; an infinite loop
00000005                ;yoooo
00000005 00002D00       ldc 45 ;yoooo
