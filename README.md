# Robot manager
Проект на С++, предлагаемый студентам на 2 курсе ВМК МГУ в качестве семестрового задания
## Overview
Программа представляет собой бота, который должен уметь в игру "Менеджмент", 
быть программируемым (предполагается создание своего языка программирования 
с использованием грамматик) и иметь возможность обучаться на основе действий 
других игроков.  
Более подробное описание правил игры и самого проекта можно найти в 
[пособии](http://stolyarov.info/books) А. В. Столярова.
Код игрового сервера можно найти на [сайте](https://al.cs.msu.ru/classes.html)
кафедры алгоритмических языков ВМК МГУ.  

## Branches
**[main](https://github.com/vanc0uv3r/robot-manager)** - бот, играющий в менеджера по тупой стратегии

**[lang](https://github.com/vanc0uv3r/robot-manager/tree/lang)** - язык программирования для робота
## Installation & Usage
```
git clone https://github.com/vanc0uv3r/robot-manager.git && cd robot-manager
make
./robot_manager <ip_addr> <port> <bot_nick> (join <room> | create <players>)
```
