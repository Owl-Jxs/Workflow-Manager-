# 1. Buscar recursivamente todos los archivos .cpp en src y sus subcarpetas
$cppFiles = Get-ChildItem -Path "src" -Filter "*.cpp" -Recurse | Select-Object -ExpandProperty FullName

if (-not $cppFiles) {
    Write-Host "Error: No se encontraron archivos .cpp en la carpeta src/" -ForegroundColor Red
    exit 1
}

# 2. Crear la carpeta 'bin' si no existe (para mantener todo ordenado)
if (-not (Test-Path "bin")) {
    New-Item -ItemType Directory -Path "bin" | Out-Null
}

# 3. Informar al usuario
Write-Host "Compilando proyecto... Encontrados $($cppFiles.Count) archivos fuente." -ForegroundColor Cyan

# 4. Ejecutar el compilador g++ pasándole todos los archivos encontrados
& "C:\msys64\ucrt64\bin\g++.exe" -g -fdiagnostics-color=always $cppFiles -o "bin\WorkflowManager.exe"

# 5. Comprobar si hubo errores
if ($LASTEXITCODE -eq 0) {
    Write-Host "¡Compilación exitosa! Ejecutable creado en: bin\WorkflowManager.exe" -ForegroundColor Green
} else {
    Write-Host "Hubo errores durante la compilación." -ForegroundColor Red
    exit $LASTEXITCODE
}