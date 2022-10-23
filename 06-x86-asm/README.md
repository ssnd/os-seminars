# x86 ассемблер
Ридинги: 
- [Яковлев](https://github.com/victor-yacovlev/fpmi-caos/tree/master/practice/asm/x86_basics)
- Печатнов [раз](https://github.com/yuri-pechatnov/caos/blob/master/caos_2020-2021/sem07-x86-asm/asm_x86.md) [два](https://github.com/yuri-pechatnov/caos/blob/master/caos_2020-2021/sem07-x86-asm/asm_x86-64.md)

Ссылки: 
- https://en.wikibooks.org/wiki/X86_Assembly
- https://www.felixcloutier.com/x86/

На берегу надо обсудить одну вещь: **нельзя** путать х86-32 и х86-64 ассемблер, это разные (пусть иногда и похожие вещи). Акцент мы будем делать на х86, но я буду упоминать некоторые штуки про 32 чтобы мы имели более ясную картину мира. Задачки в любом случае вы будете писать на х86-64 ~~царствие небесное~~.


## Регистры 
Про регистры очень хорошо написано у [Печатнова](https://github.com/yuri-pechatnov/caos/blob/master/caos_2020-2021/sem07-x86-asm/asm_x86-64.md#-%D1%80%D0%B5%D0%B3%D0%B8%D1%81%D1%82%D1%80%D1%8B).
Не вижу смысла копипастить.

## Чуть-чуть про calling conventions
Первая важная вещь, которую нужно осознавать: calling conventions - **не** часть языка, это фича компилятора.
Calling conventions для x32 и того что было раньше дизайнились десятилетиями разными людьми и для очень разных целей. Неплохой ответ со <a href="https://stackoverflow.com/questions/3428332/why-are-there-so-many-different-calling-conventions">stackoverflow</a> на эту тему:
> The calling conventions you mention were designed over the course of decades for different languages and different hardware. They all had different goals. cdecl supported variable arguments for printf. stdcall resulted in smaller code gen, but no variable arguments. Fastcall could greatly speed up the performance of simple functions with only one or two arguments on older machines (but is rarely a speed up today.)


Небольшой экскурс в историю calling conventions от Raymon Chen: <a href="https://devblogs.microsoft.com/oldnewthing/20040102-00/?p=41213">раз</a>, <a href="https://devblogs.microsoft.com/oldnewthing/20040107-00/?p=41183">два</a>, <a href="https://devblogs.microsoft.com/oldnewthing/20040108-00/?p=41163">три</a>

Ещё есть достаточно полезная статья на [википедии](https://en.wikipedia.org/wiki/X86_calling_conventions) на эту же тему.

Ну, мы же на акосе собрались как-никак и интересно потрогать как эти соглашения о вызовах работают. Заглядываем в файл <a href="./x86-32-calling-conventions/x86-32-calling-conventions.c">`x86-32-calling-conventions.c`</a>. 
- Компилим с флагом `-m32`. Не забываем перед этим поставить 
- Cмотрим на `objdump -D`
- Осознаём что директивы для компилятора сработали и семантика вызова функций действительно меняется в зависимости выбранного calling convention
- ~~Благополучно об этом забываем и идём писать х86-64 ассемблер.~~

## System V AMD64 ABI
На х86-64 сильных холиваров между разными calling convention-ами не было, поэтому в основном все пользуются **System V AMD64 ABI**, на ней мы чуть больше сконцетрируемся.

`rax`, `rbx`, `rcx`, `rdx` - регистры общего назначения.
`rsp` - указатель на вершину стека
`rbp` - указатель на начало фрейма (но можно использовать аккуратно использовать как регистр общего назначения)
`rsi`, `rdi` - странные регистры для копирования массива, по сути регистры общего назначения, но ограниченные в возможностях.
`r8`...`r15`

Целочисленные аргументы передаются последовательно в регистрах: `rdi`, `rsi`, `rdx`, `rcx`, `r8`, `r9`. Если передается более 6 аргументов, то оставшиеся - через стек. Вещественные аргументы передаются через регистры `xmm0`...`xmm7`.

Возвращаемое значение записывается в регистр `rax`.

Вызываемая функция обязана сохранять на стеке значения регистров общего назначения `rbx`, `rbp`, `r12`...`r15`.

Кроме того, при вызове функции для 64-разрядной архитектуры есть дополнительное требование - перед вызовом функции стек должен быть выровнен по границе 16  байт (ловим флешбеки с ARMa), то есть необходимо уменьшить значение rsp таким образом, оно было кратно 16. 

Если кроме регистров задействуется стек для передачи параметров, то они должны быть прижаты к нижней выровненной границе стека.


## Красная зона
Для функций гарантируется 128-байтная "красная зона" в стеке ниже регистра rsp - область, которая не будет затронута внешним событием, например, обработчиком сигнала. Таким образом, можно задействовать для адресации локальных переменных память до rsp-128.
Статья про это от того же Рэймонда Чена: https://devblogs.microsoft.com/oldnewthing/20190111-00/?p=100685


## Intel & AT&T syntaxes
Всё что ниже стырил у [Яковлева](https://github.com/victor-yacovlev/fpmi-caos/tree/master/practice/asm/x86_basics#%D1%81%D0%B8%D0%BD%D1%82%D0%B0%D0%BA%D1%81%D0%B8%D1%81-att-%D0%B8-intel).

Исторически сложилось два синтаксиса языка ассемблера x86: синтаксис AT&T, используемый в UNIX-системах, и синтаксис Intel, используемый в DOS/Windows.

Различие, в первую очередь, относится к порядку аргументов команд.

Компилятор gcc по умолчанию использует синтаксис AT&T, но с указанием опции -masm=intel может переключаться в синтаксис Intel.

Кроме того, можно указать используемый синтаксис первой строкой в тексте самой программы:

`.intel_syntax noprefix`  
Здесь параметр noprefix после `.intel_syntax` указывает на то, что помимо порядка аргументов, соответствующих синтаксису Intel, ещё и имена регистров не должны начинаться с символа %, а константы - с символа $, как это принято в синтаксисе AT&T.

Мы будем использовать именно этот синтаксис, поскольку с его использованием написано большинство доступной документации и примеров, включая документацию от производителей процессоров.


Также, не пугайтесь .globl, это тоже самое что .global. [Пруф](https://sourceware.org/binutils/docs/as/Global.html).

Для синтаксиса Intel первым аргументов команды является тот, значение которого будет модифицировано, а вторым - которое остается неизменным. Для синтаксиса AT&T порядок аргументов - противоположный, то есть команда add %eax, %ebx вычислит сумму %eax и %ebx, после чего сохранит результат в регистр %ebx, который указан вторым аргументом.

```
add     DST, SRC        /* DST += SRC */
sub     DST, SRC        /* DST -= SRC */
inc     DST             /* ++DST */
dec     DST             /* --DST */
neg     DST             /* DST = -DST */
mov     DST, SRC        /* DST = SRC */
imul    SRC             /* (eax,edx) = eax * SRC - знаковое */
mul     SRC             /* (eax,edx) = eax * SRC - беззнаковое */
and     DST, SRC        /* DST &= SRC */
or      DST, SRC        /* DST |= SRC */
xor     DST, SRC        /* DST ^= SRC */
not     DST             /* DST = ~DST */
cmp     DST, SRC        /* DST - SRC, результат не сохраняется, */
test    DST, SRC        /* DST & SRC, результат не сохраняется  */
adc     DST, SRC        /* DST += SRC + CF */
sbb     DST, SRC        /* DST -= SRC - CF */
```


В отличии от процессоров ARM, где обновление регистра флагов производится только при наличии специального флага в команде, обозначаемого суффиксом `s`, в процессорах Intel флаги обновляются всегда большинстом инструкций.

Флаг `ZF` устанавливается, если в результате операции был получен нуль.

Флаг `SF` устанавливается, если в результате операции было получено отрицательное число.

Флаг `CF` устанавливается, если в результате выполнения операции произошел перенос из старшего бита результата. Например, для сложения `CF` устанавливается если результат сложения двух беззнаковых чисел не может быть представлен 32-битным беззнаковым числом.

Флаг `OF` устанавливается, если в результате выполняния операции произошло переполнение знакового результата. Например, при сложении `OF` устанавливается, если результат сложения двух знаковых чисел не может быть представлен 32-битным знаковым числом.

Обратите внимание, что и сложение `add`, и вычитание `sub` устанавливают одновременно и флаг `CF`, и флаг `OF`. Сложение и вычитание знаковых и беззнаковых чисел выполняется совершенно одинаково, и поэтому используется одна инструкция и для знаковой, и для беззнаковой операции.

Инструкции `test` и `cmp` не сохраняют результат, а только меняют флаги.

# Про адресацию памяти
Тырил у Яковлева:

В отличии от RISC-процессоров, x86 позволяет использовать в качестве один из аргументов команды как адрес в памяти.

В синтаксисе AT&T такая адресация записывается в виде: OFFSET(BASE, INDEX, SCALE), где OFFSET - это константа, BASE и INDEX - регистры, а SCALE - одно из значений: 1, 2, 4 или 8.

Адрес в памяти вычисляется как OFFSET+BASE+INDEX*SCALE. Параметры OFFSET, INDEX и SCALE являются опциональными. При их отсутсвтвии подразумевается, что OFFSET=0, INDEX=0, SCALE равен размеру машинного слова.

В синтаксисе Intel используется более очевидная нотация: [BASE + INDEX * SCALE + OFFSET].

## Семантика вызова функций

```
call LABEL:
    push next instruction address (“return address”) to stack
    jump to LABEL
```
При вызове функции колл, на стек пушится адрес возврата, потом мы прыгаем к метке. Если нам в метке, в которую мы прыгаем с коллом надо вызвать какую-то другую функцию, то нужно будет не забыть выровнять стек (запушить что-то на него или отнять нужной количество байт чтобы выравнивание было правильным).

```
ret — opposite of call:
    pop address from the stack
    jump to that address
```
## Выравнивание стека 
Стек, как было упомянуто выше, должен быть выровнен по 16 байт. И когда мы с вами будем заниматься этим выравниванием, важно осознавать одну вещь: при вызове call на стек автоматически пушится адрес след инструкции. 
То есть на стеке на момент когда нам нужно будет думать про выравнивание уже будет "заполнено" 8 байт.

## Пролог / эпилог

С прошлого раза мы помним что можно использовать использовать base pointer для быстрой развёртки стека, в х86-64 ассемблере это тоже можно делать и для этого есть также инструкции, которые позволяют лишний раз не писать этот код.

```
enter n,0 <=> 

push  %ebp
mov   %esp, %ebp     # ebp = esp,  mov  ebp,esp in Intel syntax
sub   $n, %esp       # allocate space on the stack.  Omit if n=0
```

```
leave <=>
mov   %ebp, %esp     # esp = ebp,  mov  esp,ebp in Intel syntax
pop   %ebp
```

## `lea` & `mov`
Логика загрузки значений/адресов  в х86 несколько отличается от того что мы видели в АРМе. Если у нас LDR загружал значения с адреса, а STR "сторил" значение по адресу, то тут мы используем инструкции LEA и MOV:  
```
LEA ax, [BP+SI+5] ; Compute address of value

MOV ax, [BP+SI+5] ; Load value at that address
```

Причём в работе с LEA+rip есть всякие неочевидные приёмчики:
```
The x86-64 architecture adds an RIP (instruction pointer relative) addressing. This addressing mode is specified by using ‘rip’ as a base register. Only constant offsets are valid. For example:  
AT&T: ‘1234(%rip)’, Intel: ‘[rip + 1234]’


Points to the address 1234 bytes past the end of the current instruction.


AT&T: ‘symbol(%rip)’, Intel: ‘[rip + symbol]’


Points to the symbol in RIP relative way, this is shorter than the default absolute addressing.
```
Чуть более развёрнутый ответ + ссылки на документацию: https://stackoverflow.com/questions/54745872/how-do-rip-relative-variable-references-like-rip-a-in-x86-64-gas-intel-sy

Те, если нам надо подгрузить некоторый символ, мы это делаем при помощи `[rip+.the_label_we_want]` или `.the_label_we_want[rip]` и понимаем что ассемблер посчитает адрес нужного нам символа относительно текущей инструкции.

https://news.ycombinator.com/item?id=17343140

### Size directives
Пусть нам нужно в какой-то регистр положить чиселку (самую обычную). Какого "размера" чиселку нам надо в этот регистр класть: однобайтовую, двухбайтовую или четырёхбайтовую (мы ведь помним что умеем обращаться к "младшим" частям наших регистров)? 

Чтобы в этом помочь ассемблеру, мы должны явно сказать о каком размере речь идёт при помощи size directives (директивы размера..??). 

Примеры: 
```asm
mov BYTE PTR [rbx], 2; Move 2 into the single byte at the address stored in EBX.
mov WORD PTR [rbx], 2; Move the 16-bit integer representation of 2 into the 2 bytes starting at the address in EBX.
mov DWORD PTR [rbx], 2; Move the 32-bit integer representation of 2 into the 4 bytes starting at the address in EBX.
```

Пример по работе с памятью (от Печатнова): см папочку `mem-access`

### endbrXX

Статья на хабре https://habr.com/ru/post/494000/
https://news.ycombinator.com/item?id=26061230
https://windows-internals.com/cet-on-windows/


## Полезные ссылки для того чтобы быстро заботать х86 ассемблер
https://www.cs.virginia.edu/~evans/cs216/guides/x86.html
https://sourceware.org/binutils/docs/as/i386_002dMemory.html
https://cs.brown.edu/courses/cs033/docs/guides/x64_cheatsheet.pdf
