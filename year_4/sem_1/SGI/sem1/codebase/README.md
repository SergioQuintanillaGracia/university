
# CÓDIGO BASE y Utlidades para OpenGL 2.1 sobre Windows

 :warning: **EL CONTENIDO DE ESTE REPOSITORIO ES EXCLUSIVAMENTE PARA USO DOCENTE**

## Contenido

Se suministran ficheros de cabecera (.h), bibliotecas (.lib) y bibliotecas dinámicas (.dll) para Win32:
* OpenGL
* Freeglut (versión 3.4.0) compilada con MSVC_2022 por <rvivo@upv.es> en ene2023 a partir del código [Freeglut 3.4.0](http://freeglut.sourceforge.net/) 
* Freeimage (version 3.18.0), distribución descargada de [Freeimage 3.18.0](https://freeimage.sourceforge.io/) en jun2022 por <rvivo@upv.es>

## Uso

* `./GL`: encaminar a través de la hoja de propiedades del proyecto para ficheros de inclusión (*.h .hpp*)
* `./lib/x86`: encaminar a través de la hoja de propiedades del proyecto para bibliotecas (*.lib*) para plataforma `Release|Win32`
* `./lib/x64`: encaminar a través de la hoja de propiedades del proyecto para bibliotecas (*.lib*) para plataforma `Release|x64`
* `./bin/x86`: encaminar con la variable PATH de la cuenta de usuario para ejecutables en plataforma `Win32`
* `./bin/x64`: encaminar con la variable PATH de la cuenta de usuario para ejecutables en plataforma `x64`

## Código adicional

* Bibliotecas para x64 de *Freeimage 3.18.0* y *Freeglut 3.2.2*
* *Utilidades.h*: funciones de utilidad (deprecada)
* *codebase.h*: Clases y funciones de utilidad


*(c) Roberto Vivó <rvivo@upv.es>, 2023*