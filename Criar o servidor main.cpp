#include "crow_all.h"
#include <fstream>
#include <string>
#include <sstream>

bool verificarCredenciais(const std::string& usuario, const std::string& senha) {
    std::ifstream arquivo("usuarios.txt");
    std::string linha, u, s;
    while (std::getline(arquivo, linha)) {
        std::istringstream iss(linha);
        iss >> u >> s;
        if (u == usuario && s == senha) {
            return true;
        }
    }
    return false;
}

void salvarUsuario(const std::string& usuario, const std::string& senha) {
    std::ofstream arquivo("usuarios.txt", std::ios::app);
    arquivo << usuario << " " << senha << "\n";
}

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/login").methods("POST"_method)([](const crow::request& req){
        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400);

        std::string usuario = body["usuario"].s();
        std::string senha = body["senha"].s();

        if (verificarCredenciais(usuario, senha)) {
            return crow::response(200, "Login bem-sucedido");
        } else {
            return crow::response(401, "Usuário ou senha incorretos");
        }
    });

    CROW_ROUTE(app, "/cadastro").methods("POST"_method)([](const crow::request& req){
        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400);

        std::string usuario = body["usuario"].s();
        std::string senha = body["senha"].s();

        salvarUsuario(usuario, senha);
        return crow::response(200, "Usuário cadastrado");
    });

    app.port(18080).multithreaded().run();
}
