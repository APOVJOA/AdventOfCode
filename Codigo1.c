#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

int main() {
    std::ifstream file("EntradaDia1.txt");
    if (!file.is_open()) {
        std::cout << "No se pudo abrir el archivo.\n";
        return 1;
    }

    
    std::unordered_map<char, int> signo = {
        {'L', -1},
        {'R', 1}
    };

    int pos = 50;            
    int contador_ceros = 0;  

    std::string linea;
    while (std::getline(file, linea)) {
        char direccion = linea[0];            
        int distancia = std::stoi(linea.substr(1));  

        
        pos = (pos + signo[direccion] * distancia) % 100;

        
        if (pos < 0)
            pos += 100;

       
        if (pos == 0)
            contador_ceros++;
    }

    std::cout << "La contrasena es: " << contador_ceros << std::endl;
    return 0;
}

