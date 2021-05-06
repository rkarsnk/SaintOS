```
(qemu) info registers
RAX=000000003fb7b3e0 RBX=0000000000000000 RCX=000000003fb7b3e0 RDX=000000003fea03f8
RSI=0000000000000000 RDI=00000000001010a2 RBP=0000000000000000 RSP=000000003fea83d0
R8 =0000000000000001 R9 =0000000000000000 R10=0000000000000000 R11=0000000000000000
R12=000000003f308198 R13=00000000000004d0 R14=000000003fb684f4 R15=000000000000000e
RIP=000000003fb73016 RFL=00000046 [---Z-P-] CPL=0 II=0 A20=1 SMM=0 HLT=0
ES =0030 0000000000000000 ffffffff 00cf9300 DPL=0 DS   [-WA]
CS =0038 0000000000000000 ffffffff 00af9a00 DPL=0 CS64 [-R-]
SS =0030 0000000000000000 ffffffff 00cf9300 DPL=0 DS   [-WA]
DS =0030 0000000000000000 ffffffff 00cf9300 DPL=0 DS   [-WA]
FS =0030 0000000000000000 ffffffff 00cf9300 DPL=0 DS   [-WA]
GS =0030 0000000000000000 ffffffff 00cf9300 DPL=0 DS   [-WA]
LDT=0000 0000000000000000 0000ffff 00008200 DPL=0 LDT
TR =0000 0000000000000000 0000ffff 00008b00 DPL=0 TSS64-busy
GDT=     000000003fbee698 00000047
IDT=     000000003f306018 00000fff
CR0=80010033 CR2=fffffffe8044a000 CR3=000000003fc01000 CR4=00000668
DR0=0000000000000000 DR1=0000000000000000 DR2=0000000000000000 DR3=0000000000000000 
DR6=00000000ffff0ff0 DR7=0000000000000400
EFER=0000000000000500
FCW=037f FSW=0000 [ST=0] FTW=00 MXCSR=00001f80
FPR0=0000000000000000 0000 FPR1=0000000000000000 0000
FPR2=0000000000000000 0000 FPR3=0000000000000000 0000
FPR4=0000000000000000 0000 FPR5=0000000000000000 0000
FPR6=0000000000000000 0000 FPR7=0000000000000000 0000
XMM00=00000000000000000000000000000000 XMM01=00000000000000000000000000000000
XMM02=00000000000000000000000000000000 XMM03=00000000000000000000000000000000
XMM04=00000000000000000000000000000000 XMM05=00000000000000000000000000000000
XMM06=00000000000000000000000000000000 XMM07=00000000000000000000000000000000
XMM08=00000000000000000000000000000000 XMM09=00000000000000000000000000000000
XMM10=00000000000000000000000000000000 XMM11=00000000000000000000000000000000
XMM12=00000000000000000000000000000000 XMM13=00000000000000000000000000000000
XMM14=00000000000000000000000000000000 XMM15=00000000000000000000000000000000
```
```
0x3fb73016:  48 83 7c 24 40 00        cmpq     $0, 0x40(%rsp)
0x3fb7301c:  74 f8                    je       0x3fb73016
```

```
StOS/kernel/kernel.elf:     ファイル形式 elf64-x86-64


セクション .text の逆アセンブル:

0000000000101000 <_Z10WritePixelRK17FrameBufferConfigiiRK10PixelColor>:
  101000:	55                   	push   %rbp
  101001:	48 89 e5             	mov    %rsp,%rbp
  101004:	0f af f2             	imul   %edx,%esi
  101007:	0f af 77 08          	imul   0x8(%rdi),%esi
  10100b:	8b 57 14             	mov    0x14(%rdi),%edx
  10100e:	83 fa 01             	cmp    $0x1,%edx
  101011:	74 25                	je     101038 <_Z10WritePixelRK17FrameBufferConfigiiRK10PixelColor+0x38>
  101013:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
  101018:	85 d2                	test   %edx,%edx
  10101a:	75 3b                	jne    101057 <_Z10WritePixelRK17FrameBufferConfigiiRK10PixelColor+0x57>
  10101c:	48 8b 17             	mov    (%rdi),%rdx
  10101f:	48 63 f6             	movslq %esi,%rsi
  101022:	4c 8d 04 b2          	lea    (%rdx,%rsi,4),%r8
  101026:	8a 01                	mov    (%rcx),%al
  101028:	88 04 b2             	mov    %al,(%rdx,%rsi,4)
  10102b:	8a 41 01             	mov    0x1(%rcx),%al
  10102e:	88 44 b2 01          	mov    %al,0x1(%rdx,%rsi,4)
  101032:	48 83 c1 02          	add    $0x2,%rcx
  101036:	eb 17                	jmp    10104f <_Z10WritePixelRK17FrameBufferConfigiiRK10PixelColor+0x4f>
  101038:	48 8b 3f             	mov    (%rdi),%rdi
  10103b:	48 63 d6             	movslq %esi,%rdx
  10103e:	4c 8d 04 97          	lea    (%rdi,%rdx,4),%r8
  101042:	8a 41 02             	mov    0x2(%rcx),%al
  101045:	88 04 97             	mov    %al,(%rdi,%rdx,4)
  101048:	8a 41 01             	mov    0x1(%rcx),%al
  10104b:	88 44 97 01          	mov    %al,0x1(%rdi,%rdx,4)
  10104f:	8a 01                	mov    (%rcx),%al
  101051:	41 88 40 02          	mov    %al,0x2(%r8)
  101055:	31 c0                	xor    %eax,%eax
  101057:	5d                   	pop    %rbp
  101058:	c3                   	retq   
  101059:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000101060 <KernelMain>:
  101060:	55                   	push   %rbp
  101061:	48 89 e5             	mov    %rsp,%rbp
  101064:	83 7f 0c 00          	cmpl   $0x0,0xc(%rdi)
  101068:	74 5c                	je     1010c6 <KernelMain+0x66>
  10106a:	4c 8d 57 14          	lea    0x14(%rdi),%r10
  10106e:	8b 4f 10             	mov    0x10(%rdi),%ecx
  101071:	45 31 c0             	xor    %r8d,%r8d
  101074:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
  10107b:	00 00 00 
  10107e:	66 90                	xchg   %ax,%ax
  101080:	85 c9                	test   %ecx,%ecx
  101082:	b9 00 00 00 00       	mov    $0x0,%ecx
  101087:	74 31                	je     1010ba <KernelMain+0x5a>
  101089:	31 d2                	xor    %edx,%edx
  10108b:	31 f6                	xor    %esi,%esi
  10108d:	0f 1f 00             	nopl   (%rax)
  101090:	41 83 3a 01          	cmpl   $0x1,(%r10)
  101094:	77 17                	ja     1010ad <KernelMain+0x4d>
  101096:	8b 4f 08             	mov    0x8(%rdi),%ecx
  101099:	0f af ca             	imul   %edx,%ecx
  10109c:	48 8b 07             	mov    (%rdi),%rax
  10109f:	48 63 c9             	movslq %ecx,%rcx
  1010a2:	66 c7 04 88 ff ff    	movw   $0xffff,(%rax,%rcx,4)
  1010a8:	c6 44 88 02 ff       	movb   $0xff,0x2(%rax,%rcx,4)
  1010ad:	83 c6 01             	add    $0x1,%esi
  1010b0:	8b 4f 10             	mov    0x10(%rdi),%ecx
  1010b3:	44 01 c2             	add    %r8d,%edx
  1010b6:	39 ce                	cmp    %ecx,%esi
  1010b8:	72 d6                	jb     101090 <KernelMain+0x30>
  1010ba:	41 83 c0 01          	add    $0x1,%r8d
  1010be:	44 3b 47 0c          	cmp    0xc(%rdi),%r8d
  1010c2:	72 bc                	jb     101080 <KernelMain+0x20>
  1010c4:	eb 04                	jmp    1010ca <KernelMain+0x6a>
  1010c6:	4c 8d 57 14          	lea    0x14(%rdi),%r10
  1010ca:	45 31 c0             	xor    %r8d,%r8d
  1010cd:	41 b9 10 27 00 00    	mov    $0x2710,%r9d
  1010d3:	41 bb 64 00 00 00    	mov    $0x64,%r11d
  1010d9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
  1010e0:	44 89 ca             	mov    %r9d,%edx
  1010e3:	b9 64 00 00 00       	mov    $0x64,%ecx
  1010e8:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
  1010ef:	00 
  1010f0:	41 83 3a 01          	cmpl   $0x1,(%r10)
  1010f4:	77 16                	ja     10110c <KernelMain+0xac>
  1010f6:	8b 47 08             	mov    0x8(%rdi),%eax
  1010f9:	0f af c2             	imul   %edx,%eax
  1010fc:	48 8b 37             	mov    (%rdi),%rsi
  1010ff:	48 98                	cltq   
  101101:	66 c7 04 86 00 ff    	movw   $0xff00,(%rsi,%rax,4)
  101107:	c6 44 86 02 00       	movb   $0x0,0x2(%rsi,%rax,4)
  10110c:	44 01 da             	add    %r11d,%edx
  10110f:	83 c1 ff             	add    $0xffffffff,%ecx
  101112:	75 dc                	jne    1010f0 <KernelMain+0x90>
  101114:	41 83 c0 01          	add    $0x1,%r8d
  101118:	41 83 c1 64          	add    $0x64,%r9d
  10111c:	41 83 c3 01          	add    $0x1,%r11d
  101120:	41 81 f8 c8 00 00 00 	cmp    $0xc8,%r8d
  101127:	75 b7                	jne    1010e0 <KernelMain+0x80>
  101129:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
  101130:	f4                   	hlt    
  101131:	eb fd                	jmp    101130 <KernelMain+0xd0>
```
```
edk2/Build/StOSLoaderX64/DEBUG_CLANG38/X64/Loader.efi:     ファイル形式 pei-x86-64


セクション .text の逆アセンブル:

0000000000000240 <.text>:
     240:	48 8b 57 08          	mov    0x8(%rdi),%rdx
     244:	48 85 d2             	test   %rdx,%rdx
     247:	74 2f                	je     0x278
     249:	48 83 ec 28          	sub    $0x28,%rsp
     24d:	48 8b 07             	mov    (%rdi),%rax
     250:	48 89 47 10          	mov    %rax,0x10(%rdi)
     254:	48 8b 05 5d 24 00 00 	mov    0x245d(%rip),%rax        # 0x26b8
     25b:	48 8d 4f 10          	lea    0x10(%rdi),%rcx
     25f:	4c 8d 47 18          	lea    0x18(%rdi),%r8
     263:	4c 8d 4f 20          	lea    0x20(%rdi),%r9
     267:	48 8d 77 28          	lea    0x28(%rdi),%rsi
     26b:	48 89 74 24 20       	mov    %rsi,0x20(%rsp)
     270:	ff 50 38             	callq  *0x38(%rax)
     273:	48 83 c4 28          	add    $0x28,%rsp
     277:	c3                   	retq   
     278:	48 b8 05 00 00 00 00 	movabs $0x8000000000000005,%rax
     27f:	00 00 80 
     282:	c3                   	retq   
     283:	55                   	push   %rbp
     284:	48 89 e5             	mov    %rsp,%rbp
     287:	41 57                	push   %r15
     289:	41 56                	push   %r14
     28b:	41 55                	push   %r13
     28d:	41 54                	push   %r12
     28f:	56                   	push   %rsi
     290:	57                   	push   %rdi
     291:	53                   	push   %rbx
     292:	48 81 ec 88 41 00 00 	sub    $0x4188,%rsp
     299:	49 89 ce             	mov    %rcx,%r14
     29c:	48 8b 05 1d 24 00 00 	mov    0x241d(%rip),%rax        # 0x26c0
     2a3:	48 83 ec 20          	sub    $0x20,%rsp
     2a7:	48 8d b5 48 ff ff ff 	lea    -0xb8(%rbp),%rsi
     2ae:	31 d2                	xor    %edx,%edx
     2b0:	48 89 f1             	mov    %rsi,%rcx
     2b3:	ff 50 18             	callq  *0x18(%rax)
     2b6:	48 83 c4 20          	add    $0x20,%rsp
     2ba:	0f b7 16             	movzwl (%rsi),%edx
     2bd:	44 0f b6 46 02       	movzbl 0x2(%rsi),%r8d
     2c2:	44 0f b6 4e 03       	movzbl 0x3(%rsi),%r9d
     2c7:	48 83 ec 20          	sub    $0x20,%rsp
     2cb:	48 8d 0d 16 1b 00 00 	lea    0x1b16(%rip),%rcx        # 0x1de8
     2d2:	e8 4c 15 00 00       	callq  0x1823
     2d7:	48 83 c4 20          	add    $0x20,%rsp
     2db:	48 8d bd 60 ff ff ff 	lea    -0xa0(%rbp),%rdi
     2e2:	48 c7 07 00 40 00 00 	movq   $0x4000,(%rdi)
     2e9:	48 8d 85 40 be ff ff 	lea    -0x41c0(%rbp),%rax
     2f0:	48 89 47 08          	mov    %rax,0x8(%rdi)
     2f4:	83 67 28 00          	andl   $0x0,0x28(%rdi)
     2f8:	48 83 67 20 00       	andq   $0x0,0x20(%rdi)
     2fd:	48 83 67 18 00       	andq   $0x0,0x18(%rdi)
     302:	48 83 67 10 00       	andq   $0x0,0x10(%rdi)
     307:	e8 34 ff ff ff       	callq  0x240
     30c:	48 85 c0             	test   %rax,%rax
     30f:	0f 88 c4 01 00 00    	js     0x4d9
     315:	48 8b 05 9c 23 00 00 	mov    0x239c(%rip),%rax        # 0x26b8
     31c:	48 83 ec 30          	sub    $0x30,%rsp
     320:	48 83 64 24 20 00    	andq   $0x0,0x20(%rsp)
     326:	6a 01                	pushq  $0x1
     328:	5f                   	pop    %rdi
     329:	89 7c 24 28          	mov    %edi,0x28(%rsp)
     32d:	48 8d 15 44 23 00 00 	lea    0x2344(%rip),%rdx        # 0x2678
     334:	48 8d b5 40 fe ff ff 	lea    -0x1c0(%rbp),%rsi
     33b:	4c 89 f1             	mov    %r14,%rcx
     33e:	49 89 f0             	mov    %rsi,%r8
     341:	4d 89 f1             	mov    %r14,%r9
     344:	ff 90 18 01 00 00    	callq  *0x118(%rax)
     34a:	48 83 c4 30          	add    $0x30,%rsp
     34e:	48 8b 05 63 23 00 00 	mov    0x2363(%rip),%rax        # 0x26b8
     355:	48 8b 0e             	mov    (%rsi),%rcx
     358:	48 8b 49 18          	mov    0x18(%rcx),%rcx
     35c:	48 83 ec 30          	sub    $0x30,%rsp
     360:	48 83 64 24 20 00    	andq   $0x0,0x20(%rsp)
     366:	89 7c 24 28          	mov    %edi,0x28(%rsp)
     36a:	48 8d 15 17 23 00 00 	lea    0x2317(%rip),%rdx        # 0x2688
     371:	48 8d 7d c0          	lea    -0x40(%rbp),%rdi
     375:	49 89 f8             	mov    %rdi,%r8
     378:	4d 89 f1             	mov    %r14,%r9
     37b:	ff 90 18 01 00 00    	callq  *0x118(%rax)
     381:	48 83 c4 30          	add    $0x30,%rsp
     385:	48 8b 0f             	mov    (%rdi),%rcx
     388:	48 83 ec 20          	sub    $0x20,%rsp
     38c:	48 8d 75 90          	lea    -0x70(%rbp),%rsi
     390:	48 89 f2             	mov    %rsi,%rdx
     393:	ff 51 08             	callq  *0x8(%rcx)
     396:	48 83 c4 20          	add    $0x20,%rsp
     39a:	48 8b 0e             	mov    (%rsi),%rcx
     39d:	48 83 ec 30          	sub    $0x30,%rsp
     3a1:	48 83 64 24 20 00    	andq   $0x0,0x20(%rsp)
     3a7:	4c 8d 05 ca 1a 00 00 	lea    0x1aca(%rip),%r8        # 0x1e78
     3ae:	48 8d 55 a8          	lea    -0x58(%rbp),%rdx
     3b2:	49 b9 03 00 00 00 00 	movabs $0x8000000000000003,%r9
     3b9:	00 00 80 
     3bc:	ff 51 08             	callq  *0x8(%rcx)
     3bf:	48 83 c4 30          	add    $0x30,%rsp
     3c3:	48 85 c0             	test   %rax,%rax
     3c6:	0f 88 27 01 00 00    	js     0x4f3
     3cc:	4c 89 75 a0          	mov    %r14,-0x60(%rbp)
     3d0:	4c 8b 65 a8          	mov    -0x58(%rbp),%r12
     3d4:	48 8d 55 c0          	lea    -0x40(%rbp),%rdx
     3d8:	48 c7 02 41 00 00 00 	movq   $0x41,(%rdx)
     3df:	48 83 ec 20          	sub    $0x20,%rsp
     3e3:	4c 8d 05 7a 1d 00 00 	lea    0x1d7a(%rip),%r8        # 0x2164
     3ea:	4c 89 e1             	mov    %r12,%rcx
     3ed:	41 ff 54 24 28       	callq  *0x28(%r12)
     3f2:	48 83 c4 20          	add    $0x20,%rsp
     3f6:	48 8b 95 68 ff ff ff 	mov    -0x98(%rbp),%rdx
     3fd:	4c 8b 85 70 ff ff ff 	mov    -0x90(%rbp),%r8
     404:	48 83 ec 20          	sub    $0x20,%rsp
     408:	48 8d 0d 63 18 00 00 	lea    0x1863(%rip),%rcx        # 0x1c72
     40f:	e8 0f 14 00 00       	callq  0x1823
     414:	48 83 c4 20          	add    $0x20,%rsp
     418:	31 db                	xor    %ebx,%ebx
     41a:	48 8b 85 68 ff ff ff 	mov    -0x98(%rbp),%rax
     421:	4c 8d 2d c0 15 00 00 	lea    0x15c0(%rip),%r13        # 0x19e8
     428:	4c 8d b5 40 fe ff ff 	lea    -0x1c0(%rbp),%r14
     42f:	49 89 c7             	mov    %rax,%r15
     432:	eb 7c                	jmp    0x4b0
     434:	49 63 07             	movslq (%r15),%rax
     437:	4c 89 e9             	mov    %r13,%rcx
     43a:	48 83 f8 0f          	cmp    $0xf,%rax
     43e:	77 0b                	ja     0x44b
     440:	48 8d 0d 79 21 00 00 	lea    0x2179(%rip),%rcx        # 0x25c0
     447:	48 8b 0c c1          	mov    (%rcx,%rax,8),%rcx
     44b:	49 8b 57 08          	mov    0x8(%r15),%rdx
     44f:	49 8b 77 18          	mov    0x18(%r15),%rsi
     453:	49 8b 7f 20          	mov    0x20(%r15),%rdi
     457:	81 e7 ff ff 0f 00    	and    $0xfffff,%edi
     45d:	48 83 ec 50          	sub    $0x50,%rsp
     461:	48 89 74 24 38       	mov    %rsi,0x38(%rsp)
     466:	48 89 54 24 30       	mov    %rdx,0x30(%rsp)
     46b:	48 89 4c 24 28       	mov    %rcx,0x28(%rsp)
     470:	89 44 24 20          	mov    %eax,0x20(%rsp)
     474:	48 89 7c 24 40       	mov    %rdi,0x40(%rsp)
     479:	4c 89 f1             	mov    %r14,%rcx
     47c:	41 89 d9             	mov    %ebx,%r9d
     47f:	e8 70 13 00 00       	callq  0x17f4
     484:	48 83 c4 50          	add    $0x50,%rsp
     488:	48 89 45 c0          	mov    %rax,-0x40(%rbp)
     48c:	48 83 ec 20          	sub    $0x20,%rsp
     490:	4c 89 e1             	mov    %r12,%rcx
     493:	48 8d 55 c0          	lea    -0x40(%rbp),%rdx
     497:	4d 89 f0             	mov    %r14,%r8
     49a:	41 ff 54 24 28       	callq  *0x28(%r12)
     49f:	48 83 c4 20          	add    $0x20,%rsp
     4a3:	4c 03 7d 80          	add    -0x80(%rbp),%r15
     4a7:	ff c3                	inc    %ebx
     4a9:	48 8b 85 68 ff ff ff 	mov    -0x98(%rbp),%rax
     4b0:	48 03 85 70 ff ff ff 	add    -0x90(%rbp),%rax
     4b7:	49 39 c7             	cmp    %rax,%r15
     4ba:	0f 82 74 ff ff ff    	jb     0x434
     4c0:	48 8b 4d a8          	mov    -0x58(%rbp),%rcx
     4c4:	48 83 ec 20          	sub    $0x20,%rsp
     4c8:	ff 51 10             	callq  *0x10(%rcx)
     4cb:	48 83 c4 20          	add    $0x20,%rsp
     4cf:	4c 8b 75 a0          	mov    -0x60(%rbp),%r14
     4d3:	48 8d 7d c0          	lea    -0x40(%rbp),%rdi
     4d7:	eb 3d                	jmp    0x516
     4d9:	48 83 ec 20          	sub    $0x20,%rsp
     4dd:	48 8d 0d 58 19 00 00 	lea    0x1958(%rip),%rcx        # 0x1e3c
     4e4:	48 89 c2             	mov    %rax,%rdx
     4e7:	e8 37 13 00 00       	callq  0x1823
     4ec:	48 83 c4 20          	add    $0x20,%rsp
     4f0:	f4                   	hlt    
     4f1:	eb fd                	jmp    0x4f0
     4f3:	48 83 ec 20          	sub    $0x20,%rsp
     4f7:	48 8d 0d 8a 19 00 00 	lea    0x198a(%rip),%rcx        # 0x1e88
     4fe:	48 89 c2             	mov    %rax,%rdx
     501:	e8 1d 13 00 00       	callq  0x1823
     506:	48 8d 0d c1 19 00 00 	lea    0x19c1(%rip),%rcx        # 0x1ece
     50d:	e8 11 13 00 00       	callq  0x1823
     512:	48 83 c4 20          	add    $0x20,%rsp
     516:	4c 8d 8d 40 fe ff ff 	lea    -0x1c0(%rbp),%r9
     51d:	49 83 21 00          	andq   $0x0,(%r9)
     521:	48 83 65 c0 00       	andq   $0x0,-0x40(%rbp)
     526:	48 8b 05 8b 21 00 00 	mov    0x218b(%rip),%rax        # 0x26b8
     52d:	48 83 ec 30          	sub    $0x30,%rsp
     531:	48 89 7c 24 20       	mov    %rdi,0x20(%rsp)
     536:	48 8d 35 5b 21 00 00 	lea    0x215b(%rip),%rsi        # 0x2698
     53d:	6a 02                	pushq  $0x2
     53f:	59                   	pop    %rcx
     540:	45 31 c0             	xor    %r8d,%r8d
     543:	48 89 f2             	mov    %rsi,%rdx
     546:	ff 90 38 01 00 00    	callq  *0x138(%rax)
     54c:	48 83 c4 30          	add    $0x30,%rsp
     550:	48 8b 05 61 21 00 00 	mov    0x2161(%rip),%rax        # 0x26b8
     557:	48 8b 4d c0          	mov    -0x40(%rbp),%rcx
     55b:	48 8b 09             	mov    (%rcx),%rcx
     55e:	48 83 ec 30          	sub    $0x30,%rsp
     562:	c7 44 24 28 01 00 00 	movl   $0x1,0x28(%rsp)
     569:	00 
     56a:	48 83 64 24 20 00    	andq   $0x0,0x20(%rsp)
     570:	48 8d 7d b8          	lea    -0x48(%rbp),%rdi
     574:	48 89 f2             	mov    %rsi,%rdx
     577:	49 89 f8             	mov    %rdi,%r8
     57a:	4d 89 f1             	mov    %r14,%r9
     57d:	ff 90 18 01 00 00    	callq  *0x118(%rax)
     583:	48 83 c4 30          	add    $0x30,%rsp
     587:	48 8b 4d c0          	mov    -0x40(%rbp),%rcx
     58b:	48 8b 05 26 21 00 00 	mov    0x2126(%rip),%rax        # 0x26b8
     592:	48 83 ec 20          	sub    $0x20,%rsp
     596:	ff 50 48             	callq  *0x48(%rax)
     599:	48 83 c4 20          	add    $0x20,%rsp
     59d:	48 8b 07             	mov    (%rdi),%rax
     5a0:	48 8b 40 18          	mov    0x18(%rax),%rax
     5a4:	48 8b 40 08          	mov    0x8(%rax),%rax
     5a8:	8b 50 04             	mov    0x4(%rax),%edx
     5ab:	44 8b 40 08          	mov    0x8(%rax),%r8d
     5af:	48 63 48 0c          	movslq 0xc(%rax),%rcx
     5b3:	6a 04                	pushq  $0x4
     5b5:	5b                   	pop    %rbx
     5b6:	48 39 d9             	cmp    %rbx,%rcx
     5b9:	77 0d                	ja     0x5c8
     5bb:	48 8d 1d 7e 20 00 00 	lea    0x207e(%rip),%rbx        # 0x2640
     5c2:	4c 8b 0c cb          	mov    (%rbx,%rcx,8),%r9
     5c6:	eb 07                	jmp    0x5cf
     5c8:	4c 8d 0d 09 17 00 00 	lea    0x1709(%rip),%r9        # 0x1cd8
     5cf:	8b 40 20             	mov    0x20(%rax),%eax
     5d2:	48 83 ec 30          	sub    $0x30,%rsp
     5d6:	89 44 24 20          	mov    %eax,0x20(%rsp)
     5da:	48 8d 0d 01 19 00 00 	lea    0x1901(%rip),%rcx        # 0x1ee2
     5e1:	e8 3d 12 00 00       	callq  0x1823
     5e6:	48 83 c4 30          	add    $0x30,%rsp
     5ea:	48 8b 45 b8          	mov    -0x48(%rbp),%rax
     5ee:	48 8b 40 18          	mov    0x18(%rax),%rax
     5f2:	48 8b 50 18          	mov    0x18(%rax),%rdx
     5f6:	4c 8b 48 20          	mov    0x20(%rax),%r9
     5fa:	4d 8d 04 11          	lea    (%r9,%rdx,1),%r8
     5fe:	48 83 ec 20          	sub    $0x20,%rsp
     602:	48 8d 0d 43 19 00 00 	lea    0x1943(%rip),%rcx        # 0x1f4c
     609:	e8 15 12 00 00       	callq  0x1823
     60e:	48 83 c4 20          	add    $0x20,%rsp
     612:	48 8b 45 b8          	mov    -0x48(%rbp),%rax
     616:	48 8b 50 18          	mov    0x18(%rax),%rdx
     61a:	48 8b 42 18          	mov    0x18(%rdx),%rax
     61e:	31 c9                	xor    %ecx,%ecx
     620:	eb 0f                	jmp    0x631
     622:	c6 04 08 ff          	movb   $0xff,(%rax,%rcx,1)
     626:	48 ff c1             	inc    %rcx
     629:	48 8b 55 b8          	mov    -0x48(%rbp),%rdx
     62d:	48 8b 52 18          	mov    0x18(%rdx),%rdx
     631:	48 3b 4a 20          	cmp    0x20(%rdx),%rcx
     635:	72 eb                	jb     0x622
     637:	48 8b 4d 90          	mov    -0x70(%rbp),%rcx
     63b:	48 83 ec 30          	sub    $0x30,%rsp
     63f:	48 83 64 24 20 00    	andq   $0x0,0x20(%rsp)
     645:	4c 8d 05 60 19 00 00 	lea    0x1960(%rip),%r8        # 0x1fac
     64c:	48 8d 55 c0          	lea    -0x40(%rbp),%rdx
     650:	6a 01                	pushq  $0x1
     652:	41 59                	pop    %r9
     654:	ff 51 08             	callq  *0x8(%rcx)
     657:	48 83 c4 30          	add    $0x30,%rsp
     65b:	48 85 c0             	test   %rax,%rax
     65e:	0f 88 3e 01 00 00    	js     0x7a2
     664:	4c 8d 85 58 ff ff ff 	lea    -0xa8(%rbp),%r8
     66b:	49 c7 00 70 00 00 00 	movq   $0x70,(%r8)
     672:	48 89 e6             	mov    %rsp,%rsi
     675:	48 83 c6 90          	add    $0xffffffffffffff90,%rsi
     679:	48 89 f4             	mov    %rsi,%rsp
     67c:	48 8b 4d c0          	mov    -0x40(%rbp),%rcx
     680:	48 83 ec 20          	sub    $0x20,%rsp
     684:	48 8d 15 dd 1f 00 00 	lea    0x1fdd(%rip),%rdx        # 0x2668
     68b:	49 89 f1             	mov    %rsi,%r9
     68e:	ff 51 40             	callq  *0x40(%rcx)
     691:	48 83 c4 20          	add    $0x20,%rsp
     695:	48 85 c0             	test   %rax,%rax
     698:	0f 88 1e 01 00 00    	js     0x7bc
     69e:	4c 8b 46 08          	mov    0x8(%rsi),%r8
     6a2:	4c 89 45 98          	mov    %r8,-0x68(%rbp)
     6a6:	4c 8d 4d b0          	lea    -0x50(%rbp),%r9
     6aa:	49 c7 01 00 00 10 00 	movq   $0x100000,(%r9)
     6b1:	48 8b 05 00 20 00 00 	mov    0x2000(%rip),%rax        # 0x26b8
     6b8:	49 81 c0 ff 0f 00 00 	add    $0xfff,%r8
     6bf:	49 c1 e8 0c          	shr    $0xc,%r8
     6c3:	48 83 ec 20          	sub    $0x20,%rsp
     6c7:	6a 02                	pushq  $0x2
     6c9:	59                   	pop    %rcx
     6ca:	89 ca                	mov    %ecx,%edx
     6cc:	ff 50 28             	callq  *0x28(%rax)
     6cf:	48 83 c4 20          	add    $0x20,%rsp
     6d3:	48 85 c0             	test   %rax,%rax
     6d6:	0f 88 fa 00 00 00    	js     0x7d6
     6dc:	48 8b 4d c0          	mov    -0x40(%rbp),%rcx
     6e0:	4c 8b 45 b0          	mov    -0x50(%rbp),%r8
     6e4:	48 83 ec 20          	sub    $0x20,%rsp
     6e8:	48 8d 75 98          	lea    -0x68(%rbp),%rsi
     6ec:	48 89 f2             	mov    %rsi,%rdx
     6ef:	ff 51 20             	callq  *0x20(%rcx)
     6f2:	48 83 c4 20          	add    $0x20,%rsp
     6f6:	48 8b 55 b0          	mov    -0x50(%rbp),%rdx
     6fa:	4c 8b 06             	mov    (%rsi),%r8
     6fd:	48 83 ec 20          	sub    $0x20,%rsp
     701:	48 8d 0d 8e 19 00 00 	lea    0x198e(%rip),%rcx        # 0x2096
     708:	e8 16 11 00 00       	callq  0x1823
     70d:	48 83 c4 20          	add    $0x20,%rsp
     711:	48 8b 05 a0 1f 00 00 	mov    0x1fa0(%rip),%rax        # 0x26b8
     718:	48 8b 95 78 ff ff ff 	mov    -0x88(%rbp),%rdx
     71f:	48 83 ec 20          	sub    $0x20,%rsp
     723:	4c 89 f1             	mov    %r14,%rcx
     726:	ff 90 e8 00 00 00    	callq  *0xe8(%rax)
     72c:	48 83 c4 20          	add    $0x20,%rsp
     730:	48 85 c0             	test   %rax,%rax
     733:	0f 88 b7 00 00 00    	js     0x7f0
     739:	48 8b 45 b0          	mov    -0x50(%rbp),%rax
     73d:	48 8b 40 18          	mov    0x18(%rax),%rax
     741:	48 8b 4d b8          	mov    -0x48(%rbp),%rcx
     745:	48 8b 49 18          	mov    0x18(%rcx),%rcx
     749:	48 8b 51 18          	mov    0x18(%rcx),%rdx
     74d:	48 89 95 40 fe ff ff 	mov    %rdx,-0x1c0(%rbp)
     754:	48 8b 51 08          	mov    0x8(%rcx),%rdx
     758:	8b 52 20             	mov    0x20(%rdx),%edx
     75b:	89 95 48 fe ff ff    	mov    %edx,-0x1b8(%rbp)
     761:	48 8b 51 08          	mov    0x8(%rcx),%rdx
     765:	8b 52 04             	mov    0x4(%rdx),%edx
     768:	89 95 4c fe ff ff    	mov    %edx,-0x1b4(%rbp)
     76e:	48 8b 51 08          	mov    0x8(%rcx),%rdx
     772:	8b 52 08             	mov    0x8(%rdx),%edx
     775:	89 95 50 fe ff ff    	mov    %edx,-0x1b0(%rbp)
     77b:	83 a5 54 fe ff ff 00 	andl   $0x0,-0x1ac(%rbp)
     782:	48 8b 49 08          	mov    0x8(%rcx),%rcx
     786:	8b 51 0c             	mov    0xc(%rcx),%edx
     789:	85 d2                	test   %edx,%edx
     78b:	0f 84 b2 00 00 00    	je     0x843
     791:	83 fa 01             	cmp    $0x1,%edx
     794:	0f 85 cd 00 00 00    	jne    0x867
     79a:	6a 01                	pushq  $0x1
     79c:	59                   	pop    %rcx
     79d:	e9 a3 00 00 00       	jmpq   0x845
     7a2:	48 83 ec 20          	sub    $0x20,%rsp
     7a6:	48 8d 0d 17 18 00 00 	lea    0x1817(%rip),%rcx        # 0x1fc4
     7ad:	48 89 c2             	mov    %rax,%rdx
     7b0:	e8 6e 10 00 00       	callq  0x1823
     7b5:	48 83 c4 20          	add    $0x20,%rsp
     7b9:	f4                   	hlt    
     7ba:	eb fd                	jmp    0x7b9
     7bc:	48 83 ec 20          	sub    $0x20,%rsp
     7c0:	48 8d 0d 4b 18 00 00 	lea    0x184b(%rip),%rcx        # 0x2012
     7c7:	48 89 c2             	mov    %rax,%rdx
     7ca:	e8 54 10 00 00       	callq  0x1823
     7cf:	48 83 c4 20          	add    $0x20,%rsp
     7d3:	f4                   	hlt    
     7d4:	eb fd                	jmp    0x7d3
     7d6:	48 83 ec 20          	sub    $0x20,%rsp
     7da:	48 8d 0d 79 18 00 00 	lea    0x1879(%rip),%rcx        # 0x205a
     7e1:	48 89 c2             	mov    %rax,%rdx
     7e4:	e8 3a 10 00 00       	callq  0x1823
     7e9:	48 83 c4 20          	add    $0x20,%rsp
     7ed:	f4                   	hlt    
     7ee:	eb fd                	jmp    0x7ed
     7f0:	48 8d bd 60 ff ff ff 	lea    -0xa0(%rbp),%rdi
     7f7:	e8 44 fa ff ff       	callq  0x240
     7fc:	48 85 c0             	test   %rax,%rax
     7ff:	78 7d                	js     0x87e
     801:	48 8b 05 b0 1e 00 00 	mov    0x1eb0(%rip),%rax        # 0x26b8
     808:	48 8b 95 78 ff ff ff 	mov    -0x88(%rbp),%rdx
     80f:	48 83 ec 20          	sub    $0x20,%rsp
     813:	4c 89 f1             	mov    %r14,%rcx
     816:	ff 90 e8 00 00 00    	callq  *0xe8(%rax)
     81c:	48 83 c4 20          	add    $0x20,%rsp
     820:	48 85 c0             	test   %rax,%rax
     823:	0f 89 10 ff ff ff    	jns    0x739
     829:	48 83 ec 20          	sub    $0x20,%rsp
     82d:	48 8d 0d 9a 18 00 00 	lea    0x189a(%rip),%rcx        # 0x20ce
     834:	48 89 c2             	mov    %rax,%rdx
     837:	e8 e7 0f 00 00       	callq  0x1823
     83c:	48 83 c4 20          	add    $0x20,%rsp
     840:	f4                   	hlt    
     841:	eb fd                	jmp    0x840
     843:	31 c9                	xor    %ecx,%ecx
     845:	48 8d bd 40 fe ff ff 	lea    -0x1c0(%rbp),%rdi
     84c:	89 4f 14             	mov    %ecx,0x14(%rdi)
     84f:	ff d0                	callq  *%rax
     851:	48 83 ec 20          	sub    $0x20,%rsp
     855:	48 8d 0d f4 18 00 00 	lea    0x18f4(%rip),%rcx        # 0x2150
     85c:	e8 c2 0f 00 00       	callq  0x1823
     861:	48 83 c4 20          	add    $0x20,%rsp
     865:	eb fe                	jmp    0x865
     867:	48 83 ec 20          	sub    $0x20,%rsp
     86b:	48 8d 0d 9e 18 00 00 	lea    0x189e(%rip),%rcx        # 0x2110
     872:	e8 ac 0f 00 00       	callq  0x1823
     877:	48 83 c4 20          	add    $0x20,%rsp
     87b:	f4                   	hlt    
     87c:	eb fd                	jmp    0x87b
     87e:	48 83 ec 20          	sub    $0x20,%rsp
     882:	48 8d 0d b3 15 00 00 	lea    0x15b3(%rip),%rcx        # 0x1e3c
     889:	48 89 c2             	mov    %rax,%rdx
     88c:	e8 92 0f 00 00       	callq  0x1823
     891:	48 83 c4 20          	add    $0x20,%rsp
     895:	f4                   	hlt    
     896:	eb fd                	jmp    0x895
     898:	55                   	push   %rbp
     899:	41 57                	push   %r15
     89b:	41 56                	push   %r14
     89d:	41 55                	push   %r13
     89f:	41 54                	push   %r12
     8a1:	53                   	push   %rbx
     8a2:	48 81 ec 18 01 00 00 	sub    $0x118,%rsp
     8a9:	49 89 d4             	mov    %rdx,%r12
     8ac:	44 89 e0             	mov    %r12d,%eax
     8af:	25 00 20 00 00       	and    $0x2000,%eax
     8b4:	31 ed                	xor    %ebp,%ebp
     8b6:	48 09 f8             	or     %rdi,%rax
     8b9:	0f 84 fd 0e 00 00    	je     0x17bc
     8bf:	48 85 c9             	test   %rcx,%rcx
     8c2:	0f 84 f4 0e 00 00    	je     0x17bc
     8c8:	44 89 e0             	mov    %r12d,%eax
     8cb:	83 e0 40             	and    $0x40,%eax
     8ce:	48 83 f8 01          	cmp    $0x1,%rax
     8d2:	6a 02                	pushq  $0x2
     8d4:	41 5e                	pop    %r14
     8d6:	4d 89 f5             	mov    %r14,%r13
     8d9:	49 83 dd 00          	sbb    $0x0,%r13
     8dd:	31 ed                	xor    %ebp,%ebp
     8df:	41 0f ba e4 08       	bt     $0x8,%r12d
     8e4:	72 2c                	jb     0x912
     8e6:	6a 01                	pushq  $0x1
     8e8:	41 5e                	pop    %r14
     8ea:	b8 ff 00 00 00       	mov    $0xff,%eax
     8ef:	48 89 84 24 c0 00 00 	mov    %rax,0xc0(%rsp)
     8f6:	00 
     8f7:	31 c0                	xor    %eax,%eax
     8f9:	eb 0f                	jmp    0x90a
     8fb:	48 3d 3f 42 0f 00    	cmp    $0xf423f,%rax
     901:	0f 87 b5 0e 00 00    	ja     0x17bc
     907:	48 ff c0             	inc    %rax
     90a:	80 3c 01 00          	cmpb   $0x0,(%rcx,%rax,1)
     90e:	75 eb                	jne    0x8fb
     910:	eb 27                	jmp    0x939
     912:	b8 ff ff 00 00       	mov    $0xffff,%eax
     917:	48 89 84 24 c0 00 00 	mov    %rax,0xc0(%rsp)
     91e:	00 
     91f:	31 c0                	xor    %eax,%eax
     921:	eb 0f                	jmp    0x932
     923:	48 3d 3f 42 0f 00    	cmp    $0xf423f,%rax
     929:	0f 87 8d 0e 00 00    	ja     0x17bc
     92f:	48 ff c0             	inc    %rax
     932:	66 83 3c 41 00       	cmpw   $0x0,(%rcx,%rax,2)
     937:	75 ea                	jne    0x923
     939:	4c 89 84 24 b0 00 00 	mov    %r8,0xb0(%rsp)
     940:	00 
     941:	48 ff ce             	dec    %rsi
     944:	49 0f af f5          	imul   %r13,%rsi
     948:	48 01 fe             	add    %rdi,%rsi
     94b:	48 85 ff             	test   %rdi,%rdi
     94e:	48 0f 44 f7          	cmove  %rdi,%rsi
     952:	0f b6 11             	movzbl (%rcx),%edx
     955:	31 c0                	xor    %eax,%eax
     957:	49 83 fe 01          	cmp    $0x1,%r14
     95b:	74 07                	je     0x964
     95d:	0f b6 41 01          	movzbl 0x1(%rcx),%eax
     961:	c1 e0 08             	shl    $0x8,%eax
     964:	09 d0                	or     %edx,%eax
     966:	31 ed                	xor    %ebp,%ebp
     968:	4c 8d 05 ed 0f 00 00 	lea    0xfed(%rip),%r8        # 0x195c
     96f:	4c 8d 1d ca 18 00 00 	lea    0x18ca(%rip),%r11        # 0x2240
     976:	4c 89 a4 24 88 00 00 	mov    %r12,0x88(%rsp)
     97d:	00 
     97e:	48 89 bc 24 08 01 00 	mov    %rdi,0x108(%rsp)
     985:	00 
     986:	48 89 fb             	mov    %rdi,%rbx
     989:	23 84 24 c0 00 00 00 	and    0xc0(%rsp),%eax
     990:	48 89 84 24 c8 00 00 	mov    %rax,0xc8(%rsp)
     997:	00 
     998:	0f 84 ca 0d 00 00    	je     0x1768
     99e:	48 39 f3             	cmp    %rsi,%rbx
     9a1:	72 09                	jb     0x9ac
     9a3:	48 85 db             	test   %rbx,%rbx
     9a6:	0f 85 bc 0d 00 00    	jne    0x1768
     9ac:	48 8b 94 24 88 00 00 	mov    0x88(%rsp),%rdx
     9b3:	00 
     9b4:	81 e2 40 21 00 00    	and    $0x2140,%edx
     9ba:	48 89 94 24 88 00 00 	mov    %rdx,0x88(%rsp)
     9c1:	00 
     9c2:	66 83 f8 0a          	cmp    $0xa,%ax
     9c6:	48 89 ac 24 b8 00 00 	mov    %rbp,0xb8(%rsp)
     9cd:	00 
     9ce:	0f 84 6d 02 00 00    	je     0xc41
     9d4:	66 83 f8 0d          	cmp    $0xd,%ax
     9d8:	0f 84 b9 02 00 00    	je     0xc97
     9de:	66 83 f8 25          	cmp    $0x25,%ax
     9e2:	0f 85 42 03 00 00    	jne    0xd2a
     9e8:	b2 01                	mov    $0x1,%dl
     9ea:	45 31 c9             	xor    %r9d,%r9d
     9ed:	6a 01                	pushq  $0x1
     9ef:	5d                   	pop    %rbp
     9f0:	49 89 ea             	mov    %rbp,%r10
     9f3:	31 ed                	xor    %ebp,%ebp
     9f5:	48 89 ac 24 a8 00 00 	mov    %rbp,0xa8(%rsp)
     9fc:	00 
     9fd:	e9 ad 01 00 00       	jmpq   0xbaf
     a02:	4b 8d 0c 37          	lea    (%r15,%r14,1),%rcx
     a06:	31 d2                	xor    %edx,%edx
     a08:	43 0f b6 04 37       	movzbl (%r15,%r14,1),%eax
     a0d:	49 83 fe 01          	cmp    $0x1,%r14
     a11:	74 07                	je     0xa1a
     a13:	0f b6 51 01          	movzbl 0x1(%rcx),%edx
     a17:	c1 e2 08             	shl    $0x8,%edx
     a1a:	09 d0                	or     %edx,%eax
     a1c:	23 84 24 c0 00 00 00 	and    0xc0(%rsp),%eax
     a23:	48 89 84 24 c8 00 00 	mov    %rax,0xc8(%rsp)
     a2a:	00 
     a2b:	31 d2                	xor    %edx,%edx
     a2d:	8d 68 d6             	lea    -0x2a(%rax),%ebp
     a30:	66 83 fd 22          	cmp    $0x22,%bp
     a34:	0f 87 90 00 00 00    	ja     0xaca
     a3a:	0f b7 ed             	movzwl %bp,%ebp
     a3d:	49 63 2c a8          	movslq (%r8,%rbp,4),%rbp
     a41:	4c 01 c5             	add    %r8,%rbp
     a44:	ff e5                	jmpq   *%rbp
     a46:	31 d2                	xor    %edx,%edx
     a48:	eb 34                	jmp    0xa7e
     a4a:	48 6b ea 0a          	imul   $0xa,%rdx,%rbp
     a4e:	31 d2                	xor    %edx,%edx
     a50:	43 0f b6 04 77       	movzbl (%r15,%r14,2),%eax
     a55:	49 83 fe 01          	cmp    $0x1,%r14
     a59:	74 09                	je     0xa64
     a5b:	43 0f b6 54 77 01    	movzbl 0x1(%r15,%r14,2),%edx
     a61:	c1 e2 08             	shl    $0x8,%edx
     a64:	48 01 e9             	add    %rbp,%rcx
     a67:	09 d0                	or     %edx,%eax
     a69:	23 84 24 c0 00 00 00 	and    0xc0(%rsp),%eax
     a70:	48 89 84 24 c8 00 00 	mov    %rax,0xc8(%rsp)
     a77:	00 
     a78:	4d 01 f7             	add    %r14,%r15
     a7b:	48 89 ca             	mov    %rcx,%rdx
     a7e:	48 8d 48 d0          	lea    -0x30(%rax),%rcx
     a82:	48 83 f9 09          	cmp    $0x9,%rcx
     a86:	76 c2                	jbe    0xa4a
     a88:	48 89 d5             	mov    %rdx,%rbp
     a8b:	48 8b 94 24 88 00 00 	mov    0x88(%rsp),%rdx
     a92:	00 
     a93:	48 89 d1             	mov    %rdx,%rcx
     a96:	48 81 c9 00 02 00 00 	or     $0x200,%rcx
     a9d:	0f ba e2 0b          	bt     $0xb,%edx
     aa1:	48 0f 43 d1          	cmovae %rcx,%rdx
     aa5:	48 89 94 24 88 00 00 	mov    %rdx,0x88(%rsp)
     aac:	00 
     aad:	4c 0f 43 cd          	cmovae %rbp,%r9
     ab1:	48 89 e9             	mov    %rbp,%rcx
     ab4:	48 89 ac 24 a8 00 00 	mov    %rbp,0xa8(%rsp)
     abb:	00 
     abc:	4c 0f 42 d5          	cmovb  %rbp,%r10
     ac0:	4c 89 f9             	mov    %r15,%rcx
     ac3:	b2 01                	mov    $0x1,%dl
     ac5:	e9 e5 00 00 00       	jmpq   0xbaf
     aca:	66 85 c0             	test   %ax,%ax
     acd:	0f 84 ac 00 00 00    	je     0xb7f
     ad3:	66 83 f8 20          	cmp    $0x20,%ax
     ad7:	0f 84 ac 00 00 00    	je     0xb89
     add:	66 83 f8 6c          	cmp    $0x6c,%ax
     ae1:	0f 85 c8 00 00 00    	jne    0xbaf
     ae7:	48 83 8c 24 88 00 00 	orq    $0x10,0x88(%rsp)
     aee:	00 10 
     af0:	b2 01                	mov    $0x1,%dl
     af2:	e9 b8 00 00 00       	jmpq   0xbaf
     af7:	48 8b 94 24 88 00 00 	mov    0x88(%rsp),%rdx
     afe:	00 
     aff:	0f ba e2 0b          	bt     $0xb,%edx
     b03:	0f 82 8d 00 00 00    	jb     0xb96
     b09:	48 81 ca 00 02 00 00 	or     $0x200,%rdx
     b10:	48 89 94 24 88 00 00 	mov    %rdx,0x88(%rsp)
     b17:	00 
     b18:	48 8b 94 24 b0 00 00 	mov    0xb0(%rsp),%rdx
     b1f:	00 
     b20:	4c 8b 0a             	mov    (%rdx),%r9
     b23:	eb 7c                	jmp    0xba1
     b25:	48 83 8c 24 88 00 00 	orq    $0x2,0x88(%rsp)
     b2c:	00 02 
     b2e:	b2 01                	mov    $0x1,%dl
     b30:	eb 7d                	jmp    0xbaf
     b32:	48 83 8c 24 88 00 00 	orq    $0x8,0x88(%rsp)
     b39:	00 08 
     b3b:	b2 01                	mov    $0x1,%dl
     b3d:	eb 70                	jmp    0xbaf
     b3f:	48 83 8c 24 88 00 00 	orq    $0x1,0x88(%rsp)
     b46:	00 01 
     b48:	b2 01                	mov    $0x1,%dl
     b4a:	eb 63                	jmp    0xbaf
     b4c:	48 81 8c 24 88 00 00 	orq    $0x800,0x88(%rsp)
     b53:	00 00 08 00 00 
     b58:	b2 01                	mov    $0x1,%dl
     b5a:	eb 53                	jmp    0xbaf
     b5c:	48 8b 8c 24 88 00 00 	mov    0x88(%rsp),%rcx
     b63:	00 
     b64:	48 89 ca             	mov    %rcx,%rdx
     b67:	c1 e9 06             	shr    $0x6,%ecx
     b6a:	f7 d1                	not    %ecx
     b6c:	83 e1 20             	and    $0x20,%ecx
     b6f:	48 09 ca             	or     %rcx,%rdx
     b72:	48 89 94 24 88 00 00 	mov    %rdx,0x88(%rsp)
     b79:	00 
     b7a:	e9 c7 fe ff ff       	jmpq   0xa46
     b7f:	45 31 d2             	xor    %r10d,%r10d
     b82:	4c 89 f9             	mov    %r15,%rcx
     b85:	31 d2                	xor    %edx,%edx
     b87:	eb 26                	jmp    0xbaf
     b89:	48 83 8c 24 88 00 00 	orq    $0x4,0x88(%rsp)
     b90:	00 04 
     b92:	b2 01                	mov    $0x1,%dl
     b94:	eb 19                	jmp    0xbaf
     b96:	48 8b 94 24 b0 00 00 	mov    0xb0(%rsp),%rdx
     b9d:	00 
     b9e:	4c 8b 12             	mov    (%rdx),%r10
     ba1:	48 83 c2 08          	add    $0x8,%rdx
     ba5:	48 89 94 24 b0 00 00 	mov    %rdx,0xb0(%rsp)
     bac:	00 
     bad:	b2 01                	mov    $0x1,%dl
     baf:	49 89 cf             	mov    %rcx,%r15
     bb2:	f6 c2 01             	test   $0x1,%dl
     bb5:	0f 85 47 fe ff ff    	jne    0xa02
     bbb:	48 8d 48 9f          	lea    -0x61(%rax),%rcx
     bbf:	48 83 f9 17          	cmp    $0x17,%rcx
     bc3:	48 8b ac 24 b8 00 00 	mov    0xb8(%rsp),%rbp
     bca:	00 
     bcb:	4c 89 8c 24 d0 00 00 	mov    %r9,0xd0(%rsp)
     bd2:	00 
     bd3:	0f 87 ab 01 00 00    	ja     0xd84
     bd9:	48 8d 05 1c 0d 00 00 	lea    0xd1c(%rip),%rax        # 0x18fc
     be0:	48 89 c2             	mov    %rax,%rdx
     be3:	48 63 04 88          	movslq (%rax,%rcx,4),%rax
     be7:	48 01 d0             	add    %rdx,%rax
     bea:	48 8b 8c 24 88 00 00 	mov    0x88(%rsp),%rcx
     bf1:	00 
     bf2:	ff e0                	jmpq   *%rax
     bf4:	4c 89 94 24 98 00 00 	mov    %r10,0x98(%rsp)
     bfb:	00 
     bfc:	48 8b 8c 24 b0 00 00 	mov    0xb0(%rsp),%rcx
     c03:	00 
     c04:	0f b7 01             	movzwl (%rcx),%eax
     c07:	48 83 c1 08          	add    $0x8,%rcx
     c0b:	48 89 8c 24 b0 00 00 	mov    %rcx,0xb0(%rsp)
     c12:	00 
     c13:	48 8b 8c 24 88 00 00 	mov    0x88(%rsp),%rcx
     c1a:	00 
     c1b:	48 89 84 24 10 01 00 	mov    %rax,0x110(%rsp)
     c22:	00 
     c23:	48 81 c9 00 04 00 00 	or     $0x400,%rcx
     c2a:	31 c0                	xor    %eax,%eax
     c2c:	48 89 84 24 90 00 00 	mov    %rax,0x90(%rsp)
     c33:	00 
     c34:	4c 8d 9c 24 10 01 00 	lea    0x110(%rsp),%r11
     c3b:	00 
     c3c:	e9 eb 01 00 00       	jmpq   0xe2c
     c41:	4a 8d 04 31          	lea    (%rcx,%r14,1),%rax
     c45:	31 ed                	xor    %ebp,%ebp
     c47:	42 0f b6 14 31       	movzbl (%rcx,%r14,1),%edx
     c4c:	49 83 fe 01          	cmp    $0x1,%r14
     c50:	74 07                	je     0xc59
     c52:	0f b6 68 01          	movzbl 0x1(%rax),%ebp
     c56:	c1 e5 08             	shl    $0x8,%ebp
     c59:	09 ea                	or     %ebp,%edx
     c5b:	23 94 24 c0 00 00 00 	and    0xc0(%rsp),%edx
     c62:	48 89 94 24 c8 00 00 	mov    %rdx,0xc8(%rsp)
     c69:	00 
     c6a:	83 fa 0d             	cmp    $0xd,%edx
     c6d:	48 0f 44 c8          	cmove  %rax,%rcx
     c71:	31 c0                	xor    %eax,%eax
     c73:	48 89 84 24 d0 00 00 	mov    %rax,0xd0(%rsp)
     c7a:	00 
     c7b:	49 89 cf             	mov    %rcx,%r15
     c7e:	6a 01                	pushq  $0x1
     c80:	58                   	pop    %rax
     c81:	48 89 84 24 98 00 00 	mov    %rax,0x98(%rsp)
     c88:	00 
     c89:	4c 8d 1d 93 15 00 00 	lea    0x1593(%rip),%r11        # 0x2223
     c90:	ba 00 00 00 00       	mov    $0x0,%edx
     c95:	eb 5c                	jmp    0xcf3
     c97:	4a 8d 04 31          	lea    (%rcx,%r14,1),%rax
     c9b:	31 ed                	xor    %ebp,%ebp
     c9d:	42 0f b6 14 31       	movzbl (%rcx,%r14,1),%edx
     ca2:	49 83 fe 01          	cmp    $0x1,%r14
     ca6:	74 07                	je     0xcaf
     ca8:	0f b6 68 01          	movzbl 0x1(%rax),%ebp
     cac:	c1 e5 08             	shl    $0x8,%ebp
     caf:	09 ea                	or     %ebp,%edx
     cb1:	23 94 24 c0 00 00 00 	and    0xc0(%rsp),%edx
     cb8:	48 89 94 24 c8 00 00 	mov    %rdx,0xc8(%rsp)
     cbf:	00 
     cc0:	83 fa 0a             	cmp    $0xa,%edx
     cc3:	48 0f 44 c8          	cmove  %rax,%rcx
     cc7:	4c 8d 1d 58 15 00 00 	lea    0x1558(%rip),%r11        # 0x2226
     cce:	48 8d 05 4e 15 00 00 	lea    0x154e(%rip),%rax        # 0x2223
     cd5:	4c 0f 44 d8          	cmove  %rax,%r11
     cd9:	31 c0                	xor    %eax,%eax
     cdb:	48 89 84 24 d0 00 00 	mov    %rax,0xd0(%rsp)
     ce2:	00 
     ce3:	49 89 cf             	mov    %rcx,%r15
     ce6:	6a 01                	pushq  $0x1
     ce8:	58                   	pop    %rax
     ce9:	48 89 84 24 98 00 00 	mov    %rax,0x98(%rsp)
     cf0:	00 
     cf1:	31 d2                	xor    %edx,%edx
     cf3:	31 c0                	xor    %eax,%eax
     cf5:	48 89 84 24 90 00 00 	mov    %rax,0x90(%rsp)
     cfc:	00 
     cfd:	45 31 d2             	xor    %r10d,%r10d
     d00:	c7 84 24 84 00 00 00 	movl   $0x0,0x84(%rsp)
     d07:	00 00 00 00 
     d0b:	31 c0                	xor    %eax,%eax
     d0d:	48 89 84 24 a0 00 00 	mov    %rax,0xa0(%rsp)
     d14:	00 
     d15:	48 8b ac 24 b8 00 00 	mov    0xb8(%rsp),%rbp
     d1c:	00 
     d1d:	48 8b 8c 24 88 00 00 	mov    0x88(%rsp),%rcx
     d24:	00 
     d25:	e9 ae 05 00 00       	jmpq   0x12d8
     d2a:	48 8b 84 24 88 00 00 	mov    0x88(%rsp),%rax
     d31:	00 
     d32:	48 0d 00 04 00 00    	or     $0x400,%rax
     d38:	31 d2                	xor    %edx,%edx
     d3a:	48 89 94 24 d0 00 00 	mov    %rdx,0xd0(%rsp)
     d41:	00 
     d42:	49 89 cf             	mov    %rcx,%r15
     d45:	48 89 c1             	mov    %rax,%rcx
     d48:	6a 01                	pushq  $0x1
     d4a:	58                   	pop    %rax
     d4b:	48 89 84 24 98 00 00 	mov    %rax,0x98(%rsp)
     d52:	00 
     d53:	4c 8d 9c 24 c8 00 00 	lea    0xc8(%rsp),%r11
     d5a:	00 
     d5b:	31 d2                	xor    %edx,%edx
     d5d:	31 c0                	xor    %eax,%eax
     d5f:	48 89 84 24 90 00 00 	mov    %rax,0x90(%rsp)
     d66:	00 
     d67:	45 31 d2             	xor    %r10d,%r10d
     d6a:	c7 84 24 84 00 00 00 	movl   $0x0,0x84(%rsp)
     d71:	00 00 00 00 
     d75:	31 c0                	xor    %eax,%eax
     d77:	48 89 84 24 a0 00 00 	mov    %rax,0xa0(%rsp)
     d7e:	00 
     d7f:	e9 54 05 00 00       	jmpq   0x12d8
     d84:	48 83 f8 0a          	cmp    $0xa,%rax
     d88:	0f 84 09 04 00 00    	je     0x1197
     d8e:	48 83 f8 0d          	cmp    $0xd,%rax
     d92:	0f 84 4d 04 00 00    	je     0x11e5
     d98:	48 83 f8 53          	cmp    $0x53,%rax
     d9c:	48 8b 8c 24 88 00 00 	mov    0x88(%rsp),%rcx
     da3:	00 
     da4:	74 2d                	je     0xdd3
     da6:	48 83 f8 58          	cmp    $0x58,%rax
     daa:	0f 84 62 01 00 00    	je     0xf12
     db0:	4c 89 94 24 98 00 00 	mov    %r10,0x98(%rsp)
     db7:	00 
     db8:	48 81 c9 00 04 00 00 	or     $0x400,%rcx
     dbf:	31 c0                	xor    %eax,%eax
     dc1:	48 89 84 24 90 00 00 	mov    %rax,0x90(%rsp)
     dc8:	00 
     dc9:	4c 8d 9c 24 c8 00 00 	lea    0xc8(%rsp),%r11
     dd0:	00 
     dd1:	eb 59                	jmp    0xe2c
     dd3:	48 81 c9 00 04 00 00 	or     $0x400,%rcx
     dda:	48 8b 84 24 b0 00 00 	mov    0xb0(%rsp),%rax
     de1:	00 
     de2:	4c 8b 18             	mov    (%rax),%r11
     de5:	48 83 c0 08          	add    $0x8,%rax
     de9:	48 89 84 24 b0 00 00 	mov    %rax,0xb0(%rsp)
     df0:	00 
     df1:	48 89 c8             	mov    %rcx,%rax
     df4:	48 25 ff fb ff ff    	and    $0xfffffffffffffbff,%rax
     dfa:	4d 85 db             	test   %r11,%r11
     dfd:	48 0f 44 c8          	cmove  %rax,%rcx
     e01:	48 8d 05 bd 13 00 00 	lea    0x13bd(%rip),%rax        # 0x21c5
     e08:	4c 0f 44 d8          	cmove  %rax,%r11
     e0c:	48 89 c8             	mov    %rcx,%rax
     e0f:	48 c1 e0 34          	shl    $0x34,%rax
     e13:	48 c1 f8 3f          	sar    $0x3f,%rax
     e17:	49 21 c2             	and    %rax,%r10
     e1a:	4c 89 94 24 98 00 00 	mov    %r10,0x98(%rsp)
     e21:	00 
     e22:	31 c0                	xor    %eax,%eax
     e24:	48 89 84 24 90 00 00 	mov    %rax,0x90(%rsp)
     e2b:	00 
     e2c:	45 31 d2             	xor    %r10d,%r10d
     e2f:	c7 84 24 84 00 00 00 	movl   $0x0,0x84(%rsp)
     e36:	00 00 00 00 
     e3a:	31 c0                	xor    %eax,%eax
     e3c:	48 89 84 24 a0 00 00 	mov    %rax,0xa0(%rsp)
     e43:	00 
     e44:	e9 87 04 00 00       	jmpq   0x12d0
     e49:	4c 89 94 24 98 00 00 	mov    %r10,0x98(%rsp)
     e50:	00 
     e51:	48 8b 8c 24 b0 00 00 	mov    0xb0(%rsp),%rcx
     e58:	00 
     e59:	48 8b 01             	mov    (%rcx),%rax
     e5c:	48 83 c1 08          	add    $0x8,%rcx
     e60:	48 89 8c 24 b0 00 00 	mov    %rcx,0xb0(%rsp)
     e67:	00 
     e68:	48 85 c0             	test   %rax,%rax
     e6b:	0f 84 c0 03 00 00    	je     0x1231
     e71:	8b 08                	mov    (%rax),%ecx
     e73:	89 8c 24 84 00 00 00 	mov    %ecx,0x84(%rsp)
     e7a:	0f b7 48 04          	movzwl 0x4(%rax),%ecx
     e7e:	89 8c 24 d8 00 00 00 	mov    %ecx,0xd8(%rsp)
     e85:	44 0f b7 50 06       	movzwl 0x6(%rax),%r10d
     e8a:	44 0f b6 58 08       	movzbl 0x8(%rax),%r11d
     e8f:	44 0f b6 48 09       	movzbl 0x9(%rax),%r9d
     e94:	44 0f b6 40 0a       	movzbl 0xa(%rax),%r8d
     e99:	0f b6 68 0b          	movzbl 0xb(%rax),%ebp
     e9d:	0f b6 78 0c          	movzbl 0xc(%rax),%edi
     ea1:	0f b6 50 0d          	movzbl 0xd(%rax),%edx
     ea5:	0f b6 48 0e          	movzbl 0xe(%rax),%ecx
     ea9:	0f b6 40 0f          	movzbl 0xf(%rax),%eax
     ead:	89 44 24 70          	mov    %eax,0x70(%rsp)
     eb1:	89 4c 24 68          	mov    %ecx,0x68(%rsp)
     eb5:	89 54 24 60          	mov    %edx,0x60(%rsp)
     eb9:	89 7c 24 58          	mov    %edi,0x58(%rsp)
     ebd:	89 6c 24 50          	mov    %ebp,0x50(%rsp)
     ec1:	48 8b ac 24 b8 00 00 	mov    0xb8(%rsp),%rbp
     ec8:	00 
     ec9:	44 89 44 24 48       	mov    %r8d,0x48(%rsp)
     ece:	44 89 4c 24 40       	mov    %r9d,0x40(%rsp)
     ed3:	44 89 5c 24 38       	mov    %r11d,0x38(%rsp)
     ed8:	44 89 54 24 30       	mov    %r10d,0x30(%rsp)
     edd:	8b 84 24 d8 00 00 00 	mov    0xd8(%rsp),%eax
     ee4:	89 44 24 28          	mov    %eax,0x28(%rsp)
     ee8:	8b 84 24 84 00 00 00 	mov    0x84(%rsp),%eax
     eef:	89 44 24 20          	mov    %eax,0x20(%rsp)
     ef3:	48 8d bc 24 e0 00 00 	lea    0xe0(%rsp),%rdi
     efa:	00 
     efb:	48 89 f9             	mov    %rdi,%rcx
     efe:	4c 8d 0d ce 12 00 00 	lea    0x12ce(%rip),%r9        # 0x21d3
     f05:	e9 94 03 00 00       	jmpq   0x129e
     f0a:	48 83 e1 c9          	and    $0xffffffffffffffc9,%rcx
     f0e:	48 83 c9 10          	or     $0x10,%rcx
     f12:	48 83 c9 20          	or     $0x20,%rcx
     f16:	48 81 c9 80 00 00 00 	or     $0x80,%rcx
     f1d:	84 c9                	test   %cl,%cl
     f1f:	78 0e                	js     0xf2f
     f21:	48 81 e1 fd bf ff ff 	and    $0xffffffffffffbffd,%rcx
     f28:	48 81 c9 00 40 00 00 	or     $0x4000,%rcx
     f2f:	f6 c1 10             	test   $0x10,%cl
     f32:	48 8b 84 24 b0 00 00 	mov    0xb0(%rsp),%rax
     f39:	00 
     f3a:	48 89 ca             	mov    %rcx,%rdx
     f3d:	48 8b 08             	mov    (%rax),%rcx
     f40:	48 63 c1             	movslq %ecx,%rax
     f43:	48 0f 45 c1          	cmovne %rcx,%rax
     f47:	48 89 d1             	mov    %rdx,%rcx
     f4a:	f6 c1 02             	test   $0x2,%cl
     f4d:	74 04                	je     0xf53
     f4f:	b2 2b                	mov    $0x2b,%dl
     f51:	eb 08                	jmp    0xf5b
     f53:	89 ca                	mov    %ecx,%edx
     f55:	c0 e2 03             	shl    $0x3,%dl
     f58:	80 e2 20             	and    $0x20,%dl
     f5b:	48 89 94 24 90 00 00 	mov    %rdx,0x90(%rsp)
     f62:	00 
     f63:	4c 89 94 24 98 00 00 	mov    %r10,0x98(%rsp)
     f6a:	00 
     f6b:	84 c9                	test   %cl,%cl
     f6d:	78 56                	js     0xfc5
     f6f:	89 ca                	mov    %ecx,%edx
     f71:	83 e2 08             	and    $0x8,%edx
     f74:	c1 ea 03             	shr    $0x3,%edx
     f77:	89 94 24 84 00 00 00 	mov    %edx,0x84(%rsp)
     f7e:	49 89 ca             	mov    %rcx,%r10
     f81:	49 83 e2 df          	and    $0xffffffffffffffdf,%r10
     f85:	f6 c1 08             	test   $0x8,%cl
     f88:	4c 0f 44 d1          	cmove  %rcx,%r10
     f8c:	6a 01                	pushq  $0x1
     f8e:	59                   	pop    %rcx
     f8f:	48 8b 94 24 98 00 00 	mov    0x98(%rsp),%rdx
     f96:	00 
     f97:	48 0f 45 d1          	cmovne %rcx,%rdx
     f9b:	48 89 94 24 98 00 00 	mov    %rdx,0x98(%rsp)
     fa2:	00 
     fa3:	41 0f ba e2 0e       	bt     $0xe,%r10d
     fa8:	72 40                	jb     0xfea
     faa:	48 85 c0             	test   %rax,%rax
     fad:	79 3b                	jns    0xfea
     faf:	49 83 ca 02          	or     $0x2,%r10
     fb3:	48 f7 d8             	neg    %rax
     fb6:	b1 2d                	mov    $0x2d,%cl
     fb8:	48 89 8c 24 90 00 00 	mov    %rcx,0x90(%rsp)
     fbf:	00 
     fc0:	49 89 c0             	mov    %rax,%r8
     fc3:	eb 3c                	jmp    0x1001
     fc5:	48 85 c0             	test   %rax,%rax
     fc8:	41 89 c0             	mov    %eax,%r8d
     fcb:	4c 0f 49 c0          	cmovns %rax,%r8
     fcf:	f6 c1 10             	test   $0x10,%cl
     fd2:	4c 0f 45 c0          	cmovne %rax,%r8
     fd6:	c7 84 24 84 00 00 00 	movl   $0x0,0x84(%rsp)
     fdd:	00 00 00 00 
     fe1:	49 89 ca             	mov    %rcx,%r10
     fe4:	6a 10                	pushq  $0x10
     fe6:	41 59                	pop    %r9
     fe8:	eb 1b                	jmp    0x1005
     fea:	44 89 d1             	mov    %r10d,%ecx
     fed:	81 e1 10 40 00 00    	and    $0x4010,%ecx
     ff3:	48 81 f9 00 40 00 00 	cmp    $0x4000,%rcx
     ffa:	41 89 c0             	mov    %eax,%r8d
     ffd:	4c 0f 45 c0          	cmovne %rax,%r8
    1001:	6a 0a                	pushq  $0xa
    1003:	41 59                	pop    %r9
    1005:	48 8d ac 24 e0 00 00 	lea    0xe0(%rsp),%rbp
    100c:	00 
    100d:	48 83 84 24 b0 00 00 	addq   $0x8,0xb0(%rsp)
    1014:	00 08 
    1016:	c6 84 24 e0 00 00 00 	movb   $0x0,0xe0(%rsp)
    101d:	00 
    101e:	4c 89 c1             	mov    %r8,%rcx
    1021:	31 d2                	xor    %edx,%edx
    1023:	48 89 c8             	mov    %rcx,%rax
    1026:	49 f7 f1             	div    %r9
    1029:	42 8a 14 1a          	mov    (%rdx,%r11,1),%dl
    102d:	88 55 01             	mov    %dl,0x1(%rbp)
    1030:	48 ff c5             	inc    %rbp
    1033:	49 39 c9             	cmp    %rcx,%r9
    1036:	48 89 c1             	mov    %rax,%rcx
    1039:	76 e6                	jbe    0x1021
    103b:	48 8d 84 24 e0 00 00 	lea    0xe0(%rsp),%rax
    1042:	00 
    1043:	48 29 c5             	sub    %rax,%rbp
    1046:	4c 0b 84 24 98 00 00 	or     0x98(%rsp),%r8
    104d:	00 
    104e:	b9 00 00 00 00       	mov    $0x0,%ecx
    1053:	48 0f 45 cd          	cmovne %rbp,%rcx
    1057:	31 d2                	xor    %edx,%edx
    1059:	48 89 c8             	mov    %rcx,%rax
    105c:	6a 03                	pushq  $0x3
    105e:	5d                   	pop    %rbp
    105f:	48 f7 f5             	div    %rbp
    1062:	48 89 d0             	mov    %rdx,%rax
    1065:	48 83 f0 03          	xor    $0x3,%rax
    1069:	48 85 d2             	test   %rdx,%rdx
    106c:	48 0f 44 c2          	cmove  %rdx,%rax
    1070:	48 89 84 24 a0 00 00 	mov    %rax,0xa0(%rsp)
    1077:	00 
    1078:	48 89 c8             	mov    %rcx,%rax
    107b:	80 bc 24 84 00 00 00 	cmpb   $0x0,0x84(%rsp)
    1082:	00 
    1083:	74 17                	je     0x109c
    1085:	48 89 c8             	mov    %rcx,%rax
    1088:	48 85 c9             	test   %rcx,%rcx
    108b:	74 0f                	je     0x109c
    108d:	48 8d 41 ff          	lea    -0x1(%rcx),%rax
    1091:	31 d2                	xor    %edx,%edx
    1093:	6a 03                	pushq  $0x3
    1095:	5d                   	pop    %rbp
    1096:	48 f7 f5             	div    %rbp
    1099:	48 01 c8             	add    %rcx,%rax
    109c:	4c 8d 1c 0c          	lea    (%rsp,%rcx,1),%r11
    10a0:	49 81 c3 e0 00 00 00 	add    $0xe0,%r11
    10a7:	31 c9                	xor    %ecx,%ecx
    10a9:	80 bc 24 90 00 00 00 	cmpb   $0x0,0x90(%rsp)
    10b0:	00 
    10b1:	0f 95 c1             	setne  %cl
    10b4:	48 8b 94 24 98 00 00 	mov    0x98(%rsp),%rdx
    10bb:	00 
    10bc:	48 01 ca             	add    %rcx,%rdx
    10bf:	48 01 c8             	add    %rcx,%rax
    10c2:	4c 89 d1             	mov    %r10,%rcx
    10c5:	48 81 c9 00 10 00 00 	or     $0x1000,%rcx
    10cc:	41 81 e2 21 0a 00 00 	and    $0xa21,%r10d
    10d3:	49 81 fa 20 02 00 00 	cmp    $0x220,%r10
    10da:	48 0f 44 94 24 d0 00 	cmove  0xd0(%rsp),%rdx
    10e1:	00 00 
    10e3:	48 89 94 24 98 00 00 	mov    %rdx,0x98(%rsp)
    10ea:	00 
    10eb:	48 89 c2             	mov    %rax,%rdx
    10ee:	41 b2 01             	mov    $0x1,%r10b
    10f1:	48 8b ac 24 b8 00 00 	mov    0xb8(%rsp),%rbp
    10f8:	00 
    10f9:	e9 da 01 00 00       	jmpq   0x12d8
    10fe:	48 8b 84 24 b0 00 00 	mov    0xb0(%rsp),%rax
    1105:	00 
    1106:	48 8b 10             	mov    (%rax),%rdx
    1109:	48 85 d2             	test   %rdx,%rdx
    110c:	0f 88 32 01 00 00    	js     0x1244
    1112:	4c 6b da 19          	imul   $0x19,%rdx,%r11
    1116:	48 8d 05 f3 13 00 00 	lea    0x13f3(%rip),%rax        # 0x2510
    111d:	49 01 c3             	add    %rax,%r11
    1120:	48 83 fa 06          	cmp    $0x6,%rdx
    1124:	e9 38 01 00 00       	jmpq   0x1261
    1129:	4c 89 94 24 98 00 00 	mov    %r10,0x98(%rsp)
    1130:	00 
    1131:	48 8b 8c 24 b0 00 00 	mov    0xb0(%rsp),%rcx
    1138:	00 
    1139:	48 8b 01             	mov    (%rcx),%rax
    113c:	48 83 c1 08          	add    $0x8,%rcx
    1140:	48 89 8c 24 b0 00 00 	mov    %rcx,0xb0(%rsp)
    1147:	00 
    1148:	48 85 c0             	test   %rax,%rax
    114b:	0f 84 01 06 00 00    	je     0x1752
    1151:	0f b6 48 02          	movzbl 0x2(%rax),%ecx
    1155:	0f b6 50 03          	movzbl 0x3(%rax),%edx
    1159:	0f b7 28             	movzwl (%rax),%ebp
    115c:	0f b6 78 04          	movzbl 0x4(%rax),%edi
    1160:	0f b6 40 05          	movzbl 0x5(%rax),%eax
    1164:	89 44 24 40          	mov    %eax,0x40(%rsp)
    1168:	89 7c 24 38          	mov    %edi,0x38(%rsp)
    116c:	89 6c 24 30          	mov    %ebp,0x30(%rsp)
    1170:	48 8b ac 24 b8 00 00 	mov    0xb8(%rsp),%rbp
    1177:	00 
    1178:	89 54 24 28          	mov    %edx,0x28(%rsp)
    117c:	89 4c 24 20          	mov    %ecx,0x20(%rsp)
    1180:	48 8d bc 24 e0 00 00 	lea    0xe0(%rsp),%rdi
    1187:	00 
    1188:	48 89 f9             	mov    %rdi,%rcx
    118b:	4c 8d 0d 72 10 00 00 	lea    0x1072(%rip),%r9        # 0x2204
    1192:	e9 07 01 00 00       	jmpq   0x129e
    1197:	4c 89 94 24 98 00 00 	mov    %r10,0x98(%rsp)
    119e:	00 
    119f:	4b 8d 04 37          	lea    (%r15,%r14,1),%rax
    11a3:	31 d2                	xor    %edx,%edx
    11a5:	43 0f b6 0c 37       	movzbl (%r15,%r14,1),%ecx
    11aa:	49 83 fe 01          	cmp    $0x1,%r14
    11ae:	74 07                	je     0x11b7
    11b0:	0f b6 50 01          	movzbl 0x1(%rax),%edx
    11b4:	c1 e2 08             	shl    $0x8,%edx
    11b7:	09 d1                	or     %edx,%ecx
    11b9:	23 8c 24 c0 00 00 00 	and    0xc0(%rsp),%ecx
    11c0:	48 89 8c 24 c8 00 00 	mov    %rcx,0xc8(%rsp)
    11c7:	00 
    11c8:	83 f9 0d             	cmp    $0xd,%ecx
    11cb:	4c 0f 44 f8          	cmove  %rax,%r15
    11cf:	31 c0                	xor    %eax,%eax
    11d1:	48 89 84 24 90 00 00 	mov    %rax,0x90(%rsp)
    11d8:	00 
    11d9:	4c 8d 1d 43 10 00 00 	lea    0x1043(%rip),%r11        # 0x2223
    11e0:	e9 cb 00 00 00       	jmpq   0x12b0
    11e5:	4c 89 94 24 98 00 00 	mov    %r10,0x98(%rsp)
    11ec:	00 
    11ed:	4b 8d 04 37          	lea    (%r15,%r14,1),%rax
    11f1:	31 d2                	xor    %edx,%edx
    11f3:	43 0f b6 0c 37       	movzbl (%r15,%r14,1),%ecx
    11f8:	49 83 fe 01          	cmp    $0x1,%r14
    11fc:	74 07                	je     0x1205
    11fe:	0f b6 50 01          	movzbl 0x1(%rax),%edx
    1202:	c1 e2 08             	shl    $0x8,%edx
    1205:	09 d1                	or     %edx,%ecx
    1207:	23 8c 24 c0 00 00 00 	and    0xc0(%rsp),%ecx
    120e:	48 89 8c 24 c8 00 00 	mov    %rcx,0xc8(%rsp)
    1215:	00 
    1216:	83 f9 0a             	cmp    $0xa,%ecx
    1219:	4c 0f 44 f8          	cmove  %rax,%r15
    121d:	4c 8d 1d 02 10 00 00 	lea    0x1002(%rip),%r11        # 0x2226
    1224:	48 8d 05 f8 0f 00 00 	lea    0xff8(%rip),%rax        # 0x2223
    122b:	4c 0f 44 d8          	cmove  %rax,%r11
    122f:	eb 52                	jmp    0x1283
    1231:	31 c0                	xor    %eax,%eax
    1233:	48 89 84 24 90 00 00 	mov    %rax,0x90(%rsp)
    123a:	00 
    123b:	4c 8d 1d e6 0f 00 00 	lea    0xfe6(%rip),%r11        # 0x2228
    1242:	eb 6c                	jmp    0x12b0
    1244:	48 89 d0             	mov    %rdx,%rax
    1247:	48 0f ba f0 3f       	btr    $0x3f,%rax
    124c:	48 ff c8             	dec    %rax
    124f:	4c 6b d8 15          	imul   $0x15,%rax,%r11
    1253:	48 8d 0d f6 0f 00 00 	lea    0xff6(%rip),%rcx        # 0x2250
    125a:	49 01 cb             	add    %rcx,%r11
    125d:	48 83 f8 21          	cmp    $0x21,%rax
    1261:	48 8d 8c 24 e0 00 00 	lea    0xe0(%rsp),%rcx
    1268:	00 
    1269:	4c 0f 43 d9          	cmovae %rcx,%r11
    126d:	4c 89 94 24 98 00 00 	mov    %r10,0x98(%rsp)
    1274:	00 
    1275:	48 83 84 24 b0 00 00 	addq   $0x8,0xb0(%rsp)
    127c:	00 08 
    127e:	49 39 cb             	cmp    %rcx,%r11
    1281:	74 0c                	je     0x128f
    1283:	31 c0                	xor    %eax,%eax
    1285:	48 89 84 24 90 00 00 	mov    %rax,0x90(%rsp)
    128c:	00 
    128d:	eb 21                	jmp    0x12b0
    128f:	48 89 54 24 20       	mov    %rdx,0x20(%rsp)
    1294:	4c 8d 0d 83 0f 00 00 	lea    0xf83(%rip),%r9        # 0x221e
    129b:	48 89 cf             	mov    %rcx,%rdi
    129e:	e8 2e 05 00 00       	callq  0x17d1
    12a3:	31 c0                	xor    %eax,%eax
    12a5:	48 89 84 24 90 00 00 	mov    %rax,0x90(%rsp)
    12ac:	00 
    12ad:	49 89 fb             	mov    %rdi,%r11
    12b0:	45 31 d2             	xor    %r10d,%r10d
    12b3:	c7 84 24 84 00 00 00 	movl   $0x0,0x84(%rsp)
    12ba:	00 00 00 00 
    12be:	31 c0                	xor    %eax,%eax
    12c0:	48 89 84 24 a0 00 00 	mov    %rax,0xa0(%rsp)
    12c7:	00 
    12c8:	48 8b 8c 24 88 00 00 	mov    0x88(%rsp),%rcx
    12cf:	00 
    12d0:	48 8b 94 24 a8 00 00 	mov    0xa8(%rsp),%rdx
    12d7:	00 
    12d8:	48 89 8c 24 88 00 00 	mov    %rcx,0x88(%rsp)
    12df:	00 
    12e0:	31 c0                	xor    %eax,%eax
    12e2:	0f ba e1 0a          	bt     $0xa,%ecx
    12e6:	0f 93 c0             	setae  %al
    12e9:	bf ff ff 00 00       	mov    $0xffff,%edi
    12ee:	b9 ff 00 00 00       	mov    $0xff,%ecx
    12f3:	48 0f 43 f9          	cmovae %rcx,%rdi
    12f7:	48 89 bc 24 d8 00 00 	mov    %rdi,0xd8(%rsp)
    12fe:	00 
    12ff:	48 8b 8c 24 88 00 00 	mov    0x88(%rsp),%rcx
    1306:	00 
    1307:	6a 02                	pushq  $0x2
    1309:	41 59                	pop    %r9
    130b:	49 29 c1             	sub    %rax,%r9
    130e:	0f ba e1 0c          	bt     $0xc,%ecx
    1312:	72 71                	jb     0x1385
    1314:	49 8d 7b 01          	lea    0x1(%r11),%rdi
    1318:	31 d2                	xor    %edx,%edx
    131a:	eb 06                	jmp    0x1322
    131c:	48 ff c2             	inc    %rdx
    131f:	4c 01 cf             	add    %r9,%rdi
    1322:	0f b6 6f ff          	movzbl -0x1(%rdi),%ebp
    1326:	48 85 ed             	test   %rbp,%rbp
    1329:	74 14                	je     0x133f
    132b:	0f ba e1 0b          	bt     $0xb,%ecx
    132f:	73 0a                	jae    0x133b
    1331:	48 3b 94 24 98 00 00 	cmp    0x98(%rsp),%rdx
    1338:	00 
    1339:	73 72                	jae    0x13ad
    133b:	8a 07                	mov    (%rdi),%al
    133d:	eb 33                	jmp    0x1372
    133f:	0f ba e1 0a          	bt     $0xa,%ecx
    1343:	73 45                	jae    0x138a
    1345:	0f ba e1 0b          	bt     $0xb,%ecx
    1349:	48 89 d1             	mov    %rdx,%rcx
    134c:	0f 93 c2             	setae  %dl
    134f:	8a 07                	mov    (%rdi),%al
    1351:	49 89 c8             	mov    %rcx,%r8
    1354:	48 3b 8c 24 98 00 00 	cmp    0x98(%rsp),%rcx
    135b:	00 
    135c:	0f 92 c1             	setb   %cl
    135f:	84 c0                	test   %al,%al
    1361:	74 2d                	je     0x1390
    1363:	08 ca                	or     %cl,%dl
    1365:	48 8b 8c 24 88 00 00 	mov    0x88(%rsp),%rcx
    136c:	00 
    136d:	4c 89 c2             	mov    %r8,%rdx
    1370:	74 37                	je     0x13a9
    1372:	0f b6 c0             	movzbl %al,%eax
    1375:	c1 e0 08             	shl    $0x8,%eax
    1378:	09 e8                	or     %ebp,%eax
    137a:	85 84 24 d8 00 00 00 	test   %eax,0xd8(%rsp)
    1381:	75 99                	jne    0x131c
    1383:	eb 28                	jmp    0x13ad
    1385:	49 f7 d9             	neg    %r9
    1388:	eb 2b                	jmp    0x13b5
    138a:	6a 01                	pushq  $0x1
    138c:	41 59                	pop    %r9
    138e:	eb 1d                	jmp    0x13ad
    1390:	6a 02                	pushq  $0x2
    1392:	41 59                	pop    %r9
    1394:	48 8b ac 24 b8 00 00 	mov    0xb8(%rsp),%rbp
    139b:	00 
    139c:	48 8b 8c 24 88 00 00 	mov    0x88(%rsp),%rcx
    13a3:	00 
    13a4:	4c 89 c2             	mov    %r8,%rdx
    13a7:	eb 0c                	jmp    0x13b5
    13a9:	6a 02                	pushq  $0x2
    13ab:	41 59                	pop    %r9
    13ad:	48 8b ac 24 b8 00 00 	mov    0xb8(%rsp),%rbp
    13b4:	00 
    13b5:	48 89 94 24 a8 00 00 	mov    %rdx,0xa8(%rsp)
    13bc:	00 
    13bd:	48 8b 84 24 98 00 00 	mov    0x98(%rsp),%rax
    13c4:	00 
    13c5:	48 39 d0             	cmp    %rdx,%rax
    13c8:	48 0f 42 c2          	cmovb  %rdx,%rax
    13cc:	89 cf                	mov    %ecx,%edi
    13ce:	81 e7 01 02 00 00    	and    $0x201,%edi
    13d4:	48 81 ff 00 02 00 00 	cmp    $0x200,%rdi
    13db:	48 89 84 24 98 00 00 	mov    %rax,0x98(%rsp)
    13e2:	00 
    13e3:	75 67                	jne    0x144c
    13e5:	48 8b 8c 24 d0 00 00 	mov    0xd0(%rsp),%rcx
    13ec:	00 
    13ed:	48 29 c1             	sub    %rax,%rcx
    13f0:	48 89 c8             	mov    %rcx,%rax
    13f3:	49 0f af c5          	imul   %r13,%rax
    13f7:	48 01 c5             	add    %rax,%rbp
    13fa:	48 85 db             	test   %rbx,%rbx
    13fd:	74 45                	je     0x1444
    13ff:	48 8b 84 24 88 00 00 	mov    0x88(%rsp),%rax
    1406:	00 
    1407:	25 00 20 00 00       	and    $0x2000,%eax
    140c:	48 85 c0             	test   %rax,%rax
    140f:	75 33                	jne    0x1444
    1411:	31 d2                	xor    %edx,%edx
    1413:	48 8b 84 24 90 00 00 	mov    0x90(%rsp),%rax
    141a:	00 
    141b:	eb 13                	jmp    0x1430
    141d:	c6 03 20             	movb   $0x20,(%rbx)
    1420:	41 f6 c4 40          	test   $0x40,%r12b
    1424:	74 04                	je     0x142a
    1426:	c6 43 01 00          	movb   $0x0,0x1(%rbx)
    142a:	4c 01 eb             	add    %r13,%rbx
    142d:	48 ff c2             	inc    %rdx
    1430:	48 39 f3             	cmp    %rsi,%rbx
    1433:	73 05                	jae    0x143a
    1435:	48 39 ca             	cmp    %rcx,%rdx
    1438:	7c e3                	jl     0x141d
    143a:	48 8b 8c 24 88 00 00 	mov    0x88(%rsp),%rcx
    1441:	00 
    1442:	eb 10                	jmp    0x1454
    1444:	48 8b 8c 24 88 00 00 	mov    0x88(%rsp),%rcx
    144b:	00 
    144c:	48 8b 84 24 90 00 00 	mov    0x90(%rsp),%rax
    1453:	00 
    1454:	45 84 d2             	test   %r10b,%r10b
    1457:	74 53                	je     0x14ac
    1459:	84 c0                	test   %al,%al
    145b:	0f 84 d5 00 00 00    	je     0x1536
    1461:	4c 01 ed             	add    %r13,%rbp
    1464:	49 89 ca             	mov    %rcx,%r10
    1467:	49 81 e2 00 20 00 00 	and    $0x2000,%r10
    146e:	48 8b 94 24 a8 00 00 	mov    0xa8(%rsp),%rdx
    1475:	00 
    1476:	0f 85 cc 00 00 00    	jne    0x1548
    147c:	48 85 db             	test   %rbx,%rbx
    147f:	0f 84 c3 00 00 00    	je     0x1548
    1485:	b1 01                	mov    $0x1,%cl
    1487:	eb 11                	jmp    0x149a
    1489:	88 03                	mov    %al,(%rbx)
    148b:	41 f6 c4 40          	test   $0x40,%r12b
    148f:	74 04                	je     0x1495
    1491:	c6 43 01 00          	movb   $0x0,0x1(%rbx)
    1495:	4c 01 eb             	add    %r13,%rbx
    1498:	31 c9                	xor    %ecx,%ecx
    149a:	48 39 f3             	cmp    %rsi,%rbx
    149d:	73 05                	jae    0x14a4
    149f:	f6 c1 01             	test   $0x1,%cl
    14a2:	75 e5                	jne    0x1489
    14a4:	45 31 d2             	xor    %r10d,%r10d
    14a7:	e9 9c 00 00 00       	jmpq   0x1548
    14ac:	48 8b 94 24 98 00 00 	mov    0x98(%rsp),%rdx
    14b3:	00 
    14b4:	48 2b 94 24 a8 00 00 	sub    0xa8(%rsp),%rdx
    14bb:	00 
    14bc:	49 89 d0             	mov    %rdx,%r8
    14bf:	4d 0f af c5          	imul   %r13,%r8
    14c3:	49 89 ca             	mov    %rcx,%r10
    14c6:	49 81 e2 00 20 00 00 	and    $0x2000,%r10
    14cd:	75 26                	jne    0x14f5
    14cf:	48 85 db             	test   %rbx,%rbx
    14d2:	74 21                	je     0x14f5
    14d4:	31 c9                	xor    %ecx,%ecx
    14d6:	eb 13                	jmp    0x14eb
    14d8:	c6 03 20             	movb   $0x20,(%rbx)
    14db:	41 f6 c4 40          	test   $0x40,%r12b
    14df:	74 04                	je     0x14e5
    14e1:	c6 43 01 00          	movb   $0x0,0x1(%rbx)
    14e5:	4c 01 eb             	add    %r13,%rbx
    14e8:	48 ff c1             	inc    %rcx
    14eb:	48 39 f3             	cmp    %rsi,%rbx
    14ee:	73 05                	jae    0x14f5
    14f0:	48 39 d1             	cmp    %rdx,%rcx
    14f3:	7c e3                	jl     0x14d8
    14f5:	4c 01 c5             	add    %r8,%rbp
    14f8:	84 c0                	test   %al,%al
    14fa:	0f 84 a0 00 00 00    	je     0x15a0
    1500:	4c 01 ed             	add    %r13,%rbp
    1503:	4d 85 d2             	test   %r10,%r10
    1506:	0f 85 94 00 00 00    	jne    0x15a0
    150c:	48 85 db             	test   %rbx,%rbx
    150f:	0f 84 8b 00 00 00    	je     0x15a0
    1515:	b1 01                	mov    $0x1,%cl
    1517:	eb 11                	jmp    0x152a
    1519:	88 03                	mov    %al,(%rbx)
    151b:	41 f6 c4 40          	test   $0x40,%r12b
    151f:	74 04                	je     0x1525
    1521:	c6 43 01 00          	movb   $0x0,0x1(%rbx)
    1525:	4c 01 eb             	add    %r13,%rbx
    1528:	31 c9                	xor    %ecx,%ecx
    152a:	48 39 f3             	cmp    %rsi,%rbx
    152d:	73 64                	jae    0x1593
    152f:	f6 c1 01             	test   $0x1,%cl
    1532:	75 e5                	jne    0x1519
    1534:	eb 5d                	jmp    0x1593
    1536:	41 89 ca             	mov    %ecx,%r10d
    1539:	41 81 e2 00 20 00 00 	and    $0x2000,%r10d
    1540:	48 8b 94 24 a8 00 00 	mov    0xa8(%rsp),%rdx
    1547:	00 
    1548:	48 89 e8             	mov    %rbp,%rax
    154b:	48 8b 8c 24 98 00 00 	mov    0x98(%rsp),%rcx
    1552:	00 
    1553:	48 29 d1             	sub    %rdx,%rcx
    1556:	48 89 cd             	mov    %rcx,%rbp
    1559:	49 0f af ed          	imul   %r13,%rbp
    155d:	48 01 c5             	add    %rax,%rbp
    1560:	4d 85 d2             	test   %r10,%r10
    1563:	75 33                	jne    0x1598
    1565:	48 85 db             	test   %rbx,%rbx
    1568:	48 8b 84 24 90 00 00 	mov    0x90(%rsp),%rax
    156f:	00 
    1570:	74 2e                	je     0x15a0
    1572:	31 d2                	xor    %edx,%edx
    1574:	eb 13                	jmp    0x1589
    1576:	c6 03 30             	movb   $0x30,(%rbx)
    1579:	41 f6 c4 40          	test   $0x40,%r12b
    157d:	74 04                	je     0x1583
    157f:	c6 43 01 00          	movb   $0x0,0x1(%rbx)
    1583:	4c 01 eb             	add    %r13,%rbx
    1586:	48 ff c2             	inc    %rdx
    1589:	48 39 f3             	cmp    %rsi,%rbx
    158c:	73 05                	jae    0x1593
    158e:	48 39 ca             	cmp    %rcx,%rdx
    1591:	7c e3                	jl     0x1576
    1593:	45 31 d2             	xor    %r10d,%r10d
    1596:	eb 08                	jmp    0x15a0
    1598:	48 8b 84 24 90 00 00 	mov    0x90(%rsp),%rax
    159f:	00 
    15a0:	31 c9                	xor    %ecx,%ecx
    15a2:	84 c0                	test   %al,%al
    15a4:	0f 95 c1             	setne  %cl
    15a7:	e9 f2 00 00 00       	jmpq   0x169e
    15ac:	4d 85 d2             	test   %r10,%r10
    15af:	75 38                	jne    0x15e9
    15b1:	48 85 db             	test   %rbx,%rbx
    15b4:	74 33                	je     0x15e9
    15b6:	45 0f b6 43 01       	movzbl 0x1(%r11),%r8d
    15bb:	41 c1 e0 08          	shl    $0x8,%r8d
    15bf:	44 23 84 24 d8 00 00 	and    0xd8(%rsp),%r8d
    15c6:	00 
    15c7:	41 c1 e8 08          	shr    $0x8,%r8d
    15cb:	b0 01                	mov    $0x1,%al
    15cd:	eb 11                	jmp    0x15e0
    15cf:	88 0b                	mov    %cl,(%rbx)
    15d1:	41 f6 c4 40          	test   $0x40,%r12b
    15d5:	74 04                	je     0x15db
    15d7:	44 88 43 01          	mov    %r8b,0x1(%rbx)
    15db:	4c 01 eb             	add    %r13,%rbx
    15de:	31 c0                	xor    %eax,%eax
    15e0:	48 39 f3             	cmp    %rsi,%rbx
    15e3:	73 04                	jae    0x15e9
    15e5:	a8 01                	test   $0x1,%al
    15e7:	75 e6                	jne    0x15cf
    15e9:	4c 01 ed             	add    %r13,%rbp
    15ec:	4d 01 cb             	add    %r9,%r11
    15ef:	48 8d 4a 01          	lea    0x1(%rdx),%rcx
    15f3:	80 bc 24 84 00 00 00 	cmpb   $0x0,0x84(%rsp)
    15fa:	00 
    15fb:	0f 84 9d 00 00 00    	je     0x169e
    1601:	48 8b 84 24 a0 00 00 	mov    0xa0(%rsp),%rax
    1608:	00 
    1609:	49 89 f8             	mov    %rdi,%r8
    160c:	48 89 c7             	mov    %rax,%rdi
    160f:	48 ff c7             	inc    %rdi
    1612:	48 89 f8             	mov    %rdi,%rax
    1615:	48 89 bc 24 a0 00 00 	mov    %rdi,0xa0(%rsp)
    161c:	00 
    161d:	48 83 ff 03          	cmp    $0x3,%rdi
    1621:	4c 89 c7             	mov    %r8,%rdi
    1624:	75 78                	jne    0x169e
    1626:	48 83 c2 02          	add    $0x2,%rdx
    162a:	48 89 d1             	mov    %rdx,%rcx
    162d:	b8 00 00 00 00       	mov    $0x0,%eax
    1632:	48 89 84 24 a0 00 00 	mov    %rax,0xa0(%rsp)
    1639:	00 
    163a:	48 3b 94 24 a8 00 00 	cmp    0xa8(%rsp),%rdx
    1641:	00 
    1642:	73 5a                	jae    0x169e
    1644:	4c 01 ed             	add    %r13,%rbp
    1647:	48 89 d1             	mov    %rdx,%rcx
    164a:	b8 00 00 00 00       	mov    $0x0,%eax
    164f:	48 89 84 24 a0 00 00 	mov    %rax,0xa0(%rsp)
    1656:	00 
    1657:	4d 85 d2             	test   %r10,%r10
    165a:	75 42                	jne    0x169e
    165c:	48 89 d1             	mov    %rdx,%rcx
    165f:	b8 00 00 00 00       	mov    $0x0,%eax
    1664:	48 89 84 24 a0 00 00 	mov    %rax,0xa0(%rsp)
    166b:	00 
    166c:	48 85 db             	test   %rbx,%rbx
    166f:	74 2d                	je     0x169e
    1671:	b1 01                	mov    $0x1,%cl
    1673:	eb 12                	jmp    0x1687
    1675:	c6 03 2c             	movb   $0x2c,(%rbx)
    1678:	41 f6 c4 40          	test   $0x40,%r12b
    167c:	74 04                	je     0x1682
    167e:	c6 43 01 00          	movb   $0x0,0x1(%rbx)
    1682:	4c 01 eb             	add    %r13,%rbx
    1685:	31 c9                	xor    %ecx,%ecx
    1687:	48 39 f3             	cmp    %rsi,%rbx
    168a:	73 05                	jae    0x1691
    168c:	f6 c1 01             	test   $0x1,%cl
    168f:	75 e4                	jne    0x1675
    1691:	48 89 d1             	mov    %rdx,%rcx
    1694:	31 c0                	xor    %eax,%eax
    1696:	48 89 84 24 a0 00 00 	mov    %rax,0xa0(%rsp)
    169d:	00 
    169e:	48 3b 8c 24 a8 00 00 	cmp    0xa8(%rsp),%rcx
    16a5:	00 
    16a6:	73 1f                	jae    0x16c7
    16a8:	48 89 ca             	mov    %rcx,%rdx
    16ab:	41 8a 0b             	mov    (%r11),%cl
    16ae:	84 c9                	test   %cl,%cl
    16b0:	0f 85 f6 fe ff ff    	jne    0x15ac
    16b6:	49 83 f9 02          	cmp    $0x2,%r9
    16ba:	7c 0b                	jl     0x16c7
    16bc:	41 80 7b 01 00       	cmpb   $0x0,0x1(%r11)
    16c1:	0f 85 e5 fe ff ff    	jne    0x15ac
    16c7:	48 81 ff 01 02 00 00 	cmp    $0x201,%rdi
    16ce:	75 55                	jne    0x1725
    16d0:	48 8b 8c 24 d0 00 00 	mov    0xd0(%rsp),%rcx
    16d7:	00 
    16d8:	48 2b 8c 24 98 00 00 	sub    0x98(%rsp),%rcx
    16df:	00 
    16e0:	48 89 c8             	mov    %rcx,%rax
    16e3:	49 0f af c5          	imul   %r13,%rax
    16e7:	48 01 c5             	add    %rax,%rbp
    16ea:	4d 85 d2             	test   %r10,%r10
    16ed:	75 36                	jne    0x1725
    16ef:	48 85 db             	test   %rbx,%rbx
    16f2:	74 31                	je     0x1725
    16f4:	31 c0                	xor    %eax,%eax
    16f6:	4c 8d 05 5f 02 00 00 	lea    0x25f(%rip),%r8        # 0x195c
    16fd:	4c 8d 1d 3c 0b 00 00 	lea    0xb3c(%rip),%r11        # 0x2240
    1704:	eb 13                	jmp    0x1719
    1706:	c6 03 20             	movb   $0x20,(%rbx)
    1709:	41 f6 c4 40          	test   $0x40,%r12b
    170d:	74 04                	je     0x1713
    170f:	c6 43 01 00          	movb   $0x0,0x1(%rbx)
    1713:	4c 01 eb             	add    %r13,%rbx
    1716:	48 ff c0             	inc    %rax
    1719:	48 39 f3             	cmp    %rsi,%rbx
    171c:	73 15                	jae    0x1733
    171e:	48 39 c8             	cmp    %rcx,%rax
    1721:	7c e3                	jl     0x1706
    1723:	eb 0e                	jmp    0x1733
    1725:	4c 8d 05 30 02 00 00 	lea    0x230(%rip),%r8        # 0x195c
    172c:	4c 8d 1d 0d 0b 00 00 	lea    0xb0d(%rip),%r11        # 0x2240
    1733:	31 c0                	xor    %eax,%eax
    1735:	43 0f b6 14 37       	movzbl (%r15,%r14,1),%edx
    173a:	4b 8d 0c 37          	lea    (%r15,%r14,1),%rcx
    173e:	49 83 fe 01          	cmp    $0x1,%r14
    1742:	74 07                	je     0x174b
    1744:	0f b6 41 01          	movzbl 0x1(%rcx),%eax
    1748:	c1 e0 08             	shl    $0x8,%eax
    174b:	09 d0                	or     %edx,%eax
    174d:	e9 37 f2 ff ff       	jmpq   0x989
    1752:	31 c0                	xor    %eax,%eax
    1754:	48 89 84 24 90 00 00 	mov    %rax,0x90(%rsp)
    175b:	00 
    175c:	4c 8d 1d d1 0a 00 00 	lea    0xad1(%rip),%r11        # 0x2234
    1763:	e9 48 fb ff ff       	jmpq   0x12b0
    1768:	0f ba a4 24 88 00 00 	btl    $0xd,0x88(%rsp)
    176f:	00 0d 
    1771:	72 3b                	jb     0x17ae
    1773:	4c 01 ee             	add    %r13,%rsi
    1776:	b1 01                	mov    $0x1,%cl
    1778:	48 89 d8             	mov    %rbx,%rax
    177b:	eb 17                	jmp    0x1794
    177d:	f6 c1 01             	test   $0x1,%cl
    1780:	74 17                	je     0x1799
    1782:	c6 00 00             	movb   $0x0,(%rax)
    1785:	41 f6 c4 40          	test   $0x40,%r12b
    1789:	74 04                	je     0x178f
    178b:	c6 40 01 00          	movb   $0x0,0x1(%rax)
    178f:	4c 01 e8             	add    %r13,%rax
    1792:	31 c9                	xor    %ecx,%ecx
    1794:	48 39 f0             	cmp    %rsi,%rax
    1797:	72 e4                	jb     0x177d
    1799:	48 2b 9c 24 08 01 00 	sub    0x108(%rsp),%rbx
    17a0:	00 
    17a1:	48 89 d8             	mov    %rbx,%rax
    17a4:	48 99                	cqto   
    17a6:	49 f7 fd             	idiv   %r13
    17a9:	48 89 c5             	mov    %rax,%rbp
    17ac:	eb 0e                	jmp    0x17bc
    17ae:	41 83 e4 40          	and    $0x40,%r12d
    17b2:	41 c1 ec 06          	shr    $0x6,%r12d
    17b6:	44 89 e1             	mov    %r12d,%ecx
    17b9:	48 d3 ed             	shr    %cl,%rbp
    17bc:	48 89 e8             	mov    %rbp,%rax
    17bf:	48 81 c4 18 01 00 00 	add    $0x118,%rsp
    17c6:	5b                   	pop    %rbx
    17c7:	41 5c                	pop    %r12
    17c9:	41 5d                	pop    %r13
    17cb:	41 5e                	pop    %r14
    17cd:	41 5f                	pop    %r15
    17cf:	5d                   	pop    %rbp
    17d0:	c3                   	retq   
    17d1:	56                   	push   %rsi
    17d2:	57                   	push   %rdi
    17d3:	50                   	push   %rax
    17d4:	4c 8d 44 24 40       	lea    0x40(%rsp),%r8
    17d9:	4c 89 04 24          	mov    %r8,(%rsp)
    17dd:	6a 26                	pushq  $0x26
    17df:	5e                   	pop    %rsi
    17e0:	31 d2                	xor    %edx,%edx
    17e2:	48 89 cf             	mov    %rcx,%rdi
    17e5:	4c 89 c9             	mov    %r9,%rcx
    17e8:	e8 ab f0 ff ff       	callq  0x898
    17ed:	48 83 c4 08          	add    $0x8,%rsp
    17f1:	5f                   	pop    %rdi
    17f2:	5e                   	pop    %rsi
    17f3:	c3                   	retq   
    17f4:	56                   	push   %rsi
    17f5:	57                   	push   %rdi
    17f6:	50                   	push   %rax
    17f7:	4c 8d 44 24 38       	lea    0x38(%rsp),%r8
    17fc:	4d 89 08             	mov    %r9,(%r8)
    17ff:	4c 89 04 24          	mov    %r8,(%rsp)
    1803:	48 8d 05 9c 09 00 00 	lea    0x99c(%rip),%rax        # 0x21a6
    180a:	be 00 01 00 00       	mov    $0x100,%esi
    180f:	31 d2                	xor    %edx,%edx
    1811:	48 89 cf             	mov    %rcx,%rdi
    1814:	48 89 c1             	mov    %rax,%rcx
    1817:	e8 7c f0 ff ff       	callq  0x898
    181c:	48 83 c4 08          	add    $0x8,%rsp
    1820:	5f                   	pop    %rdi
    1821:	5e                   	pop    %rsi
    1822:	c3                   	retq   
    1823:	41 57                	push   %r15
    1825:	41 56                	push   %r14
    1827:	41 54                	push   %r12
    1829:	56                   	push   %rsi
    182a:	57                   	push   %rdi
    182b:	53                   	push   %rbx
    182c:	48 83 ec 38          	sub    $0x38,%rsp
    1830:	49 89 cf             	mov    %rcx,%r15
    1833:	4c 8d 64 24 78       	lea    0x78(%rsp),%r12
    1838:	4d 89 4c 24 10       	mov    %r9,0x10(%r12)
    183d:	4d 89 44 24 08       	mov    %r8,0x8(%r12)
    1842:	49 89 14 24          	mov    %rdx,(%r12)
    1846:	4c 89 64 24 30       	mov    %r12,0x30(%rsp)
    184b:	48 8b 05 5e 0e 00 00 	mov    0xe5e(%rip),%rax        # 0x26b0
    1852:	4c 8b 70 40          	mov    0x40(%rax),%r14
    1856:	48 8b 05 5b 0e 00 00 	mov    0xe5b(%rip),%rax        # 0x26b8
    185d:	6a 04                	pushq  $0x4
    185f:	59                   	pop    %rcx
    1860:	4c 8d 44 24 28       	lea    0x28(%rsp),%r8
    1865:	ba 82 02 00 00       	mov    $0x282,%edx
    186a:	ff 50 40             	callq  *0x40(%rax)
    186d:	48 85 c0             	test   %rax,%rax
    1870:	78 07                	js     0x1879
    1872:	48 8b 5c 24 28       	mov    0x28(%rsp),%rbx
    1877:	eb 08                	jmp    0x1881
    1879:	48 83 64 24 28 00    	andq   $0x0,0x28(%rsp)
    187f:	31 db                	xor    %ebx,%ebx
    1881:	be 41 01 00 00       	mov    $0x141,%esi
    1886:	ba 40 01 00 00       	mov    $0x140,%edx
    188b:	48 89 df             	mov    %rbx,%rdi
    188e:	4c 89 f9             	mov    %r15,%rcx
    1891:	4d 89 e0             	mov    %r12,%r8
    1894:	e8 ff ef ff ff       	callq  0x898
    1899:	4d 85 f6             	test   %r14,%r14
    189c:	74 0f                	je     0x18ad
    189e:	48 85 c0             	test   %rax,%rax
    18a1:	74 0a                	je     0x18ad
    18a3:	4c 89 f1             	mov    %r14,%rcx
    18a6:	48 89 da             	mov    %rbx,%rdx
    18a9:	41 ff 56 08          	callq  *0x8(%r14)
    18ad:	48 8b 05 04 0e 00 00 	mov    0xe04(%rip),%rax        # 0x26b8
    18b4:	48 89 d9             	mov    %rbx,%rcx
    18b7:	ff 50 48             	callq  *0x48(%rax)
    18ba:	48 83 c4 38          	add    $0x38,%rsp
    18be:	5b                   	pop    %rbx
    18bf:	5f                   	pop    %rdi
    18c0:	5e                   	pop    %rsi
    18c1:	41 5c                	pop    %r12
    18c3:	41 5e                	pop    %r14
    18c5:	41 5f                	pop    %r15
    18c7:	c3                   	retq   
    18c8:	48 83 ec 28          	sub    $0x28,%rsp
    18cc:	48 89 0d d5 0d 00 00 	mov    %rcx,0xdd5(%rip)        # 0x26a8
    18d3:	48 89 15 d6 0d 00 00 	mov    %rdx,0xdd6(%rip)        # 0x26b0
    18da:	48 8b 42 60          	mov    0x60(%rdx),%rax
    18de:	48 89 05 d3 0d 00 00 	mov    %rax,0xdd3(%rip)        # 0x26b8
    18e5:	48 8b 42 58          	mov    0x58(%rdx),%rax
    18e9:	48 89 05 d0 0d 00 00 	mov    %rax,0xdd0(%rip)        # 0x26c0
    18f0:	e8 8e e9 ff ff       	callq  0x283
    18f5:	48 83 ec 20          	sub    $0x20,%rsp
    18f9:	0f 1f 00             	nopl   (%rax)
    18fc:	de f4                	fdivp  %st,%st(4)
    18fe:	ff                   	(bad)  
    18ff:	ff b4 f4 ff ff f8 f2 	pushq  -0xd070001(%rsp,%rsi,8)
    1906:	ff                   	(bad)  
    1907:	ff 33                	pushq  (%rbx)
    1909:	f6 ff                	idiv   %bh
    190b:	ff b4 f4 ff ff b4 f4 	pushq  -0xb4b0001(%rsp,%rsi,8)
    1912:	ff                   	(bad)  
    1913:	ff 4d f5             	decl   -0xb(%rbp)
    1916:	ff                   	(bad)  
    1917:	ff b4 f4 ff ff b4 f4 	pushq  -0xb4b0001(%rsp,%rsi,8)
    191e:	ff                   	(bad)  
    191f:	ff b4 f4 ff ff b4 f4 	pushq  -0xb4b0001(%rsp,%rsi,8)
    1926:	ff                   	(bad)  
    1927:	ff b4 f4 ff ff b4 f4 	pushq  -0xb4b0001(%rsp,%rsi,8)
    192e:	ff                   	(bad)  
    192f:	ff b4 f4 ff ff b4 f4 	pushq  -0xb4b0001(%rsp,%rsi,8)
    1936:	ff                   	(bad)  
    1937:	ff 0e                	decl   (%rsi)
    1939:	f6 ff                	idiv   %bh
    193b:	ff b4 f4 ff ff 02 f8 	pushq  -0x7fd0001(%rsp,%rsi,8)
    1942:	ff                   	(bad)  
    1943:	ff d7                	callq  *%rdi
    1945:	f4                   	hlt    
    1946:	ff                   	(bad)  
    1947:	ff 2d f8 ff ff 21    	ljmp   *0x21fffff8(%rip)        # 0x22001945
    194d:	f6 ff                	idiv   %bh
    194f:	ff b4 f4 ff ff b4 f4 	pushq  -0xb4b0001(%rsp,%rsi,8)
    1956:	ff                   	(bad)  
    1957:	ff 1a                	lcall  *(%rdx)
    1959:	f6 ff                	idiv   %bh
    195b:	ff 9b f1 ff ff c9    	lcall  *-0x3600000f(%rbx)
    1961:	f1                   	icebp  
    1962:	ff                   	(bad)  
    1963:	ff d6                	callq  *%rsi
    1965:	f1                   	icebp  
    1966:	ff                   	(bad)  
    1967:	ff e3                	jmpq   *%rbx
    1969:	f1                   	icebp  
    196a:	ff                   	(bad)  
    196b:	ff f0                	push   %rax
    196d:	f1                   	icebp  
    196e:	ff                   	(bad)  
    196f:	ff 53 f2             	callq  *-0xe(%rbx)
    1972:	ff                   	(bad)  
    1973:	ff 00                	incl   (%rax)
    1975:	f2 ff                	repnz (bad) 
    1977:	ff                   	(bad)  
    1978:	ea                   	(bad)  
    1979:	f0 ff                	lock (bad) 
    197b:	ff                   	(bad)  
    197c:	ea                   	(bad)  
    197d:	f0 ff                	lock (bad) 
    197f:	ff                   	(bad)  
    1980:	ea                   	(bad)  
    1981:	f0 ff                	lock (bad) 
    1983:	ff                   	(bad)  
    1984:	ea                   	(bad)  
    1985:	f0 ff                	lock (bad) 
    1987:	ff                   	(bad)  
    1988:	ea                   	(bad)  
    1989:	f0 ff                	lock (bad) 
    198b:	ff                   	(bad)  
    198c:	ea                   	(bad)  
    198d:	f0 ff                	lock (bad) 
    198f:	ff                   	(bad)  
    1990:	ea                   	(bad)  
    1991:	f0 ff                	lock (bad) 
    1993:	ff                   	(bad)  
    1994:	ea                   	(bad)  
    1995:	f0 ff                	lock (bad) 
    1997:	ff                   	(bad)  
    1998:	ea                   	(bad)  
    1999:	f0 ff                	lock (bad) 
    199b:	ff 53 f2             	callq  *-0xe(%rbx)
    199e:	ff                   	(bad)  
    199f:	ff 53 f2             	callq  *-0xe(%rbx)
    19a2:	ff                   	(bad)  
    19a3:	ff 53 f2             	callq  *-0xe(%rbx)
    19a6:	ff                   	(bad)  
    19a7:	ff 53 f2             	callq  *-0xe(%rbx)
    19aa:	ff                   	(bad)  
    19ab:	ff 53 f2             	callq  *-0xe(%rbx)
    19ae:	ff                   	(bad)  
    19af:	ff 53 f2             	callq  *-0xe(%rbx)
    19b2:	ff                   	(bad)  
    19b3:	ff 53 f2             	callq  *-0xe(%rbx)
    19b6:	ff                   	(bad)  
    19b7:	ff 53 f2             	callq  *-0xe(%rbx)
    19ba:	ff                   	(bad)  
    19bb:	ff 53 f2             	callq  *-0xe(%rbx)
    19be:	ff                   	(bad)  
    19bf:	ff 53 f2             	callq  *-0xe(%rbx)
    19c2:	ff                   	(bad)  
    19c3:	ff 53 f2             	callq  *-0xe(%rbx)
    19c6:	ff                   	(bad)  
    19c7:	ff 53 f2             	callq  *-0xe(%rbx)
    19ca:	ff                   	(bad)  
    19cb:	ff 53 f2             	callq  *-0xe(%rbx)
    19ce:	ff                   	(bad)  
    19cf:	ff 53 f2             	callq  *-0xe(%rbx)
    19d2:	ff                   	(bad)  
    19d3:	ff 53 f2             	callq  *-0xe(%rbx)
    19d6:	ff                   	(bad)  
    19d7:	ff 53 f2             	callq  *-0xe(%rbx)
    19da:	ff                   	(bad)  
    19db:	ff 53 f2             	callq  *-0xe(%rbx)
    19de:	ff                   	(bad)  
    19df:	ff 53 f2             	callq  *-0xe(%rbx)
    19e2:	ff                   	(bad)  
    19e3:	ff 8b f1 ff ff 49    	decl   0x49fffff1(%rbx)
    19e9:	00 6e 00             	add    %ch,0x0(%rsi)
    19ec:	76 00                	jbe    0x19ee
    19ee:	61                   	(bad)  
    19ef:	00 6c 00 69          	add    %ch,0x69(%rax,%rax,1)
    19f3:	00 64 00 4d          	add    %ah,0x4d(%rax,%rax,1)
    19f7:	00 65 00             	add    %ah,0x0(%rbp)
    19fa:	6d                   	insl   (%dx),%es:(%rdi)
    19fb:	00 6f 00             	add    %ch,0x0(%rdi)
    19fe:	72 00                	jb     0x1a00
    1a00:	79 00                	jns    0x1a02
    1a02:	54                   	push   %rsp
    1a03:	00 79 00             	add    %bh,0x0(%rcx)
    1a06:	70 00                	jo     0x1a08
    1a08:	65 00 00             	add    %al,%gs:(%rax)
    1a0b:	00 45 00             	add    %al,0x0(%rbp)
    1a0e:	66 00 69 00          	data16 add %ch,0x0(%rcx)
    1a12:	52                   	push   %rdx
    1a13:	00 65 00             	add    %ah,0x0(%rbp)
    1a16:	73 00                	jae    0x1a18
    1a18:	65 00 72 00          	add    %dh,%gs:0x0(%rdx)
    1a1c:	76 00                	jbe    0x1a1e
    1a1e:	65 00 64 00 4d       	add    %ah,%gs:0x4d(%rax,%rax,1)
    1a23:	00 65 00             	add    %ah,0x0(%rbp)
    1a26:	6d                   	insl   (%dx),%es:(%rdi)
    1a27:	00 6f 00             	add    %ch,0x0(%rdi)
    1a2a:	72 00                	jb     0x1a2c
    1a2c:	79 00                	jns    0x1a2e
    1a2e:	54                   	push   %rsp
    1a2f:	00 79 00             	add    %bh,0x0(%rcx)
    1a32:	70 00                	jo     0x1a34
    1a34:	65 00 00             	add    %al,%gs:(%rax)
    1a37:	00 45 00             	add    %al,0x0(%rbp)
    1a3a:	66 00 69 00          	data16 add %ch,0x0(%rcx)
    1a3e:	4c 00 6f 00          	rex.WR add %r13b,0x0(%rdi)
    1a42:	61                   	(bad)  
    1a43:	00 64 00 65          	add    %ah,0x65(%rax,%rax,1)
    1a47:	00 72 00             	add    %dh,0x0(%rdx)
    1a4a:	43 00 6f 00          	rex.XB add %bpl,0x0(%r15)
    1a4e:	64 00 65 00          	add    %ah,%fs:0x0(%rbp)
    1a52:	00 00                	add    %al,(%rax)
    1a54:	45 00 66 00          	add    %r12b,0x0(%r14)
    1a58:	69 00 4c 00 6f 00    	imul   $0x6f004c,(%rax),%eax
    1a5e:	61                   	(bad)  
    1a5f:	00 64 00 65          	add    %ah,0x65(%rax,%rax,1)
    1a63:	00 72 00             	add    %dh,0x0(%rdx)
    1a66:	44 00 61 00          	add    %r12b,0x0(%rcx)
    1a6a:	74 00                	je     0x1a6c
    1a6c:	61                   	(bad)  
    1a6d:	00 00                	add    %al,(%rax)
    1a6f:	00 45 00             	add    %al,0x0(%rbp)
    1a72:	66 00 69 00          	data16 add %ch,0x0(%rcx)
    1a76:	42 00 6f 00          	rex.X add %bpl,0x0(%rdi)
    1a7a:	6f                   	outsl  %ds:(%rsi),(%dx)
    1a7b:	00 74 00 53          	add    %dh,0x53(%rax,%rax,1)
    1a7f:	00 65 00             	add    %ah,0x0(%rbp)
    1a82:	72 00                	jb     0x1a84
    1a84:	76 00                	jbe    0x1a86
    1a86:	69 00 63 00 65 00    	imul   $0x650063,(%rax),%eax
    1a8c:	73 00                	jae    0x1a8e
    1a8e:	43 00 6f 00          	rex.XB add %bpl,0x0(%r15)
    1a92:	64 00 65 00          	add    %ah,%fs:0x0(%rbp)
    1a96:	00 00                	add    %al,(%rax)
    1a98:	45 00 66 00          	add    %r12b,0x0(%r14)
    1a9c:	69 00 42 00 6f 00    	imul   $0x6f0042,(%rax),%eax
    1aa2:	6f                   	outsl  %ds:(%rsi),(%dx)
    1aa3:	00 74 00 53          	add    %dh,0x53(%rax,%rax,1)
    1aa7:	00 65 00             	add    %ah,0x0(%rbp)
    1aaa:	72 00                	jb     0x1aac
    1aac:	76 00                	jbe    0x1aae
    1aae:	69 00 63 00 65 00    	imul   $0x650063,(%rax),%eax
    1ab4:	73 00                	jae    0x1ab6
    1ab6:	44 00 61 00          	add    %r12b,0x0(%rcx)
    1aba:	74 00                	je     0x1abc
    1abc:	61                   	(bad)  
    1abd:	00 00                	add    %al,(%rax)
    1abf:	00 45 00             	add    %al,0x0(%rbp)
    1ac2:	66 00 69 00          	data16 add %ch,0x0(%rcx)
    1ac6:	52                   	push   %rdx
    1ac7:	00 75 00             	add    %dh,0x0(%rbp)
    1aca:	6e                   	outsb  %ds:(%rsi),(%dx)
    1acb:	00 74 00 69          	add    %dh,0x69(%rax,%rax,1)
    1acf:	00 6d 00             	add    %ch,0x0(%rbp)
    1ad2:	65 00 53 00          	add    %dl,%gs:0x0(%rbx)
    1ad6:	65 00 72 00          	add    %dh,%gs:0x0(%rdx)
    1ada:	76 00                	jbe    0x1adc
    1adc:	69 00 63 00 65 00    	imul   $0x650063,(%rax),%eax
    1ae2:	73 00                	jae    0x1ae4
    1ae4:	43 00 6f 00          	rex.XB add %bpl,0x0(%r15)
    1ae8:	64 00 65 00          	add    %ah,%fs:0x0(%rbp)
    1aec:	00 00                	add    %al,(%rax)
    1aee:	45 00 66 00          	add    %r12b,0x0(%r14)
    1af2:	69 00 52 00 75 00    	imul   $0x750052,(%rax),%eax
    1af8:	6e                   	outsb  %ds:(%rsi),(%dx)
    1af9:	00 74 00 69          	add    %dh,0x69(%rax,%rax,1)
    1afd:	00 6d 00             	add    %ch,0x0(%rbp)
    1b00:	65 00 53 00          	add    %dl,%gs:0x0(%rbx)
    1b04:	65 00 72 00          	add    %dh,%gs:0x0(%rdx)
    1b08:	76 00                	jbe    0x1b0a
    1b0a:	69 00 63 00 65 00    	imul   $0x650063,(%rax),%eax
    1b10:	73 00                	jae    0x1b12
    1b12:	44 00 61 00          	add    %r12b,0x0(%rcx)
    1b16:	74 00                	je     0x1b18
    1b18:	61                   	(bad)  
    1b19:	00 00                	add    %al,(%rax)
    1b1b:	00 45 00             	add    %al,0x0(%rbp)
    1b1e:	66 00 69 00          	data16 add %ch,0x0(%rcx)
    1b22:	43 00 6f 00          	rex.XB add %bpl,0x0(%r15)
    1b26:	6e                   	outsb  %ds:(%rsi),(%dx)
    1b27:	00 76 00             	add    %dh,0x0(%rsi)
    1b2a:	65 00 6e 00          	add    %ch,%gs:0x0(%rsi)
    1b2e:	74 00                	je     0x1b30
    1b30:	69 00 6f 00 6e 00    	imul   $0x6e006f,(%rax),%eax
    1b36:	61                   	(bad)  
    1b37:	00 6c 00 4d          	add    %ch,0x4d(%rax,%rax,1)
    1b3b:	00 65 00             	add    %ah,0x0(%rbp)
    1b3e:	6d                   	insl   (%dx),%es:(%rdi)
    1b3f:	00 6f 00             	add    %ch,0x0(%rdi)
    1b42:	72 00                	jb     0x1b44
    1b44:	79 00                	jns    0x1b46
    1b46:	00 00                	add    %al,(%rax)
    1b48:	45 00 66 00          	add    %r12b,0x0(%r14)
    1b4c:	69 00 55 00 6e 00    	imul   $0x6e0055,(%rax),%eax
    1b52:	75 00                	jne    0x1b54
    1b54:	73 00                	jae    0x1b56
    1b56:	61                   	(bad)  
    1b57:	00 62 00             	add    %ah,0x0(%rdx)
    1b5a:	6c                   	insb   (%dx),%es:(%rdi)
    1b5b:	00 65 00             	add    %ah,0x0(%rbp)
    1b5e:	4d 00 65 00          	rex.WRB add %r12b,0x0(%r13)
    1b62:	6d                   	insl   (%dx),%es:(%rdi)
    1b63:	00 6f 00             	add    %ch,0x0(%rdi)
    1b66:	72 00                	jb     0x1b68
    1b68:	79 00                	jns    0x1b6a
    1b6a:	00 00                	add    %al,(%rax)
    1b6c:	45 00 66 00          	add    %r12b,0x0(%r14)
    1b70:	69 00 41 00 43 00    	imul   $0x430041,(%rax),%eax
    1b76:	50                   	push   %rax
    1b77:	00 49 00             	add    %cl,0x0(%rcx)
    1b7a:	52                   	push   %rdx
    1b7b:	00 65 00             	add    %ah,0x0(%rbp)
    1b7e:	63 00                	movslq (%rax),%eax
    1b80:	6c                   	insb   (%dx),%es:(%rdi)
    1b81:	00 61 00             	add    %ah,0x0(%rcx)
    1b84:	69 00 6d 00 4d 00    	imul   $0x4d006d,(%rax),%eax
    1b8a:	65 00 6d 00          	add    %ch,%gs:0x0(%rbp)
    1b8e:	6f                   	outsl  %ds:(%rsi),(%dx)
    1b8f:	00 72 00             	add    %dh,0x0(%rdx)
    1b92:	79 00                	jns    0x1b94
    1b94:	00 00                	add    %al,(%rax)
    1b96:	45 00 66 00          	add    %r12b,0x0(%r14)
    1b9a:	69 00 41 00 43 00    	imul   $0x430041,(%rax),%eax
    1ba0:	50                   	push   %rax
    1ba1:	00 49 00             	add    %cl,0x0(%rcx)
    1ba4:	4d 00 65 00          	rex.WRB add %r12b,0x0(%r13)
    1ba8:	6d                   	insl   (%dx),%es:(%rdi)
    1ba9:	00 6f 00             	add    %ch,0x0(%rdi)
    1bac:	72 00                	jb     0x1bae
    1bae:	79 00                	jns    0x1bb0
    1bb0:	4e 00 56 00          	rex.WRX add %r10b,0x0(%rsi)
    1bb4:	53                   	push   %rbx
    1bb5:	00 00                	add    %al,(%rax)
    1bb7:	00 45 00             	add    %al,0x0(%rbp)
    1bba:	66 00 69 00          	data16 add %ch,0x0(%rcx)
    1bbe:	4d 00 65 00          	rex.WRB add %r12b,0x0(%r13)
    1bc2:	6d                   	insl   (%dx),%es:(%rdi)
    1bc3:	00 6f 00             	add    %ch,0x0(%rdi)
    1bc6:	72 00                	jb     0x1bc8
    1bc8:	79 00                	jns    0x1bca
    1bca:	4d 00 61 00          	rex.WRB add %r12b,0x0(%r9)
    1bce:	70 00                	jo     0x1bd0
    1bd0:	70 00                	jo     0x1bd2
    1bd2:	65 00 64 00 49       	add    %ah,%gs:0x49(%rax,%rax,1)
    1bd7:	00 4f 00             	add    %cl,0x0(%rdi)
    1bda:	00 00                	add    %al,(%rax)
    1bdc:	45 00 66 00          	add    %r12b,0x0(%r14)
    1be0:	69 00 4d 00 65 00    	imul   $0x65004d,(%rax),%eax
    1be6:	6d                   	insl   (%dx),%es:(%rdi)
    1be7:	00 6f 00             	add    %ch,0x0(%rdi)
    1bea:	72 00                	jb     0x1bec
    1bec:	79 00                	jns    0x1bee
    1bee:	4d 00 61 00          	rex.WRB add %r12b,0x0(%r9)
    1bf2:	70 00                	jo     0x1bf4
    1bf4:	70 00                	jo     0x1bf6
    1bf6:	65 00 64 00 49       	add    %ah,%gs:0x49(%rax,%rax,1)
    1bfb:	00 4f 00             	add    %cl,0x0(%rdi)
    1bfe:	50                   	push   %rax
    1bff:	00 6f 00             	add    %ch,0x0(%rdi)
    1c02:	72 00                	jb     0x1c04
    1c04:	74 00                	je     0x1c06
    1c06:	53                   	push   %rbx
    1c07:	00 70 00             	add    %dh,0x0(%rax)
    1c0a:	61                   	(bad)  
    1c0b:	00 63 00             	add    %ah,0x0(%rbx)
    1c0e:	65 00 00             	add    %al,%gs:(%rax)
    1c11:	00 45 00             	add    %al,0x0(%rbp)
    1c14:	66 00 69 00          	data16 add %ch,0x0(%rcx)
    1c18:	50                   	push   %rax
    1c19:	00 61 00             	add    %ah,0x0(%rcx)
    1c1c:	6c                   	insb   (%dx),%es:(%rdi)
    1c1d:	00 43 00             	add    %al,0x0(%rbx)
    1c20:	6f                   	outsl  %ds:(%rsi),(%dx)
    1c21:	00 64 00 65          	add    %ah,0x65(%rax,%rax,1)
    1c25:	00 00                	add    %al,(%rax)
    1c27:	00 45 00             	add    %al,0x0(%rbp)
    1c2a:	66 00 69 00          	data16 add %ch,0x0(%rcx)
    1c2e:	50                   	push   %rax
    1c2f:	00 65 00             	add    %ah,0x0(%rbp)
    1c32:	72 00                	jb     0x1c34
    1c34:	73 00                	jae    0x1c36
    1c36:	69 00 73 00 74 00    	imul   $0x740073,(%rax),%eax
    1c3c:	65 00 6e 00          	add    %ch,%gs:0x0(%rsi)
    1c40:	74 00                	je     0x1c42
    1c42:	4d 00 65 00          	rex.WRB add %r12b,0x0(%r13)
    1c46:	6d                   	insl   (%dx),%es:(%rdi)
    1c47:	00 6f 00             	add    %ch,0x0(%rdi)
    1c4a:	72 00                	jb     0x1c4c
    1c4c:	79 00                	jns    0x1c4e
    1c4e:	00 00                	add    %al,(%rax)
    1c50:	45 00 66 00          	add    %r12b,0x0(%r14)
    1c54:	69 00 4d 00 61 00    	imul   $0x61004d,(%rax),%eax
    1c5a:	78 00                	js     0x1c5c
    1c5c:	4d 00 65 00          	rex.WRB add %r12b,0x0(%r13)
    1c60:	6d                   	insl   (%dx),%es:(%rdi)
    1c61:	00 6f 00             	add    %ch,0x0(%rdi)
    1c64:	72 00                	jb     0x1c66
    1c66:	79 00                	jns    0x1c68
    1c68:	54                   	push   %rsp
    1c69:	00 79 00             	add    %bh,0x0(%rcx)
    1c6c:	70 00                	jo     0x1c6e
    1c6e:	65 00 00             	add    %al,%gs:(%rax)
    1c71:	00 5b 00             	add    %bl,0x0(%rbx)
    1c74:	49 00 4e 00          	rex.WB add %cl,0x0(%r14)
    1c78:	46 00 4f 00          	rex.RX add %r9b,0x0(%rdi)
    1c7c:	5d                   	pop    %rbp
    1c7d:	00 20                	add    %ah,(%rax)
    1c7f:	00 6d 00             	add    %ch,0x0(%rbp)
    1c82:	61                   	(bad)  
    1c83:	00 70 00             	add    %dh,0x0(%rax)
    1c86:	2d 00 3e 00 62       	sub    $0x62003e00,%eax
    1c8b:	00 75 00             	add    %dh,0x0(%rbp)
    1c8e:	66 00 66 00          	data16 add %ah,0x0(%rsi)
    1c92:	65 00 72 00          	add    %dh,%gs:0x0(%rdx)
    1c96:	20 00                	and    %al,(%rax)
    1c98:	3d 00 20 00 25       	cmp    $0x25002000,%eax
    1c9d:	00 30                	add    %dh,(%rax)
    1c9f:	00 38                	add    %bh,(%rax)
    1ca1:	00 6c 00 78          	add    %ch,0x78(%rax,%rax,1)
    1ca5:	00 2c 00             	add    %ch,(%rax,%rax,1)
    1ca8:	20 00                	and    %al,(%rax)
    1caa:	6d                   	insl   (%dx),%es:(%rdi)
    1cab:	00 61 00             	add    %ah,0x0(%rcx)
    1cae:	70 00                	jo     0x1cb0
    1cb0:	2d 00 3e 00 6d       	sub    $0x6d003e00,%eax
    1cb5:	00 61 00             	add    %ah,0x0(%rcx)
    1cb8:	70 00                	jo     0x1cba
    1cba:	5f                   	pop    %rdi
    1cbb:	00 73 00             	add    %dh,0x0(%rbx)
    1cbe:	69 00 7a 00 65 00    	imul   $0x65007a,(%rax),%eax
    1cc4:	20 00                	and    %al,(%rax)
    1cc6:	3d 00 20 00 25       	cmp    $0x25002000,%eax
    1ccb:	00 30                	add    %dh,(%rax)
    1ccd:	00 38                	add    %bh,(%rax)
    1ccf:	00 6c 00 78          	add    %ch,0x78(%rax,%rax,1)
    1cd3:	00 0a                	add    %cl,(%rdx)
    1cd5:	00 00                	add    %al,(%rax)
    1cd7:	00 49 00             	add    %cl,0x0(%rcx)
    1cda:	6e                   	outsb  %ds:(%rsi),(%dx)
    1cdb:	00 76 00             	add    %dh,0x0(%rsi)
    1cde:	61                   	(bad)  
    1cdf:	00 6c 00 69          	add    %ch,0x69(%rax,%rax,1)
    1ce3:	00 64 00 50          	add    %ah,0x50(%rax,%rax,1)
    1ce7:	00 69 00             	add    %ch,0x0(%rcx)
    1cea:	78 00                	js     0x1cec
    1cec:	65 00 6c 00 46       	add    %ch,%gs:0x46(%rax,%rax,1)
    1cf1:	00 6f 00             	add    %ch,0x0(%rdi)
    1cf4:	72 00                	jb     0x1cf6
    1cf6:	6d                   	insl   (%dx),%es:(%rdi)
    1cf7:	00 61 00             	add    %ah,0x0(%rcx)
    1cfa:	74 00                	je     0x1cfc
    1cfc:	00 00                	add    %al,(%rax)
    1cfe:	50                   	push   %rax
    1cff:	00 69 00             	add    %ch,0x0(%rcx)
    1d02:	78 00                	js     0x1d04
    1d04:	65 00 6c 00 52       	add    %ch,%gs:0x52(%rax,%rax,1)
    1d09:	00 65 00             	add    %ah,0x0(%rbp)
    1d0c:	64 00 47 00          	add    %al,%fs:0x0(%rdi)
    1d10:	72 00                	jb     0x1d12
    1d12:	65 00 65 00          	add    %ah,%gs:0x0(%rbp)
    1d16:	6e                   	outsb  %ds:(%rsi),(%dx)
    1d17:	00 42 00             	add    %al,0x0(%rdx)
    1d1a:	6c                   	insb   (%dx),%es:(%rdi)
    1d1b:	00 75 00             	add    %dh,0x0(%rbp)
    1d1e:	65 00 52 00          	add    %dl,%gs:0x0(%rdx)
    1d22:	65 00 73 00          	add    %dh,%gs:0x0(%rbx)
    1d26:	65 00 72 00          	add    %dh,%gs:0x0(%rdx)
    1d2a:	76 00                	jbe    0x1d2c
    1d2c:	65 00 64 00 38       	add    %ah,%gs:0x38(%rax,%rax,1)
    1d31:	00 42 00             	add    %al,0x0(%rdx)
    1d34:	69 00 74 00 50 00    	imul   $0x500074,(%rax),%eax
    1d3a:	65 00 72 00          	add    %dh,%gs:0x0(%rdx)
    1d3e:	43 00 6f 00          	rex.XB add %bpl,0x0(%r15)
    1d42:	6c                   	insb   (%dx),%es:(%rdi)
    1d43:	00 6f 00             	add    %ch,0x0(%rdi)
    1d46:	72 00                	jb     0x1d48
    1d48:	00 00                	add    %al,(%rax)
    1d4a:	50                   	push   %rax
    1d4b:	00 69 00             	add    %ch,0x0(%rcx)
    1d4e:	78 00                	js     0x1d50
    1d50:	65 00 6c 00 42       	add    %ch,%gs:0x42(%rax,%rax,1)
    1d55:	00 6c 00 75          	add    %ch,0x75(%rax,%rax,1)
    1d59:	00 65 00             	add    %ah,0x0(%rbp)
    1d5c:	47 00 72 00          	rex.RXB add %r14b,0x0(%r10)
    1d60:	65 00 65 00          	add    %ah,%gs:0x0(%rbp)
    1d64:	6e                   	outsb  %ds:(%rsi),(%dx)
    1d65:	00 52 00             	add    %dl,0x0(%rdx)
    1d68:	65 00 64 00 52       	add    %ah,%gs:0x52(%rax,%rax,1)
    1d6d:	00 65 00             	add    %ah,0x0(%rbp)
    1d70:	73 00                	jae    0x1d72
    1d72:	65 00 72 00          	add    %dh,%gs:0x0(%rdx)
    1d76:	76 00                	jbe    0x1d78
    1d78:	65 00 64 00 38       	add    %ah,%gs:0x38(%rax,%rax,1)
    1d7d:	00 42 00             	add    %al,0x0(%rdx)
    1d80:	69 00 74 00 50 00    	imul   $0x500074,(%rax),%eax
    1d86:	65 00 72 00          	add    %dh,%gs:0x0(%rdx)
    1d8a:	43 00 6f 00          	rex.XB add %bpl,0x0(%r15)
    1d8e:	6c                   	insb   (%dx),%es:(%rdi)
    1d8f:	00 6f 00             	add    %ch,0x0(%rdi)
    1d92:	72 00                	jb     0x1d94
    1d94:	00 00                	add    %al,(%rax)
    1d96:	50                   	push   %rax
    1d97:	00 69 00             	add    %ch,0x0(%rcx)
    1d9a:	78 00                	js     0x1d9c
    1d9c:	65 00 6c 00 42       	add    %ch,%gs:0x42(%rax,%rax,1)
    1da1:	00 69 00             	add    %ch,0x0(%rcx)
    1da4:	74 00                	je     0x1da6
    1da6:	4d 00 61 00          	rex.WRB add %r12b,0x0(%r9)
    1daa:	73 00                	jae    0x1dac
    1dac:	6b 00 00             	imul   $0x0,(%rax),%eax
    1daf:	00 50 00             	add    %dl,0x0(%rax)
    1db2:	69 00 78 00 65 00    	imul   $0x650078,(%rax),%eax
    1db8:	6c                   	insb   (%dx),%es:(%rdi)
    1db9:	00 42 00             	add    %al,0x0(%rdx)
    1dbc:	6c                   	insb   (%dx),%es:(%rdi)
    1dbd:	00 74 00 4f          	add    %dh,0x4f(%rax,%rax,1)
    1dc1:	00 6e 00             	add    %ch,0x0(%rsi)
    1dc4:	6c                   	insb   (%dx),%es:(%rdi)
    1dc5:	00 79 00             	add    %bh,0x0(%rcx)
    1dc8:	00 00                	add    %al,(%rax)
    1dca:	50                   	push   %rax
    1dcb:	00 69 00             	add    %ch,0x0(%rcx)
    1dce:	78 00                	js     0x1dd0
    1dd0:	65 00 6c 00 46       	add    %ch,%gs:0x46(%rax,%rax,1)
    1dd5:	00 6f 00             	add    %ch,0x0(%rdi)
    1dd8:	72 00                	jb     0x1dda
    1dda:	6d                   	insl   (%dx),%es:(%rdi)
    1ddb:	00 61 00             	add    %ah,0x0(%rcx)
    1dde:	74 00                	je     0x1de0
    1de0:	4d 00 61 00          	rex.WRB add %r12b,0x0(%r9)
    1de4:	78 00                	js     0x1de6
    1de6:	00 00                	add    %al,(%rax)
    1de8:	48 00 65 00          	rex.W add %spl,0x0(%rbp)
    1dec:	6c                   	insb   (%dx),%es:(%rdi)
    1ded:	00 6c 00 6f          	add    %ch,0x6f(%rax,%rax,1)
    1df1:	00 2c 00             	add    %ch,(%rax,%rax,1)
    1df4:	20 00                	and    %al,(%rax)
    1df6:	53                   	push   %rbx
    1df7:	00 74 00 2e          	add    %dh,0x2e(%rax,%rax,1)
    1dfb:	00 4f 00             	add    %cl,0x0(%rdi)
    1dfe:	53                   	push   %rbx
    1dff:	00 20                	add    %ah,(%rax)
    1e01:	00 4c 00 6f          	add    %cl,0x6f(%rax,%rax,1)
    1e05:	00 61 00             	add    %ah,0x0(%rcx)
    1e08:	64 00 65 00          	add    %ah,%fs:0x0(%rbp)
    1e0c:	72 00                	jb     0x1e0e
    1e0e:	20 00                	and    %al,(%rax)
    1e10:	57                   	push   %rdi
    1e11:	00 6f 00             	add    %ch,0x0(%rdi)
    1e14:	72 00                	jb     0x1e16
    1e16:	6c                   	insb   (%dx),%es:(%rdi)
    1e17:	00 64 00 21          	add    %ah,0x21(%rax,%rax,1)
    1e1b:	00 20                	add    %ah,(%rax)
    1e1d:	00 25 00 34 00 75    	add    %ah,0x75003400(%rip)        # 0x75005223
    1e23:	00 2d 00 25 00 30    	add    %ch,0x30002500(%rip)        # 0x30004329
    1e29:	00 32                	add    %dh,(%rdx)
    1e2b:	00 75 00             	add    %dh,0x0(%rbp)
    1e2e:	2d 00 25 00 32       	sub    $0x32002500,%eax
    1e33:	00 75 00             	add    %dh,0x0(%rbp)
    1e36:	20 00                	and    %al,(%rax)
    1e38:	0a 00                	or     (%rax),%al
    1e3a:	00 00                	add    %al,(%rax)
    1e3c:	66 00 61 00          	data16 add %ah,0x0(%rcx)
    1e40:	69 00 6c 00 65 00    	imul   $0x65006c,(%rax),%eax
    1e46:	64 00 20             	add    %ah,%fs:(%rax)
    1e49:	00 74 00 6f          	add    %dh,0x6f(%rax,%rax,1)
    1e4d:	00 20                	add    %ah,(%rax)
    1e4f:	00 67 00             	add    %ah,0x0(%rdi)
    1e52:	65 00 74 00 20       	add    %dh,%gs:0x20(%rax,%rax,1)
    1e57:	00 6d 00             	add    %ch,0x0(%rbp)
    1e5a:	65 00 6d 00          	add    %ch,%gs:0x0(%rbp)
    1e5e:	6f                   	outsl  %ds:(%rsi),(%dx)
    1e5f:	00 72 00             	add    %dh,0x0(%rdx)
    1e62:	79 00                	jns    0x1e64
    1e64:	20 00                	and    %al,(%rax)
    1e66:	6d                   	insl   (%dx),%es:(%rdi)
    1e67:	00 61 00             	add    %ah,0x0(%rcx)
    1e6a:	70 00                	jo     0x1e6c
    1e6c:	3a 00                	cmp    (%rax),%al
    1e6e:	20 00                	and    %al,(%rax)
    1e70:	25 00 72 00 0a       	and    $0xa007200,%eax
    1e75:	00 00                	add    %al,(%rax)
    1e77:	00 5c 00 6d          	add    %bl,0x6d(%rax,%rax,1)
    1e7b:	00 65 00             	add    %ah,0x0(%rbp)
    1e7e:	6d                   	insl   (%dx),%es:(%rdi)
    1e7f:	00 6d 00             	add    %ch,0x0(%rbp)
    1e82:	61                   	(bad)  
    1e83:	00 70 00             	add    %dh,0x0(%rax)
    1e86:	00 00                	add    %al,(%rax)
    1e88:	66 00 61 00          	data16 add %ah,0x0(%rcx)
    1e8c:	69 00 6c 00 65 00    	imul   $0x65006c,(%rax),%eax
    1e92:	64 00 20             	add    %ah,%fs:(%rax)
    1e95:	00 74 00 6f          	add    %dh,0x6f(%rax,%rax,1)
    1e99:	00 20                	add    %ah,(%rax)
    1e9b:	00 6f 00             	add    %ch,0x0(%rdi)
    1e9e:	70 00                	jo     0x1ea0
    1ea0:	65 00 6e 00          	add    %ch,%gs:0x0(%rsi)
    1ea4:	20 00                	and    %al,(%rax)
    1ea6:	66 00 69 00          	data16 add %ch,0x0(%rcx)
    1eaa:	6c                   	insb   (%dx),%es:(%rdi)
    1eab:	00 65 00             	add    %ah,0x0(%rbp)
    1eae:	20 00                	and    %al,(%rax)
    1eb0:	27                   	(bad)  
    1eb1:	00 5c 00 6d          	add    %bl,0x6d(%rax,%rax,1)
    1eb5:	00 65 00             	add    %ah,0x0(%rbp)
    1eb8:	6d                   	insl   (%dx),%es:(%rdi)
    1eb9:	00 6d 00             	add    %ch,0x0(%rbp)
    1ebc:	61                   	(bad)  
    1ebd:	00 70 00             	add    %dh,0x0(%rax)
    1ec0:	27                   	(bad)  
    1ec1:	00 3a                	add    %bh,(%rdx)
    1ec3:	00 20                	add    %ah,(%rax)
    1ec5:	00 25 00 72 00 0a    	add    %ah,0xa007200(%rip)        # 0xa0090cb
    1ecb:	00 00                	add    %al,(%rax)
    1ecd:	00 49 00             	add    %cl,0x0(%rcx)
    1ed0:	67 00 6e 00          	add    %ch,0x0(%esi)
    1ed4:	6f                   	outsl  %ds:(%rsi),(%dx)
    1ed5:	00 72 00             	add    %dh,0x0(%rdx)
    1ed8:	65 00 64 00 2e       	add    %ah,%gs:0x2e(%rax,%rax,1)
    1edd:	00 0a                	add    %cl,(%rdx)
    1edf:	00 00                	add    %al,(%rax)
    1ee1:	00 52 00             	add    %dl,0x0(%rdx)
    1ee4:	65 00 73 00          	add    %dh,%gs:0x0(%rbx)
    1ee8:	6f                   	outsl  %ds:(%rsi),(%dx)
    1ee9:	00 6c 00 75          	add    %ch,0x75(%rax,%rax,1)
    1eed:	00 74 00 69          	add    %dh,0x69(%rax,%rax,1)
    1ef1:	00 6f 00             	add    %ch,0x0(%rdi)
    1ef4:	6e                   	outsb  %ds:(%rsi),(%dx)
    1ef5:	00 3a                	add    %bh,(%rdx)
    1ef7:	00 20                	add    %ah,(%rax)
    1ef9:	00 25 00 75 00 78    	add    %ah,0x78007500(%rip)        # 0x780093ff
    1eff:	00 25 00 75 00 2c    	add    %ah,0x2c007500(%rip)        # 0x2c009405
    1f05:	00 20                	add    %ah,(%rax)
    1f07:	00 50 00             	add    %dl,0x0(%rax)
    1f0a:	69 00 78 00 65 00    	imul   $0x650078,(%rax),%eax
    1f10:	6c                   	insb   (%dx),%es:(%rdi)
    1f11:	00 20                	add    %ah,(%rax)
    1f13:	00 46 00             	add    %al,0x0(%rsi)
    1f16:	6f                   	outsl  %ds:(%rsi),(%dx)
    1f17:	00 72 00             	add    %dh,0x0(%rdx)
    1f1a:	6d                   	insl   (%dx),%es:(%rdi)
    1f1b:	00 61 00             	add    %ah,0x0(%rcx)
    1f1e:	74 00                	je     0x1f20
    1f20:	3a 00                	cmp    (%rax),%al
    1f22:	20 00                	and    %al,(%rax)
    1f24:	25 00 73 00 2c       	and    $0x2c007300,%eax
    1f29:	00 20                	add    %ah,(%rax)
    1f2b:	00 25 00 75 00 20    	add    %ah,0x20007500(%rip)        # 0x20009431
    1f31:	00 70 00             	add    %dh,0x0(%rax)
    1f34:	69 00 78 00 65 00    	imul   $0x650078,(%rax),%eax
    1f3a:	6c                   	insb   (%dx),%es:(%rdi)
    1f3b:	00 73 00             	add    %dh,0x0(%rbx)
    1f3e:	2f                   	(bad)  
    1f3f:	00 6c 00 69          	add    %ch,0x69(%rax,%rax,1)
    1f43:	00 6e 00             	add    %ch,0x0(%rsi)
    1f46:	65 00 0a             	add    %cl,%gs:(%rdx)
    1f49:	00 00                	add    %al,(%rax)
    1f4b:	00 46 00             	add    %al,0x0(%rsi)
    1f4e:	72 00                	jb     0x1f50
    1f50:	61                   	(bad)  
    1f51:	00 6d 00             	add    %ch,0x0(%rbp)
    1f54:	65 00 20             	add    %ah,%gs:(%rax)
    1f57:	00 42 00             	add    %al,0x0(%rdx)
    1f5a:	75 00                	jne    0x1f5c
    1f5c:	66 00 66 00          	data16 add %ah,0x0(%rsi)
    1f60:	65 00 72 00          	add    %dh,%gs:0x0(%rdx)
    1f64:	3a 00                	cmp    (%rax),%al
    1f66:	20 00                	and    %al,(%rax)
    1f68:	30 00                	xor    %al,(%rax)
    1f6a:	78 00                	js     0x1f6c
    1f6c:	25 00 30 00 6c       	and    $0x6c003000,%eax
    1f71:	00 78 00             	add    %bh,0x0(%rax)
    1f74:	20 00                	and    %al,(%rax)
    1f76:	2d 00 20 00 30       	sub    $0x30002000,%eax
    1f7b:	00 78 00             	add    %bh,0x0(%rax)
    1f7e:	25 00 30 00 6c       	and    $0x6c003000,%eax
    1f83:	00 78 00             	add    %bh,0x0(%rax)
    1f86:	2c 00                	sub    $0x0,%al
    1f88:	20 00                	and    %al,(%rax)
    1f8a:	53                   	push   %rbx
    1f8b:	00 69 00             	add    %ch,0x0(%rcx)
    1f8e:	7a 00                	jp     0x1f90
    1f90:	65 00 3a             	add    %bh,%gs:(%rdx)
    1f93:	00 20                	add    %ah,(%rax)
    1f95:	00 25 00 6c 00 75    	add    %ah,0x75006c00(%rip)        # 0x75008b9b
    1f9b:	00 20                	add    %ah,(%rax)
    1f9d:	00 62 00             	add    %ah,0x0(%rdx)
    1fa0:	79 00                	jns    0x1fa2
    1fa2:	74 00                	je     0x1fa4
    1fa4:	65 00 73 00          	add    %dh,%gs:0x0(%rbx)
    1fa8:	0a 00                	or     (%rax),%al
    1faa:	00 00                	add    %al,(%rax)
    1fac:	5c                   	pop    %rsp
    1fad:	00 6b 00             	add    %ch,0x0(%rbx)
    1fb0:	65 00 72 00          	add    %dh,%gs:0x0(%rdx)
    1fb4:	6e                   	outsb  %ds:(%rsi),(%dx)
    1fb5:	00 65 00             	add    %ah,0x0(%rbp)
    1fb8:	6c                   	insb   (%dx),%es:(%rdi)
    1fb9:	00 2e                	add    %ch,(%rsi)
    1fbb:	00 65 00             	add    %ah,0x0(%rbp)
    1fbe:	6c                   	insb   (%dx),%es:(%rdi)
    1fbf:	00 66 00             	add    %ah,0x0(%rsi)
    1fc2:	00 00                	add    %al,(%rax)
    1fc4:	66 00 61 00          	data16 add %ah,0x0(%rcx)
    1fc8:	69 00 6c 00 65 00    	imul   $0x65006c,(%rax),%eax
    1fce:	64 00 20             	add    %ah,%fs:(%rax)
    1fd1:	00 74 00 6f          	add    %dh,0x6f(%rax,%rax,1)
    1fd5:	00 20                	add    %ah,(%rax)
    1fd7:	00 6f 00             	add    %ch,0x0(%rdi)
    1fda:	70 00                	jo     0x1fdc
    1fdc:	65 00 6e 00          	add    %ch,%gs:0x0(%rsi)
    1fe0:	20 00                	and    %al,(%rax)
    1fe2:	66 00 69 00          	data16 add %ch,0x0(%rcx)
    1fe6:	6c                   	insb   (%dx),%es:(%rdi)
    1fe7:	00 65 00             	add    %ah,0x0(%rbp)
    1fea:	20 00                	and    %al,(%rax)
    1fec:	27                   	(bad)  
    1fed:	00 5c 00 6b          	add    %bl,0x6b(%rax,%rax,1)
    1ff1:	00 65 00             	add    %ah,0x0(%rbp)
    1ff4:	72 00                	jb     0x1ff6
    1ff6:	6e                   	outsb  %ds:(%rsi),(%dx)
    1ff7:	00 65 00             	add    %ah,0x0(%rbp)
    1ffa:	6c                   	insb   (%dx),%es:(%rdi)
    1ffb:	00 2e                	add    %ch,(%rsi)
    1ffd:	00 65 00             	add    %ah,0x0(%rbp)
    2000:	6c                   	insb   (%dx),%es:(%rdi)
    2001:	00 66 00             	add    %ah,0x0(%rsi)
    2004:	27                   	(bad)  
    2005:	00 3a                	add    %bh,(%rdx)
    2007:	00 20                	add    %ah,(%rax)
    2009:	00 25 00 72 00 0a    	add    %ah,0xa007200(%rip)        # 0xa00920f
    200f:	00 00                	add    %al,(%rax)
    2011:	00 66 00             	add    %ah,0x0(%rsi)
    2014:	61                   	(bad)  
    2015:	00 69 00             	add    %ch,0x0(%rcx)
    2018:	6c                   	insb   (%dx),%es:(%rdi)
    2019:	00 65 00             	add    %ah,0x0(%rbp)
    201c:	64 00 20             	add    %ah,%fs:(%rax)
    201f:	00 74 00 6f          	add    %dh,0x6f(%rax,%rax,1)
    2023:	00 20                	add    %ah,(%rax)
    2025:	00 67 00             	add    %ah,0x0(%rdi)
    2028:	65 00 74 00 20       	add    %dh,%gs:0x20(%rax,%rax,1)
    202d:	00 66 00             	add    %ah,0x0(%rsi)
    2030:	69 00 6c 00 65 00    	imul   $0x65006c,(%rax),%eax
    2036:	20 00                	and    %al,(%rax)
    2038:	69 00 6e 00 66 00    	imul   $0x66006e,(%rax),%eax
    203e:	6f                   	outsl  %ds:(%rsi),(%dx)
    203f:	00 72 00             	add    %dh,0x0(%rdx)
    2042:	6d                   	insl   (%dx),%es:(%rdi)
    2043:	00 61 00             	add    %ah,0x0(%rcx)
    2046:	74 00                	je     0x2048
    2048:	69 00 6f 00 6e 00    	imul   $0x6e006f,(%rax),%eax
    204e:	3a 00                	cmp    (%rax),%al
    2050:	20 00                	and    %al,(%rax)
    2052:	25 00 72 00 0a       	and    $0xa007200,%eax
    2057:	00 00                	add    %al,(%rax)
    2059:	00 66 00             	add    %ah,0x0(%rsi)
    205c:	61                   	(bad)  
    205d:	00 69 00             	add    %ch,0x0(%rcx)
    2060:	6c                   	insb   (%dx),%es:(%rdi)
    2061:	00 65 00             	add    %ah,0x0(%rbp)
    2064:	64 00 20             	add    %ah,%fs:(%rax)
    2067:	00 74 00 6f          	add    %dh,0x6f(%rax,%rax,1)
    206b:	00 20                	add    %ah,(%rax)
    206d:	00 61 00             	add    %ah,0x0(%rcx)
    2070:	6c                   	insb   (%dx),%es:(%rdi)
    2071:	00 6c 00 6f          	add    %ch,0x6f(%rax,%rax,1)
    2075:	00 63 00             	add    %ah,0x0(%rbx)
    2078:	61                   	(bad)  
    2079:	00 74 00 65          	add    %dh,0x65(%rax,%rax,1)
    207d:	00 20                	add    %ah,(%rax)
    207f:	00 70 00             	add    %dh,0x0(%rax)
    2082:	61                   	(bad)  
    2083:	00 67 00             	add    %ah,0x0(%rdi)
    2086:	65 00 73 00          	add    %dh,%gs:0x0(%rbx)
    208a:	3a 00                	cmp    (%rax),%al
    208c:	20 00                	and    %al,(%rax)
    208e:	25 00 72 00 0a       	and    $0xa007200,%eax
    2093:	00 00                	add    %al,(%rax)
    2095:	00 4b 00             	add    %cl,0x0(%rbx)
    2098:	65 00 72 00          	add    %dh,%gs:0x0(%rdx)
    209c:	6e                   	outsb  %ds:(%rsi),(%dx)
    209d:	00 65 00             	add    %ah,0x0(%rbp)
    20a0:	6c                   	insb   (%dx),%es:(%rdi)
    20a1:	00 3a                	add    %bh,(%rdx)
    20a3:	00 20                	add    %ah,(%rax)
    20a5:	00 30                	add    %dh,(%rax)
    20a7:	00 78 00             	add    %bh,0x0(%rax)
    20aa:	25 00 30 00 6c       	and    $0x6c003000,%eax
    20af:	00 78 00             	add    %bh,0x0(%rax)
    20b2:	20 00                	and    %al,(%rax)
    20b4:	28 00                	sub    %al,(%rax)
    20b6:	25 00 6c 00 75       	and    $0x75006c00,%eax
    20bb:	00 20                	add    %ah,(%rax)
    20bd:	00 62 00             	add    %ah,0x0(%rdx)
    20c0:	79 00                	jns    0x20c2
    20c2:	74 00                	je     0x20c4
    20c4:	65 00 73 00          	add    %dh,%gs:0x0(%rbx)
    20c8:	29 00                	sub    %eax,(%rax)
    20ca:	0a 00                	or     (%rax),%al
    20cc:	00 00                	add    %al,(%rax)
    20ce:	43 00 6f 00          	rex.XB add %bpl,0x0(%r15)
    20d2:	75 00                	jne    0x20d4
    20d4:	6c                   	insb   (%dx),%es:(%rdi)
    20d5:	00 64 00 20          	add    %ah,0x20(%rax,%rax,1)
    20d9:	00 6e 00             	add    %ch,0x0(%rsi)
    20dc:	6f                   	outsl  %ds:(%rsi),(%dx)
    20dd:	00 74 00 20          	add    %dh,0x20(%rax,%rax,1)
    20e1:	00 65 00             	add    %ah,0x0(%rbp)
    20e4:	78 00                	js     0x20e6
    20e6:	69 00 74 00 20 00    	imul   $0x200074,(%rax),%eax
    20ec:	62                   	(bad)  
    20ed:	00 6f 00             	add    %ch,0x0(%rdi)
    20f0:	6f                   	outsl  %ds:(%rsi),(%dx)
    20f1:	00 74 00 20          	add    %dh,0x20(%rax,%rax,1)
    20f5:	00 73 00             	add    %dh,0x0(%rbx)
    20f8:	65 00 72 00          	add    %dh,%gs:0x0(%rdx)
    20fc:	76 00                	jbe    0x20fe
    20fe:	69 00 63 00 65 00    	imul   $0x650063,(%rax),%eax
    2104:	3a 00                	cmp    (%rax),%al
    2106:	20 00                	and    %al,(%rax)
    2108:	25 00 72 00 0a       	and    $0xa007200,%eax
    210d:	00 00                	add    %al,(%rax)
    210f:	00 55 00             	add    %dl,0x0(%rbp)
    2112:	6e                   	outsb  %ds:(%rsi),(%dx)
    2113:	00 69 00             	add    %ch,0x0(%rcx)
    2116:	6d                   	insl   (%dx),%es:(%rdi)
    2117:	00 70 00             	add    %dh,0x0(%rax)
    211a:	6c                   	insb   (%dx),%es:(%rdi)
    211b:	00 65 00             	add    %ah,0x0(%rbp)
    211e:	6d                   	insl   (%dx),%es:(%rdi)
    211f:	00 65 00             	add    %ah,0x0(%rbp)
    2122:	6e                   	outsb  %ds:(%rsi),(%dx)
    2123:	00 74 00 65          	add    %dh,0x65(%rax,%rax,1)
    2127:	00 64 00 20          	add    %ah,0x20(%rax,%rax,1)
    212b:	00 70 00             	add    %dh,0x0(%rax)
    212e:	69 00 78 00 65 00    	imul   $0x650078,(%rax),%eax
    2134:	6c                   	insb   (%dx),%es:(%rdi)
    2135:	00 20                	add    %ah,(%rax)
    2137:	00 66 00             	add    %ah,0x0(%rsi)
    213a:	6f                   	outsl  %ds:(%rsi),(%dx)
    213b:	00 72 00             	add    %dh,0x0(%rdx)
    213e:	6d                   	insl   (%dx),%es:(%rdi)
    213f:	00 61 00             	add    %ah,0x0(%rcx)
    2142:	74 00                	je     0x2144
    2144:	3a 00                	cmp    (%rax),%al
    2146:	20 00                	and    %al,(%rax)
    2148:	25 00 64 00 0a       	and    $0xa006400,%eax
    214d:	00 00                	add    %al,(%rax)
    214f:	00 41 00             	add    %al,0x0(%rcx)
    2152:	6c                   	insb   (%dx),%es:(%rdi)
    2153:	00 6c 00 20          	add    %ch,0x20(%rax,%rax,1)
    2157:	00 64 00 6f          	add    %ah,0x6f(%rax,%rax,1)
    215b:	00 6e 00             	add    %ch,0x0(%rsi)
    215e:	65 00 0a             	add    %cl,%gs:(%rdx)
    2161:	00 00                	add    %al,(%rax)
    2163:	00 49 6e             	add    %cl,0x6e(%rcx)
    2166:	64 65 78 2c          	fs gs js 0x2196
    216a:	20 54 79 70          	and    %dl,0x70(%rcx,%rdi,2)
    216e:	65 2c 20             	gs sub $0x20,%al
    2171:	54                   	push   %rsp
    2172:	79 70                	jns    0x21e4
    2174:	65 28 6e 61          	sub    %ch,%gs:0x61(%rsi)
    2178:	6d                   	insl   (%dx),%es:(%rdi)
    2179:	65 29 2c 20          	sub    %ebp,%gs:(%rax,%riz,1)
    217d:	50                   	push   %rax
    217e:	68 79 73 69 63       	pushq  $0x63697379
    2183:	61                   	(bad)  
    2184:	6c                   	insb   (%dx),%es:(%rdi)
    2185:	53                   	push   %rbx
    2186:	74 61                	je     0x21e9
    2188:	72 74                	jb     0x21fe
    218a:	2c 20                	sub    $0x20,%al
    218c:	4e 75 6d             	rex.WRX jne 0x21fc
    218f:	62                   	(bad)  
    2190:	65 72 4f             	gs jb  0x21e2
    2193:	66 50                	push   %ax
    2195:	61                   	(bad)  
    2196:	67 65 73 2c          	addr32 gs jae 0x21c6
    219a:	20 41 74             	and    %al,0x74(%rcx)
    219d:	74 72                	je     0x2211
    219f:	69 62 75 74 65 0a 00 	imul   $0xa6574,0x75(%rdx),%esp
    21a6:	25 75 2c 20 25       	and    $0x25202c75,%eax
    21ab:	78 2c                	js     0x21d9
    21ad:	20 25 2d 6c 73 2c    	and    %ah,0x2c736c2d(%rip)        # 0x2c738de0
    21b3:	20 25 30 38 6c 78    	and    %ah,0x786c3830(%rip)        # 0x786c59e9
    21b9:	2c 20                	sub    $0x20,%al
    21bb:	25 6c 78 2c 20       	and    $0x202c786c,%eax
    21c0:	25 6c 78 0a 00       	and    $0xa786c,%eax
    21c5:	3c 6e                	cmp    $0x6e,%al
    21c7:	75 6c                	jne    0x2235
    21c9:	6c                   	insb   (%dx),%es:(%rdi)
    21ca:	20 73 74             	and    %dh,0x74(%rbx)
    21cd:	72 69                	jb     0x2238
    21cf:	6e                   	outsb  %ds:(%rsi),(%dx)
    21d0:	67 3e 00 25 30 38 78 	add    %ah,%ds:0x2d783830(%eip)        # 0x2d785a08
    21d7:	2d 
    21d8:	25 30 34 78 2d       	and    $0x2d783430,%eax
    21dd:	25 30 34 78 2d       	and    $0x2d783430,%eax
    21e2:	25 30 32 78 25       	and    $0x25783230,%eax
    21e7:	30 32                	xor    %dh,(%rdx)
    21e9:	78 2d                	js     0x2218
    21eb:	25 30 32 78 25       	and    $0x25783230,%eax
    21f0:	30 32                	xor    %dh,(%rdx)
    21f2:	78 25                	js     0x2219
    21f4:	30 32                	xor    %dh,(%rdx)
    21f6:	78 25                	js     0x221d
    21f8:	30 32                	xor    %dh,(%rdx)
    21fa:	78 25                	js     0x2221
    21fc:	30 32                	xor    %dh,(%rdx)
    21fe:	78 25                	js     0x2225
    2200:	30 32                	xor    %dh,(%rdx)
    2202:	78 00                	js     0x2204
    2204:	25 30 32 64 2f       	and    $0x2f643230,%eax
    2209:	25 30 32 64 2f       	and    $0x2f643230,%eax
    220e:	25 30 34 64 20       	and    $0x20643430,%eax
    2213:	20 25 30 32 64 3a    	and    %ah,0x3a643230(%rip)        # 0x3a645449
    2219:	25 30 32 64 00       	and    $0x643230,%eax
    221e:	25 30 38 58 00       	and    $0x583830,%eax
    2223:	0d 0a 00 0d 00       	or     $0xd000a,%eax
    2228:	3c 6e                	cmp    $0x6e,%al
    222a:	75 6c                	jne    0x2298
    222c:	6c                   	insb   (%dx),%es:(%rdi)
    222d:	20 67 75             	and    %ah,0x75(%rdi)
    2230:	69 64 3e 00 3c 6e 75 	imul   $0x6c756e3c,0x0(%rsi,%rdi,1),%esp
    2237:	6c 
    2238:	6c                   	insb   (%dx),%es:(%rdi)
    2239:	20 74 69 6d          	and    %dh,0x6d(%rcx,%rbp,2)
    223d:	65 3e 00 30          	gs add %dh,%ds:(%rax)
    2241:	31 32                	xor    %esi,(%rdx)
    2243:	33 34 35 36 37 38 39 	xor    0x39383736(,%rsi,1),%esi
    224a:	41                   	rex.B
    224b:	42                   	rex.X
    224c:	43                   	rex.XB
    224d:	44                   	rex.R
    224e:	45                   	rex.RB
    224f:	46                   	rex.RX
    2250:	4c 6f                	rex.WR outsl %ds:(%rsi),(%dx)
    2252:	61                   	(bad)  
    2253:	64 20 45 72          	and    %al,%fs:0x72(%rbp)
    2257:	72 6f                	jb     0x22c8
    2259:	72 00                	jb     0x225b
	...
    2263:	00 00                	add    %al,(%rax)
    2265:	49 6e                	rex.WB outsb %ds:(%rsi),(%dx)
    2267:	76 61                	jbe    0x22ca
    2269:	6c                   	insb   (%dx),%es:(%rdi)
    226a:	69 64 20 50 61 72 61 	imul   $0x6d617261,0x50(%rax,%riz,1),%esp
    2271:	6d 
    2272:	65 74 65             	gs je  0x22da
    2275:	72 00                	jb     0x2277
    2277:	00 00                	add    %al,(%rax)
    2279:	00 55 6e             	add    %dl,0x6e(%rbp)
    227c:	73 75                	jae    0x22f3
    227e:	70 70                	jo     0x22f0
    2280:	6f                   	outsl  %ds:(%rsi),(%dx)
    2281:	72 74                	jb     0x22f7
    2283:	65 64 00 00          	gs add %al,%fs:(%rax)
	...
    228f:	42 61                	rex.X (bad) 
    2291:	64 20 42 75          	and    %al,%fs:0x75(%rdx)
    2295:	66 66 65 72 20       	data16 data16 gs jb 0x22ba
    229a:	53                   	push   %rbx
    229b:	69 7a 65 00 00 00 00 	imul   $0x0,0x65(%rdx),%edi
    22a2:	00 00                	add    %al,(%rax)
    22a4:	42 75 66             	rex.X jne 0x230d
    22a7:	66 65 72 20          	data16 gs jb 0x22cb
    22ab:	54                   	push   %rsp
    22ac:	6f                   	outsl  %ds:(%rsi),(%dx)
    22ad:	6f                   	outsl  %ds:(%rsi),(%dx)
    22ae:	20 53 6d             	and    %dl,0x6d(%rbx)
    22b1:	61                   	(bad)  
    22b2:	6c                   	insb   (%dx),%es:(%rdi)
    22b3:	6c                   	insb   (%dx),%es:(%rdi)
    22b4:	00 00                	add    %al,(%rax)
    22b6:	00 00                	add    %al,(%rax)
    22b8:	00 4e 6f             	add    %cl,0x6f(%rsi)
    22bb:	74 20                	je     0x22dd
    22bd:	52                   	push   %rdx
    22be:	65 61                	gs (bad) 
    22c0:	64 79 00             	fs jns 0x22c3
	...
    22cb:	00 00                	add    %al,(%rax)
    22cd:	00 44 65 76          	add    %al,0x76(%rbp,%riz,2)
    22d1:	69 63 65 20 45 72 72 	imul   $0x72724520,0x65(%rbx),%esp
    22d8:	6f                   	outsl  %ds:(%rsi),(%dx)
    22d9:	72 00                	jb     0x22db
	...
    22e3:	57                   	push   %rdi
    22e4:	72 69                	jb     0x234f
    22e6:	74 65                	je     0x234d
    22e8:	20 50 72             	and    %dl,0x72(%rax)
    22eb:	6f                   	outsl  %ds:(%rsi),(%dx)
    22ec:	74 65                	je     0x2353
    22ee:	63 74 65 64          	movslq 0x64(%rbp,%riz,2),%esi
    22f2:	00 00                	add    %al,(%rax)
    22f4:	00 00                	add    %al,(%rax)
    22f6:	00 00                	add    %al,(%rax)
    22f8:	4f 75 74             	rex.WRXB jne 0x236f
    22fb:	20 6f 66             	and    %ch,0x66(%rdi)
    22fe:	20 52 65             	and    %dl,0x65(%rdx)
    2301:	73 6f                	jae    0x2372
    2303:	75 72                	jne    0x2377
    2305:	63 65 73             	movslq 0x73(%rbp),%esp
    2308:	00 00                	add    %al,(%rax)
    230a:	00 00                	add    %al,(%rax)
    230c:	00 56 6f             	add    %dl,0x6f(%rsi)
    230f:	6c                   	insb   (%dx),%es:(%rdi)
    2310:	75 6d                	jne    0x237f
    2312:	65 20 43 6f          	and    %al,%gs:0x6f(%rbx)
    2316:	72 72                	jb     0x238a
    2318:	75 70                	jne    0x238a
    231a:	74 00                	je     0x231c
    231c:	00 00                	add    %al,(%rax)
    231e:	00 00                	add    %al,(%rax)
    2320:	00 00                	add    %al,(%rax)
    2322:	56                   	push   %rsi
    2323:	6f                   	outsl  %ds:(%rsi),(%dx)
    2324:	6c                   	insb   (%dx),%es:(%rdi)
    2325:	75 6d                	jne    0x2394
    2327:	65 20 46 75          	and    %al,%gs:0x75(%rsi)
    232b:	6c                   	insb   (%dx),%es:(%rdi)
    232c:	6c                   	insb   (%dx),%es:(%rdi)
	...
    2335:	00 00                	add    %al,(%rax)
    2337:	4e 6f                	rex.WRX outsl %ds:(%rsi),(%dx)
    2339:	20 4d 65             	and    %cl,0x65(%rbp)
    233c:	64 69 61 00 00 00 00 	imul   $0x0,%fs:0x0(%rcx),%esp
    2343:	00 
	...
    234c:	4d                   	rex.WRB
    234d:	65 64 69 61 20 63 68 	gs imul $0x6e616863,%fs:0x20(%rcx),%esp
    2354:	61 6e 
    2356:	67 65 64 00 00       	gs add %al,%fs:(%eax)
    235b:	00 00                	add    %al,(%rax)
    235d:	00 00                	add    %al,(%rax)
    235f:	00 00                	add    %al,(%rax)
    2361:	4e 6f                	rex.WRX outsl %ds:(%rsi),(%dx)
    2363:	74 20                	je     0x2385
    2365:	46 6f                	rex.RX outsl %ds:(%rsi),(%dx)
    2367:	75 6e                	jne    0x23d7
    2369:	64 00 00             	add    %al,%fs:(%rax)
	...
    2374:	00 00                	add    %al,(%rax)
    2376:	41 63 63 65          	movslq 0x65(%r11),%esp
    237a:	73 73                	jae    0x23ef
    237c:	20 44 65 6e          	and    %al,0x6e(%rbp,%riz,2)
    2380:	69 65 64 00 00 00 00 	imul   $0x0,0x64(%rbp),%esp
    2387:	00 00                	add    %al,(%rax)
    2389:	00 00                	add    %al,(%rax)
    238b:	4e 6f                	rex.WRX outsl %ds:(%rsi),(%dx)
    238d:	20 52 65             	and    %dl,0x65(%rdx)
    2390:	73 70                	jae    0x2402
    2392:	6f                   	outsl  %ds:(%rsi),(%dx)
    2393:	6e                   	outsb  %ds:(%rsi),(%dx)
    2394:	73 65                	jae    0x23fb
	...
    239e:	00 00                	add    %al,(%rax)
    23a0:	4e 6f                	rex.WRX outsl %ds:(%rsi),(%dx)
    23a2:	20 6d 61             	and    %ch,0x61(%rbp)
    23a5:	70 70                	jo     0x2417
    23a7:	69 6e 67 00 00 00 00 	imul   $0x0,0x67(%rsi),%ebp
    23ae:	00 00                	add    %al,(%rax)
    23b0:	00 00                	add    %al,(%rax)
    23b2:	00 00                	add    %al,(%rax)
    23b4:	00 54 69 6d          	add    %dl,0x6d(%rcx,%rbp,2)
    23b8:	65 20 6f 75          	and    %ch,%gs:0x75(%rdi)
    23bc:	74 00                	je     0x23be
	...
    23ca:	4e 6f                	rex.WRX outsl %ds:(%rsi),(%dx)
    23cc:	74 20                	je     0x23ee
    23ce:	73 74                	jae    0x2444
    23d0:	61                   	(bad)  
    23d1:	72 74                	jb     0x2447
    23d3:	65 64 00 00          	gs add %al,%fs:(%rax)
	...
    23df:	41 6c                	rex.B insb (%dx),%es:(%rdi)
    23e1:	72 65                	jb     0x2448
    23e3:	61                   	(bad)  
    23e4:	64 79 20             	fs jns 0x2407
    23e7:	73 74                	jae    0x245d
    23e9:	61                   	(bad)  
    23ea:	72 74                	jb     0x2460
    23ec:	65 64 00 00          	gs add %al,%fs:(%rax)
    23f0:	00 00                	add    %al,(%rax)
    23f2:	00 00                	add    %al,(%rax)
    23f4:	41 62                	rex.B (bad) 
    23f6:	6f                   	outsl  %ds:(%rsi),(%dx)
    23f7:	72 74                	jb     0x246d
    23f9:	65 64 00 00          	gs add %al,%fs:(%rax)
	...
    2409:	49                   	rex.WB
    240a:	43                   	rex.XB
    240b:	4d 50                	rex.WRB push %r8
    240d:	20 45 72             	and    %al,0x72(%rbp)
    2410:	72 6f                	jb     0x2481
    2412:	72 00                	jb     0x2414
	...
    241c:	00 00                	add    %al,(%rax)
    241e:	54                   	push   %rsp
    241f:	46 54                	rex.RX push %rsp
    2421:	50                   	push   %rax
    2422:	20 45 72             	and    %al,0x72(%rbp)
    2425:	72 6f                	jb     0x2496
    2427:	72 00                	jb     0x2429
	...
    2431:	00 00                	add    %al,(%rax)
    2433:	50                   	push   %rax
    2434:	72 6f                	jb     0x24a5
    2436:	74 6f                	je     0x24a7
    2438:	63 6f 6c             	movslq 0x6c(%rdi),%ebp
    243b:	20 45 72             	and    %al,0x72(%rbp)
    243e:	72 6f                	jb     0x24af
    2440:	72 00                	jb     0x2442
    2442:	00 00                	add    %al,(%rax)
    2444:	00 00                	add    %al,(%rax)
    2446:	00 00                	add    %al,(%rax)
    2448:	49 6e                	rex.WB outsb %ds:(%rsi),(%dx)
    244a:	63 6f 6d             	movslq 0x6d(%rdi),%ebp
    244d:	70 61                	jo     0x24b0
    244f:	74 69                	je     0x24ba
    2451:	62                   	(bad)  
    2452:	6c                   	insb   (%dx),%es:(%rdi)
    2453:	65 20 56 65          	and    %dl,%gs:0x65(%rsi)
    2457:	72 73                	jb     0x24cc
    2459:	69 6f 6e 00 53 65 63 	imul   $0x63655300,0x6e(%rdi),%ebp
    2460:	75 72                	jne    0x24d4
    2462:	69 74 79 20 56 69 6f 	imul   $0x6c6f6956,0x20(%rcx,%rdi,2),%esi
    2469:	6c 
    246a:	61                   	(bad)  
    246b:	74 69                	je     0x24d6
    246d:	6f                   	outsl  %ds:(%rsi),(%dx)
    246e:	6e                   	outsb  %ds:(%rsi),(%dx)
    246f:	00 00                	add    %al,(%rax)
    2471:	00 43 52             	add    %al,0x52(%rbx)
    2474:	43 20 45 72          	rex.XB and %al,0x72(%r13)
    2478:	72 6f                	jb     0x24e9
    247a:	72 00                	jb     0x247c
	...
    2484:	00 00                	add    %al,(%rax)
    2486:	00 45 6e             	add    %al,0x6e(%rbp)
    2489:	64 20 6f 66          	and    %ch,%fs:0x66(%rdi)
    248d:	20 4d 65             	and    %cl,0x65(%rbp)
    2490:	64 69 61 00 00 00 00 	imul   $0x0,%fs:0x0(%rcx),%esp
    2497:	00 
    2498:	00 00                	add    %al,(%rax)
    249a:	00 00                	add    %al,(%rax)
    249c:	52                   	push   %rdx
    249d:	65 73 65             	gs jae 0x2505
    24a0:	72 76                	jb     0x2518
    24a2:	65 64 20 28          	gs and %ch,%fs:(%rax)
    24a6:	32 39                	xor    (%rcx),%bh
    24a8:	29 00                	sub    %eax,(%rax)
    24aa:	00 00                	add    %al,(%rax)
    24ac:	00 00                	add    %al,(%rax)
    24ae:	00 00                	add    %al,(%rax)
    24b0:	00 52 65             	add    %dl,0x65(%rdx)
    24b3:	73 65                	jae    0x251a
    24b5:	72 76                	jb     0x252d
    24b7:	65 64 20 28          	gs and %ch,%fs:(%rax)
    24bb:	33 30                	xor    (%rax),%esi
    24bd:	29 00                	sub    %eax,(%rax)
    24bf:	00 00                	add    %al,(%rax)
    24c1:	00 00                	add    %al,(%rax)
    24c3:	00 00                	add    %al,(%rax)
    24c5:	00 45 6e             	add    %al,0x6e(%rbp)
    24c8:	64 20 6f 66          	and    %ch,%fs:0x66(%rdi)
    24cc:	20 46 69             	and    %al,0x69(%rsi)
    24cf:	6c                   	insb   (%dx),%es:(%rdi)
    24d0:	65 00 00             	add    %al,%gs:(%rax)
	...
    24db:	49 6e                	rex.WB outsb %ds:(%rsi),(%dx)
    24dd:	76 61                	jbe    0x2540
    24df:	6c                   	insb   (%dx),%es:(%rdi)
    24e0:	69 64 20 4c 61 6e 67 	imul   $0x75676e61,0x4c(%rax,%riz,1),%esp
    24e7:	75 
    24e8:	61                   	(bad)  
    24e9:	67 65 00 00          	add    %al,%gs:(%eax)
    24ed:	00 00                	add    %al,(%rax)
    24ef:	00 43 6f             	add    %al,0x6f(%rbx)
    24f2:	6d                   	insl   (%dx),%es:(%rdi)
    24f3:	70 72                	jo     0x2567
    24f5:	6f                   	outsl  %ds:(%rsi),(%dx)
    24f6:	6d                   	insl   (%dx),%es:(%rdi)
    24f7:	69 73 65 64 20 44 61 	imul   $0x61442064,0x65(%rbx),%esi
    24fe:	74 61                	je     0x2561
    2500:	00 00                	add    %al,(%rax)
    2502:	00 00                	add    %al,(%rax)
    2504:	00 66 2e             	add    %ah,0x2e(%rsi)
    2507:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
    250e:	00 
    250f:	90                   	nop
    2510:	53                   	push   %rbx
    2511:	75 63                	jne    0x2576
    2513:	63 65 73             	movslq 0x73(%rbp),%esp
    2516:	73 00                	jae    0x2518
	...
    2528:	00 57 61             	add    %dl,0x61(%rdi)
    252b:	72 6e                	jb     0x259b
    252d:	69 6e 67 20 55 6e 6b 	imul   $0x6b6e5520,0x67(%rsi),%ebp
    2534:	6e                   	outsb  %ds:(%rsi),(%dx)
    2535:	6f                   	outsl  %ds:(%rsi),(%dx)
    2536:	77 6e                	ja     0x25a6
    2538:	20 47 6c             	and    %al,0x6c(%rdi)
    253b:	79 70                	jns    0x25ad
    253d:	68 00 00 00 00       	pushq  $0x0
    2542:	57                   	push   %rdi
    2543:	61                   	(bad)  
    2544:	72 6e                	jb     0x25b4
    2546:	69 6e 67 20 44 65 6c 	imul   $0x6c654420,0x67(%rsi),%ebp
    254d:	65 74 65             	gs je  0x25b5
    2550:	20 46 61             	and    %al,0x61(%rsi)
    2553:	69 6c 75 72 65 00 00 	imul   $0x65,0x72(%rbp,%rsi,2),%ebp
    255a:	00 
    255b:	57                   	push   %rdi
    255c:	61                   	(bad)  
    255d:	72 6e                	jb     0x25cd
    255f:	69 6e 67 20 57 72 69 	imul   $0x69725720,0x67(%rsi),%ebp
    2566:	74 65                	je     0x25cd
    2568:	20 46 61             	and    %al,0x61(%rsi)
    256b:	69 6c 75 72 65 00 00 	imul   $0x65,0x72(%rbp,%rsi,2),%ebp
    2572:	00 
    2573:	00 57 61             	add    %dl,0x61(%rdi)
    2576:	72 6e                	jb     0x25e6
    2578:	69 6e 67 20 42 75 66 	imul   $0x66754220,0x67(%rsi),%ebp
    257f:	66 65 72 20          	data16 gs jb 0x25a3
    2583:	54                   	push   %rsp
    2584:	6f                   	outsl  %ds:(%rsi),(%dx)
    2585:	6f                   	outsl  %ds:(%rsi),(%dx)
    2586:	20 53 6d             	and    %dl,0x6d(%rbx)
    2589:	61                   	(bad)  
    258a:	6c                   	insb   (%dx),%es:(%rdi)
    258b:	6c                   	insb   (%dx),%es:(%rdi)
    258c:	00 57 61             	add    %dl,0x61(%rdi)
    258f:	72 6e                	jb     0x25ff
    2591:	69 6e 67 20 53 74 61 	imul   $0x61745320,0x67(%rsi),%ebp
    2598:	6c                   	insb   (%dx),%es:(%rdi)
    2599:	65 20 44 61 74       	and    %al,%gs:0x74(%rcx,%riz,2)
    259e:	61                   	(bad)  
	...
```