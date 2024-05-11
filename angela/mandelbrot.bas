100 rem a basic, ascii mandelbrot
110 rem
120 rem this implementation copyright (c)2019, gordon henderson
130 rem
140 rem permission to use/abuse anywhere for any purpose granted, but
150 rem it comes with no warranty whatsoever. goodluck!
160 rem
161 rem adapted for applesoft and apple-1 by claudio parmigiani, 2024
162 rem
163 rem
170 c$=".,'~=+:;[/<*?&o0x# ":rem 'palette' lightest to darkest...
180 s1=1: rem set to 0 if your mid$() indexes from 0.
190 m1=len(c$)
200 m2=4
210 l5=-2.0
220 t1=1.25
230 x5=2.5
240 y5=-2.5
250 w=38
260 h=21
270 s2=x5/w
280 s3=y5/h
300 for y=0 to h
310 c3=y*s3+t1
320 for x=0 to w
330 c2=x*s2+l5
340 z2=0
350 z3=0
360 c1=s1
370 x2=z2*z2
380 y2=z3*z3
390 if c1>m1 then 460
400 if (x2+y2)>m2 then 460
410 t=x2-y2+c2
420 z3=2*z2*z3+c3
430 z2=t
440 c1=c1+1
450 goto 370
460 print mid$(c$,c1-s1,1);
470 next x
480 print
490 next y
520 end

