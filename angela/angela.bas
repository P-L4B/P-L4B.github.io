1 REM ANGELA GAME FOR APPLESOFT BASIC
10  DIM V$(2)                                                                   
20 P = 0                                                                        
30 A1 = 0                                                                       
40 M1 = 0                                                                       
50 R1 = 0                                                                       
60 B1 = 0                                                                       
70 B2 = 0                                                                       
80 C1 = 0                                                                       
90 C2 = 0                                                                       
100 D1 = 0                                                                      
110 R = 0                                                                       
120 T = 0                                                                       
130 A = 0                                                                       
140 P1 = 0                                                                      
150 U = 0                                                                       
160 V =  - 1                                                                    
170 F = 0                                                                       
180 C = 6                                                                       
185 A$ = ""                                                                     
190  PRINT "ANGELA GAME"                                                        
200  PRINT "-----------": PRINT                                                 
210  PRINT "I FOR INSTRUCTIONS": PRINT "C FOR CREDITS"                          
211  PRINT "ENTER TO PLAY": PRINT                                               
212  INPUT "YOUR CHOICE?";A$                                                    
213  IF A$ = "I" THEN  GOSUB 2500                                               
214  IF A$ = "C" THEN  GOSUB 2300                                               
215  PRINT : PRINT "DO YOU WANT TO SEE THE P101 REGISTERS"                      
216  INPUT "DURING THE GAME?";R$                                                
220  REM                                                                        
230  INPUT "GOAL? ";M1                                                          
240  IF M1 >  = 30 AND M1 <  = 100 THEN 280                                     
250  PRINT "PLEASE ENTER A NUMBER BETWEEN 30 AND 100"                           
260  GOTO 220                                                                   
280 V$ = "B1": GOSUB 1860                                                       
300 V$ = "B2": GOSUB 2080                                                       
310 V$ = "C1": GOSUB 1860                                                       
320 P = 1                                                                       
330  INPUT "YOUR NUMBER? ";M1                                                   
340  PRINT : PRINT "COMPUTING..";                                               
350 M1 =  INT (M1)                                                              
360  IF  NOT ((M1 < 0 OR M1 > 6) AND P = 1) THEN 362                            
361  PRINT : PRINT "VALUE NOT ALLOWED": GOTO 330                                
362  REM                                                                        
370  IF  NOT ((M1 < 1 OR M1 > 6) AND P = 0) THEN 373                            
371  PRINT : PRINT "VALUE NOT ALLOWED": GOTO 330                                
373  REM                                                                        
380 P = 0                                                                       
390  GOSUB 440                                                                  
400  GOSUB 1570                                                                 
410  IF F = 0 THEN  GOSUB 1510                                                  
420  IF F = 1 THEN 5000                                                         
430  GOTO 330                                                                   
440 V$ = "D1": GOSUB 1860                                                       
450 A = D1                                                                      
460 V$ = "C2": GOSUB 2080                                                       
470  GOSUB 1020                                                                 
480  RETURN                                                                     
490 V$ = "B2": GOSUB 1900                                                       
500 V$ = "D1": GOSUB 1590                                                       
510 V$ = "B2": GOSUB 1940                                                       
520 U = B2                                                                      
530  GOSUB 1450: IF F = 1 THEN  RETURN                                          
540 V$ = "B1": GOSUB 1900                                                       
550 V$ = "B2": GOSUB 1650                                                       
560 V = 9: GOSUB 2050                                                           
570 V$ = "M": GOSUB 1770                                                        
580 V$ = "R": GOSUB 1940                                                        
590  IF A1 > 0 THEN 592                                                         
591 V$ = "M": GOSUB 1590                                                        
592  REM                                                                        
600  GOSUB 620                                                                  
610  RETURN                                                                     
620 V$ = "A": GOSUB 1770                                                        
630 V$ = "C1": GOSUB 1860                                                       
640 V$ = "M": GOSUB 1590                                                        
650 V$ = "C2": GOSUB 1940                                                       
660 V$ = "C1": GOSUB 1900                                                       
670 V = 3: GOSUB 2050                                                           
680 V$ = "M": GOSUB 1650                                                        
690  GOSUB 2020                                                                 
700  IF A1 > 0 THEN 702                                                         
701 V = 5: GOSUB 2050:V$ = "C2": GOSUB 1860: GOSUB 1020: RETURN                 
702  REM                                                                        
710  GOSUB 730                                                                  
720  RETURN                                                                     
730 V$ = "M": GOSUB 1650                                                        
740  IF A1 <  = 0 THEN 742                                                      
741  GOSUB 800: RETURN                                                          
742  REM                                                                        
750  GOSUB 2020                                                                 
760  IF A1 <  = 0 THEN 762                                                      
761  GOSUB 1020: RETURN                                                         
762  REM                                                                        
770 V$ = "C2": GOSUB 1860                                                       
780  GOSUB 1020                                                                 
790  RETURN                                                                     
800 V = 1: GOSUB 2050                                                           
810 V$ = "M": GOSUB 1650                                                        
820  IF A1 <  = 0 THEN 822                                                      
821  GOSUB 880: RETURN                                                          
822  REM                                                                        
830 V$ = "C2": GOSUB 1860                                                       
840 V = 3: GOSUB 2050                                                           
850 V$ = "C1": GOSUB 1860                                                       
860  GOSUB 1020                                                                 
870  RETURN                                                                     
880 V$ = "M": GOSUB 1650                                                        
890  IF A1 <  = 0 THEN 892                                                      
891  GOSUB 950: RETURN                                                          
892  REM                                                                        
900 V$ = "C2": GOSUB 1860                                                       
910 V = 4: GOSUB 2050                                                           
920 V$ = "C1": GOSUB 1860                                                       
930  GOSUB 1020                                                                 
940  RETURN                                                                     
950 V$ = "M": GOSUB 1650                                                        
960 V$ = "C1": GOSUB 1860                                                       
970 V$ = "M": GOSUB 1590                                                        
980 V$ = "M": GOSUB 1590                                                        
990 V$ = "C2": GOSUB 1940                                                       
1000  GOSUB 1020                                                                
1010  RETURN                                                                    
1020 V$ = "D1": GOSUB 1900                                                      
1030 V$ = "C1": GOSUB 1650                                                      
1040  GOSUB 2020                                                                
1050  IF A1 <  = 0 THEN 1052                                                    
1051  GOSUB 1080: RETURN                                                        
1052  REM                                                                       
1060  GOSUB 1150                                                                
1070  RETURN                                                                    
1080 V = 7: GOSUB 2050                                                          
1090 V$ = "M": GOSUB 1900                                                       
1100 V$ = "D1": GOSUB 1650                                                      
1110 V$ = "C1": GOSUB 1650                                                      
1120  GOSUB 2020                                                                
1130  IF A1 > 0 THEN 1132                                                       
1131  GOSUB 1150: GOTO 1133                                                     
1132  GOSUB 1230                                                                
1133  REM                                                                       
1140  RETURN                                                                    
1150 V$ = "C2": GOSUB 1900                                                      
1160  IF A1 <  = 0 THEN 1162                                                    
1161  GOSUB 1200: RETURN                                                        
1162  REM                                                                       
1170  PRINT : PRINT "THIS VALUE IS NOT ALLOWED"                                 
1180 V = 0                                                                      
1190  RETURN                                                                    
1200 V$ = "C1": GOSUB 1940                                                      
1210  GOSUB 1230                                                                
1220  RETURN                                                                    
1230 V$ = "C2": GOSUB 1900                                                      
1240  IF A1 > 0 THEN 1242                                                       
1241  GOSUB 490: GOTO 1243                                                      
1242  GOSUB 1260                                                                
1243  REM                                                                       
1250  RETURN                                                                    
1260 V$ = "B1": GOSUB 1900                                                      
1270 V$ = "B2": GOSUB 1650                                                      
1280 V = 2: GOSUB 2050                                                          
1290 V$ = "M": GOSUB 1650                                                       
1300  IF A1 <  = 0 THEN 1302                                                    
1301  GOSUB 1390: RETURN                                                        
1302  REM                                                                       
1310 V$ = "D1": GOSUB 1900                                                      
1320 V$ = "M": GOSUB 1650                                                       
1330  GOSUB 2020                                                                
1340  IF A1 <  = 0 THEN 1342                                                    
1341  GOSUB 1390: RETURN                                                        
1342  REM                                                                       
1350 V = 1: GOSUB 2050                                                          
1360 V$ = "C1": GOSUB 1860                                                      
1370  GOSUB 1390                                                                
1380  RETURN                                                                    
1390 P1 = C1                                                                    
1400 V$ = "B2": GOSUB 1900                                                      
1410 V$ = "C1": GOSUB 1590                                                      
1420 V$ = "B2": GOSUB 1940                                                      
1430 U = B2                                                                     
1440  RETURN                                                                    
1450 F = 0                                                                      
1460 V$ = "B1": GOSUB 1900                                                      
1470 V$ = "B2": GOSUB 1650                                                      
1480  IF A1 <  > 0 THEN 1483                                                    
1481  PRINT : PRINT "YOU WIN! CONGRATULATIONS!! :-D":F = 1                      
1482  PRINT                                                                     
1483  REM                                                                       
1490  IF A1 >  = 0 THEN 1492                                                    
1491  PRINT : PRINT : PRINT "YOU LOSE... :'(":F = 1                             
1492  REM                                                                       
1500  RETURN                                                                    
1510 F = 0                                                                      
1520 V$ = "B1": GOSUB 1900                                                      
1530 V$ = "B2": GOSUB 1650                                                      
1540  IF A1 <  > 0 THEN 1542                                                    
1541  PRINT : PRINT : PRINT "YOU LOSE... :'(":F = 1: PRINT                      
1542  REM                                                                       
1550  IF A1 >  = 0 THEN 1552                                                    
1551  PRINT : PRINT "YOU WIN! CONGRATULATIONS!! :-D":F = 1: PRINT               
1552  REM                                                                       
1560  RETURN                                                                    
1570  IF V THEN 1572                                                            
1571 V =  - 1:A = 0:P1 = 0: GOTO 1574                                           
1572  PRINT : PRINT "YOU:";A;" COMPUTER:";P1;" SUM:";U:C = C + 1:A = 0:P1 = 0   
1573  PRINT                                                                     
1574  REM                                                                       
1580  RETURN                                                                    
1590  GOSUB 2120                                                                
1600 M1 = R                                                                     
1620 A1 = A1 + M1                                                               
1640  RETURN                                                                    
1650  GOSUB 2120                                                                
1660 M1 = R                                                                     
1680 A1 = A1 - M1                                                               
1700  RETURN                                                                    
1710  GOSUB 2120                                                                
1720 M1 = R                                                                     
1740 A1 = A1 * M1                                                               
1760  RETURN                                                                    
1770 T = A1                                                                     
1780  GOSUB 2120                                                                
1790 M1 = R                                                                     
1810 A1 = A1 / M1                                                               
1830 R1 = T -  INT (T / M1) * M1: REM   T MOD M1                                
1850  RETURN                                                                    
1860 R = M1                                                                     
1870  GOSUB 2210                                                                
1890  RETURN                                                                    
1900  GOSUB 2120                                                                
1910 A1 = R                                                                     
1930  RETURN                                                                    
1940 T = A1                                                                     
1950  GOSUB 2120                                                                
1960 A1 = R                                                                     
1980 R = T                                                                      
1990  GOSUB 2210                                                                
2010  RETURN                                                                    
2020 A1 =  ABS (A1)                                                             
2040  RETURN                                                                    
2050 M1 = V                                                                     
2070  RETURN                                                                    
2080 R = 0                                                                      
2090  GOSUB 2210                                                                
2110  RETURN                                                                    
2120  IF V$ <  > "A" THEN 2122                                                  
2121 R = A1: RETURN                                                             
2122  IF R$ <  > "Y" THEN 2125                                                  
2123  GOSUB 3000                                                                
2125  IF R$ <  > "Y" THEN  PRINT ".";                                           
2130  IF V$ <  > "M" THEN 2132                                                  
2131 R = M1: RETURN                                                             
2132  REM                                                                       
2140  IF V$ <  > "R" THEN 2142                                                  
2141 R = R1: RETURN                                                             
2142  REM                                                                       
2150  IF V$ <  > "B1" THEN 2152                                                 
2151 R = B1: RETURN                                                             
2152  REM                                                                       
2160  IF V$ <  > "B2" THEN 2162                                                 
2161 R = B2: RETURN                                                             
2162  REM                                                                       
2170  IF V$ <  > "C1" THEN 2172                                                 
2171 R = C1: RETURN                                                             
2172  REM                                                                       
2180  IF V$ <  > "C2" THEN 2182                                                 
2181 R = C2: RETURN                                                             
2182  REM                                                                       
2190  IF V$ <  > "D1" THEN 2192                                                 
2191 R = D1: RETURN                                                             
2192  REM                                                                       
2200  RETURN                                                                    
2210  IF V$ <  > "A" THEN 2212                                                  
2211 A1 = R: RETURN                                                             
2212  REM                                                                       
2220  IF V$ <  > "M" THEN 2222                                                  
2221 M1 = R: RETURN                                                             
2222  REM                                                                       
2230  IF V$ <  > "R" THEN 2232                                                  
2231 R1 = R: RETURN                                                             
2232  REM                                                                       
2240  IF V$ <  > "B1" THEN 2242                                                 
2241 B1 = R: RETURN                                                             
2242  REM                                                                       
2250  IF V$ <  > "B2" THEN 2252                                                 
2251 B2 = R: RETURN                                                             
2252  REM                                                                       
2260  IF V$ <  > "C1" THEN 2262                                                 
2261 C1 = R: RETURN                                                             
2262  REM                                                                       
2270  IF V$ <  > "C2" THEN 2272                                                 
2271 C2 = R: RETURN                                                             
2272  REM                                                                       
2280  IF V$ <  > "D1" THEN 2282                                                 
2281 D1 = R: RETURN                                                             
2282  REM                                                                       
2290  RETURN                                                                    
2300  PRINT : PRINT                                                             
2310  PRINT "ANGELA GAME, ORIGINALLY DEVELOPED FOR"                             
2320  PRINT "OLIVETTI PROGRAMMA 101 DESKTOP COMPUTER"                           
2330  PRINT "BY:                                                                
2340  PRINT "- PIER GIORGIO PEROTTO"                                             
2350  PRINT "- GASTONE GARZIERA"                                                
2360  PRINT "- GIOVANNI DE SANDRE"                                              
2370  PRINT "AS A DEMO FOR BEMA IN NEW YORK IN 1965."                           
2380  PRINT                                                                     
2390  PRINT "CONVERSION TO QB 4.5 FROM JAVA IN 2013:"                           
2400  PRINT "- CLAUDIO LARINI"                                                  
2410  PRINT "                                                                   
2420  PRINT "CONVERSION TO APPLESOFT BASIC IN 2024:"                            
2430  PRINT "- FRANCESCO SBLENDORIO"                                            
2440  PRINT "- CLAUDIO PARMIGIANI"                                              
2450  PRINT : RETURN                                                            
2500  PRINT "INSTRUCTIONS:"                                                     
2510  PRINT "A NUMERICAL GOAL (BETWEEN 30 AND 100)"                             
2520  PRINT "IS SET TO BE REACHED."                                             
2530  PRINT "THE PLAYER CHOOSES A VALUE FROM 1 TO 6"                            
2540  PRINT "AND THE PROGRAM WILL THEN DO THE SAME."                            
2550  PRINT "THE NUMBERS WILL ADD UP AND, IN DOING"                             
2560  PRINT "SO, YOU WILL GET CLOSER TO THE GOAL."                              
2570  PRINT                                                                     
2580  PRINT "IT IS NOT ALLOWED TO PLAY THE PREVIOUS"                            
2590  PRINT "NUMBER OF THE OPPONENT, NOR THE VALUE"                             
2600  PRINT "CORRESPONDING TO ITS COMPLEMENT TO 7:"                             
2610  PRINT "1/6, 2/5, 3/4."                                                    
2620  PRINT                                                                     
2630  PRINT "ON THE FIRST ROUND ONLY: ENTER 0 TO"                               
2640  PRINT "MAKE THE COMPUTER START THE GAME."                                 
2650  PRINT "THE PLAYER WHO REACHES THE GOAL FIRST,"                            
2660  PRINT "OR FORCES THE OPPONENT TO EXCEED IT,"                              
2670  PRINT "WINS.                                                              
2680  PRINT                                                                     
2690  RETURN                                                                    
3000  PRINT "A=";A1;" M=";M1;" R=";R1;" B=";B1;                                 
3010  PRINT " B'=";B2;" C=";C1;" C'=";C2;" D=";D1                               
3020  RETURN                                                                    
5000  INPUT "ANOTHER GAME?";S$                                                  
5010  IF S$ = "Y" THEN 230                                                      
5020  END 
