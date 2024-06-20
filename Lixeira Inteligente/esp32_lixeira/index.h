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
    <script src="https://www.gstatic.com/charts/loader.js"></script>
    <style>
        #myHeader {
            background-color: silver;
            color: black;
            padding: 40px;
            text-align: center;
            font-family: Verdana, Geneva, Tahoma, sans-serif;
        }

        #subtext {

            color: black;

            text-align: center;
            font-family: Verdana, Geneva, Tahoma, sans-serif;
        }
        
        div.grafico {
            width: 600px;
            margin: auto;
        }

        /*div.graficos {
            width: 100%;
            overflow: auto;
        }*/


        /* div.graficos div {
            width: 100%;
            display: inline-block;
            text-align: center;
        }*/
        .botao {
            background-color: #b1b3b2;
            /* Green */
            border: none;
            color: black;
            padding: 16px 32px;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            font-size: 16px;
            margin: 4px 2px;
            transition-duration: 0.4s;
            cursor: pointer;
            font-family: Verdana, Geneva, Tahoma, sans-serif;
            border-radius: 6px;

        }

        .botao {
            box-shadow: 0 8px 16px 0 rgba(0, 0, 0, 0.2), 0 6px 20px 0 rgba(0, 0, 0, 0.19);
        }

        .botao:hover {
            box-shadow: 0 12px 16px 0 rgba(0, 0, 0, 0.24), 0 17px 50px 0 rgba(0, 0, 0, 0.19);
        }
    </style>
</head>

<body>
    <h1 id="myHeader">Lixeira Inteligente</h1>
   
    <div>
        <h3 id="subtext">Sistema de Gerenciamento de Lixeiras para sua empresa!</h3>
        <center>
            <p>Bem-vindo ao nosso inovador Sistema de Gerenciamento de Lixeiras, projetado para otimizar a coleta e
                o
                descarte de resíduos urbanos. Este sistema avançado oferece uma interface intuitiva e fácil de usar,
                com
                uma variedade de funcionalidades para garantir uma gestão eficiente e sustentável do lixo.</p>

            <p>Ao lado direito da tela, você encontrará um gráfico de pizza interativo que exibe o volume de lixo
                coletado em tempo real. Este gráfico mostra claramente a proporção entre o volume livre e o volume
                ocupado nas lixeiras, permitindo uma visualização imediata do estado atual das mesmas. Com essa
                ferramenta, os gestores podem monitorar e analisar os dados de forma eficaz, facilitando a tomada de
                decisões e a implementação de estratégias de coleta.</p>

            <p>Nosso sistema não só melhora a eficiência operacional, mas também contribui para um ambiente mais
                limpo
                e sustentável. Com a capacidade de gerar relatórios detalhados e alertas automáticos para a coleta
                de
                lixo, garantimos que as lixeiras nunca fiquem superlotadas, promovendo um ambiente urbano mais
                organizado e saudável.</p>
        </center>
        <center><div class="grafico">
            <div id="myChart" style="width:100%; max-width:800px; height:400px;"></div>
        </div></center>
        <center><button class="botao" onclick="recarregarAPagina()"><b>Atualizar Dados</b></button></center>
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

            const options = {
                title: 'Porcentagem do Uso da Lixeira',
                is3D: true
            };

            // Draw
            const chart = new google.visualization.PieChart(document.getElementById('myChart'));
            chart.draw(data, options);

        }

        function recarregarAPagina() {
            window.location.reload();
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