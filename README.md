# Планировщик

## Описание

Класс `Scheduler`, управляющий выполнением связанных между собой задач. Задачи могут зависеть друг от друга, и будущий результат выполнения одной задачи может быть использован в качестве параметра для другой.

## Пример использования

Рассмотрим пример нахождения корней квадратного уравнения:

```c++
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

## Публичный интерфейс `Scheduler`

### `add()`

```c++
template <typename Fn, typename... Args>
TaskId add(Fn&& fn, Args&&... args);
```

Добавляет задачу в планировщик.

### Параметры

| Имя    | Тип                | Описание                                    |
|--------|--------------------|---------------------------------------------|
| `fn`   | `typename Fn`      | Функция, для выполнения в качестве задачи.  |
| `args` | `typename... Args` | Аргументы для передачи в `fn`.              |

### Возвращаемое значение

Идентификатор задачи типа `TaskId`.

### `get_future_result()`

```c++
template <typename ReturnType>
FutureResult<ReturnType> get_future_result(TaskId id);
```

Возвращает объект соответствующий будущему результату выполнения задачи.
Используется для передачи в другую задачу ещё не подсчитанного результата выполнения данной.

### Параметры

| Имя    | Тип      | Описание              |
|--------|----------|-----------------------|
| `id`   | `TaskId` | Идентификатор задачи. |

### Возвращаемое значение

Объект типа `FutureResult<ReturnType>`.

### `get_result()`

```c++
template <typename ReturnType>
ReturnType& get_result(TaskId id);
```

Возвращает результат выполнения задачи типа `ReturnType`, вычисляет его, если он еще не был подсчитан.

### Параметры

| Имя    | Тип      | Описание              |
|--------|----------|-----------------------|
| `id`   | `TaskId` | Идентификатор задачи. |

### Возвращаемое значение

Ссылка на результат выполнения задачи.

### `execute_all()`

```c++
void execute_all();
```

Выполняет все запланированные задачи.

### `remove()`

```c++
void remove(TaskId id);
```

Удаляет задачу из планировщика.

### Параметры

| Имя    | Тип      | Описание              |
|--------|----------|-----------------------|
| `id`   | `TaskId` | Идентификатор задачи. |

## Требования и ограничения

- Задачи должны быть [Callable objects](https://en.cppreference.com/w/cpp/named_req/Callable) (возможно, move-only).
- Возвращаемое значение [CopyConstructible](https://en.cppreference.com/w/cpp/named_req/CopyConstructible).
- Аргументы могут быть move-only.

## Тестирование

Код покрыт unit-тестами при помощи [gtest](https://github.com/google/googletest).
