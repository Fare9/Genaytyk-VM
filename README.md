# Genaytyk-VM
My notes about Genyatyk VM crackme

Here I have my notes about Genyatyk VM, I have my analysis of the binary (once unpacked from MEW packer with qunpack), I tried to rename every funcion and every variable (even you have structs and enums). Trying to resolve this VM, I wrote my first disassembler, and I learned about this kind of obfuscation, I think VMs are one of the most complex packers, but well I had fun.

+ In this URL, you can find the python disassembler of the VM: <a href="https://github.com/Fare9/Genaytyk-VM/blob/master/vm_disassembler.py">vm_disassembler.py</a>

+ Here, the output of this python script, commented: <a href="https://github.com/Fare9/Genaytyk-VM/blob/master/vm_instructions.txt">vm_instructions.txt</a>

+ Crackme without mew packer: <a href="https://github.com/Fare9/Genaytyk-VM/blob/master/vmkgme__.exe">vmkgme__.exe</a>

+ Crackme .idb from IDA 6.8: <a href="https://github.com/Fare9/Genaytyk-VM/blob/master/vmkgme__.idb">vmkgme__.idb</a>

+ Crackme with mew packer: <a href="https://github.com/Fare9/Genaytyk-VM/blob/master/VMkgme.exe.exe">vmkgme.exe</a>

+ Crackme .idb (with mew) from IDA 6.8: <a href="https://github.com/Fare9/Genaytyk-VM/blob/master/VMkgme.idb">vmkgme.idb</a>

+ Some functions from Crackme in C, and the keygen from Andrewl resolution: <a href="https://github.com/Fare9/Genaytyk-VM/blob/master/GenaytykVM.cpp">GenaytykVM.cpp</a>

+ Translator from Genaytyk virtual code to LLVM IR (compiled with LLVM 3.8.1): <a href="https://github.com/Fare9/Genaytyk-VM/tree/master/genaytyk2llvmir">genaytyk2llvmir</a>

+ Code of Genaytyk in LLVM IR: <a href="https://github.com/Fare9/Genaytyk-VM/blob/master/genaytyk2llvmir/genaytyk.ll">genaytyk.ll</a>

As you can see, I was not able to resolve the encrypt function =( I hope to continue learning how this function works and continue working with VMs.

Finally I wrote the genaytyk VM code lifter with LLVM IR, so what you can find in <a href="https://github.com/Fare9/Genaytyk-VM/blob/master/genaytyk2llvmir/genaytyk.ll">genaytyk.ll</a> it would be the LLVM IR version of <a href="https://github.com/Fare9/Genaytyk-VM/blob/master/vm_instructions.txt">vm_instructions.txt</a> or <a href="https://github.com/Fare9/Genaytyk-VM/blob/master/vm_instructions.asm">vm_instructions.asm</a>. The translation library can be found in <a href="https://github.com/Fare9/Genaytyk-VM/tree/master/genaytyk2llvmir/src/genaytyk">genaytyk code lifter library</a>, and the disassembler code in <a href="https://github.com/Fare9/Genaytyk-VM/tree/master/genaytyk2llvmir/src/disassembler">genaytyk llvm ir disassembler</a>.
