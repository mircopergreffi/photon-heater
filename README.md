# photon-heater

## Per Enrico

\flash\web-client

Web Interface:
- plot:
    * x-axis: time \[minutes\];
    * y-axis: temperature \[Celsius\] (3 values: Heather, Fan Air, Ambient Air), fan-speed \[%\];
- set temperature: (slider, text-input, ...);
- fan-mode: switch auto/manual;
- fan-speed: (disabled in auto mode);
- configs: text-area (JSON), save button, reset button;

NB: see \flash\config.json for maximum and minimum settable temperatures (```ambient_temp_max``` e ```ambient_temp_min```), maxium and minimum settable fan speed (```fan_speed_max``` e ```fan_speed_min```).

```
/config.json
/history.json?timestamp=x (returns only values with timestamp greater (and not equals) than what specified, if omitted all history is returned)
/set?fanSpeed=x.x
/set?fanMode=auto|manual
/set?temperature=x.x
```