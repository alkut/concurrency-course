# Getting Started

## Отказ от ответственности

Единое рабочее окружение позволяет обеспечить предсказуемое поведение сборки и тестов на ваших локальных машинах.

Работая в окружении, отличном от описанного ниже, вы **лишаетесь техподдержки**.

## Первые шаги

Пожалуйста, выполняйте описанные шаги в точности так, как они описаны!

### Шаг 1

Заведите в удобном месте директорию для работы с курсом:

```shell
# Можно выбрать произвольное имя
mkdir workspace
```

Полный путь созданной рабочей директории не должен содержать пробельных символов!

В созданной директории будут жить _два_ репозитория:
1) Репозиторий курса с задачами и тестами
2) Ваш персональный репозиторий для решений (он будет создан позже)


### Шаг 2

Клонируйте в эту директорию репозиторий курса:

```shell
cd workspace
git clone https://gitlab.com/Lipovsky/concurrency-course.git
```

Репозиторий курса должен оказаться в собственной поддиректории:

```
workspace
|
+--- concurrency-course
```

### Шаг 3

Подтяните необходимые репозиторию [сабмодули git](https://git-scm.com/book/en/v2/Git-Tools-Submodules):

```shell
cd concurrency-course
git submodule update --init --recursive
```

### Шаг 4

На этом шаге нужно подготовить рабочее окружение.

Есть два варианта:
* **Рекомендуемый**: вы разворачиваете докер-контейнер со всеми необходимыми зависимостями и работаете в нем.
* Вы самостоятельно устанавливаете необходимые зависимости в хост-систему. В этом случае вы **лишаетесь техподдержки**.


#### Докер

Следуйте [инструкции](/docs/docker.md).

Репозиторий курса в контейнере находится в директории `/workspace`.

Дальнейшие шаги инструкции выполняйте **внутри контейнера**.

#### Ручная установка

Если вы решили пойти по этому пути, то убедитесь, что у вас свежий стабильный 64-битный Linux.

Установите [необходимые зависимости](/docker/image/install_deps.sh).

### Шаг 5

Теперь нужно установить [консольный клиент Clippy](https://gitlab.com/Lipovsky/clippy) для работы с курсом. С его помощью вы будете сдавать задачи.

Если вы развернули контейнер, то залогинтесь в него и перейдите в корень репозитория. Если вы работаете без докера, то просто перейдите в корень репозитория.

Запустите установку клиента:
```shell
./install_client.sh
```

Внимательно и осознанно отвечайте на все вопросы, которые задает вам скрипт установки.

После завершения установки скрипт запустит клиент с тестовой командой `hi`.

**Перелогиньтесь в контейнер** (или перезапустите шелл если вы работаете без докера).

### Шаг 6

Теперь у вас есть клиент для работы с курсом, он называется `clippy`.

Выполните:

```shell
clippy cmake # генерируем файлы для сборки проекта курса
clippy warmup # собираем общие библиотеки
```

#### Работа с клиентом

- [Команды](https://gitlab.com/Lipovsky/clippy/-/blob/master/docs/commands.md)
- [Диаграмма](https://gitlab.com/Lipovsky/clippy/-/blob/master/docs/diagram.png)

Самые полезные команды:

- `update` – обновить локальный репозиторий с задачами
- `cmake` – перегенерировать файлы сборки (`cmake --clean` – начисто)
- `test` – запустить тесты текущей задачи
- `target` – запустить конкретную цель (CMake) задачи

### Шаг 7

[Настройте Clion](/docs/clion.md) для комфортной работы с курсом.

Этот шаг – не обязательный, но рекомендуемый: вы будете много перемещаться по коду с помощью `Go To` и выполнять автоматические рефакторинги.

В любом случае, к настройке IDE можно будет вернуться позже.

Обратите внимание: сборка проекта курса через Clippy и сборка через CLion работают **независимо** друг от друга!

### Шаг 8

Теперь вы готовы к решению задач.

Решите задачу [A+B](/tasks/intro/aplusb).

### Шаг 9

Прочтите [FAQ](/docs/faq.md).