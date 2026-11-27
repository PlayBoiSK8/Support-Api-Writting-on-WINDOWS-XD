# Memory Map Scan RWX access ( like x64dbg )

<p align="center">
&#x20; <img src="./pic1.png" alt="pic1.png">
</p>

<p align="center">
&#x20; <img src="./pic2.png" alt="pic2.png">
</p>

Just a stupid homework :DD , in this shit , i used RAII handle + api VirtualQueryExW for scan remote process. Start address from 0x0000000000010000 to 0x00007FFFFFFEFFFF. Becau that space is safe , or if u wanna scan from 0x00..00 , just edit in my code at LPBYTE baseAddress.

<p align="center">
&#x20; <img src="./pic3.png" alt="pic3.png">
</p>

As u can see the problem , i will find solution in future XD ! 