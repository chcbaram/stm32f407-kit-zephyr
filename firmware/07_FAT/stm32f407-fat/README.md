## BUILD
```
source ~/hdd/develop/zephyr/.venv/bin/activate
export ZEPHYR_BASE=~/hdd/develop/zephyr/zephyr_4.3.0/zephyr
west build -p -b stm32f407_kit -- -DBOARD_ROOT=.
```