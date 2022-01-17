const graphConfig = {
    type: 'scatter',
    data: {
        datasets: [{
            label: "Temperature",
            borderColor: "#e02900",
            backgroundColor: "#e0290066",
            showLine: true,
            data: [{x: 10, y: 20}, {x: 20, y: 10}],
        }],
    },
    options: {
        scales: {
            y: {
                beginAtZero: true
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
                        yMin: 10,
                        yMax: 10,
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

const sliders = Array.from(document.getElementsByClassName("range-slidecontainer"))

sliders.forEach(s =>
    {
        const input = s.getElementsByClassName("range-slider")[0]
        const setValue = () => {
            const out = s.getElementsByClassName("range-output")[0]
            console.log(out)
            console.log(input)
            out.innerHTML = out.innerHTML.substring(0, out.innerHTML.indexOf(": ")+2) + input.value
        }
        input.addEventListener('input', setValue)
        setValue()
    })

const fanSpeed = document.getElementById("speed")

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
    "heater_temp_max": 60,
    "heater_temp_min": 0,
    "heater_temp_critical": 80,
    "heater_power_max": 1.0,
    "heater_duty_cycle_max": 0.95,
    "heater_duty_cycle_min": 0.05,
    "heater_pid": [1.0, 0.0, 0.0],
    
    "fan_air_temp_max": 35,
    "fan_air_temp_min": 0,
    "fan_speed_max": 1.0,
    "fan_speed_min": 0.4,
    "fan_duty_cycle_max": 0.95,
    "fan_duty_cycle_min": 0.05,

    "ambient_temp_max": 30,
    "ambient_temp_min": 25,
    "ambient_temp": 2,
}

const configFormContainer = document.getElementById("config-form").getElementsByClassName("config-inputs")[0]

function populateForm(configs)
{
    configFormContainer.innerHTML = ""
    for(const config in configs)
    {
        const container = document.createElement("div")
        const label = document.createElement("label")
        const input = document.createElement("input")
        input.type = "text"
        input.id = input.name = label.htmlFor = label.textContent = config
        input.value = configs[config]
        container.classList.add("mb-4")
        label.classList.add("mr-8")
        container.appendChild(label)
        container.appendChild(input)
        configFormContainer.appendChild(container)
    }
}

populateForm(configs)

/* minAjax({
    url:"config.json",
    type:"GET",
    success: function(data){
      console.log(data)
    }
}) */