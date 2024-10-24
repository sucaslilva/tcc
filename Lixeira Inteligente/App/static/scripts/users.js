async function getData() {
    try {
        const response = await fetch("/usuarios");

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
        const nome = row.insertCell(1);
        const login = row.insertCell(2);
        const email = row.insertCell(3);
        cod.textContent = user.codusu; // ID começa em 1
        nome.textContent = user.nome; // Ajuste os nomes das propriedades conforme seu JSON
        email.textContent = user.email; // Ajuste os nomes das propriedades conforme seu JSON
        login.textContent = user.login;
    });
}

// Chama a função getData quando a página é carregada
window.onload = getData;
async function cadastrarUsuario(event) {
    event.preventDefault();
    const form = document.getElementById("cadastroForm");
    const formData = new FormData(form);
    const data = {
        email: formData.get("email"),
        login: formData.get("login"),
        senha: formData.get("senha"),
        nome: formData.get("nome")
    };

    const response = await fetch("/cadastrar", {
        method: "POST",
        headers: {
            "Content-Type": "application/json",
        },
        body: JSON.stringify(data),
    });

    const result = await response.json();
    if (response.ok) {
        window.location.href = "/user";
    } else {
        alert("Erro: " + result.mensagem);
    }
}

async function excluirUsuario(event) {
    event.preventDefault();
    const form = document.getElementById("cadastroForm2");
    const formData = new FormData(form);
    const codusu = formData.get("codusu");

    const response = await fetch("/user/" + codusu, {
        method: "DELETE",
        headers: {
            "Content-Type": "application/json",
        },
    });

    const result = await response.json();
    if (response.ok) {
        window.location.href = "/user";
    } else {
        alert("Erro: " + result.mensagem);
    }
}