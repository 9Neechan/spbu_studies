## Задание 1. Основы контейнеризации
Оценка: 1 балл

Описание: Создайте сеть из 2 контейнеров Docker:
 - В одном размещается база данных (SQLite или другая).
 - Другой является точкой входа.

Вы должны продемонстрировать возможность запросить БД контейнера1 из контейнера2. Данные в базе данных SQLite не должны исчезать при перезапуске контейнеров!
DoD: GitLab/GitHub repo с Docker-файлами для обоих контейнеров, docker-compose для всей системы, файл README со списком команд для запуска и тестирования.


## Описание решения  
### Список команд

Запушить на Docker hub
- ```docker build -t neechan9/cloud-computing-task:latest .```
- ```docker image tag neechan9/cloud-computing-task:latest neechan9/cloud-computing-task:latest```
- ```docker image push neechan9/cloud-computing-task:latest```


Запустить систему контейнеров 
- ```docker-compose up -d```