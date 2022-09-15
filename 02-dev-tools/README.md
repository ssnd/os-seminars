# Инструменты разработки

## Полезные ссылки
- [Ридинг Яковлева](https://github.com/victor-yacovlev/fpmi-caos/blob/master/practice/linux_basics/devtools.md)
- [Ридинг Печатнова (очень крутой!)](https://github.com/yuri-pechatnov/caos/tree/master/caos_2020-2021/sem02-instruments-compilation)

## Этапы компиляции программы на C
Когда мы компилируем программу из кода на СИ в исполняемый файл происходит очень много всего. Мы с вами разберём три основных этапа (на самом деле их [больше](https://en.cppreference.com/w/c/language/translation_phases)):

### Препроцессинг

`gcc -E example.c -oexample_preprocessed.c`

Открываем файл `example_preprocessed.c`  и видим:
```c
// ...

# 2 "example.c" 2
# 1 "./some_header.h" 1
int bla(int a, char b);
# 3 "example.c" 2

int bla(int a , char b) {
    return 0;
}

int main() {
    printf("Hello world\n");
    printf("max(2,3)=%d\n", ((2) > (3) ? (2) : (3)));
    return 0;
}
```

Про то, что значат строчки вида `# linenum filename flags` можно прочитать вот [тут](https://gcc.gnu.org/onlinedocs/cpp/Preprocessor-Output.html).

Результат препроцессинга - тоже код на си, просто с раскрытыми инструкциями для препроцессора.


### Компиляция
Теперь поехали превращать исходный код в ассемблерный
`gcc -S  -fverbose-asm -o example.S example.c`

Получаем 
```asm
	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 12, 0	sdk_version 12, 3
	.globl	_bla                            ; -- Begin function bla
	.p2align	2
_bla:                                   ; @bla
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	str	w0, [sp, #12]
	strb	w1, [sp, #11]
	mov	w0, #0
	add	sp, sp, #16
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_main                           ; -- Begin function main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #32
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w8, #0
	str	w8, [sp, #8]                    ; 4-byte Folded Spill
	stur	wzr, [x29, #-4]
	adrp	x0, l_.str@PAGE
	add	x0, x0, l_.str@PAGEOFF
	bl	_printf
	adrp	x0, l_.str.1@PAGE
	add	x0, x0, l_.str.1@PAGEOFF
	mov	x9, sp
	mov	x8, #3
	str	x8, [x9]
	bl	_printf
	ldr	w0, [sp, #8]                    ; 4-byte Folded Reload
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	add	sp, sp, #32
	ret
	.cfi_endproc
                                        ; -- End function
	.section	__TEXT,__cstring,cstring_literals
l_.str:                                 ; @.str
	.asciz	"Hello world\n"

l_.str.1:                               ; @.str.1
	.asciz	"max(2,3)=%d\n"

.subsections_via_symbols
```

### Линковка
При линковке мы получаем один или несколько т.н. "объектных файлов" (машинный код).  
Если очень коротко: линковщик комбинирует несколько объектных файлов в один и делает финальный файл исполняемым.  
Чуть предыстории: раньше умные люди писали прямиком на assembly, потом при помощи ассемблера делали из написанного кода исполняемый. Со временем появилась необходимость комбинировать некоторые части, которые хотелось переиспользовать и надо было каким-то образом получать один файл, с программой, которая а) будет работать и б) будет включать в себя все необходимые функции/библиотеки.   
Так и появился линковщик. Теперь ассемблер, вместо готового исполняемого файла, генерит объектный файл, а задача линковщика состоит в том чтобы, собственно, "слинковать" (те скомбинировать) все нужные части вместе и выдать готовый исполняемый файл.

#### ELF

Если заглянуть в содержимое любого скомпилированного файла (он может быть библиотекой, объектным или исполняемым файлом), то можно будет увидеть кое-что интересное.

```bash
# compile any (valid) program
gcc -oa example.c
hexdump -C a
```

```
00000000  7f 45 4c 46 02 01 01 00  00 00 00 00 00 00 00 00  |.ELF............|
00000010  01 00 3e 00 01 00 00 00  00 00 00 00 00 00 00 00  |..>.............|
00000020  00 00 00 00 00 00 00 00  18 03 00 00 00 00 00 00  |................|
00000030  00 00 00 00 40 00 00 00  00 00 40 00 0e 00 0d 00  |....@.....@.....|
00000040  f3 0f 1e fa 55 48 89 e5  48 8d 3d 00 00 00 00 e8  |....UH..H.=.....|
00000050  00 00 00 00 b8 00 00 00  00 5d c3 48 65 6c 6c 6f  |.........].Hello|
00000060  20 77 6f 72 6c 64 00 00  47 43 43 3a 20 28 55 62  | world..GCC: (Ub|
00000070  75 6e 74 75 20 39 2e 34  2e 30 2d 31 75 62 75 6e  |untu 9.4.0-1ubun|
00000080  74 75 31 7e 32 30 2e 30  34 2e 31 29 20 39 2e 34  |tu1~20.04.1) 9.4|
00000090  2e 30 00 00 00 00 00 00  04 00 00 00 10 00 00 00  |.0..............|
...
```

Идея в том, что файлы компилируются в формат ELF (Executable and Linkable Format). Его видно по байтам `7f 45 4c 46` в начале файла. [Прикольная картинка](https://upload.wikimedia.org/wikipedia/commons/e/e4/ELF_Executable_and_Linkable_Format_diagram_by_Ange_Albertini.png)

Файл исполняемый, если
1) Он начинается с волшебных байтов
2) Если файл текстовый, то
   1) в начале файла должен стоять шебанг (`#! + абсолютный путь`). Нужно проверить чтобы файл за шебангом был исполняемым (см. пункт 1)
   2) Файл в принципе можно исполнять `ls -l`. (`chmod +x`, если нет).

\* нужно не забывать, что, например, скомпилированная библиотека тоже будет начинаться с "магических чиселок", но исполняемым  файлом не будет.



<!-- демо на конец, если останется время
на тачке с линухой
gcc -c -v -oa printf.c
gcc  -v -oa.final printf.c

objdump -d a
readelf -r a
objdump -s -j .rodata a


objdump -d a.final
-->
<!-- 
тут я для пытливых умов скопил то что понаходил. но, кажется, что лучше об этом начать говорить уже после того как мы чуть-чуть ~~помучаемся~~ поработаем с ассемблером. там скорее всего эти ссылки будут полезны.

https://stackoverflow.com/questions/12122446/how-does-c-linking-work-in-practice 
https://wh0rd.org/books/linkers-and-loaders/linkers_and_loaders.pdf
https://www.airs.com/blog/archives/38 
https://www.airs.com/blog/archives/39
http://www.sco.com/developers/gabi/2003-12-17/ch4.reloc.html
https://cirosantilli.com/elf-hello-world
https://stackoverflow.com/questions/7718299/whats-an-object-file-in-c
https://dustin.schultz.io/how-is-glibc-loaded-at-runtime.html
https://www.cs.cmu.edu/afs/cs/academic/class/15213-s13/www/lectures/12-linking.pdf

vim hello_world.asm
` nasm -felf64 hello_world.asm`
` ld -o hello_world.out hello_world.o`
` objdump -d hello_world.o`
readelf -r hello_world.o

```
0000000000000000 <main>:
   0:   f3 0f 1e fa             endbr64 
   4:   48 83 ec 08             sub    $0x8,%rsp
   8:   31 c0                   xor    %eax,%eax
   a:   e8 00 00 00 00          callq  f <main+0xf> <-- до линковки
   f:   31 c0                   xor    %eax,%eax
  11:   48 83 c4 08             add    $0x8,%rsp
  15:   c3                      retq   
```

```
0000000000001040 <main>:
    1040:       f3 0f 1e fa             endbr64 
    1044:       48 83 ec 08             sub    $0x8,%rsp
    1048:       31 c0                   xor    %eax,%eax
    104a:       e8 01 01 00 00          callq  1150 <swap> <-- после линковки
    104f:       31 c0                   xor    %eax,%eax
    1051:       48 83 c4 08             add    $0x8,%rsp 
    1055:       c3                      retq   
    1056:       66 2e 0f 1f 84 00 00    nopw   %cs:0x0(%rax,%rax,1)
    105d:       00 00 00 

```

-->

## Make
Для удобства сборки проектов и для автоматизации сборки больших проектов используют утилиту Make.

Вот её общий формат :

```
имя_цели: список зависимостей
	команда 1
	команда 2
	...
	команда N
```

Зависимостями могут быть определенные файлы исходных текстов, либо файлы промежуточного этапа компиляции, которые сами являются другими целям сборки. 
Целей может быть несколько, причем имена целей не обязаны совпадать с именами генерируемых ими файлов. При запуске make без параметров выполняется сборка той цели, которая описана самой первой, поэтому часто выделяют отдельную цель под названием first. Кроме того, предусматривают отдельную цель под названием clean, которая должна оставить после себя только конечный результат, удалив все временные файлы.


### Переменные 
Примеры использования [тут](https://makefiletutorial.com/#variables-pt-2)
`=` - рекурсивные. Ищет используемые значения других переменных когда переменная используется, а не когда объявлена.
`:=` - ищем значения переменных внутри когда объявляем новую переменную.
`?=` - пишем новое значение только если переменная не объявлена.
`+=` - append

### Implicit rules
Make умеет компилировать си/си++ без того, чтобы указывать для этого специальные цели.
Вот список "подразумеваемых" правил:
- Компиляция программы на си: n.o автоматически создаётся из n.c при помощи команды `$(CC) -c $(CPPFLAGS) $(CFLAGS)`
- Компиляция программы на си++: n.o создаётся из n.cc/n.cpp `$(CXX) -c (CPPFLAGS) $(CXXFLAGS)`
- Линковка: n создаётся автоматически из n.o `$(CC) $(LDFLAGS) n.o $(LOADLIBES) $(LDLIBS)`

Пример [Makefile](./Makefile)

Тырил часть инфы [отсюда](https://github.com/victor-yacovlev/fpmi-caos/tree/master/practice/linux_basics#%D0%B0%D0%B2%D1%82%D0%BE%D0%BC%D0%B0%D1%82%D0%B8%D0%B7%D0%B0%D1%86%D0%B8%D1%8F-%D1%81%D0%B1%D0%BE%D1%80%D0%BA%D0%B8-%D1%81-%D0%BF%D0%BE%D0%BC%D0%BE%D1%89%D1%8C%D1%8E-makefile)

Очень полный туториал по Make: [essence of make](https://makefiletutorial.com/#the-essence-of-make).

## GDB
Чтобы эффективнее пользоваться дебаггером, нужно компилировать программу с опцией  `-g`.

Основные команды gdb:
- run - запуск программы, после run можно указать аргументы;
- break - установить точку останова, параметрами этой команды может имя функции или пара ИМЯ_ФАЙЛА:НОМЕР_СТРОКИ;
- s (step) - шаг внутрь функции;
- return - выйти из текущей функции наверх;
- continue - продолжить выполнение до следующей точки останова или исключительной ситуации;
- print - вывести значение переменной из текущего контекста выполнения;
- layout src|regs|split - переключение вида консольного отладчика;