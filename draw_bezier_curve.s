	section .data

const_0:    dw 0.0
const_1:    dw 1.0
const_4:    dw 4.0
const_6:    dw 6.0
iter_less:  dw 0.0001	
	
	section .text
	global draw_bezier_curve

draw_bezier_curve:
;prolog

	mov         rax, const_1
	movss	    xmm0, [rax]		; t, które na początku ustawiam na 1
	mov         rax, const_4
	movss	    xmm2, [rax]
	mov         rax, const_6
	movss	    xmm3, [rax]
	cvtsi2ss    xmm15, [rsi]	;ładowanie współrzędnych x
	cvtsi2ss    xmm14, [rsi+4]
	cvtsi2ss    xmm13, [rsi+8]
	cvtsi2ss    xmm12, [rsi+12]
	cvtsi2ss    xmm11, [rsi+16]
	cvtsi2ss    xmm10, [rdx]	;ładowanie współrzędnych y
	cvtsi2ss    xmm9, [rdx+4]
	cvtsi2ss    xmm8, [rdx+8]
	cvtsi2ss    xmm7, [rdx+12]
	cvtsi2ss    xmm6, [rdx+16]

;body
;wyliczam współrzędne krzywej ze wzoru B(t) = ((1-t)^4)*P0 + 4*((1-t)^3)*t*P1 + 6*(((1-t)*t)^2)*P2 + 4*(1-t)*(t^3)*P3 + (t^4)*P4, dla 0 =< t <= 1

	lea rdi, [rdi+54]		;ustawienie wskaźnika na początek zapisu kolorów pikseli w bitmapie

loop_draw_pixel:
	
	mov         rax, const_1
	movss	    xmm1, [rax]
	subss	    xmm1, xmm0		;1-t

;wyliczam współrzędna x

	movss	xmm5, xmm15	;x0
	mulss	xmm5, xmm1
	mulss	xmm5, xmm1
	mulss	xmm5, xmm1
	mulss	xmm5, xmm1	;*((1-t)^4)
	
	movss   xmm4, xmm5	;((1-t)^4)*x0

	movss	xmm5, xmm14	;x1
	mulss	xmm5, xmm1
	mulss	xmm5, xmm1
	mulss	xmm5, xmm1	;*((1-t)^3)
	mulss	xmm5, xmm0	;*t
	mulss	xmm5, xmm2	;*4.0

	addss	xmm4, xmm5	;((1-t)^4)*x0 + 4*((1-t)^3)*t*x1

	movss	xmm5, xmm13	;x2
	mulss	xmm5, xmm1
	mulss	xmm5, xmm1	;*((1-t)^2)
	mulss	xmm5, xmm0
	mulss	xmm5, xmm0	;*(t^2)
	mulss	xmm5, xmm3	;*6

	addss	xmm4, xmm5	;((1-t)^4)*x0 + 4*((1-t)^3)*t*x1 + 6*(((1-t)*t)^2)*x2

	movss	xmm5, xmm12	;x3
	mulss	xmm5, xmm1	;*(1-t)
	mulss	xmm5, xmm0
	mulss	xmm5, xmm0
	mulss	xmm5, xmm0	;*(t^3)
	mulss	xmm5, xmm2	;*4

	addss	xmm4, xmm5	;((1-t)^4)*x0 + 4*((1-t)^3)*t*x1 + 6*(((1-t)*t)^2)*x2 + 4*(1-t)*(t^3)*x3
		
	movss	xmm5, xmm11	;x4
	mulss	xmm5, xmm0
	mulss	xmm5, xmm0
	mulss	xmm5, xmm0
	mulss	xmm5, xmm0	;*(t^4)

	addss	xmm4, xmm5	;((1-t)^4)*x0 + 4*((1-t)^3)*t*x1 + 6*(((1-t)*t)^2)*x2 + 4*(1-t)*(t^3)*x3 + (t^4)*x4	

	cvtss2si r8 , xmm4	;konwertuję do int

;wyliczam współrzędna y

	movss	xmm5, xmm10	;y0
	mulss	xmm5, xmm1
	mulss	xmm5, xmm1
	mulss	xmm5, xmm1
	mulss	xmm5, xmm1	;*((1-t)^4)
	
	movss   xmm4, xmm5	;((1-t)^4)*y0

	movss	xmm5, xmm9	;y1
	mulss	xmm5, xmm1
	mulss	xmm5, xmm1
	mulss	xmm5, xmm1	;*((1-t)^3)
	mulss	xmm5, xmm0	;*t
	mulss	xmm5, xmm2	;*4.0

	addss	xmm4, xmm5	;((1-t)^4)*y0 + 4*((1-t)^3)*t*y1

	movss	xmm5, xmm8	;y2
	mulss	xmm5, xmm1
	mulss	xmm5, xmm1	;*((1-t)^2)
	mulss	xmm5, xmm0
	mulss	xmm5, xmm0	;*(t^2)
	mulss	xmm5, xmm3	;*6

	addss	xmm4, xmm5	;((1-t)^4)*y0 + 4*((1-t)^3)*t*y1 + 6*(((1-t)*t)^2)*y2

	movss	xmm5, xmm7	;y3
	mulss	xmm5, xmm1	;*(1-t)
	mulss	xmm5, xmm0
	mulss	xmm5, xmm0
	mulss	xmm5, xmm0	;*(t^3)
	mulss	xmm5, xmm2	;*4

	addss	xmm4, xmm5	;((1-t)^4)*y0 + 4*((1-t)^3)*t*y1 + 6*(((1-t)*t)^2)*y2 + 4*(1-t)*(t^3)*y3
		
	movss	xmm5, xmm6	;y4
	mulss	xmm5, xmm0
	mulss	xmm5, xmm0
	mulss	xmm5, xmm0
	mulss	xmm5, xmm0	;*(t^4)

	addss	xmm4, xmm5	;((1-t)^4)*y0 + 4*((1-t)^3)*t*y1 + 6*(((1-t)*t)^2)*y2 + 4*(1-t)*(t^3)*y3 + (t^4)*y4	

	cvtss2si r9 , xmm4	;konwertuję do int

;zaznaczam punkt o wyliczonych koordynatach

	mov	rax, r8
	imul	rax, rcx
	imul    rax, r9
	lea	rax, [rax + rdi]
	;mov	[rax],	byte 0
	;mov     [rax+1], byte 0
	;mov     [rax+2], byte 0

;powtarzam obliczenia dla zmienionej wartości t

	mov rax, iter_less
	subss	xmm0, [rax]
	mov rax, const_0
	ucomiss xmm0, [rax]
	jne loop_draw_pixel

;end
	ret
