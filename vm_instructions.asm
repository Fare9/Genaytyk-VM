;****************************************
;* Version: 2.0
;* Author: F9(@Farenain)
;* Date: 2018/05/24
;* Module: vm_disassembler_x86.py
;****************************************



.486
.model flat, stdcall
.stack 100h
option casemap :none



.data

HardcodedString db 'aAb0cBd1eCf2gDh3jEk4lFm5nGp6qHr7sJt8uKv9w',0
aOkay_guy db 'OKAY_GUY',0
hashedName db 05Fh dup(0)
db 9 dup(0)
MySerialHash dd 0
	dd 0
	dd 0
db 014h dup(0)
MyName db 025h dup(0)
MySerial db 025h dup(0)
nameLength dd 0
finalComparation dd 0
REG0x24 dd 0
REG0x28 dd 0


.code

start:
Label_00000000:	CALL VIRTUAL_FUNCTION_000001F9

Label_00000005:	CMP EAX , 0FFFFFFFFh
			JZ  Label_000001F8

Label_0000000F:	XOR ECX , ECX 

Label_00000013:	MOV EDI , 0E0h 

Label_0000001A:	CALL VIRTUAL_FUNCTION_0000025D

Label_0000001F:	CMP EAX , 06h
			JNZ  Label_000001F8

Label_00000029:	ADD EDI , 07h 

Label_00000030:	INC ECX 

Label_00000033:	CMP ECX , 03h
			JNZ  Label_0000001A

Label_0000003D:	XOR ECX , ECX 

Label_00000041:	MOV EDI , 0E0h 

Label_00000048:	CALL VIRTUAL_FUNCTION_0000027D

Label_0000004D:	INC EDI 

Label_00000050:	INC ECX 

Label_00000053:	CMP ECX , 03h
			JNZ  Label_00000048

Label_0000005D:	XOR ECX , ECX 

Label_00000061:	MOV EDI , 0E0h 

Label_00000068:	MOV EBX , 09Bh 

Label_0000006F:	CALL VIRTUAL_FUNCTION_000002AF

Label_00000074:	CMP EAX , 0FFFFFFFFh
			JZ  Label_000001F8

Label_0000007E:	MOV dword ptr [HardcodedString + EBX] , EAX

Label_00000082:	ADD EBX , 04h 

Label_00000089:	ADD EDI , 07h 

Label_00000090:	INC ECX 

Label_00000093:	CMP ECX , 03h
			JNZ  Label_0000006F

Label_0000009D:	push ECX
				MOV ECX , [REG0x24]
				MOV ECX , dword ptr [HardcodedString + 00000009Bh]
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_000000A3:	push ECX
				MOV ECX , [REG0x24]
				ROL ECX , 07h 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_000000A7:	push ECX
				MOV ECX , [REG0x24]
				MOV dword ptr [HardcodedString + 00000009Bh] , ECX
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_000000AD:	push ECX
				MOV ECX , [REG0x24]
				MOV ECX , dword ptr [HardcodedString + 00000009Fh]
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_000000B3:	push ECX
				MOV ECX , [REG0x24]
				ROR ECX , 09h 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_000000B7:	push ECX
				MOV ECX , [REG0x24]
				MOV dword ptr [HardcodedString + 00000009Fh] , ECX
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_000000BD:	push ECX
				MOV ECX , [REG0x24]
				MOV ECX , dword ptr [HardcodedString + 0000000A3h]
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_000000C3:	push ECX
				MOV ECX , [REG0x24]
				ROR ECX , 0Bh 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_000000C7:	push ECX
				MOV ECX , [REG0x24]
				MOV dword ptr [HardcodedString + 0000000A3h] , ECX
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_000000CD:	MOV EAX , dword ptr [HardcodedString + 00000009Fh]

Label_000000D3:	XOR dword ptr [HardcodedString + 00000009Bh] , EAX

Label_000000D9:	ADD dword ptr [HardcodedString + 0000000A3h] , EAX

Label_000000DF:	MOV EAX , dword ptr [HardcodedString + 00000009Bh]

Label_000000E5:	SUB dword ptr [HardcodedString + 0000000A3h] , EAX

Label_000000EB:	CALL VIRTUAL_FUNCTION_000002FF

Label_000000F0:	CALL VIRTUAL_FUNCTION_00000417

Label_000000F5:	MOV dword ptr [HardcodedString + 0000000A7h] , EAX

Label_000000FB:	CALL VIRTUAL_FUNCTION_0000036C

Label_00000100:	MOV EAX , dword ptr [HardcodedString + 00000009Bh]

Label_00000106:	MOV dword ptr [HardcodedString + 000000093h] , EAX

Label_0000010C:	MOV EAX , dword ptr [HardcodedString + 00000009Fh]

Label_00000112:	push ECX
				MOV ECX , [REG0x24]
				MOV ECX , 093h 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_00000119:	push ECX
				MOV ECX , [REG0x24]
				ADD ECX , 04h 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_00000120:	push ECX
				MOV ECX , [REG0x24]
				MOV dword ptr [HardcodedString + ECX] , EAX
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_00000124:	MOV ECX , 0Ah 

Label_0000012B:	MOV EDX , 0ABh 

Label_00000132:	MOV EAX , 093h 

Label_00000139:	CALL VIRTUAL_FUNCTION_00000458

Label_0000013E:	DEC CL 

Label_00000141:	CMP CL , 00h
			JNZ  Label_0000012B

Label_00000148:	CALL VIRTUAL_FUNCTION_000003F2

Label_0000014D:	push ECX
				MOV ECX , [REG0x24]
				MOV ECX , 093h 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_00000154:	push ECX
				MOV ECX , [REG0x24]
				ADD ECX , 04h 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_0000015B:	push ECX
				MOV ECX , [REG0x24]
				CMP EAX , dword ptr [HardcodedString + ECX]
				MOV dword ptr [REG0x24] , ECX
				pop ECX

			JNZ  Label_000001F8

Label_00000162:	MOV EAX , dword ptr [HardcodedString + 0000000A3h]

Label_00000168:	push ECX
				MOV ECX , [REG0x24]
				MOV ECX , 093h 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_0000016F:	push ECX
				MOV ECX , [REG0x24]
				ADD ECX , 04h 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_00000176:	push ECX
				MOV ECX , [REG0x24]
				MOV dword ptr [HardcodedString + ECX] , EAX
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_0000017A:	MOV EAX , dword ptr [HardcodedString + 0000000A7h]

Label_00000180:	MOV dword ptr [HardcodedString + 0000000B3h] , EAX

Label_00000186:	push ECX
				MOV ECX , [REG0x24]
				MOV ECX , 0B3h 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_0000018D:	push ECX
				MOV ECX , [REG0x24]
				ADD ECX , 04h 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_00000194:	push ECX
				MOV ECX , [REG0x24]
				MOV dword ptr [HardcodedString + ECX] , 06B79745Fh
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_0000019B:	MOV ECX , 0Fh 

Label_000001A2:	MOV EDX , 0B3h 

Label_000001A9:	MOV EAX , 093h 

Label_000001B0:	CALL VIRTUAL_FUNCTION_00000458

Label_000001B5:	DEC ECX 

Label_000001B8:	CMP ECX , 00h
			JNZ  Label_000001A2

Label_000001C2:	MOV EDI , 093h 

Label_000001C9:	MOV ESI , 02Ah 

Label_000001D0:	XOR ECX , ECX 

Label_000001D4:	MOV AL , byte ptr [HardcodedString + EDI]

Label_000001D8:	CMP AL , byte ptr [HardcodedString + ESI]
			JNZ  Label_000001F8

Label_000001DF:	INC EDI 

Label_000001E2:	INC ESI 

Label_000001E5:	INC ECX 

Label_000001E8:	CMP ECX , 08h
			JNZ  Label_000001D4

Label_000001F2:	MOV dword ptr [HardcodedString + 000000109h] , 01h

Label_000001F8:	INT 3


VIRTUAL_FUNCTION_000001F9	proc

Label_000001F9:	XOR EAX , EAX 

Label_000001FD:	XOR EDX , EDX 

Label_00000201:	MOV EDI , 0E0h 

Label_00000208:	MOV AL , byte ptr [HardcodedString + EDI]

Label_0000020C:	CMP AL , 02Dh
			JNZ  Label_0000021F

Label_00000213:	INC EDX 

Label_00000216:	MOV dword ptr [HardcodedString + EDI] , 00h

Label_0000021A:	JMP Label_0000023C

Label_0000021F:	MOV ECX , 00h 

Label_00000226:	CMP AL , byte ptr [HardcodedString + ECX]
			JZ  Label_0000023C

Label_0000022D:	INC ECX 

Label_00000230:	CMP dword ptr [HardcodedString + ECX] , 00h
			JNZ  Label_00000226

Label_00000237:	JMP Label_00000255

Label_0000023C:	INC EDI 

Label_0000023F:	CMP dword ptr [HardcodedString + EDI] , 00h
			JNZ  Label_00000208

Label_00000246:	CMP EDX , 02h
			JNZ  Label_00000255

Label_00000250:	XOR EAX , EAX 

Label_00000254:	RET 


Label_00000255:	MOV EAX , 0FFFFFFFFh 

Label_0000025C:	RET 

VIRTUAL_FUNCTION_000001F9	endp


VIRTUAL_FUNCTION_0000025D	proc

Label_0000025D:	MOV ESI , EDI 

Label_00000261:	MOV AL , byte ptr [HardcodedString + ESI]

Label_00000265:	CMP AL , 00h
			JZ  Label_00000274

Label_0000026C:	INC ESI 

Label_0000026F:	JMP Label_00000261

Label_00000274:	SUB ESI , EDI 

Label_00000278:	MOV EAX , ESI 

Label_0000027C:	RET 

VIRTUAL_FUNCTION_0000025D	endp


VIRTUAL_FUNCTION_0000027D	proc

Label_0000027D:	XOR EDX , EDX 

Label_00000281:	MOV AL , byte ptr [HardcodedString + EDI]

Label_00000285:	MOV EBX , 00h 

Label_0000028C:	DEC EBX 

Label_0000028F:	INC EBX 

Label_00000292:	CMP AL , byte ptr [HardcodedString + EBX]
			JNZ  Label_0000028F

Label_00000299:	SUB EBX , 00h 

Label_000002A0:	MOV byte ptr [HardcodedString + EDI] , BL

Label_000002A4:	INC EDI 

Label_000002A7:	CMP dword ptr [HardcodedString + EDI] , 00h
			JNZ  Label_0000027D

Label_000002AE:	RET 

VIRTUAL_FUNCTION_0000027D	endp


VIRTUAL_FUNCTION_000002AF	proc

Label_000002AF:	PUSH EBX 

Label_000002B2:	XOR EBX , EBX 

Label_000002B6:	XOR EAX , EAX 

Label_000002BA:	MOV EDX , EDI 

Label_000002BE:	ADD EDI , 05h 

Label_000002C5:	SUB EDX , 01h 

Label_000002CC:	MOV ESI , 01h 

Label_000002D3:	XOR EAX , EAX 

Label_000002D7:	MOV AL , byte ptr [HardcodedString + EDI]

Label_000002DB:	IMUL EAX , ESI 

Label_000002DF:	ADD EBX , EAX 

Label_000002E3:	IMUL ESI , 029h 

Label_000002EA:	DEC EDI 

Label_000002ED:	CMP EDI , EDX 
			JNZ  Label_000002D3

Label_000002F4:	INC EDI 

Label_000002F7:	MOV EAX , EBX 

Label_000002FB:	POP EBX 

Label_000002FE:	RET 

VIRTUAL_FUNCTION_000002AF	endp


VIRTUAL_FUNCTION_000002FF	proc

Label_000002FF:	MOV ESI , 0BBh 

Label_00000306:	MOV EDX , ESI 

Label_0000030A:	ADD EDX , dword ptr [HardcodedString + 000000105h]

Label_00000310:	MOV EDI , 033h 

Label_00000317:	ADD EDI , 05Fh 

Label_0000031E:	XOR ECX , ECX 

Label_00000322:	XOR EAX , EAX 

Label_00000326:	XOR AL , byte ptr [HardcodedString + ESI]

Label_0000032A:	XOR AL , 075h 

Label_0000032E:	ADD AL , CL 

Label_00000332:	ADD ECX , 03h 

Label_00000339:	XOR ECX , 045h 

Label_00000340:	ROL ECX , 03h 

Label_00000344:	INC ESI 

Label_00000347:	MOV byte ptr [HardcodedString + EDI] , AL

Label_0000034B:	CMP EDI , 033h
			JZ  Label_0000036B

Label_00000355:	DEC EDI 

Label_00000358:	CMP ESI , EDX 
			JNZ  Label_00000326

Label_0000035F:	MOV ESI , 0BBh 

Label_00000366:	JMP Label_00000326

Label_0000036B:	RET 

VIRTUAL_FUNCTION_000002FF	endp


VIRTUAL_FUNCTION_0000036C	proc

Label_0000036C:	MOV EAX , 013A1DCB4h 

Label_00000373:	MOV EBX , 045E8ADC1h 

Label_0000037A:	MOV ECX , 028h 

Label_00000381:	MOV EDI , 033h 

Label_00000388:	MOV ESI , EDI 

Label_0000038C:	ADD ESI , 030h 

Label_00000393:	MOV EDX , ESI 

Label_00000397:	XOR EAX , dword ptr [HardcodedString + EDI]

Label_0000039B:	ROL EAX , 03h 

Label_0000039F:	ADD EAX , dword ptr [HardcodedString + ESI]

Label_000003A3:	ADD EDI , 04h 

Label_000003AA:	ADD ESI , 04h 

Label_000003B1:	CMP EDI , EDX 
			JNZ  Label_00000397

Label_000003B8:	XOR EAX , EBX 

Label_000003BC:	push ECX
				MOV ECX , [REG0x24]
				MOV ECX , EAX 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_000003C0:	push ECX
				MOV ECX , [REG0x28]
				MOV ECX , EBX 
				MOV dword ptr [REG0x28] , ECX
				pop ECX


Label_000003C4:	push ECX
				MOV ECX , [REG0x24]
				MOV EBX , ECX 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_000003C8:	push ECX
				MOV ECX , [REG0x28]
				MOV EAX , ECX 
				MOV dword ptr [REG0x28] , ECX
				pop ECX


Label_000003CC:	DEC ECX 

Label_000003CF:	CMP ECX , 00h
			JNZ  Label_00000381

Label_000003D9:	MOV dword ptr [HardcodedString + 0000000ABh] , EAX

Label_000003DF:	push ECX
				MOV ECX , [REG0x24]
				MOV ECX , 0ABh 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_000003E6:	push ECX
				MOV ECX , [REG0x24]
				ADD ECX , 04h 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_000003ED:	push ECX
				MOV ECX , [REG0x24]
				MOV dword ptr [HardcodedString + ECX] , EBX
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_000003F1:	RET 

VIRTUAL_FUNCTION_0000036C	endp


VIRTUAL_FUNCTION_000003F2	proc

Label_000003F2:	XOR EAX , EAX 

Label_000003F6:	MOV EDI , 033h 

Label_000003FD:	MOV ESI , EDI 

Label_00000401:	ADD ESI , 05Dh 

Label_00000408:	ADD EAX , dword ptr [HardcodedString + EDI]

Label_0000040C:	INC EDI 

Label_0000040F:	CMP EDI , ESI 
			JNZ  Label_00000408

Label_00000416:	RET 

VIRTUAL_FUNCTION_000003F2	endp


VIRTUAL_FUNCTION_00000417	proc

Label_00000417:	MOV EAX , 01A2B3C4Dh 

Label_0000041E:	MOV ECX , 0FFh 

Label_00000425:	MOV EDI , 033h 

Label_0000042C:	MOV ESI , EDI 

Label_00000430:	ADD ESI , 05Dh 

Label_00000437:	MOV EBX , dword ptr [HardcodedString + EDI]

Label_0000043B:	XOR EAX , EBX 

Label_0000043F:	ROL EAX , 07h 

Label_00000443:	INC EDI 

Label_00000446:	CMP EDI , ESI 
			JNZ  Label_00000437

Label_0000044D:	DEC CL 

Label_00000450:	CMP CL , 00h
			JNZ  Label_00000425

Label_00000457:	RET 

VIRTUAL_FUNCTION_00000417	endp


VIRTUAL_FUNCTION_00000458	proc

Label_00000458:	PUSHAD 

Label_00000459:	PUSH EAX 

Label_0000045C:	MOV ECX , dword ptr [HardcodedString + EDX]

Label_00000460:	MOV dword ptr [HardcodedString + 00000010Ah] , ECX

Label_00000466:	push ECX
				MOV ECX , [REG0x24]
				MOV ECX , EDX 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_0000046A:	push ECX
				MOV ECX , [REG0x24]
				ADD ECX , 04h 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_00000471:	push EBX
				MOV EBX , [REG0x24]
				MOV ECX , dword ptr [HardcodedString + EBX]
				MOV dword ptr [REG0x24] , EBX
				pop EBX


Label_00000475:	push ECX
				MOV ECX , [REG0x24]
				MOV ECX , 010Ah 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_0000047C:	push ECX
				MOV ECX , [REG0x24]
				ADD ECX , 04h 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_00000483:	push EBX
				MOV EBX , [REG0x24]
				MOV dword ptr [HardcodedString + EBX] , ECX
				MOV dword ptr [REG0x24] , EBX
				pop EBX


Label_00000487:	CALL VIRTUAL_FUNCTION_00000682

Label_0000048C:	MOV ESI , EAX 

Label_00000490:	POP EDI 

Label_00000493:	XOR EAX , EAX 

Label_00000497:	MOV ECX , 064h 

Label_0000049E:	XOR EBX , EBX 

Label_000004A2:	push ECX
				MOV ECX , [REG0x24]
				MOV ECX , EDI 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_000004A6:	push ECX
				MOV ECX , [REG0x24]
				MOV EBX , ECX 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_000004AA:	ADD EBX , 07h 

Label_000004B1:	push ECX
				MOV ECX , [REG0x28]
				MOV ECX , EDX 
				MOV dword ptr [REG0x28] , ECX
				pop ECX


Label_000004B5:	push ECX
				MOV ECX , [REG0x24]
				MOV AL , byte ptr [HardcodedString + ECX]
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_000004B9:	push ECX
				MOV ECX , [REG0x28]
				XOR AL , byte ptr [HardcodedString + ECX]
				MOV dword ptr [REG0x28] , ECX
				pop ECX


Label_000004BD:	push ECX
				MOV ECX , [REG0x28]
				INC ECX 
				MOV dword ptr [REG0x28] , ECX
				pop ECX


Label_000004C0:	push ECX
				MOV ECX , [REG0x28]
				SUB AL , byte ptr [HardcodedString + ECX]
				MOV dword ptr [REG0x28] , ECX
				pop ECX


Label_000004C4:	push ECX
				MOV ECX , [REG0x28]
				DEC ECX 
				MOV dword ptr [REG0x28] , ECX
				pop ECX


Label_000004C7:	ADD AL , 012h 

Label_000004CB:	push ECX
				MOV ECX , [REG0x24]
				MOV byte ptr [HardcodedString + ECX] , AL
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_000004CF:	push ECX
				MOV ECX , [REG0x28]
				INC ECX 
				MOV dword ptr [REG0x28] , ECX
				pop ECX


Label_000004D2:	push ECX
				MOV ECX , [REG0x24]
				INC ECX 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_000004D5:	push ECX
				MOV ECX , [REG0x24]
				CMP ECX , EBX 
				MOV dword ptr [REG0x24] , ECX
				pop ECX

			JNZ  Label_000004B5

Label_000004DC:	XOR EBX , EBX 

Label_000004E0:	push ECX
				MOV ECX , [REG0x24]
				MOV ECX , EDI 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_000004E4:	push ECX
				push EBX
				MOV ECX , dword ptr [REG0x24]
				MOV EBX , dword ptr [REG0x28]
				MOV EBX , dword ptr [HardcodedString + ECX]
				MOV dword ptr [REG0x28] , EBX
				MOV dword ptr [REG0x24] , ECX
				pop EBX
				pop ECX


Label_000004E8:	push ECX
				MOV ECX , [REG0x28]
				ROL ECX , 03h 
				MOV dword ptr [REG0x28] , ECX
				pop ECX


Label_000004EC:	push ECX
				push EBX
				MOV ECX , dword ptr [REG0x24]
				MOV EBX , dword ptr [REG0x28]
				MOV dword ptr [HardcodedString + ECX] , EBX
				MOV dword ptr [REG0x28] , EBX
				MOV dword ptr [REG0x24] , ECX
				pop EBX
				pop ECX


Label_000004F0:	push ECX
				MOV ECX , [REG0x24]
				ADD ECX , 04h 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_000004F7:	push ECX
				push EBX
				MOV ECX , dword ptr [REG0x24]
				MOV EBX , dword ptr [REG0x28]
				MOV EBX , dword ptr [HardcodedString + ECX]
				MOV dword ptr [REG0x28] , EBX
				MOV dword ptr [REG0x24] , ECX
				pop EBX
				pop ECX


Label_000004FB:	push ECX
				MOV ECX , [REG0x28]
				ROR ECX , 03h 
				MOV dword ptr [REG0x28] , ECX
				pop ECX


Label_000004FF:	push ECX
				push EBX
				MOV ECX , dword ptr [REG0x24]
				MOV EBX , dword ptr [REG0x28]
				MOV dword ptr [HardcodedString + ECX] , EBX
				MOV dword ptr [REG0x28] , EBX
				MOV dword ptr [REG0x24] , ECX
				pop EBX
				pop ECX


Label_00000503:	push ECX
				MOV ECX , [REG0x24]
				MOV ECX , EDI 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_00000507:	push ECX
				MOV ECX , [REG0x28]
				MOV ECX , EDX 
				MOV dword ptr [REG0x28] , ECX
				pop ECX


Label_0000050B:	push ECX
				MOV ECX , [REG0x28]
				MOV EAX , dword ptr [HardcodedString + ECX]
				MOV dword ptr [REG0x28] , ECX
				pop ECX


Label_0000050F:	push ECX
				MOV ECX , [REG0x24]
				XOR dword ptr [HardcodedString + ECX] , EAX
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_00000513:	push ECX
				MOV ECX , [REG0x28]
				ADD ECX , 04h 
				MOV dword ptr [REG0x28] , ECX
				pop ECX


Label_0000051A:	push ECX
				MOV ECX , [REG0x24]
				ADD ECX , 04h 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_00000521:	push ECX
				MOV ECX , [REG0x28]
				MOV EAX , dword ptr [HardcodedString + ECX]
				MOV dword ptr [REG0x28] , ECX
				pop ECX


Label_00000525:	push ECX
				MOV ECX , [REG0x24]
				XOR dword ptr [HardcodedString + ECX] , EAX
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_00000529:	push ECX
				MOV ECX , [REG0x24]
				MOV ECX , EDI 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_0000052D:	push ECX
				MOV ECX , [REG0x28]
				MOV ECX , EDX 
				MOV dword ptr [REG0x28] , ECX
				pop ECX


Label_00000531:	push ECX
				MOV ECX , [REG0x24]
				ADD dword ptr [HardcodedString + ECX] , EAX
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_00000535:	push ECX
				MOV ECX , [REG0x28]
				MOV EAX , dword ptr [HardcodedString + ECX]
				MOV dword ptr [REG0x28] , ECX
				pop ECX


Label_00000539:	push ECX
				MOV ECX , [REG0x24]
				ADD ECX , 04h 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_00000540:	push ECX
				MOV ECX , [REG0x24]
				ADD dword ptr [HardcodedString + ECX] , EAX
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_00000544:	INC EBX 

Label_00000547:	CMP EBX , 010h
			JNZ  Label_000004E0

Label_00000551:	XOR EBX , EBX 

Label_00000555:	MOV EBP , 020h 

Label_0000055C:	CALL VIRTUAL_FUNCTION_00000764

Label_00000561:	CMP EAX , 00h
			JZ  Label_000005A9

Label_0000056B:	PUSH EBP 

Label_0000056E:	CMP EBP , 07h
			JBE  Label_00000589

Label_00000578:	SUB EBP , 07h 

Label_0000057F:	CMP EBP , 07h
			JA  Label_00000578

Label_00000589:	push ECX
				MOV ECX , [REG0x24]
				MOV ECX , EDX 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_0000058D:	push ECX
				MOV ECX , [REG0x24]
				ADD ECX , EBX 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_00000591:	push ECX
				MOV ECX , [REG0x24]
				MOV AL , byte ptr [HardcodedString + ECX]
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_00000595:	push ECX
				MOV ECX , [REG0x24]
				MOV ECX , EDI 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_00000599:	push ECX
				MOV ECX , [REG0x24]
				ADD ECX , EBP 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_0000059D:	push ECX
				MOV ECX , [REG0x24]
				XOR byte ptr [HardcodedString + ECX] , AL
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_000005A1:	POP EBP 

Label_000005A4:	JMP Label_000005D9

Label_000005A9:	PUSH EBX 

Label_000005AC:	CMP EBX , 07h
			JBE  Label_000005C7

Label_000005B6:	SUB EBX , 07h 

Label_000005BD:	CMP EBX , 07h
			JA  Label_000005B6

Label_000005C7:	push ECX
				MOV ECX , [REG0x24]
				MOV ECX , EDI 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_000005CB:	push ECX
				MOV ECX , [REG0x24]
				ADD ECX , EBX 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_000005CF:	push ECX
				MOV ECX , [REG0x24]
				NOT dword ptr [HardcodedString + ECX]
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_000005D2:	push ECX
				MOV ECX , [REG0x24]
				XOR byte ptr [HardcodedString + ECX] , BL
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_000005D6:	POP EBX 

Label_000005D9:	INC EBX 

Label_000005DC:	CMP EBX , 08h
			JB  Label_000005EA

Label_000005E6:	XOR EBX , EBX 

Label_000005EA:	DEC EBP 

Label_000005ED:	CMP EBP , 00h
			JNZ  Label_0000055C

Label_000005F7:	XOR EBX , EBX 

Label_000005FB:	push ECX
				MOV ECX , [REG0x24]
				MOV ECX , EDI 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_000005FF:	push ECX
				MOV ECX , [REG0x24]
				MOV AL , byte ptr [HardcodedString + ECX]
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_00000603:	PUSH EDI 

Label_00000606:	ADD EDI , 07h 

Label_0000060D:	SUB EDI , EBX 

Label_00000611:	XOR byte ptr [HardcodedString + EDI] , AL

Label_00000615:	POP EDI 

Label_00000618:	push ECX
				MOV ECX , [REG0x24]
				MOV AL , byte ptr [HardcodedString + ECX]
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_0000061C:	ROL AL , 03h 

Label_00000620:	push ECX
				MOV ECX , [REG0x24]
				MOV byte ptr [HardcodedString + ECX] , AL
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_00000624:	push ECX
				MOV ECX , [REG0x24]
				INC ECX 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_00000627:	INC EBX 

Label_0000062A:	CMP EBX , 08h
			JNZ  Label_000005FF

Label_00000634:	push ECX
				MOV ECX , [REG0x24]
				MOV ECX , EDI 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_00000638:	push ECX
				MOV ECX , [REG0x24]
				MOV EAX , dword ptr [HardcodedString + ECX]
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_0000063C:	ROL EAX , 07h 

Label_00000640:	push ECX
				MOV ECX , [REG0x24]
				MOV dword ptr [HardcodedString + ECX] , EAX
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_00000644:	push ECX
				MOV ECX , [REG0x24]
				ADD ECX , 04h 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_0000064B:	push ECX
				MOV ECX , [REG0x24]
				MOV EAX , dword ptr [HardcodedString + ECX]
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_0000064F:	ROR EAX , 07h 

Label_00000653:	push ECX
				MOV ECX , [REG0x24]
				MOV dword ptr [HardcodedString + ECX] , EAX
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_00000657:	DEC ECX 

Label_0000065A:	CMP ECX , 00h
			JNZ  Label_0000049E

Label_00000664:	push ECX
				MOV ECX , [REG0x24]
				MOV ECX , 010Ah 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_0000066B:	push ECX
				MOV ECX , [REG0x24]
				MOV dword ptr [HardcodedString + ECX] , 00h
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_00000672:	push ECX
				MOV ECX , [REG0x24]
				ADD ECX , 04h 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_00000679:	push ECX
				MOV ECX , [REG0x24]
				MOV dword ptr [HardcodedString + ECX] , 00h
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_00000680:	POPAD 

Label_00000681:	RET 

VIRTUAL_FUNCTION_00000458	endp


VIRTUAL_FUNCTION_00000682	proc

Label_00000682:	XOR EBX , EBX 

Label_00000686:	push ECX
				MOV ECX , [REG0x24]
				MOV ECX , 010Ah 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_0000068D:	push ECX
				MOV ECX , [REG0x24]
				INC ECX 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_00000690:	push ECX
				MOV ECX , [REG0x24]
				ADD ECX , EBX 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_00000694:	push ECX
				MOV ECX , [REG0x24]
				MOV AL , byte ptr [HardcodedString + ECX]
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_00000698:	push ECX
				MOV ECX , [REG0x24]
				DEC ECX 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_0000069B:	push ECX
				MOV ECX , [REG0x24]
				ADD byte ptr [HardcodedString + ECX] , AL
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_0000069F:	push ECX
				MOV ECX , [REG0x24]
				SUB byte ptr [HardcodedString + ECX] , BL
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_000006A3:	push ECX
				MOV ECX , [REG0x24]
				MOV ECX , 010Ah 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_000006AA:	push ECX
				MOV ECX , [REG0x24]
				MOV EAX , dword ptr [HardcodedString + ECX]
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_000006AE:	ROL EAX , 05h 

Label_000006B2:	push ECX
				MOV ECX , [REG0x24]
				MOV dword ptr [HardcodedString + ECX] , EAX
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_000006B6:	push ECX
				MOV ECX , [REG0x24]
				ADD ECX , 04h 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_000006BD:	push ECX
				MOV ECX , [REG0x24]
				MOV EAX , dword ptr [HardcodedString + ECX]
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_000006C1:	ROR EAX , 05h 

Label_000006C5:	push ECX
				MOV ECX , [REG0x24]
				MOV dword ptr [HardcodedString + ECX] , EAX
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_000006C9:	INC EBX 

Label_000006CC:	CMP EBX , 07h
			JNZ  Label_00000686

Label_000006D6:	XOR EAX , EAX 

Label_000006DA:	XOR EBX , EBX 

Label_000006DE:	XOR ECX , ECX 

Label_000006E2:	MOV EDI , 045h 

Label_000006E9:	push ECX
				MOV ECX , [REG0x24]
				MOV ECX , 010Ah 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_000006F0:	push ECX
				MOV ECX , [REG0x24]
				ADD ECX , EBX 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_000006F4:	push ECX
				MOV ECX , [REG0x24]
				MOV AL , byte ptr [HardcodedString + ECX]
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_000006F8:	ADD ECX , EAX 

Label_000006FC:	IMUL EDI , EDI 

Label_00000700:	INC ECX 

Label_00000703:	IMUL ECX , EDI 

Label_00000707:	INC EBX 

Label_0000070A:	CMP EBX , 04h
			JNZ  Label_000006E9

Label_00000714:	PUSH ECX 

Label_00000717:	XOR EAX , EAX 

Label_0000071B:	XOR EBX , EBX 

Label_0000071F:	XOR ECX , ECX 

Label_00000723:	MOV EDI , 035h 

Label_0000072A:	push ECX
				MOV ECX , [REG0x24]
				MOV ECX , 010Ah 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_00000731:	push ECX
				MOV ECX , [REG0x24]
				ADD ECX , 04h 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_00000738:	push ECX
				MOV ECX , [REG0x24]
				ADD ECX , EBX 
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_0000073C:	push ECX
				MOV ECX , [REG0x24]
				MOV AL , byte ptr [HardcodedString + ECX]
				MOV dword ptr [REG0x24] , ECX
				pop ECX


Label_00000740:	SUB ECX , EAX 

Label_00000744:	IMUL EDI , EDI 

Label_00000748:	INC ECX 

Label_0000074B:	IMUL ECX , EDI 

Label_0000074F:	INC EBX 

Label_00000752:	CMP EBX , 04h
			JNZ  Label_0000072A

Label_0000075C:	POP EAX 

Label_0000075F:	IMUL EAX , ECX 

Label_00000763:	RET 

VIRTUAL_FUNCTION_00000682	endp


VIRTUAL_FUNCTION_00000764	proc

Label_00000764:	PUSH EBP 

Label_00000767:	PUSH ESI 

Label_0000076A:	MOV EAX , 01h 

Label_00000771:	JMP Label_0000077A

Label_00000776:	SHL EAX , 01h 

Label_0000077A:	DEC EBP 

Label_0000077D:	CMP EBP , 0FFFFFFFFh
			JNZ  Label_00000776

Label_00000787:	AND ESI , EAX 

Label_0000078B:	CMP ESI , 00h
			JZ  Label_000007A1

Label_00000795:	MOV EAX , 01h 

Label_0000079C:	JMP Label_000007A5

Label_000007A1:	XOR EAX , EAX 

Label_000007A5:	POP ESI 

Label_000007A8:	POP EBP 

Label_000007AB:	RET 

VIRTUAL_FUNCTION_00000764	endp

end start