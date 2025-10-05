function Build-UtilsLibrary {
    Write-Host "=== COMPILANDO LIBRERÍA UTILS ===" -ForegroundColor Yellow
    
    # Compilar CADA archivo a objeto
    $objects = @()
    
    $files = @(
        "src/utils/shader.cpp",
        "src/utils/texture.cpp",    # ← NUEVO
        "src/utils/mesh.cpp",       # ← NUEVO  
        "src/utils/camera.cpp"      # ← NUEVO
    )
    
    foreach ($file in $files) {
        if (Test-Path $file) {
            $objName = "build/" + [System.IO.Path]::GetFileNameWithoutExtension($file) + ".o"
            & "C:\msys64\ucrt64\bin\g++.exe" -c -g -std=c++17 $file -Iinclude -o $objName
            $objects += $objName
            Write-Host "✓ Compilado: $file" -ForegroundColor Green
        }
    }
    
    # Crear librería con TODOS los objetos
    & "C:\msys64\ucrt64\bin\ar.exe" rcs lib/libutils.a $objects
    
    Write-Host "✅ Librería actualizada: lib/libutils.a" -ForegroundColor Green
    Write-Host "   Contiene: $($objects.Count) archivos objeto" -ForegroundColor Cyan
}