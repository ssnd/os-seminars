# Чуть подробнее про файлы
Ссылки: 
- Ридинг Яковлева https://github.com/victor-yacovlev/fpmi-caos/tree/master/practice/stat_fcntl
- Ридинг Печатнова
- Если вы не будете бояться никаких слов в этой статье после семинара, то я всё норм рассказал https://pdos.csail.mit.edu/6.S081/2020/readings/journal-design.pdf

В прошлый раз мы с вами поговорили про файлы на уровне пользователей, в этот раз первую половину семинара мы посвятим тому, чтобы разобраться как именно хранятся файлы на диске. Во второй части обсудим ещё чуть сисколлов (некоторые их которых были в прошлые разы), но уже будем (надеюсь) смотреть на них совсем другими словами

## Что такое файловая система и что нам от неё нужно

Во-первых, давайте поговорим о разнице между файловыми системами и виртуальными файловыми системами.

Вспомним то, о чём я твержу с первого семинара: в линуксе всё файл. На протяжении прошлых семинаров у нас была возможность в этом убедиться (дружно вспоминаем /proc). Но ведь процесс - немножечко не файл, так тогда как так выходит, что мы можем смотреть список файликов-процессов и даже из них читать. 
Всё просто, procfs (как и многие другие) - виртуальная файловая система. Можно об этом думать как о некотором интерфейсе, где нужно реализовать какие-то методы (open, read, writе и т.д.), потом смонтировать и вуаля, оно создаёт файловую систему даже из таблицы процессов.
Виртуальная файловая система позволяет не только представлять в виде файлов всё что захочется, но и абстрагироваться от реальных файловых систем (ntfs, ext2/3 и т.д.) и их форматов хранения данных и давать возможность пользователю пользоваться обычными сисколлами, если у нас есть имплементация нужного нам интерфейса. Мы с этим поиграемся в конце года, когда будем работать с FUSE и будем имплементить свои файловые системы.

Больше про это тут https://opensource.com/article/19/3/virtual-filesystems-linux и тут https://stackoverflow.com/questions/24902183/what-is-the-difference-between-virtual-file-system-and-system-call#:~:text=Virtual%20File%20System%20is%20just,is%20this%20file%20stored%20in.


## Ок ладно что насчёт реальных файловых систем
Перед тем как переходить к описанию, нужно договориться о том зачем нам нужны файловые системы в принципе
- хотим, чтобы данные выживали выключение компа
- хотим как-то организовывать файлы (иерархия, названия, линки/симлинки, перемещение, копирование)
- хотим уметь делить файлы между программами/пользователями

Уже исходя из, например, этих трёх пунктов мы понимаем
- хранить данные в памяти (RAM) не вариант, нужно постоянное хранилище
- жётско прикреплять файл к какому-то пути тоже не самый лучший вариант. мб не стоит перемещать файл на диске, если поменялась его локация в иерархии директорий (читай метаданные), а не содержение? как тогда мы сможем **один и тот же** файл держать в разных директориях?
- изменения имени файла не меняет его содержимое

https://developer.ibm.com/tutorials/l-linux-filesystem/


## Да кто такие эти ваши айноды
inodе - структура данных, которая описывает файл в Линуксе. Это по сути то как **файловая система** видит файл. Когда мы держим открытый файловые дескрипторы, на самом деле мы держим структуру, в которой есть номерок айнодны, в которой в свою очередь есть инфа про наш файл.
Смотрим на код айноды в xv6:

```c
// in-memory copy of an inode
struct inode {
  uint dev;           // Device number
  uint inum;          // Inode number
  int ref;            // Reference count
  struct sleeplock lock; // protects everything below here
  int valid;          // inode has been read from disk?

  short type;         // copy of disk inode
  short major;
  short minor;
  short nlink;
  uint size;
  uint addrs[NDIRECT+1];
};
```

## А как всё хранится на диске?
Что HDD что SSD хранят информацию блоками/секторами (в разных контекстах эти значения могут быть одинаковыми или разными, для нас сейчас это будет просто какой-то атомарный кусок информации который мы умеем писать на диск). 
Нужно знать, что SSD быстрее HDD по скорости рандомного чтения (~100 микросекунд vs 5-10миллисекунд) и по скорости последовательного чтения (500мб/сек vs 100мб/сек). 
Писать последовательно быстрее чем писать в рандомные места, и писать много оптимальнее чем писать мало. 
На обоих (в большинстве случаев) размер сектора - 512 байт.


Рассмотрим на то как организована файловая система в xv6:
```
+-------------+------------+-----+------------------------+--------+----------------+
| boot sector | superblock | log | inodes (64 bytes each) | bitmap | .... data .... |
+-------------+------------+-----+------------------------+--------+----------------+
0             1            2     32                       45       46       ------>
```

Суперблок - метаданные про метаданные. Тут хранятся все основные штуки про файловую систему: тип, размер, инфо про айноды и т.д. Иногда копий суперблока может быть несколько, чтобы убедиться что данные не потеряются. Можно ещё почитать тут про это https://unix.stackexchange.com/questions/4402/what-is-a-superblock-inode-dentry-and-a-file
Log - транзакционный лог. Если коротко: чтобы не ходить на каждую запись по диску, мы сохраняем инфу про изменения блоков и потом разом "сбрасываем" все эти изменения на диск. Это помогает если, например, произошёл физический отказ. Тогда все изменения которые мы записали на диск (сделали коммит), сможем или записать или вернуть в прежнее состояние нашу файловую систему.
inodes - мы знаем
bitmap - какие блоки свободны или нет 
data - сами данные, собственно

Нужно понимать, что структура, которую я описал выше, это всего-лишь игрушечный пример xv6. В других операционных системах структура записи на диск и все основные части обычно устроены совсем по-другому.

Например: 
FAT - file allocation table (олды поймут) https://en.wikipedia.org/wiki/File_Allocation_Table#Concepts
EXT2/3/4 - тут используются журналы вместо логов https://ru.wikipedia.org/wiki/Ext2

Ну, в общем, главная суть - диск это какая-то память и вы можете на него натянуть какую структуру данных захотите и всё будет хорошо.

## Path lookup по айнодам
Длинно и сложно: https://mjmwired.net/kernel/Documentation/filesystems/path-lookup.txt
Директория - это файл (внезапно).  В директории есть dentry (directory entries), с номером айноды и названием файла. Ну всё: рутовая айнода нам известна, дан некоторый путь, мы смотрим dentries в нашей айноде, если находим идём дальше, там снова своя айнода и т.д. пока не найдём нужный нам файл.


## Что надо не забыть
Примеры: 
- удаление открытого файла (open.c). запустите программу, потом в соседнем окне терминала попробуйте удалить файл
- stat.c: как пользоваться статом
- Запомните разницу между разными stat-ами: https://github.com/victor-yacovlev/fpmi-caos/tree/master/practice/stat_fcntl#%D1%81%D1%81%D1%8B%D0%BB%D0%BA%D0%B8-%D0%B8-%D1%84%D0%B0%D0%B9%D0%BB%D0%BE%D0%B2%D1%8B%D0%B5-%D0%B4%D0%B5%D1%81%D0%BA%D1%80%D0%B8%D0%BF%D1%82%D0%BE%D1%80%D1%8B


## Вопросы с собесов
Есть две программы. Если одна удалит файл, сможет ли другая в него читать и писать? Когда при этом файл удалится с диска?
Как сделать так, чтобы подменить поток вывода каким-то файлом?
Когда файл удаляется с диска?

