//float valor = 0.00; 

const char INDEX_HTML[] PROGMEM = R"=====(
    <!DOCTYPE html>
<html>

<head>
    <meta charset='utf-8'>
    <meta http-equiv='X-UA-Compatible' content='IE=edge'>
    <title>Sistema de Gerenciamento de Lixeiras</title>
    <meta name='viewport' content='width=device-width, initial-scale=1'>
    <script src="https://www.gstatic.com/charts/loader.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.9.4/Chart.js"></script>
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.7.1/jquery.min.js"></script>
    <style>
        #myHeader {
            background-color: silver;
            color: black;
            padding: 40px;
            text-align: center;
            font-family: Verdana, Geneva, Tahoma, sans-serif;
        }

        /*div.graficos {
            width: 100%;
            overflow: auto;
        }*/

        div.graficos {
            width: 30%;
            margin: auto;
        }

        /* div.graficos div {
            width: 100%;
            display: inline-block;
            text-align: center;
        }*/

        
    </style>
</head>

<body>
    <h1 id="myHeader">Lixeira Inteligente</h1>
    <div class="graficos">

        <div id="myChart" style="width:800px; height:500px;"></div>
    </div>

    <script>


        google.charts.load('current', { 'packages': ['corechart'] });
        google.charts.setOnLoadCallback(drawChart);



        function drawChart() {

            // Set Data
            const data = google.visualization.arrayToDataTable([
                ['Contry', 'Mhl'],
                ['Volume Livre', 100 - {VALUE}],
                ['Volume Ocupado', {VALUE}]
            ]);

            // Set Options
            const options = {
                title: 'Porcentagem do Uso da Lixeira'
            };

            // Draw
            const chart = new google.visualization.PieChart(document.getElementById('myChart'));
            chart.draw(data, options);

        }


    </script>
</body>

</html>
)=====";

String getHTML(String valor) {
  String html_with_value;

  // Loop para ler cada caractere de INDEX_HTML e construir a String html_with_value
  for (size_t i = 0; i < sizeof(INDEX_HTML); i++) {
    char c = pgm_read_byte(&INDEX_HTML[i]); // Lê um caractere de PROGMEM
    if (c != '\0') {
      html_with_value += c; // Adiciona o caractere à String html_with_value
    } else {
      break; // Sai do loop quando encontrar o caractere nulo
    }
  }

  // Substitui "{VALUE}" pelo valor de 'valor'
  html_with_value.replace("{VALUE}", String(valor));

  return html_with_value;
}