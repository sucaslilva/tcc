async function getData() {
    try {
      const response = await fetch("/sensores"); // Substitua '/endpoint' pelo seu endpoint

      if (!response.ok) {
        throw new Error(
          "Network response was not ok " + response.statusText
        );
      }

      const data = await response.json();
      populateTable(data); // Chama a função para popular a tabela com os dados
    } catch (error) {
      console.error("Houve um problema com a operação fetch:", error);
    }
  }

  function populateTable(users) {
    const tableBody = document.getElementById("userTableBody");
    tableBody.innerHTML = ""; // Limpa o conteúdo existente

    users.forEach((user, index) => {
      const row = tableBody.insertRow();

      const cod = row.insertCell(0);
      const descricao = row.insertCell(1);
      const ip = row.insertCell(2);
      const altura = row.insertCell(3);
      cod.textContent = user.codsensor; // ID começa em 1
      descricao.textContent = user.descricao; // Ajuste os nomes das propriedades conforme seu JSON
      ip.textContent = user.ip; // Ajuste os nomes das propriedades conforme seu JSON
      altura.textContent = user.altura; // Ajuste os nomes das propriedades conforme seu JSON
    });
  }

  // Chama a função getData quando a página é carregada
  window.onload = getData;

  async function cadastrarSensor(event) {
    event.preventDefault();
    const form = document.getElementById("cadastroForm");
    const formData = new FormData(form);
    const data = {
      descricao: formData.get("descricao"),
      ip: formData.get("ip"),
      altura: formData.get("altura"),
    };

    const response = await fetch("/cadastrarSensor", {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify(data),
    });

    const result = await response.json();
    if (response.ok) {
      window.location.href = "/sensor";
    } else {
      alert("Erro: " + result.mensagem);
    }
  }

  async function excluirSensor(event) {
    event.preventDefault();
    const form = document.getElementById("cadastroForm2");
    const formData = new FormData(form);
    const codsensor = formData.get("codsensor");

    const response = await fetch("/sensor/" + codsensor, {
      method: "DELETE",
      headers: {
        "Content-Type": "application/json",
      },
    });

    const result = await response.json();
    if (response.ok) {
      window.location.href = "/sensor";
    } else {
      alert("Erro: " + result.mensagem);
    }
  }