from flask import Flask,request,jsonify,render_template
from flask_sqlalchemy import SQLAlchemy
from datetime import datetime, timedelta, timezone
from flask import Blueprint
from sqlalchemy import func 
import bcrypt

servidor = Flask(__name__)
servidor.config["SQLALCHEMY_DATABASE_URI"] = "sqlite:///bin.db"

orm = SQLAlchemy()
# Definindo o fuso horário de Brasília (UTC-3)
brt = timezone(timedelta(hours=-3))
class gravaUser(orm.Model) :
    codusu = orm.Column(orm.Integer, primary_key = True, autoincrement = True)
    email = orm.Column(orm.String, nullable = False,unique=True)
    login = orm.Column(orm.String, nullable = False) 
    senha = orm.Column(orm.String, nullable = False)    
    nome = orm.Column(orm.String, nullable = False)

class sensor(orm.Model) :
    codsensor = orm.Column(orm.Integer, primary_key = True, autoincrement = True)
    descricao = orm.Column(orm.String, nullable = False)
    ip = orm.Column(orm.String, nullable = False)
    altura = orm.Column(orm.Float, nullable = False)
    logs = orm.relationship('logSensor', backref='sensor', lazy=True)

class logSensor(orm.Model):
    codlog = orm.Column(orm.Integer, primary_key=True, autoincrement=True) 
    sensor_id = orm.Column(orm.Integer, orm.ForeignKey('sensor.codsensor'), nullable=False)
    data_hora = orm.Column(orm.DateTime, default=lambda: datetime.now(brt),nullable=False)
    porc_vol = orm.Column(orm.Float, nullable=False)

orm.init_app(servidor)
#Home
@servidor.route("/")
def home():
     return render_template('login.html')

@servidor.route("/cadastro")
def cadastro_page():
    return render_template('cadastro.html')

@servidor.route("/user")
def user_page():
    return render_template('users.html')

@servidor.route("/sensor")
def sensor_page():
    return render_template('sensor.html')

@servidor.route("/dashboard")
def dashboard_page():
    return render_template('dashboard.html')

#cadastro de usuários
@servidor.route("/cadastrar",methods =["POST"])
def cadastrar_usuario():
    dados = request.get_json()
    
    # Hash da senha antes de armazenar
    hashed_password = bcrypt.hashpw(dados['senha'].encode('utf-8'), bcrypt.gensalt())

    user = gravaUser(
        email=dados['email'],
        login = dados['login'],
        senha=hashed_password.decode('utf-8'),        
        nome=dados['nome']
    )

    try:
        orm.session.add(user)
        orm.session.commit()
        orm.session.refresh(user)
        response = {"codigo_usuario": user.codusu}, 201
    except Exception as e:
        orm.session.rollback()  # Reverte qualquer mudança no banco de dados
        response = jsonify({"mensagem": "Erro ao cadastrar usuário", "erro": str(e)}), 500

    return response

#cadastro de sensor
@servidor.route("/cadastrarSensor",methods =["POST"])
def cadastrar_sensor():
    dados = request.get_json()

    sen = sensor(
        descricao=dados['descricao'],
        ip=dados['ip'], 
        altura=dados['altura']       
    )

    try:
        orm.session.add(sen)
        orm.session.commit()
        orm.session.refresh(sen)
        response = {"codigo_sensor": sen.codsensor}, 201
    except Exception as e:
        orm.session.rollback()  # Reverte qualquer mudança no banco de dados
        response = jsonify({"mensagem": "Erro ao cadastrar sensor", "erro": str(e)}), 500

    return response

#cadastro status
@servidor.route("/status",methods =["POST"])
def status():
    dados = request.get_json()

    status = logSensor(
        sensor_id=dados['sensor_id'],
        porc_vol=dados['porc_vol'],        
    )

    try:
        orm.session.add(status)
        orm.session.commit()
        orm.session.refresh(status)
        response = {"codigo_log": status.codlog}, 201
    except Exception as e:
        orm.session.rollback()  # Reverte qualquer mudança no banco de dados
        response = jsonify({"mensagem": "Erro ao cadastrar sensor", "erro": str(e)}), 500

    return response

#fazer login
@servidor.route("/login", methods=["POST"])
def login():
    dados = request.get_json()
    login = dados['email']
    senha = dados['senha']

    user = gravaUser.query.filter_by(email=login).first()  # Buscando o usuário pelo email

    if user and bcrypt.checkpw(senha.encode('utf-8'), user.senha.encode('utf-8')):
        return jsonify({"mensagem": "Login bem-sucedido!", "codigo_usuario": user.codusu}), 200
    else:
        return jsonify({"mensagem": "Email ou senha inválidos"}), 401

#listagem de usuarios
@servidor.route("/usuarios")
def listar_user():
    user = gravaUser.query.all()

    response = [{
        "codusu" : user.codusu,
        "login" : user.login,
        "nome" : user.nome,
        "email" : user.email
    } for user in user ]

    return response,200

#listagem de sensores
@servidor.route("/sensores")
def listar_sensor():
    try:
        sensores = sensor.query.all()  # Certifique-se de chamar a função .all()

        response = [{
            "codsensor": sensor.codsensor,
            "descricao": sensor.descricao,
            "ip": sensor.ip,
            "altura" : sensor.altura
        } for sensor in sensores]

        return jsonify(response), 200
    except Exception as e:
        return jsonify(str(e)), 500

#contagem de usuarios
@servidor.route("/qtdusuarios")
def qtd_user():
    try:
        user_count = gravaUser.query.count()

        response = {
            "total_usuarios" : user_count
              } 
        return jsonify(response), 200
    except Exception as e:
        return jsonify(str(e)), 400

#contagem de lixeira
@servidor.route("/qtdlixeira")
def qtd_lixeira():
    try:
        sensor_count = sensor.query.count()

        response = {
            "total_lixeira" : sensor_count
              } 
        return jsonify(response), 200
    except Exception as e:
        return jsonify(str(e)), 400

#maximo log da lixeira
@servidor.route("/maxLogSensor")
def max_lixeira():
    try:
        max_log = orm.session.query(func.max(logSensor.codlog)).scalar()
        sensor_max = orm.session.query(logSensor).filter_by(codlog=max_log).first()
        
        response = {
            "porc_max": sensor_max.porc_vol
        } 
        return jsonify(response), 200
    except Exception as e:
        return jsonify({"mensagem": "Erro: " + str(e)}), 400


@servidor.route("/user/<int:codusu>", methods=['DELETE'])
def deletar_usuario(codusu) :
    try:
        delUser = gravaUser.query.get(codusu)
        
        orm.session.delete(delUser)
        orm.session.commit()

        response = {"mensagem":"usuário deletado"},200
    except Exception as e:
        response = {"mensagem":"erro de servidor" + str(e)}, 500
    
    return response

@servidor.route("/sensor/<int:codsensor>", methods=['DELETE'])
def deletar_sensor(codsensor) :
    try:
        delSensor = sensor.query.get(codsensor)
        
        orm.session.delete(delSensor)
        orm.session.commit()

        response = {"mensagem":"sensor deletado"},200
    except Exception as e:
        response = {"mensagem":"erro de servidor" + str(e)}, 500
    
    return response

#atualiza nome do usuário
@servidor.route("/atualizaNome", methods=['PUT'])
def atualizar_nomeUsuario( ):
    dados = request.get_json()

    try:
        user = gravaUser.query.get(dados['codusu'])
        if user:

            user.nome = dados['nome']

            orm.session.commit()
            response = {"mensagem": "Nome atualizado"}, 200
        else:
            response = {"mensagem": "Usuário não encontrado"}, 400
    except Exception as e:
        response = {"mensagem": "erro ao atualizar" + str(e)}, 500
    return response

#atualiza login do usuário
@servidor.route("/atualizaLogin", methods=['PUT'])
def atualizar_loginUsuario( ):
    dados = request.get_json()

    try:
        user = gravaUser.query.get(dados['codusu'])
        if user:

            user.login = dados['login']

            orm.session.commit()
            response = {"mensagem": "Login atualizado"}, 200
        else:
            response = {"mensagem": "Usuário não encontrado"}, 400
    except Exception as e:
        response = {"mensagem": "erro ao atualizar" + str(e)}, 500
    return response

#atualiza email do usuário
@servidor.route("/atualizaEmail", methods=['PUT'])
def atualizar_emailUsuario( ):
    dados = request.get_json()

    try:
        user = gravaUser.query.get(dados['codusu'])
        if user:

            user.email = dados['email']

            orm.session.commit()
            response = {"mensagem": "E-mail atualizado"}, 200
        else:
            response = {"mensagem": "Usuário não encontrado"}, 400
    except Exception as e:
        response = {"mensagem": "erro ao atualizar" + str(e)}, 500
    return response

#atualiza senha do usuário
@servidor.route("/atualizaSenha", methods=['PUT'])
def atualizar_senhaUsuario( ):
    dados = request.get_json()
    hashed_password = bcrypt.hashpw(dados['senha'].encode('utf-8'), bcrypt.gensalt())
    try:
        user = gravaUser.query.get(dados['codusu'])
        if user:

            user.senha = hashed_password.decode('utf-8')

            orm.session.commit()
            response = {"mensagem": "Senha atualizada"}, 200
        else:
            response = {"mensagem": "Usuário não encontrado"}, 400
    except Exception as e:
        response = {"mensagem": "erro ao atualizar" + str(e)}, 500
    return response

#atualiza descricao do sensor
@servidor.route("/atualizaDescricao", methods=['PUT'])
def atualizar_descricaoSensor( ):
    dados = request.get_json()

    try:
        lixeira = sensor.query.get(dados['codsensor'])
        if lixeira:

            lixeira.descricao = dados['descricao']

            orm.session.commit()
            response = {"mensagem": "Descrição atualizada"}, 200
        else:
            response = {"mensagem": "Sensor não encontrado"}, 400
    except Exception as e:
        response = {"mensagem": "erro ao atualizar" + str(e)}, 500
    return response

#atualiza ip do sensor
@servidor.route("/atualizaIp", methods=['PUT'])
def atualizar_ipSensor( ):
    dados = request.get_json()

    try:
        lixeira = sensor.query.get(dados['codsensor'])
        if lixeira:

            lixeira.ip = dados['ip']

            orm.session.commit()
            response = {"mensagem": "IP atualizado"}, 200
        else:
            response = {"mensagem": "Sensor não encontrado"}, 400
    except Exception as e:
        response = {"mensagem": "erro ao atualizar" + str(e)}, 500
    return response

#atualiza altura do sensor
@servidor.route("/atualizaAltura", methods=['PUT'])
def atualizar_alturaSensor( ):
    dados = request.get_json()

    try:
        lixeira = sensor.query.get(dados['codsensor'])
        if lixeira:

            lixeira.altura = dados['altura']

            orm.session.commit()
            response = {"mensagem": "Altura atualizada"}, 200
        else:
            response = {"mensagem": "Sensor não encontrado"}, 400
    except Exception as e:
        response = {"mensagem": "erro ao atualizar" + str(e)}, 500
    return response

with servidor.app_context():
    orm.create_all()

##main program
if __name__ == "__main__" :
    with servidor.app_context():
        orm.create_all()
    servidor.run(host='0.0.0.0', port=5000)