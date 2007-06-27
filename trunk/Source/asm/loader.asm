;;;;;;;;;;;;;;;;;;;;;;;;;;;LOADER;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;	Onyx Kernel Loader for Phoenix OS						;;
;;	Last Modified: June 22, 2007							;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[BITS 32]
[SECTION .text]
[global start]
[extern main]
[extern end]

start:
     ;mov esp, stack_start
   
     mov ebp, es
     mov ebp, [main]	; The entry point of the kernel
						; Push the parameters for main() onto the stack																
     push bx			; This goes into ramsize in main()
     push end			; entry in main()
     
     call main			; Launch the kernel
     
						; Pop the parameters off the stack
     pop ebp
     pop bx
     popa
     
     cli
     hlt

[SECTION .bss]

     resb 8192			; reserve 8k stack
stack_start:

