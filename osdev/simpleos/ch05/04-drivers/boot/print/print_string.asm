; 
; print_string will print a string to screen using BIOS telescripting mode.
;
; INPUT:
;
; 	bx should hold the address of the string to be printed.
;
print_string:
 pusha

 mov ah, 0x0e

 loop_begin:
 	mov al, [bx]
 	cmp al, 0
 	je loop_end
 	int 0x10
 	add bx, 1
 	jmp loop_begin

 loop_end: 
 	popa
 	ret
