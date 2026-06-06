# Microprocesadores

## Práctica 1: Contador de 6 Bits y Caminata de LEDs con PIC16F887

### Descripción

En esta práctica se implementaron dos aplicaciones utilizando el microcontrolador PIC16F887: un contador binario de 6 bits y una caminata de LEDs.

| Cantidad | Componente |
|-----------|------------|
| 8 | LEDs |
| 8 | Resistencias de 330 $\Omega$ |
| 1 | Resistencia de 1 $k\Omega$ |
| 1 | PIC16F887 |
| 1 | Push Button |
| 1 | Cristal de cuarzo de $8 MHz$ |
| 1 | Fuente de alimentación de $5 V$ |

---

## Contador de 6 Bits

### Descripción

En esta sección de la práctica se desarrolló un contador de 6 bits que permite representar números binarios mediante el encendido y apagado secuencial de LEDs. Esta actividad permitió comprender el funcionamiento de las salidas digitales del microcontrolador.

### Simulación

Antes de realizar la implementación física, se utilizó Proteus para diseñar y verificar el funcionamiento del circuito. La simulación permitió validar las conexiones y el comportamiento esperado del sistema.

#### Simulación en Proteus

**Archivo de simulación:**  
[Descargar simulación](./Practica1/base_micro.pdsprj)

### Programación

La programación del PIC16F887 se realizó utilizando MPLAB. El código desarrollado controla el incremento del contador y la visualización de los estados binarios mediante los LEDs.

**Código en MPLAB:**  
[Ver código](./Practica1/Contador.c)

### Resultado

Una vez programado el microcontrolador y ensamblado el circuito, se obtuvo el funcionamiento esperado del contador binario de 6 bits.

#### Resultado físico

![Resultado contador de 6 bits](./Practica1/Contador.jpeg)

---

## Caminata de LED's

### Descripción

En esta actividad se desarrolló una caminata de LEDs utilizando los mismos componentes del montaje anterior. La secuencia consiste en encender un LED a la vez de forma consecutiva, generando el efecto visual de una luz desplazándose a través del arreglo de LEDs.

### Simulación

Al igual que en la actividad anterior, se empleó Proteus para verificar el correcto funcionamiento de la secuencia antes de la implementación física.

#### Simulación en Proteus

**Archivo de simulación:**  
[Descargar simulación](./Practica1/base_micro.pdsprj)

### Programación

La programación se realizó en MPLAB utilizando lenguaje C para controlar la secuencia de encendido y apagado de los LEDs.

 **Código en MPLAB:**  
[Ver código](./Practica1/Caminata.c)

### Resultado

Después de cargar el programa en el PIC16F887 y realizar las conexiones correspondientes, se obtuvo el efecto de caminata esperado.

#### Resultado físico

![Resultado caminata de LEDs](./Practica1/Caminata.jpeg)



##  Práctica 2: Visualización de Letras en una Matriz de LEDs con PIC16F887

### Descripción

En esta práctica se utilizó el microcontrolador PIC16F887 para controlar una matriz de LEDs y mostrar diferentes letras mediante programación.

| Cantidad | Componente |
|-----------|------------|
| 1 | Matriz de LEDs |
| 1 | Resistencia de 1 $k\Omega$ |
| 1 | PIC16F887 |
| 1 | Push Button |
| 1 | Cristal de cuarzo de $8 MHz$ |
| 1 | Fuente de alimentación de $5 V$ |

---

## Iluminación de la Letra X

### Descripción

Para esta primera actividad se programó la matriz de LEDs para representar la letra X mediante un patrón específico de encendido y apagado de LEDs. Permitiendo entender la distribución de filas y columnas necesarias para generar caracteres en una matriz.


### Simulación

Antes de realizar la implementación física, se utilizó Proteus para diseñar y verificar el funcionamiento del circuito. La simulación permitió comprobar que la letra se mostrara correctamente en la matriz de LEDs.


#### Simulación en Proteus

**Archivo de simulación:**  
[Descargar simulación](./Practica2/base_micro.pdsprj)

### Programación

La programación del PIC16F887 se realizó utilizando MPLAB. El código desarrollado define el patrón necesario para representar la letra X en la matriz de LEDs.

**Código en MPLAB:**  
[Ver código](./Practica2/X.c)

### Resultado

Después de programar el microcontrolador y realizar las conexiones correspondientes, se obtuvo la visualización correcta de la letra X en la matriz de LEDs.

#### Resultado físico

![Resultado matriz de leds con 'X'](./Practica2/X.gif)

---

## Iluminación de las Letras A, N, K e I

### Descripción

Siguiendo el mismo procedimiento realizado para la letra X, se programaron nuevos patrones de iluminación para representar las letras A, N, K e I. Para ello únicamente fue necesario modificar el código correspondiente a cada patrón de encendido dentro de la matriz de LEDs.


### Simulación

Al igual que en la actividad anterior, se empleó Proteus para verificar el correcto funcionamiento de la secuencia antes de la implementación física.

#### Simulación en Proteus

**Archivo de simulación:**  
[Descargar simulación](./Practica2/base_micro.pdsprj)

### Programación

La programación se realizó en MPLAB modificando los patrones de encendido para representar cada una de las letras en la matriz de LEDs.

 **Código en MPLAB:**  
[Ver código](./Practica2/ANKI.c)

### Resultado

Una vez cargado el programa en el PIC16F887 y realizadas las conexiones correspondientes, se logró visualizar correctamente las letras A, N, K e I en la matriz de LEDs.

#### Resultado físico

![Resultado matriz de leds con 'A', 'N', 'K' e 'I'](./Practica2/ANKI.gif)


##  Práctica 3: Visualización de Números y Caracteres Hexadecimales en un Display de 7 Segmentos con PIC16F887


### Descripción

En esta práctica se utilizó el microcontrolador PIC16F887 para controlar un display de 7 segmentos y mostrar números decimales y caracteres hexadecimales. 

| Cantidad | Componente |
|-----------|------------|
| 1 | Display de 7 segmentos |
| 7 | Resistencias de 330 $\Omega$ |
| 1 | Resistencia de 1 $k\Omega$ |
| 1 | PIC16F887 |
| 1 | Push Button |
| 1 | Cristal de cuarzo de $8 MHz$ |
| 1 | Fuente de alimentación de $5 V$ |

---

## Visualización de Números del 0 al 9

### Descripción

Para esta primera actividad se programó el microcontrolador para mostrar de manera secuencial los números del 0 al 9 en el display de 7 segmentos. Permitiendo comprender cómo se controla cada segmento para formar los diferentes dígitos.



### Simulación

Antes de realizar la implementación física, se utilizó Proteus para diseñar y verificar el funcionamiento del circuito. La simulación permitió comprobar que la secuencia numérica se mostrara correctamente.


#### Simulación en Proteus

**Archivo de simulación:**  
[Descargar simulación](./Practica3/base_micro.pdsprj)

### Programación

La programación del PIC16F887 se realizó utilizando MPLAB. El código desarrollado controla los segmentos necesarios para representar cada número de la secuencia.


**Código en MPLAB:**  
[Ver código](./Practica3/Contador.c)

### Resultado

Después de programar el microcontrolador y realizar las conexiones correspondientes, se obtuvo la visualización correcta de la secuencia numérica del 0 al 9.

#### Resultado físico

![Resultado números del 0 al 9](./Practica3/Cont.jpeg)

---

## Visualización de Caracteres Hexadecimales (0 a F)


### Descripción

Siguiendo el mismo procedimiento realizado para los números decimales, se modificó el programa para incluir los caracteres hexadecimales A, B, C, D, E y F. De esta manera se logró mostrar la secuencia completa de valores hexadecimales desde 0 hasta F.


### Simulación

Se utilizó Proteus para verificar el correcto funcionamiento de la secuencia hexadecimal antes de realizar la implementación física.

#### Simulación en Proteus

**Archivo de simulación:**  
[Descargar simulación](./Practica3/base_micro.pdsprj)

### Programación

La programación se realizó en MPLAB modificando los patrones de activación de los segmentos para representar las letras A, B, C, D, E y F en el display.

 **Código en MPLAB:**  
[Ver código](./Practica3/Todo.c)

### Resultado

Una vez cargado el programa en el PIC16F887 y realizadas las conexiones correspondientes, se logró visualizar correctamente la secuencia hexadecimal completa desde 0 hasta F.

#### Resultado físico

![Resultado hexadecimal 0 a F](./Practica2/HEX.gif)

