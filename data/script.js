const graphConfig = {
    type: 'scatter',
    data: {
        datasets: [
            {
                label: "Heater",
                borderColor: "#e02900",
                backgroundColor: "#e0290066",
                showLine: true,
                data: [{x: 10, y: 5}, {x: 20, y: 10}, {x: 30, y: 10}, {x: 40, y: 15}, {x: 50, y: 5}],
            },
            {
                label: "Air",
                borderColor: "#a832a8",
                backgroundColor: "#a832a8",
                showLine: true,
                data: [{x: 10, y: 5}, {x: 20, y: 10}, {x: 30, y: 10}, {x: 40, y: 15}, {x: 50, y: 5}],
            },
            {
                label: "Fan",
                borderColor: "#0377fc",
                backgroundColor: "#0377fc",
                showLine: true,
                data: [{x: 10, y: 5}, {x: 20, y: 10}, {x: 30, y: 10}, {x: 40, y: 15}, {x: 50, y: 5}],
            }
        ],
    },
    options: {
        scales: {
            y: {
                suggestedMax: 100,
                suggestedMin: 0,
            },
        },
        responsive: true,
        plugins: {
            legend: {
                position: 'top',
            },
            title: {
                display: true,
                text: 'Temperature chart'
            },
            annotation: {
                annotations: {
                    criticalLine: {
                        type: 'line',
                        yMin: 12,
                        yMax: 12,
                        borderColor: 'rgb(255, 99, 132)',
                        borderWidth: 2,
                    }
                }
            }
        },
    }
}

const ctx = document.getElementById("temperatureChart")
const myChart = new Chart(ctx, graphConfig)

const fanModeAuto = document.getElementById("fan-mode-auto")
const fanSpeed = document.getElementById("speed")
const temperature = document.getElementById("temperature")
fanModeAuto.addEventListener("change", e =>
{
    const mode = e.target.checked ? "auto" : "manual"
    minAjax({
            url:"/set?fanMode=" + mode,
            type:"GET",
            success: function(data){
            }
        })
})
temperature.addEventListener("change", e =>
{
    minAjax({
            url:"/set?temperature=" + e.target.value,
            type:"GET",
            success: function(data){
            }
        })
})
fanSpeed.addEventListener("change", e =>
{
    minAjax({
            url:"/set?fanSpeed=" + e.target.value,
            type:"GET",
            success: function(data){
            }
        })
})
const sliders = Array.from(document.getElementsByClassName("range-slidecontainer"))

sliders.forEach(s =>
    {
        const input = s.getElementsByClassName("range-slider")[0]
        const setValue = () => {
            const out = s.getElementsByClassName("range-output")[0]
            out.textContent = out.textContent.substring(0, out.textContent.indexOf(": ")+2) + input.value
        }
        input.addEventListener('input', setValue)
        setValue()
    })


document.getElementById("fan-mode-auto").addEventListener('change', (e) =>
{
    if(e.target.checked)
    {
        fanSpeed.disabled = true
    }
    else
    {
        fanSpeed.disabled = false
    }
})

const configs = {
	"wifi":
	{
		"ssid": "TIM-25786986",
		"password": "wR4@SNR6jhnz",
		"hostname": "tipregostampa"
	},

	"heater":
	{
		"pin": "17",
		"temp_max": 60,
		"temp_min": 0,
		"temp_critical": 80,
		"power_max": 1.0,
		"sensor": 
		{
			"name": "Heater",
			"type": "ntc",
			"B": 3950,
			"T0": 25,
			"R0": 100000,
			"pullup": 47000,
			"pin": "34"
		}
	},

	"fan":
	{
		"pin": "16",
		"speed_max": 1.0,
		"speed_min": 0.4
	},
	
	"air_temp_max": 30,
	"air_temp_min": 25,
	"air_sensor": 
	{
		"name": "Air",
		"type": "ntc",
		"B": 3950,
		"T0": 25,
		"R0": 100000,
		"pullup": 47000,
		"pin": "35"
	},

	"resin_sensor":
	{
		"name": "Resin",
		"type": "ntc",
		"B": 3950,
		"T0": 25,
		"R0": 100000,
		"pullup": 47000,
		"pin": "27"
	},

	"control":
	{
		"type": "pid",
		"p": 0.1,
		"i": 0.0,
		"d": 0.0
	}
}

const configFormContainer = document.getElementById("config-form").getElementsByClassName("config-inputs")[0]

function formSection(name, section)
{
    const container = document.createElement("div")
    const content = document.createElement("div")
    content.classList.add("pl-4")
    const title = document.createElement("h4")
    title.textContent = name
    container.appendChild(title)
    for(const item in section)
    {
        const current = section[item]
        let child = null
        if(typeof current == "object")
        {
            child = formSection(item, current)
        }
        else
        {
            child = formItem(item, current)
        }
        content.appendChild(child)
    }
    container.appendChild(content)
    return container
}

function formItem(name, value)
{
    const container = document.createElement("div")
    const label = document.createElement("label")
    const input = document.createElement("input")
    input.type = "text"
    input.id = input.name = label.htmlFor = label.textContent = name
    input.value = value
    container.classList.add("mb-4")
    label.classList.add("mr-8")
    container.appendChild(label)
    container.appendChild(input)

    return container
}

function populateForm(configs)
{
    configFormContainer.innerHTML = ""
    configFormContainer.appendChild(formSection("", configs))
}

function updateMaxMin(configs)
{
    const setRange = (e, max, min) =>
    {
        e.max = max
        e.min = min
        const l = e.parentElement.getElementsByTagName("label")[0]
        const a = l.textContent.indexOf("(")
        const b = l.textContent.indexOf(")")
        l.textContent = l.textContent.substring(0, a) + min + "-" + max + l.textContent.substring(b, l.textContent.length)
        e.dispatchEvent(new Event('input'))
    }

    setRange(temperature, configs.heater.temp_max, configs.heater.temp_min)
    setRange(fanSpeed, configs.fan.speed_max, configs.fan.speed_min)
}

function updateGraph(configs)
{
    myChart.options.plugins.annotation.annotations.criticalLine.yMin = configs.heater.temp_critical
    myChart.options.plugins.annotation.annotations.criticalLine.yMax = configs.heater.temp_critical
    myChart.options.scales.y.suggestedMax = configs.heater.temp_max
    myChart.options.scales.y.suggestedMin = configs.heater.temp_min
    // myChart.options.annotation.annotations[0].value = val
    // myChart.options.annotation.annotations[0].label.content += " "+ val + "%"
    myChart.update()
}

function loadConfigs(configs)
{
    populateForm(configs)
    updateMaxMin(configs)
    updateGraph(configs)
}

minAjax({
    url:"/get",
    type:"GET",
    success: function(data){
        data = JSON.parse(data)
        fanModeAuto.value = data.fan.mode == "auto"
        fanSpeed.value = data.fan.speed
        temperature.value = data.temperature
    }
})

// loadConfigs(configs)

minAjax({
    url:"/config.json",
    type:"GET",
    success: function(data){
        loadConfigs(JSON.parse(data))
    }
})

setInterval(() =>
{
    minAjax({
        url:"/history.json",
        type:"GET",
        success: function(data){
            data = JSON.parse(data)
            const heater = data.Heater.map((t,i) =>
            {
                return {x: data.timestamps[i], y: t}
            })
            const air = data.Air.map((a,i) =>
            {
                return {x: data.timestamps[i], y: a}
            })
            const fans = data.Fan.map((f,i) =>
            {
                return {x: data.timestamps[i], y: f*100}
            })
            myChart.data.datasets[0].data = heater
            myChart.data.datasets[2].data = air
            myChart.data.datasets[2].data = fans
            myChart.update()
        }
    })
}, 1000)