# [Stackful] Coroutine

_Сопрограмма_ (_coroutine_) или просто _корутина_ – это процедура,
из вызова которой можно выйти в середине (_остановить_ вызов, _suspend_), а затем вернуться в этот вызов
и продолжить исполнение с точки остановки.

Сопрограмма расширяет понятие _подпрограммы_ (_subroutine_), вызов которой нельзя остановить, а можно лишь завершить.

## `Coroutine`

Экземпляр класса `Coroutine` представляет вызов процедуры,
1) который может остановиться (операция `Suspend`), и
2) который затем можно возобновить (операция `Resume`).

Создадим корутину:

```cpp
Coroutine co(routine);
```

Здесь `routine` – произвольная пользовательская процедура, которая будет исполняться корутиной `co`. 

Непосредственно создание корутины не приводит к запуску `routine`.

Созданная корутина запускается вызовом `co.Resume()`. После этого управление передается процедуре `routine`, и та исполняется до первого вызова `Suspend()` (или до своего завершения).

Вызов `Suspend()` в корутине останавливает ее исполнение, передает управление обратно caller-у и завершает его вызов `co.Resume()`. Вызов `Suspend()` – это точка выхода из корутины, _suspension point_.

Следующий вызов `co.Resume()` вернет управление остановленной корутине, вызов `Suspend()` в ней завершится, и она продолжит исполнение до очередного `Suspend()` или же до завершения своей процедуры.

С исключениями корутины взаимодействуют как обычные функции: если в корутине было выброшено и не перехвачено исключение, то оно вылетит в caller-е из вызова `co.Resume()` и полетит дальше (выше) по цепочке вызовов.

Процедура, исполняемая в корутине, не имеет доступа к самому объекту `Coroutine`. Чтобы остановить исполнение, процедура вызывает статический метод `Suspend`.

Для лучшего понимания API и потока управления в корутинах изучите [тесты](tests/main.cpp) к задаче.

### Пример

```cpp
void StackfulCoroutineExample() {
  using coro::Coroutine;
  
  // Роль: caller

  // Stackful корутина
  Coroutine co([] {
    // Роль: callee
    fmt::println("Step 2");
    
    Coroutine::Suspend();  // <-- Suspension point

    fmt::println("Step 4");
  });
  // <-- Исполнение процедуры `routine` пока не стартовало

  fmt::println("Step 1");

  // Стартуем корутину
  // Управление передается процедуре routine,
  // и та исполняется до первого вызова Suspend().
  co.Resume();

  fmt::println("Step 3");

  co.Resume();

  // Вызов IsCompleted() возвращает true если корутина уже завершила 
  // свое исполнение (дойдя до конца `routine` или бросив исключение).
  assert(co.IsCompleted());

  // Попытка вызова Suspend вне корутины - UB
  // Coroutine::Suspend()
}
```

На экране будет напечатано:
```
Step 1
Step 2
Step 3
Step 4
```

## Типы корутин

В этой задаче мы говорим про конкретный вид корутин – _stackful_: 

Мы подразумеваем, что в вызове `Suspend` корутина сохраняет текущий контекст исполнения и активирует контекст исполнения родителя, останавливая тем самым **весь текущий стек вызовов**.

Другой вид корутин – _stackless_: они не используют переключение контекста, останавливаясь, они останавливают только **непосредственно текущий вызов**, но не останавливают вызовы выше по стеку.

Разная семантика обусловлена реализацией: 
- _stackful_ – это реализация на уровне библиотеки, с помощью механизма переключения контекста
- _stackless_ – это реализация корутин на уровне языка программирования, через переписывание программы прямо в компиляторе или же с помощью макросов.

## Корутины и файберы

И stackful корутины, и файберы – исполняемые сущности, которые могут останавливаться и затем возобновлять исполнение.
И те, и другие используют механизм переключения контекста для нелокальной передачи управления.

Но стоит отличать их друг от друга!

### Файберы

Файберы – это кооперативная многозадачность: независимые активности, чередующиеся на процессоре.

За исполнение файберов отвечает планировщик, его задача – распределять файберы между потоками (аналогично планировщику операционной системы, который распределяет потоки между ядрами процессора) и при остановке файбера выбирать следующий файбер ему на замену.

Помимо планировщика, файберам нужны собственные средства синхронизации (мьютексы, ивенты, каналы и т.д.), которые не будут блокировать потоки планировщика в точках ожидания.

### Корутины

Корутины гораздо ближе к обычным функциям, чем к файберам.

Корутины не имеют прямого отношения к многозадачности и потокам.

Корутины не нуждаются в примитивах синхронизации. 

Корутинам не нужен планировщик, который выбирал бы, кому передать управление при остановке исполнения. Управление передается по заданным правилам: от caller-а к callee и обратно через вызовы `Resume` и `Suspend`, как при вызове обычных функций.


## Применения корутин

### Асинхронность

Корутины – инструмент для выражения конкурентности и работы с асинхронностью в современных языках программирования.

В этой задаче с помощью корутин мы реализуем файберы – легковесные кооперативные потоки.

### Итераторы

С помощью корутин можно писать итераторы по рекурсивным структурам данных: [Iteration Inside and Out](https://journal.stuffwithstuff.com/2013/01/13/iteration-inside-and-out/)

### Процессоры / генераторы

Процессоры и генераторы позволяют описывать конвейеры / графы обработки потоков данных.

#### Push

В _процессорах_ поток данных направлен от caller к callee:

```cpp
void ProcessorExample() {
  using namespace exe::coroutine::processors;
  
  Processor<std::string> consumer([]() {
    // Функция `Receive` останавливает исполнение процессора до
    // вызова caller-ом метода `Send`
    while (auto data = Receive<std::string>()) {
      fmt::println("{}", *data);
    }
  });
  
  // Модель push
  
  // Посылаем процессору порции данных для обработки
  consumer.Send("Hello");
  // <-- На экране напечаталось `Hello`
  consumer.Send("World");
  // <-- На экране напечаталось `World`
  consumer.Close();
  // <-- Процедура `consumer`-а завершилась
}
```

Оригинальное применение корутин – реализация модульного компилятора, где каждый модуль представлен корутиной, и эти модули организованы в конвейер: [Design of a Separable Transition-Diagram Compiler](https://www.melconway.com/Home/pdf/compiler.pdf)

### Pull

В _генераторах_ поток данных направлен в обратную сторону, от callee к caller-у:

```cpp
void GeneratorExample() {
  using namespace exe::coroutine::generators;
  
  Generator<int> countdown([]() {
    for (int i = 10; i >= 0; --i) {
      Send(i);
    }
  });
  
  // Модель pull
  
  while (auto next = countdown.Receive()) {
    // next - std::optional
    fmt::printlln("{}", *next);
  }
  fmt::println("Launch");
}
```

## Реализация

### API

Публичное API класса `Coroutine` зафиксировано, менять его не следует.

### `ExecutionContext`

Корутина умеет останавливать и возобновлять свое исполнение, а значит ей потребуется [`ExecutionContext`](https://gitlab.com/Lipovsky/sure/-/blob/master/sure/context.hpp).

1) Прочтите [документацию по `ExecutionContext`](https://gitlab.com/Lipovsky/sure)
2) Посмотрите как он используется в [TinyFibers](https://gitlab.com/Lipovsky/tinyfibers)

### Исключения

Используйте [std::exception_ptr](https://en.cppreference.com/w/cpp/error/exception_ptr) для прокидывания исключения из корутины в caller-а.