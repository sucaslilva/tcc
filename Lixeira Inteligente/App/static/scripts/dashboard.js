document.addEventListener('DOMContentLoaded', initDashboard);

async function initDashboard() {
  try {
    const [users, lixeiras, maxSensor] = await Promise.all([
      getQtdUser(),
      getQtdLixeira(),
      getMaxLogSensor()
    ]);

    updateUI(users, lixeiras, maxSensor);
    createCharts(maxSensor);

    setInterval(() => {
      location.reload();
    }, 60000);
  } catch (error) {
    console.error("Erro ao inicializar o dashboard:", error);
  }
}

async function fetchData(url) {
  try {
    const response = await fetch(url);
    if (!response.ok) {
      throw new Error(`Erro na requisição: ${response.statusText}`);
    }
    return await response.json();
  } catch (error) {
    console.error(`Erro ao buscar dados de ${url}:`, error);
    throw error;
  }
}

async function getQtdUser() {
  const data = await fetchData('/qtdusuarios');
  return data.total_usuarios;
}

async function getQtdLixeira() {
  const data = await fetchData('/qtdlixeira');
  return data.total_lixeira;
}

async function getMaxLogSensor() {
  const data = await fetchData('/maxLogSensor');
  return data.porc_max;
}

function updateUI(users, lixeiras, maxSensor) {
  document.getElementById('totalUsers').textContent = users;
  document.getElementById('totalLixeira').textContent = lixeiras;
  
  const maxSensorElement = document.getElementById('maxSensor');
  const maxSensorElement2 = document.getElementById('maxSensor2');
  maxSensorElement.textContent = `${maxSensor}%`;

  // Definir a cor conforme o valor de maxSensor
  if (maxSensor >= 90) {
    maxSensorElement.style.color = "#dc3545";
    maxSensorElement2.style.color = "#dc3545";
  } else if (maxSensor >= 70) {
    maxSensorElement.style.color = "#ffd700";
    maxSensorElement2.style.color = "#ffd700";
  } else {
    maxSensorElement.style.color = "#1bb89c";
    maxSensorElement2.style.color = "#1bb89c"; // Cor padrão
  }
}

async function createCharts(maxSensor) {
  // Configuração do gráfico de donut
  const xValues = ["Volume Livre", "Volume Ocupado"];
  const yValues = [100 - maxSensor, maxSensor];
  const barColors = ["#00aba9", "#dc3545"];

  new Chart('myChart', {
    type: 'doughnut',
    data: {
      labels: xValues,
      datasets: [{
        backgroundColor: barColors,
        data: yValues
      }]
    }
  });

  // Busca dados para o gráfico de linha
  const sensorData = await fetchData('/sensoresMesAtual');
  
  // Processa dados recebidos do endpoint
  const labels = sensorData.map(item => item.data_hora);
  const dataValues = sensorData.map(item => item.porc_vol);

  // Configuração do gráfico de linha com dados do endpoint
  new Chart('myLineChart', {
    type: 'line',
    data: {
      labels: labels,  // Datas obtidas do endpoint
      datasets: [{
        label: 'Volume (%)',
        data: dataValues,  // Porcentagem de volume
        fill: false,
        borderColor: '#03366a',
        tension: 0.1
      }]
    },
    options: {
      scales: {
        y: {
          beginAtZero: true,
          title: {
            display: true,
            text: 'Porcentagem de Volume (%)'
          }
        },
        x: {
          title: {
            display: true,
            text: 'Data'
          }
        }
      }
    }
  });
}
