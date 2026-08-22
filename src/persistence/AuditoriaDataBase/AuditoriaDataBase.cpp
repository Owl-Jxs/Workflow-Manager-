#include "AuditoriaDataBase.h"
#include <chrono>
#include <iomanip>
#include <sstream>

const std::string AuditoriaDataBase::FILENAME_AUDITORIA = "data/auditoria_log.csv";

AuditoriaDataBase::AuditoriaDataBase() {}

AuditoriaDataBase::~AuditoriaDataBase() {}

std::string AuditoriaDataBase::ahora() {
    auto ahora = std::chrono::system_clock::now();
    std::time_t tiempo = std::chrono::system_clock::to_time_t(ahora);
    std::tm tmLocal{};
#ifdef _WIN32
    localtime_s(&tmLocal, &tiempo);
#else
    localtime_r(&tiempo, &tmLocal);
#endif
    std::stringstream ss;
    ss << std::put_time(&tmLocal, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string AuditoriaDataBase::formularLinea(const std::string& fechaHora, int idUsuario,
                                             const std::string& accion, int idTarea) const {
    std::stringstream linea;
    linea << "[" << fechaHora << "] | [" << idUsuario << "] | [" << accion << "] | [" << idTarea << "]";
    return linea.str();
}

void AuditoriaDataBase::registrar(int idUsuario, const std::string& accion, int idTarea) {
    registrarLinea(formularLinea(ahora(), idUsuario, accion, idTarea));
}

void AuditoriaDataBase::registrarLinea(const std::string& linea) {
    std::ofstream archivo(FILENAME_AUDITORIA, std::ios::app);
    if (!archivo.is_open()) {
        throw std::runtime_error("Error al abrir el archivo de auditoria para escritura.");
    }
    archivo << linea << std::endl;
    archivo.close();
}
