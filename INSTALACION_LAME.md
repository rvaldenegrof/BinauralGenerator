# Instalación de LAME para Exportación MP3

## ¿Qué es LAME?

LAME es un codificador MP3 de código abierto que permite exportar audio a formato MP3. El plugin Binaural Generator requiere LAME para poder exportar archivos MP3.

## Instalación por Plataforma

### macOS

#### Opción 1: Usando Homebrew (Recomendado)
```bash
brew install lame
```

#### Opción 2: Verificar si ya está instalado
```bash
which lame
# Debería mostrar: /usr/local/bin/lame o /opt/homebrew/bin/lame
```

### Linux (Ubuntu/Debian)
```bash
sudo apt-get update
sudo apt-get install lame
```

### Linux (Fedora/RHEL)
```bash
sudo dnf install lame
```

### Windows

1. Descargar LAME desde: https://lame.sourceforge.io/download.php
2. Extraer el archivo ZIP
3. Copiar `lame.exe` a una de estas ubicaciones:
   - Misma carpeta que el ejecutable del plugin
   - `C:\Program Files\LAME\`
   - O agregar a PATH del sistema

## Verificación

Después de instalar, verifica que LAME funciona:

```bash
lame --version
```

Deberías ver información sobre la versión de LAME instalada.

## Ubicaciones Buscadas por el Plugin

El plugin busca LAME en las siguientes ubicaciones (en orden):

### macOS
1. `/usr/local/bin/lame`
2. `/opt/homebrew/bin/lame`
3. Carpeta del ejecutable del plugin

### Linux
1. `/usr/bin/lame`
2. `/usr/local/bin/lame`

### Windows
1. Carpeta del ejecutable del plugin (`lame.exe`)
2. `C:\Program Files\LAME\lame.exe`

## Uso en el Plugin

Una vez instalado LAME:

1. Abre el plugin Binaural Generator
2. Ve a la sección "Export Audio"
3. Selecciona "MP3" en el selector de formato
4. Elige la calidad MP3 (128, 192, 256, o 320 kbps)
5. Selecciona el preset y duración
6. Haz clic en "Export to MP3..."
7. Elige la ubicación del archivo

## Solución de Problemas

### Error: "Failed to export audio. MP3 export requires LAME encoder"

**Solución:**
- Verifica que LAME esté instalado: `which lame` (macOS/Linux) o busca `lame.exe` (Windows)
- Asegúrate de que LAME esté en una de las ubicaciones buscadas por el plugin
- Reinicia el plugin después de instalar LAME

### El plugin no encuentra LAME

**Solución:**
- Verifica la ubicación de LAME con `which lame`
- Si está en otra ubicación, puedes crear un enlace simbólico:
  ```bash
  # macOS/Linux
  sudo ln -s /ruta/a/tu/lame /usr/local/bin/lame
  ```

### Exportación MP3 falla silenciosamente

**Solución:**
- Verifica permisos de escritura en la carpeta de destino
- Asegúrate de tener suficiente espacio en disco
- Verifica que LAME funcione correctamente ejecutando: `lame --help`

## Notas Importantes

- **LAME es gratuito y de código abierto**
- El plugin solo puede **exportar** MP3, no leer archivos MP3
- La exportación MP3 puede tomar más tiempo que WAV debido al proceso de codificación
- Los archivos MP3 son más pequeños que WAV pero con pérdida de calidad

## Alternativa: Exportar a WAV

Si no puedes instalar LAME o prefieres máxima calidad:
- Selecciona "WAV (24-bit)" en el selector de formato
- Los archivos WAV son más grandes pero sin pérdida de calidad
- Puedes convertir WAV a MP3 después usando otras herramientas




