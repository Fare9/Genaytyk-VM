# Genaytyk-VM
My notes about Genyatyk VM crackme

Here I have my notes about Genyatyk VM, I have my analysis of the binary (once unpacked from MEW packer with qunpack), I tried to rename every funcion and every variable (even you have structs and enums). Trying to resolve this VM, I wrote my first disassembler, and I learned about this kind of obfuscation, I think VMs are one of the most complex packers, but well I had fun.

+ In this URL, you can find the python disassembler of the VM: <a href="https://github.com/Fare9/Genaytyk-VM/blob/master/vm_disassembler.py">vm_disassembler.py</a>

+ Here, the output of this python script, commented: <a href="https://github.com/Fare9/Genaytyk-VM/blob/master/vm_instructions.txt">vm_instructions.txt</a>

+ Crackme without mew packer: <a href="https://github.com/Fare9/Genaytyk-VM/blob/master/vmkgme__.exe">vmkgme__.exe</a>

+ Crackme .idb from IDA 6.8: <a href="https://github.com/Fare9/Genaytyk-VM/blob/master/vmkgme__.idb">vmkgme__.idb</a>

+ Crackme with mew packer: <a href="https://github.com/Fare9/Genaytyk-VM/blob/master/vmkgme.exe">vmkgme__.exe</a>

+ Crackme .idb (with mew) from IDA 6.8: <a href="https://github.com/Fare9/Genaytyk-VM/blob/master/vmkgme.idb">vmkgme__.idb</a>

+ Some functions from Crackme in C, and the keygen from Andrewl resolution: <a href="https://github.com/Fare9/Genaytyk-VM/blob/master/GenaytykVM.cpp">GenaytykVM.cpp</a>


As you can see, I was not able to resolve the encrypt function =( I hope to continue learning how this function works and continue working with VMs.