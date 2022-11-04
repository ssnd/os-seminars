# Ядро и системные вызовы
## Полезные ссылки
- [Ридинг Яковлева](https://github.com/victor-yacovlev/fpmi-caos/tree/master/practice/asm/nostdlib_baremetal)
    Там есть часть про загрузку операционной системы - рекомендую её почитать, так как на семинаре мы скорее всего не успеем в подробностях на это поглядеть.
- Крутая [преза](https://courses.cs.washington.edu/courses/cse333/20wi/lectures/07/CSE333-L07-posix_20wi.pdf)
- [Ридинг Печатнова](https://github.com/yuri-pechatnov/caos/tree/master/caos_2020-2021/sem09-x86-asm-nostdlib)
- Ещё крутая ссылка: [Linux Insides](https://github.com/proninyaroslav/linux-insides-ru/tree/master/) - подробное и понятное объяснение (иногда с разбором исходного кода ядра) многих концептов/инструментов/абстракций линукса. Да ещё и на русском!
- [xv6](https://github.com/mit-pdos/xv6-riscv), [xv6-book](https://pdos.csail.mit.edu/6.S081/2020/xv6/book-riscv-rev1.pdf). Учебная операционная система от MIT и книга по ней. 
- [MIT 6.1810](https://pdos.csail.mit.edu/6.S081/2022/schedule.html). Очень полезный курс, с интересными заданиями на дописывание xv6 из пункта выше. Очень рекомендую! 
- https://wiki.osdev.org/ полезный портал со всякими интересностями про разработку операционных систем в принципе.


## OS organization
Для начала нужно понимать, что компьютерам в принципе больше лет чем операционным системам. Когда только появлялись, компьютеры умели одновременно исполнять только одну программу.

In the introduction, we saw that two of the main purposes of an operating system are to provide an interface between the hardware and the application programs and to manage the various pieces that make up a computer. To be more precise, these pieces are called resources. A resource is "any object which can be allocated within a system" [Balci 1996]. Some examples of resources are processors (CPUs), input/output devices, files, and memory (RAM). Thus, we can restate the purpose of the operating system in terms of resources. The operating system manages resources (resource allocation) and provides an interface to resources for application programs (resource abstraction).


Для начала стоит поговорить об "организации" операционной системы. 

Design goals операционной системы:
- Дать возможность исполнять несколько программ одновременно
- Приложения желательно должны работать изолированно друг от друга (одно приложение не должно уметь "ломать" другое)
- Хотим иметь возможность давать каждому из запущенных приложений "временные слоты" в котором они должны исполняться (time-multiplexing).


Первое что приходит на ум: давайте дадим возможность пользовательским приложениям напрямую доступ к физическим ресурсам компьютера/цпу, и пусть они сами "уступают" место на процессоре тогда, когда посчитают нужным, т.н. [cooperative/non-preemptive scheduling/multitasking](https://en.wikipedia.org/wiki/Cooperative_multitasking) (привет конькам и [корутинам](https://en.wikipedia.org/wiki/Coroutine)). Какие есть минусы у этого подхода? 
- Плохое/баганое приложение, с таким подходом мы не дадим возможность исполняться никому кроме нас
- Изолирования никакого не будет, потому что мы можем писать в любую память, в том числе в память, которая нам "не принадлежит", или, того хуже, залезть в память, которая используется ОС.


## Процессы
Чтобы обеспечить "изоляцию" ресурсов, придумали абстракцию, которая называется процесс.

Процесс определяют по-разному:

xv6 book:
> The unit of isolation in xv6 (as in other Unix operating systems) is a process. The process abstraction prevents one process from wrecking or spying on another process’s memory, CPU, file descriptors, etc. It also prevents a process from wrecking the kernel itself, so that a process can’t subvert the kernel’s isolation mechanisms. The kernel must implement the process abstraction with care because a buggy or malicious application may trick the kernel or hardware into doing something bad (e.g., circumventing isolation)

Вики (русская и английская)
> Проце́сс — это идентифицируемая абстракция совокупности взаимосвязанных системных ресурсов на основе отдельного и независимого виртуального адресного пространства в контексте которой организуется выполнение потоков. Стандарт ISO 9000:2000 Definitions определяет процесс как совокупность взаимосвязанных и взаимодействующих действий, преобразующих входящие данные в исходящие.

> In computing, a process is the instance of a computer program that is being executed by one or many threads. There are many different process models, some of which are light weight, but almost all processes (even entire virtual machines) are rooted in an operating system (OS) process which comprises the program code, assigned system resources, physical and logical access permissions, and data structures to initiate, control and coordinate execution activity. Depending on the OS, a process may be made up of multiple threads of execution that execute instructions concurrently.[1][2]


Вот что говорит код ядра: 
https://github.com/torvalds/linux/blob/master/include/linux/sched.h#L737


Вот что говорит код xv6:

```c
// Per-process state
struct proc {
  struct spinlock lock;

  // p->lock must be held when using these:
  enum procstate state;        // Process state
  void *chan;                  // If non-zero, sleeping on chan
  int killed;                  // If non-zero, have been killed
  int xstate;                  // Exit status to be returned to parent's wait
  int pid;                     // Process ID

  // wait_lock must be held when using this:
  struct proc *parent;         // Parent process

  // these are private to the process, so p->lock need not be held.
  uint64 kstack;               // Virtual address of kernel stack
  uint64 sz;                   // Size of process memory (bytes)
  pagetable_t pagetable;       // User page table
  struct trapframe *trapframe; // data page for trampoline.S
  struct context context;      // swtch() here to run process
  struct file *ofile[NOFILE];  // Open files
  struct inode *cwd;           // Current directory
  char name[16];               // Process name (debugging)
};
```

Суть всего это состоит в том, что процесс это сущность, которая абстрагирует нас от физического процессора компьютера (с ядрами, оптимизациями и другими прибамбасами). Мы просто живём в красивом мире, где инструкции исполняются последовательно, с уверенностью в том, что все процессы будут поставлены на исполнение. Это называется time-multiplexing - каждая програма получает доступ к некоторму ресурсу на некоторое время.



Отступление: таких абстракций, которые уводят нас от необходимости работать с железом, на самом деле достаточно много, вот пара примеров:
- файлы (вы когда-нибудь задумывались, что на диске на самом деле лежат не файлы), вместо disk-блоков
- те же файлы, вместо всего на свете (помните с первых семинаров что всё - файл?)
- пайпы (поговорим об этом в след семестре), вместо общей памяти
- память вместо физической памяти (тоже поговорим об этом позже).
- процессы вместо "ядер" кристала


Как же импелементить процессы?

Просто, при помощи
   1) привилегерованных и не очень инструкций
   2) планировщик
   3) виртуальной памяти

Про виртуальную память мы поговорим в следующий раз, а вот про инструкции давайте подробнее


# User space vs Kernel space

## Режимы исполнения команд
В riscv есть три режима исполнения команд: 
machine mode, supervisor mode, and user mode

В supervisor mode мы можем исполнять privileged instructions: включать/выключать прерывания и работать с важными вещами

В user mode мы можем исполять инструкции обычные, но не можем исполнять privileged инструкции

Похожий механизм используется и в х86 при помощи protection rings: у нас есть 4 уровня доступа (от 0 до 3) и ядро работает с максимальным уровнем привилегий
https://en.wikipedia.org/wiki/Protection_ring#:~:text=There%20are%204%20privilege%20levels,the%20privilege%20levels%20are%20rings.

## Как исполнять сисколлы
Просто, при помощи инструкции syscall/int/ecall в зависимости от платформы и соглашениях о вызовах (смотри папочку с нулёвкой)

## strace
strace - инструмент, который позволяет посмотреть какие сисколы были вызваны тем или иным исполняемым файлом. strace ./bin-name выведет вам всю эту информацию на экран.

## Соглашения о вызовах
The Linux AMD64 kernel uses internally the same calling conventions as userlevel applications (see section 3.2.3 for details). User-level applications that like
to call system calls should use the functions from the C library. The interface
between the C library and the Linux kernel is the same as for the user-level applications with the following differences:
123
AMD64 ABI Draft 0.99.6 – July 2, 2012 – 17:14
1. User-level applications use as integer registers for passing the sequence
%rdi, %rsi, %rdx, %rcx, %r8 and %r9. The kernel interface uses %rdi,
%rsi, %rdx, %r10, %r8 and %r9.
2. A system-call is done via the syscall instruction. The kernel destroys
registers %rcx and %r11.
3. The number of the syscall has to be passed in register %rax.
4. System-calls are limited to six arguments, no argument is passed directly on
the stack.
5. Returning from the syscall, register %rax contains the result of the
system-call. A value in the range between -4095 and -1 indicates an error,
it is -errno.
6. Only values of class INTEGER or class MEMORY are passed to the kernel.


https://stackoverflow.com/questions/2535989/what-are-the-calling-conventions-for-unix-linux-system-calls-and-user-space-f

## сисколл brk
Этот сисколл "сдвигает" границу секции data, тем самым выделяя дополнительную память на хипе. Он пригодится вам в домашке, пример его использования есть в файлике `brk.c`. Ещё полезная инфа есть [тут](https://stackoverflow.com/questions/6988487/what-does-the-brk-system-call-do) и в ридинге/записи семинара Печатнова.

Ожидается, что используя все что мы обсудили, вы сможете самостоятельно разобраться как вызывать sysbrk и другие приколы без того, чтобы вам это показывали явно.

## Что мы узнали про ядра?
В ядре не должно быть ошибок, его называют TCB (trusted computer base) https://en.wikipedia.org/wiki/Trusted_computing_base
Ошибка в ядре будет значить что у нас компьютер перестанет работать и мы потеряем данные. Любой баг в ядре - эксплойт.

Исходя из этого, давайте подумаем о правильно ли мы задизайнили наше ядро?

## Какие бывают ядра?
Да самые разные на самом деле (монолитные, микро, гибридные, экзо, нано/пико, кэш, мегалитные и т.д.), но мы с вами остановимся подробнее на трёх (на самом деле двух ):
1) [Монолитное ядро](https://wiki.osdev.org/Monolithic_Kernel). Весь менеджмент ресурсов компьютера происходит в ядре и нигде больше. Плюсы: скорость работы, тесная интеграция различных компонентов, скорость работы (через сисколы), простота имплементации. Минусы: ошибка в каком-то модуле - kernel panic и вся система упадёт, много кода.
2) [Микроядро](https://wiki.osdev.org/Microkernel). В микроядре происходит попытка все сервисы операционной системы держать в пространстве пользователя (файловая  система, работа с сетью и т.д.), в свою очередь пользовательские приложения будут общаться с сервисами и друг с другом через IPC (inter-process communication). А ядро реализует самые базовые сервисы, например, этот самый IPC, сервис для аллокации физической памяти и т.д. Из плюсов можно выделить тот факт, что у нас баг одного сервиса =/= получить кернел паник и полностью сломаться (как в монолите). В микроядре сильно меньше кода (соответственно, в нём меньше багов), меньше кода работает в пространстве ядра, соответственно микроядра, как правило, безопаснее. Минусы очевидные -  IPC это по определению очень большой оверхед, причём как для пользовательских приложений, так для интеграции сервисов друг с другом.
3) [Гибридное ядро](https://wiki.osdev.org/Hybrid_Kernel) (помесь монолитного и микроядра). Макось, например гибридное ядро, потому что оно комбинирует в себе и фичи монолитных ядер (для всех критичных тасок, в которых нужна скорость исполнения), но кроме этого есть возможность пользоваьтся фичами микроядер (например ставить кастомные драйвера для внешних устройств).


Пара прикольных переписок линуса на тему дизайна ядра:
- https://www.oreilly.com/openbook/opensources/book/appa.html 
- https://www.realworldtech.com/forum/?threadid=65915&curpostid=65936