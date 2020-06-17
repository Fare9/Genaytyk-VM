; ModuleID = 'genaytyk'

@eip = common global i32, align 4
@eax = common global i32, align 4
@ebx = common global i32, align 4
@ecx = common global i32, align 4
@edx = common global i32, align 4
@esp = common global i32, align 4
@ebp = common global i32, align 4
@edi = common global i32, align 4
@esi = common global i32, align 4
@reg_0x24 = common global i32, align 4
@reg_0x28 = common global i32, align 4
@reg_0x2c = common global i32, align 4
@reg_0x30 = common global i32, align 4
@reg_0x34 = common global i32, align 4
@reg_0x38 = common global i32, align 4
@al = common global i8, align 4
@ah = common global i8, align 4
@bl = common global i8, align 4
@bh = common global i8, align 4
@cl = common global i8, align 4
@ch = common global i8, align 4
@dl = common global i8, align 4
@dh = common global i8, align 4
@ax = common global i16, align 4
@bx = common global i16, align 4
@cx = common global i16, align 4
@dx = common global i16, align 4
@sp = common global i16, align 4
@bp = common global i16, align 4
@di = common global i16, align 4
@si = common global i16, align 4
@HardcodedString = common global <269 x i1> c"aAb0cBd1eCf2gDh3jEk4lFm5nGp6qHr7sJt8uKv9w\00", align 1
@OKAY_GUY = common global <9 x i1> c"OKAY_GUY\00", align 1
@hash_name = common global <95 x i1>, align 4
@global1 = common global <9 x i1>, align 4
@MySerialHash = common global i32, align 4
@global2 = common global i32, align 4
@global3 = common global i32, align 4
@global4 = common global <20 x i1>, align 4
@MyName = common global <37 x i1>, align 4
@MySerial = common global <37 x i1>, align 4
@nameLength = common global i32, align 4
@finalComparison = common global i32, align 4

define i32 @ror(i32 %l, i32 %r) {
entry_ror:
  %0 = and i32 %r, 4325535
  %1 = sub i32 0, %0
  %2 = and i32 %1, 4325535
  %3 = lshr i32 %l, %0
  %4 = shl i32 %l, %2
  %5 = or i32 %3, %4
  ret i32 %5
}

define i32 @rol(i32 %l, i32 %r) {
entry_rol:
  %0 = and i32 %r, 4325535
  %1 = sub i32 0, %0
  %2 = and i32 %1, 4325535
  %3 = shl i32 %l, %0
  %4 = lshr i32 %l, %2
  %5 = or i32 %3, %4
  ret i32 %5
}

define void @main(void) {
entry:

"0":                                              ; No predecessors!
  call void @"505"()

"5":                                              ; No predecessors!
  %ifcond = icmp eq i32* @eax, i32 -1
  br i1 %ifcond, label %"504", label %"15"

"15":                                             ; preds = %"5"
  store i32 zext (i32* xor (i32* @ecx, i32* @ecx) to i32), i32* @ecx

"19":                                             ; No predecessors!
  store i32 224, i32* @edi

"26":                                             ; preds = %"51"
  call void @"605"()

"31":                                             ; No predecessors!
  %ifcond1 = icmp ne i32* @eax, i32 6
  br i1 %ifcond1, label %"504", label %"41"

"41":                                             ; preds = %"31"
  store i32 zext (i32* add (i32* @edi, i32 7) to i32), i32* @edi

"48":                                             ; No predecessors!
  store i32 zext (i32* add (i32* @ecx, i32 1) to i32), i32* @ecx

"51":                                             ; No predecessors!
  %ifcond2 = icmp ne i32* @ecx, i32 3
  br i1 %ifcond2, label %"26", label %"61"

"61":                                             ; preds = %"51"
  store i32 zext (i32* xor (i32* @ecx, i32* @ecx) to i32), i32* @ecx

"65":                                             ; No predecessors!
  store i32 224, i32* @edi

"72":                                             ; preds = %"83"
  call void @"637"()

"77":                                             ; No predecessors!
  store i32 zext (i32* add (i32* @edi, i32 1) to i32), i32* @edi

"80":                                             ; No predecessors!
  store i32 zext (i32* add (i32* @ecx, i32 1) to i32), i32* @ecx

"83":                                             ; No predecessors!
  %ifcond3 = icmp ne i32* @ecx, i32 3
  br i1 %ifcond3, label %"72", label %"93"

"93":                                             ; preds = %"83"
  store i32 zext (i32* xor (i32* @ecx, i32* @ecx) to i32), i32* @ecx

"97":                                             ; No predecessors!
  store i32 224, i32* @edi

"104":                                            ; No predecessors!
  store i32 155, i32* @ebx

"111":                                            ; preds = %"147"
  call void @"687"()

"116":                                            ; No predecessors!
  %ifcond4 = icmp eq i32* @eax, i32 -1
  br i1 %ifcond4, label %"504", label %"126"

"126":                                            ; preds = %"116"
  store i32* @eax, i1* getelementptr (i1, <269 x i1>* @HardcodedString, i32* @ebx)

"130":                                            ; No predecessors!
  store i32 zext (i32* add (i32* @ebx, i32 4) to i32), i32* @ebx

"137":                                            ; No predecessors!
  store i32 zext (i32* add (i32* @edi, i32 7) to i32), i32* @edi

"144":                                            ; No predecessors!
  store i32 zext (i32* add (i32* @ecx, i32 1) to i32), i32* @ecx

"147":                                            ; No predecessors!
  %ifcond5 = icmp ne i32* @ecx, i32 3
  br i1 %ifcond5, label %"111", label %"157"

"157":                                            ; preds = %"147"
  %1 = load i1, i1* getelementptr (i1, <269 x i1>* @HardcodedString, i32 155)
  %2 = trunc i1 %1 to i32
  store i32 %2, i32* @reg_0x24

"163":                                            ; No predecessors!
  %3 = call i32 @rol(i32* @reg_0x24, i32 7)
  store i32 %3, i32* @reg_0x24

"167":                                            ; No predecessors!
  store i32* @reg_0x24, i1* getelementptr (i1, <269 x i1>* @HardcodedString, i32 155)

"173":                                            ; No predecessors!
  %4 = load i1, i1* getelementptr (i1, <269 x i1>* @HardcodedString, i32 159)
  %5 = trunc i1 %4 to i32
  store i32 %5, i32* @reg_0x24

"179":                                            ; No predecessors!
  %6 = call i32 @ror(i32* @reg_0x24, i32 9)
  store i32 %6, i32* @reg_0x24

"183":                                            ; No predecessors!
  store i32* @reg_0x24, i1* getelementptr (i1, <269 x i1>* @HardcodedString, i32 159)

"189":                                            ; No predecessors!
  %7 = load i1, i1* getelementptr (i1, <269 x i1>* @HardcodedString, i32 163)
  %8 = trunc i1 %7 to i32
  store i32 %8, i32* @reg_0x24

"195":                                            ; No predecessors!
  %9 = call i32 @ror(i32* @reg_0x24, i32 11)
  store i32 %9, i32* @reg_0x24

"199":                                            ; No predecessors!
  store i32* @reg_0x24, i1* getelementptr (i1, <269 x i1>* @HardcodedString, i32 163)

"205":                                            ; No predecessors!
  %10 = load i1, i1* getelementptr (i1, <269 x i1>* @HardcodedString, i32 159)
  %11 = trunc i1 %10 to i32
  store i32 %11, i32* @eax

"211":                                            ; No predecessors!
  store i1* xor (i1* getelementptr (i1, <269 x i1>* @HardcodedString, i32 155), i32* @eax), i1* getelementptr (i1, <269 x i1>* @HardcodedString, i32 155)

"217":                                            ; No predecessors!
  store i1* add (i1* getelementptr (i1, <269 x i1>* @HardcodedString, i32 163), i32* @eax), i1* getelementptr (i1, <269 x i1>* @HardcodedString, i32 163)

"223":                                            ; No predecessors!
  %12 = load i1, i1* getelementptr (i1, <269 x i1>* @HardcodedString, i32 155)
  %13 = trunc i1 %12 to i32
  store i32 %13, i32* @eax

"229":                                            ; No predecessors!
  store i1* sub (i1* getelementptr (i1, <269 x i1>* @HardcodedString, i32 163), i32* @eax), i1* getelementptr (i1, <269 x i1>* @HardcodedString, i32 163)

"235":                                            ; No predecessors!
  call void @"767"()

"240":                                            ; No predecessors!
  call void @"1047"()

"245":                                            ; No predecessors!
  store i32* @eax, i1* getelementptr (i1, <269 x i1>* @HardcodedString, i32 167)

"251":                                            ; No predecessors!
  call void @"876"()

"256":                                            ; No predecessors!
  %14 = load i1, i1* getelementptr (i1, <269 x i1>* @HardcodedString, i32 155)
  %15 = trunc i1 %14 to i32
  store i32 %15, i32* @eax

"262":                                            ; No predecessors!
  store i32* @eax, i1* getelementptr (i1, <269 x i1>* @HardcodedString, i32 147)

"268":                                            ; No predecessors!
  %16 = load i1, i1* getelementptr (i1, <269 x i1>* @HardcodedString, i32 159)
  %17 = trunc i1 %16 to i32
  store i32 %17, i32* @eax

"274":                                            ; No predecessors!
  store i32 147, i32* @reg_0x24

"281":                                            ; No predecessors!
  store i32 zext (i32* add (i32* @reg_0x24, i32 4) to i32), i32* @reg_0x24

"288":                                            ; No predecessors!
  store i32* @eax, i1* getelementptr (i1, <269 x i1>* @HardcodedString, i32* @reg_0x24)

"292":                                            ; No predecessors!
  store i32 10, i32* @ecx

"299":                                            ; preds = %"321"
  store i32 171, i32* @edx

"306":                                            ; No predecessors!
  store i32 147, i32* @eax

"313":                                            ; No predecessors!
  call void @"1112"()

"318":                                            ; No predecessors!
  %18 = load i32, i32* @ecx
  %19 = and i32 %18, -256
  %20 = or i32 %19, i8 zext (i8* sub (i8* @cl, i32 1) to i8)
  store i32 %20, i32* @ecx

"321":                                            ; No predecessors!
  %ifcond6 = icmp ne i8* @cl, i32 0
  br i1 %ifcond6, label %"299", label %"328"

"328":                                            ; preds = %"321"
  call void @"1010"()

"333":                                            ; No predecessors!
  store i32 147, i32* @reg_0x24

"340":                                            ; No predecessors!
  store i32 zext (i32* add (i32* @reg_0x24, i32 4) to i32), i32* @reg_0x24

"347":                                            ; No predecessors!
  %21 = load i32, i32* @eax
  %ifcond7 = icmp ne i32* @eax, i32 %21
  br i1 %ifcond7, label %"504", label %"354"

"354":                                            ; preds = %"347"
  %22 = load i1, i1* getelementptr (i1, <269 x i1>* @HardcodedString, i32 163)
  %23 = trunc i1 %22 to i32
  store i32 %23, i32* @eax

"360":                                            ; No predecessors!
  store i32 147, i32* @reg_0x24

"367":                                            ; No predecessors!
  store i32 zext (i32* add (i32* @reg_0x24, i32 4) to i32), i32* @reg_0x24

"374":                                            ; No predecessors!
  store i32* @eax, i1* getelementptr (i1, <269 x i1>* @HardcodedString, i32* @reg_0x24)

"378":                                            ; No predecessors!
  %24 = load i1, i1* getelementptr (i1, <269 x i1>* @HardcodedString, i32 167)
  %25 = trunc i1 %24 to i32
  store i32 %25, i32* @eax

"384":                                            ; No predecessors!
  store i32* @eax, i1* getelementptr (i1, <269 x i1>* @HardcodedString, i32 179)

"390":                                            ; No predecessors!
  store i32 179, i32* @reg_0x24

"397":                                            ; No predecessors!
  store i32 zext (i32* add (i32* @reg_0x24, i32 4) to i32), i32* @reg_0x24

"404":                                            ; No predecessors!
  store i32 1803121759, i1* getelementptr (i1, <269 x i1>* @HardcodedString, i32* @reg_0x24)

"411":                                            ; No predecessors!
  store i32 15, i32* @ecx

"418":                                            ; preds = %"440"
  store i32 179, i32* @edx

"425":                                            ; No predecessors!
  store i32 147, i32* @eax

"432":                                            ; No predecessors!
  call void @"1112"()

"437":                                            ; No predecessors!
  store i32 zext (i32* sub (i32* @ecx, i32 1) to i32), i32* @ecx

"440":                                            ; No predecessors!
  %ifcond8 = icmp ne i32* @ecx, i32 0
  br i1 %ifcond8, label %"418", label %"450"

"450":                                            ; preds = %"440"
  store i32 147, i32* @edi

"457":                                            ; No predecessors!
  store i32 42, i32* @esi

"464":                                            ; No predecessors!
  store i32 zext (i32* xor (i32* @ecx, i32* @ecx) to i32), i32* @ecx

"468":                                            ; preds = %"488"
  %26 = load i1, i1* getelementptr (i1, <269 x i1>* @HardcodedString, i32* @edi)
  %27 = trunc i1 %26 to i32
  %28 = trunc i32 %27 to i8
  %29 = load i32, i32* @eax
  %30 = and i32 %29, -256
  %31 = or i32 %30, i8 %28
  store i32 %31, i32* @eax

"472":                                            ; No predecessors!
  %32 = load i8, i8* @al
  %33 = trunc i8 %32 to i32
  %ifcond9 = icmp ne i8* @al, i32 %33
  br i1 %ifcond9, label %"504", label %"479"

"479":                                            ; preds = %"472"
  store i32 zext (i32* add (i32* @edi, i32 1) to i32), i32* @edi

"482":                                            ; No predecessors!
  store i32 zext (i32* add (i32* @esi, i32 1) to i32), i32* @esi

"485":                                            ; No predecessors!
  store i32 zext (i32* add (i32* @ecx, i32 1) to i32), i32* @ecx

"488":                                            ; No predecessors!
  %ifcond10 = icmp ne i32* @ecx, i32 8
  br i1 %ifcond10, label %"468", label %"498"

"498":                                            ; preds = %"488"
  store i32 1, i1* getelementptr (i1, <269 x i1>* @HardcodedString, i32 265)

"504":                                            ; preds = %"472", %"347", %"116", %"31", %"5"
  ret void
}

define void @"505"(void) {
"505":
  store i32 zext (i32* xor (i32* @eax, i32* @eax) to i32), i32* @eax

"509":                                            ; No predecessors!
  store i32 zext (i32* xor (i32* @edx, i32* @edx) to i32), i32* @edx

"513":                                            ; No predecessors!
  store i32 224, i32* @edi

"520":                                            ; preds = %"575"
  %1 = load i1, i1* getelementptr (i1, <269 x i1>* @HardcodedString, i32* @edi)
  %2 = trunc i1 %1 to i32
  %3 = trunc i32 %2 to i8
  %4 = load i32, i32* @eax
  %5 = and i32 %4, -256
  %6 = or i32 %5, i8 %3
  store i32 %6, i32* @eax

"524":                                            ; No predecessors!
  %ifcond = icmp ne i8* @al, i32 45
  br i1 %ifcond, label %"543", label %"531"

"531":                                            ; preds = %"524"
  store i32 zext (i32* add (i32* @edx, i32 1) to i32), i32* @edx

"534":                                            ; No predecessors!
  store i32 0, i1* getelementptr (i1, <269 x i1>* @HardcodedString, i32* @edi)

"538":                                            ; No predecessors!
  br label %"572"

"543":                                            ; preds = %"524"
  store i32 0, i32* @ecx

"550":                                            ; preds = %"560"
  %7 = load i8, i8* @al
  %8 = trunc i8 %7 to i32
  %ifcond1 = icmp eq i8* @al, i32 %8
  br i1 %ifcond1, label %"572", label %"557"

"557":                                            ; preds = %"550"
  store i32 zext (i32* add (i32* @ecx, i32 1) to i32), i32* @ecx

"560":                                            ; No predecessors!
  %9 = load i1, i1* getelementptr (i1, <269 x i1>* @HardcodedString, i32* @ecx)
  %10 = trunc i1 %9 to i32
  %ifcond2 = icmp ne i32 %10, 0
  br i1 %ifcond2, label %"550", label %"567"

"567":                                            ; preds = %"560"
  br label %"597"

"572":                                            ; preds = %"550", %"538"
  store i32 zext (i32* add (i32* @edi, i32 1) to i32), i32* @edi

"575":                                            ; No predecessors!
  %11 = load i1, i1* getelementptr (i1, <269 x i1>* @HardcodedString, i32* @edi)
  %12 = trunc i1 %11 to i32
  %ifcond3 = icmp ne i32 %12, 0
  br i1 %ifcond3, label %"520", label %"582"

"582":                                            ; preds = %"575"
  %ifcond4 = icmp ne i32* @edx, i32 2
  br i1 %ifcond4, label %"597", label %"592"

"592":                                            ; preds = %"582"
  store i32 zext (i32* xor (i32* @eax, i32* @eax) to i32), i32* @eax

"596":                                            ; No predecessors!

"597":                                            ; preds = %"582", %"567"
  store i32 -1, i32* @eax

"604":                                            ; No predecessors!
}

declare void @"605"(void)

declare void @"637"(void)

declare void @"687"(void)

declare void @"767"(void)

declare void @"1047"(void)

declare void @"876"(void)

declare void @"1112"(void)

declare void @"1010"(void)
