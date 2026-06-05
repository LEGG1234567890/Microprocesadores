# Práctica 1: Contador de 6 Bits y Caminata de LEDs con PIC16F887

## Descripción

En esta práctica se implementaron dos aplicaciones utilizando el microcontrolador PIC16F887: un contador binario de 6 bits y una caminata de LEDs.

| Cantidad | Componente |
|-----------|------------|
| 8 | LEDs |
| 8 | Resistencias de 330 Ω |
| 1 | PIC16F887 |
| 1 | Push Button |
| 1 | Cristal de cuarzo de 8 MHz |
| 1 | Fuente de alimentación de 5 V |

---

# Contador de 6 Bits


## Descripción

En esta sección de la práctica se desarrolló un contador de 6 bits que permite representar números binarios mediante el encendido y apagado secuencial de LEDs. Esta actividad permitió comprender el funcionamiento de las salidas digitales del microcontrolador.
## Simulación

Antes de realizar la implementación física, se utilizó Proteus para diseñar y verificar el funcionamiento del circuito. La simulación permitió validar las conexiones y el comportamiento esperado del sistema.

### Simulación en Proteus

**Archivo de simulación:**  
[Descargar simulación](./Practica1/base_micro.pdsprj)


## Programación

La programación del PIC16F887 se realizó utilizando MPLAB. El código desarrollado controla el incremento del contador y la visualización de los estados binarios mediante los LEDs.

**Código en MPLAB:**  
[Ver código](/Practica1/contador.c)

## Resultado

Una vez programado el microcontrolador y ensamblado el circuito, se obtuvo el funcionamiento esperado del contador binario de 6 bits.

### Resultado físico

![Resultado contador de 6 bits](/Practica1/Contador.jpg)

---

# Caminata de LEDs


## Descripción

En esta actividad se desarrolló una caminata de LEDs utilizando los mismos componentes del montaje anterior. La secuencia consiste en encender un LED a la vez de forma consecutiva, generando el efecto visual de una luz desplazándose a través del arreglo de LEDs.

## Simulación

Al igual que en la actividad anterior, se empleó Proteus para verificar el correcto funcionamiento de la secuencia antes de la implementación física.

### Simulación en Proteus

**Archivo de simulación:**  
[Descargar simulación](/Practica1/base_micro.pdsprj)


## Programación

La programación se realizó en MPLAB utilizando lenguaje C para controlar la secuencia de encendido y apagado de los LEDs.

 **Código en MPLAB:**  
[Ver código](/Practica1/Caminata.c)

## Resultado

Después de cargar el programa en el PIC16F887 y realizar las conexiones correspondientes, se obtuvo el efecto de caminata esperado.

### Resultado físico

![Resultado caminata de LEDs](/Practica1/caminata.jpg)



