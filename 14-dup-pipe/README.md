# Пишем баш

И снова здравствуйте! Надеюсь, все ~~хорошо отдохнули~~ сдали матан.

Весь прошлый семестр мы работали с одним процессом, в этом семестре будем в основном смотреть на разные способы заставлять эти процессы общаться друг с другом: pipe-ы, сигналы, память, сеть и тд

Поехали!

# Дубликаты файловых дескрипторов
Начнём с вопроса: что нужно сделать, чтобы при помощи сисколлов `close`/`open` подменить себе поток вывода (да и  любой другой потом на самом деле) каким-то файлом?

Мы с вами люди образованные и понимаем, как устроены таблицы файловых дескрипторов (понимаем же, да?), поэтому шарим, что когда кто-то вызывает open, открывается наименьший доступный файловый дескриптор. Получается, что если закрыть stdout, а потом вызвать open, то наши все printf-ы с std::cout-ами будут писаться в файл.

Так вот, на самом деле ничего подобного: никто не даёт вам гарантии, что это всё отработает именно так как вы хотите. может: закрыться stdin, прилететь сигнал, хендлер которого открывает файл, open может зафейлиться в конце концов, а ещё эта операция неатомарна (что бы это не значило).

Мотивация, надеюсь, понятна.

## `dup`
Поехали вместе читать [документацию](https://man7.org/linux/man-pages/man2/dup.2.html):
```
    the dup() system call allocates a new file descriptor that refers
    to the same open file description as the descriptor oldfd.  (for
    an explanation of open file descriptions, see open(2).)  the new
    file descriptor number is guaranteed to be the lowest-numbered
    file descriptor that was unused in the calling process.

    after a successful return, the old and new file descriptors may
    be used interchangeably.  since the two file descriptors refer to
    the same open file description, they share file offset and file
    status flags; for example, if the file offset is modified by
    using lseek(2) on one of the file descriptors, the offset is also
    changed for the other file descriptor.

    the two file descriptors do not share file descriptor flags (the
    close-on-exec flag).  the close-on-exec flag (fd_cloexec; see
    fcntl(2)) for the duplicate descriptor is off.
```



Пример кода: [`dup/dup.c`](dup/dup.c). Не забываем закрывать все открытые файловые дескрипторы и проверять всё что можем на ошибки!


## `dup2` ~~Ваша рабочая лошадка~~
Читаем ман дальше:
```
    the dup2() system call performs the same task as dup(), but
    instead of using the lowest-numbered unused file descriptor, it
    uses the file descriptor number specified in newfd.  in other
    words, the file descriptor newfd is adjusted so that it now
    refers to the same open file description as oldfd.

    if the file descriptor newfd was previously open, it is closed
    before being reused; the close is performed silently (i.e., any
    errors during the close are not reported by dup2()).

    the steps of closing and reusing the file descriptor newfd are
    performed atomically.  this is important, because trying to
    implement equivalent functionality using close(2) and dup() would
    be subject to race conditions, whereby newfd might be reused
    between the two steps.  such reuse could happen because the main
    program is interrupted by a signal handler that allocates a file
    descriptor, or because a parallel thread allocates a file
    descriptor.

    note the following points:

    *  if oldfd is not a valid file descriptor, then the call fails,
        and newfd is not closed.

    *  if oldfd is a valid file descriptor, and newfd has the same
        value as oldfd, then dup2() does nothing, and returns newfd.
```

Примеры: [`dup/dup2-file.c`](dup/dup2-file.c), [`dup/dup2-stdout.c`](dup/dup2-stdout.c)


## dup3 
```
    dup3() is the same as dup2(), except that:

    *  the caller can force the close-on-exec flag to be set for the
        new file descriptor by specifying o_cloexec in flags.  see the
        description of the same flag in open(2) for reasons why this
        may be useful.

    *  if oldfd equals newfd, then dup3() fails with the error
        einval.
```

# pipe (неименованный канал ~~(или труба)~~)
Смотрим ascii-art и думаем о том, что кому-то было не лень сидеть и это рисовать:
```
                      .---------------------------.
                     /                            /\
    write end --    | pipedfd[1]       pipedfd[0]|  |  -- read end
process1 -------->  |                            | -----> process2
                    | input                output|  |
                     \____________________________\/
```

Канал - пара дескрипторов, в один пишем, из другого читаем. 

Документация: [`man 7 pipe`](https://man7.org/linux/man-pages/man7/pipe.7.html), [`man 2 pipe`](https://man7.org/linux/man-pages/man2/pipe.2.html)


Интерфейс такой:
```c
#include <unistd.h>

int pipe(int pipefd[2]);
```

Важно: **pipefd[0] - для чтения, pipefd[1] - для записи**. ~~смотри не перепутай, кутузов!~~

### А как писать?
Пишем мы при помощи write, первый аргумент pipefd[1]. 

Случаи:
- Если в буффере есть место, и пишем мы меньше данных чем размер буффера, write завершится мгновенно.
- Ждём, если буффер заполнен (пример туду) (buffer-overflow.c)
- Получим по шапке sigpipe-ом, если все дескрипторы на чтение закрыты. см. пример [`pipe-examples/pipe-unclosed-reader.c`](pipe-examples/pipe-unclosed-reader.c)

### А как читать?
Читаем read-ом, первый аргумент pipefd[0].

Случаи:
- Если в буффере есть данные, читаем и завершаемся.
- Если буффер пустой, но есть хотя бы один открытый файловый дескриптор на запись, блокируемся. см пример
- Буффер пустой и все дескрипторы на запись закрыты, read вернёт 0. это штатная ситуация.

Простые примеры: [`pipe/pipe-simple.c`](pipe/pipe-simple.c), [`pipe/pipe-simple-alternative.c`](pipe/pipe-simple-alternative.c). Тут два способа делать одно и то же. Рекомендуется делать так (без ноликов и единичек в коде), чтобы самим не путаться и другим было приятно ваш код читать.

### Что бывает если не закрывать дескрипторы?
Бывает дедлок - ситуация, когда несколько процессов ожидают некоторый ресурс, и ни один из них не может продолжить исполняться. 

Примеры: 
1) Если из читателя не закрыть конец для записи.  
При вызове форка, все файловые дескрипторы дублируются. Как только у канала нет писателей, read получает eof. Если забыть закрыть фд на запись, мы этот eof не получим, потому что открытый файловый дескриптор никуда не уйдёт.  
Пример тут: [`pipe-examples/unclosed-writer.c`](./pipe-examples/unclosed-writer.c)
2) Если из писателя не закрыть конец для чтения.   
Если при этом у не все читатели будут закрыты, то мы не получим sigpipe и будем вечно ждать.   
Пример вот:  [`pipe-examples/unclosed-reader.c`](./pipe-examples/unclosed-reader.c)

3) Если перестать читать, не закрывать дескприторы на чтение и при этом продолжать писать.  
Тут всё просто: [`pipe-examples/pipe-buffer-overflow.c`](./pipe-examples/pipe-buffer-overflow.c)

### pipe capacity, `pipe_buf` 
Смотрим два эти раздела в [документации](https://man7.org/linux/man-pages/man7/pipe.7.html), потому что это важно.

#### pipe capacity && `pipe_buf`
Канал буфферизованный, это значит, что если кто-то пишет в канал, но никто не читает (при этом есть открытые файловые дескрипторы), узнаете вы об этом где-то через 65 килобайт. Менять настройку можно при помощи сисколла `fcntl` и операций `f_setpipe_sz`.  

Записи меньше, чем PIPE_BUF байт - атомарные, это значит что контент размера меньше чем PIPE_BUF в пайпе не перемешается, а вот если вы пишете больше чем PIPE_BUF из нескольких процессов, то данные могут перемешаться.


## Возвращаемся к башу
Используя то, что мы знаем, мы можем на коленке написать башовую магию, а именно скрипты, которые могут заменять наши всеми любимые операторы '|' и '>'.

~~В папке bash доступны примеры pipe.c и redir.c которые перенаправляют поток из одной программы в другую, и, соответственно, перенаправляют поток вывода в файл. именно так и реализованы эти команды в баше. теперь вы это знаете.~~  Написать pipe это ваша домашка, поэтому я их залью попозже :smile:


## Именованные каналы (fifo, named pipe)
man 3 fifo
```
a  fifo  special  file is similar to a pipe, except that it is created in a
different way.  instead of being an  anonymous  communications  channel,  a
fifo special file is entered into the filesystem by calling mkfifo().

once you have created a fifo special file in this way, any process can open
it for reading or writing, in the same way as an ordinary  file.   however,
it  has to be open at both ends simultaneously before you can proceed to do
any input or output operations on it.  opening a fifo for reading  normally
blocks  until  some other process opens the same fifo for writing, and vice
versa.  see fifo(7) for nonblocking handling of fifo special files.
```

Их удобно связывать, когда нужно связать два процесса пайпом, но при этом не можем никак сделать один процесс ребёнком другого. Примеры доступны в папочке [`fifo`](./fifo)

Ещё можно пользоваться башом.
```bash
mkfifo pipe
# 1 terminal
echo "hello world" > pipe

# 2 terminal 
cat pipe
```

<!--  потом обсудим: # addendum: ещё чуть про форки и демоны -->

# fcntl 
https://man7.org/linux/man-pages/man2/fcntl.2.html

Увеличить размер пайпа , сделать dup - fcntl это швейцарский нож, который умеет делать с файлами практически всё.

Примеры печатнова https://github.com/yuri-pechatnov/caos/tree/master/caos_2020-2021/sem16-fcntl-dup-pipe#fcntl_dup