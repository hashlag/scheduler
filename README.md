# Планировщик

### Описание

Класс `Scheduler`, управляющий выполнением связанных между собой задач. Задачи могут зависеть друг от друга, и будущий результат выполнения одной задачи может быть использован в качестве параметра для другой.

### Пример использования

Рассмотрим пример нахождения корней квадратного уравнения:

```cpp
float a = 1;
float b = -2;
float c = 0;

sched::Scheduler scheduler;

auto id1 = scheduler.add([](float a, float c){return -4 * a * c;}, a, c);
auto id2 = scheduler.add([](float b, float v){return b * b + v;}, b, scheduler.get_future_result<float>(id1));
auto id3 = scheduler.add([](float b, float d){return -b + std::sqrt(d);}, b, scheduler.get_future_result<float>(id2));
auto id4 = scheduler.add([](float b, float d){return -b - std::sqrt(d);}, b, scheduler.get_future_result<float>(id2));
auto id5 = scheduler.add([](float a, float v){return v/(2*a);}, a, scheduler.get_future_result<float>(id3));
auto id6 = scheduler.add([](float a, float v){return v/(2*a);},a, scheduler.get_future_result<float>(id4));

scheduler.execute_all();

std::cout << "x1 = " << scheduler.get_result<float>(id5) << std::endl;
std::cout << "x2 = " << scheduler.get_result<float>(id6) << std::endl;
```

### Публичный интерфейс `TTaskScheduler`

- `add` - добавляет задачу в планировщик. Возвращает идентификатор задачи.
- `get_future_result<T>` - возвращает объект, через который иная задача может запросить результат выполнения данной.
- `get_result<T>` - возвращает результат выполнения задачи типа `T`, вычисляет его, если он еще не был подсчитан.
- `execute_all` - выполняет все запланированные задачи.
- `remove` - удаляет задачу из планировщика.

### Требования и ограничения

- Задачи должны быть [Callable objects](https://en.cppreference.com/w/cpp/named_req/Callable) (возможно, move-only).
- Возвращаемое значение [CopyConstructible](https://en.cppreference.com/w/cpp/named_req/CopyConstructible).
- Аргументы могут быть move-only.
