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
  document.getElementById('maxSensor').textContent = `${maxSensor}%`;
}

function createCharts(maxSensor) {
  const xValues = ["Volume Livre", "Volume Ocupado"];
  const yValues = [100 - maxSensor, maxSensor];
  const barColors = ["#00aba9", "rgb(255, 99, 132)"];

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

  new Chart('myLineChart', {
    type: 'line',
    data: {
      labels: ["Janeiro", "Fevereiro", "Março", "Abril", "Maio", "Junho", "Julho"],
      datasets: [{
        label: 'Vendas',
        data: [65, 59, 80, 81, 56, 55, 40],
        fill: false,
        borderColor: '#03366a',
        tension: 0.1
      }]
    },
    options: {
      scales: {
        y: {
          beginAtZero: true
        }
      }
    }
  });
}