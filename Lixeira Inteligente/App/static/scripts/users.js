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
        alert("Usuário cadastrado com sucesso!")
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
        alert("Usuario excluído com sucesso!")
        window.location.href = "/user";
    } else {
        alert("Erro: " + result.mensagem);
    }
}

async function atualizarUsuario(event) {
    event.preventDefault();
    const form = document.getElementById("cadastroForm3");
    const formData = new FormData(form);
    const codusu = formData.get("codusu");
    const campo = formData.get("campo");
    const valor = formData.get("valor");
    let rota = null;
    let data = null;
    if (campo == "1") {

        data = {
            codusu: codusu,
            nome: valor
        };
        rota = "/atualizaNome";
    }
    if (campo == "2") {

        data = {
            codusu: codusu,
            login: valor
        };
        rota = "/atualizaLogin";
    }
    if (campo == "3") {

        data = {
            codusu: codusu,
            email: valor
        };
        rota = "/atualizaEmail";
    }
    if (campo == "4") {

        data = {
            codusu: codusu,
            senha: valor
        };
        rota = "/atualizaSenha";
    }

    const response = await fetch(rota, {
        method: "PUT",
        headers: {
            "Content-Type": "application/json",
        },
        body: JSON.stringify(data),
    });

    const result = await response.json();
    if (response.ok) {
        alert(result.mensagem + " com sucesso!");
        window.location.href = "/user";
    } else {
        alert("Erro: " + result.mensagem);
    }
}

function mudarTipoCampo() {
    const campoSelect = document.getElementById('campo');
    const valorInput = document.getElementById('valor');

    if (campoSelect.value === '4') { // Quando "Senha" é selecionado
        valorInput.type = 'password';
    } else if (campoSelect.value === '3') {
        valorInput.type = 'email';
    } else {
        valorInput.type = 'email';
    }
}