async function login(event) {
    event.preventDefault();
    const form = document.getElementById('loginForm');
    const formData = new FormData(form);
    const data = {
        email: formData.get('email'),
        senha: formData.get('senha'),
    };

    const response = await fetch('/login', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(data),
    });

    const result = await response.json();
    if (response.ok) {
        window.location.href = '/dashboard';
    } else {
        alert('Erro: ' + result.mensagem);
    }
}