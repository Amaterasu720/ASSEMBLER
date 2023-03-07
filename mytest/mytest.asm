main: ldc -1 ;
      stl 10 ;
    
loop:	
	ldl 0    
	brlz  end;
	add	;
	adc 5;    
	ldc 0 ;    
	add   ;    
     stl 0   ; 
     adj 1 ;
	br loop ;
end:  ldc 0;
	ldc 0;