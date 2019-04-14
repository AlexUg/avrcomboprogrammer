EESchema Schematic File Version 4
LIBS:AVRProgrammer-cache
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 7 8
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Device:LED D1
U 1 1 5C9E09EC
P 5550 3700
AR Path="/5C9DF7DB/5C9E09EC" Ref="D1"  Part="1" 
AR Path="/5C9EEA56/5C9E09EC" Ref="D2"  Part="1" 
AR Path="/5C9F27B3/5C9E09EC" Ref="D3"  Part="1" 
AR Path="/5C9F9F2C/5C9E09EC" Ref="D4"  Part="1" 
AR Path="/5C9FD77F/5C9E09EC" Ref="D5"  Part="1" 
AR Path="/5CA01110/5C9E09EC" Ref="D6"  Part="1" 
AR Path="/5CA084E5/5C9E09EC" Ref="D7"  Part="1" 
F 0 "D6" V 5588 3583 50  0000 R CNN
F 1 "LED" V 5497 3583 50  0000 R CNN
F 2 "LED_THT:LED_D3.0mm_Horizontal_O1.27mm_Z2.0mm" H 5550 3700 50  0001 C CNN
F 3 "" H 5550 3700 50  0001 C CNN
	1    5550 3700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5550 3850 5550 3900
$Comp
L power:GND #PWR021
U 1 1 5C9E09F3
P 5550 3900
AR Path="/5C9DF7DB/5C9E09F3" Ref="#PWR021"  Part="1" 
AR Path="/5C9EEA56/5C9E09F3" Ref="#PWR022"  Part="1" 
AR Path="/5C9F27B3/5C9E09F3" Ref="#PWR023"  Part="1" 
AR Path="/5C9F9F2C/5C9E09F3" Ref="#PWR024"  Part="1" 
AR Path="/5C9FD77F/5C9E09F3" Ref="#PWR025"  Part="1" 
AR Path="/5CA01110/5C9E09F3" Ref="#PWR026"  Part="1" 
AR Path="/5CA084E5/5C9E09F3" Ref="#PWR027"  Part="1" 
F 0 "#PWR026" H 5550 3650 50  0001 C CNN
F 1 "GND" H 5700 3850 50  0000 C CNN
F 2 "" H 5550 3900 50  0001 C CNN
F 3 "" H 5550 3900 50  0001 C CNN
	1    5550 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5550 3550 5550 3450
Wire Wire Line
	5550 3450 5350 3450
$Comp
L Device:R R7
U 1 1 5C9E09FB
P 5200 3450
AR Path="/5C9DF7DB/5C9E09FB" Ref="R7"  Part="1" 
AR Path="/5C9EEA56/5C9E09FB" Ref="R8"  Part="1" 
AR Path="/5C9F27B3/5C9E09FB" Ref="R9"  Part="1" 
AR Path="/5C9F9F2C/5C9E09FB" Ref="R10"  Part="1" 
AR Path="/5C9FD77F/5C9E09FB" Ref="R11"  Part="1" 
AR Path="/5CA01110/5C9E09FB" Ref="R12"  Part="1" 
AR Path="/5CA084E5/5C9E09FB" Ref="R13"  Part="1" 
F 0 "R12" V 5100 3450 50  0000 C CNN
F 1 "330" V 5200 3450 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 5130 3450 50  0001 C CNN
F 3 "" H 5200 3450 50  0001 C CNN
	1    5200 3450
	0    1    1    0   
$EndComp
Wire Wire Line
	5050 3450 4750 3450
Text HLabel 4750 3450 0    50   Input ~ 0
IN
$EndSCHEMATC
