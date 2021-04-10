/*
 * main.c
 *
 *  Created on: 28 feb. 2019
 *      Author: utnso
 */

#include "tp0.h"
#include<readline/readline.h>

int main(void)
{
	int conexion;
	char* ip;
	char* puerto;
	char* clave;

	//las variables de logger y config estan declaradas como globales en el encabezado de este tp0

		iniciar_logger();

		leer_config();

		leer_consola();

	//antes de continuar, tenemos que asegurarnos que el servidor esté corriendo porque lo necesitaremos para lo que sigue.

	//Una vez hecho eso, tenemos que conectarnos a él! Las conexiones se representan mediante enteros por cosas que hablaremos más adelante.
	//Esta parte la vamos a hacer aca en el main por simpleza.

	//Primero vas a tener que levantar los valores IP y PUERTO del archivo de configuracion con esas claves. Adivina a quien se los tenes que asignar :P

		ip = config_get_string_value(g_config, "IP");
		puerto = config_get_string_value(g_config, "PUERTO");


	//Hecho esto, vamos a conectarnos! usamos la funcion ya dada para hacerlo, y le asignamos el entero que devuelve a conexion
		conexion = crear_conexion(ip, puerto);



	//ahora que tenemos la conexion, es hora de empezar a enviar cosas. Primero, el mensaje.
	//Levantá el valor que le corresponde a CLAVE del archivo de configuracion de nuevo.

		clave = config_get_string_value(g_config, "CLAVE");


	//Ahora le podés mandar el mensaje al servidor!
		enviar_mensaje(clave, conexion);

		paquete(conexion);

		terminar_programa(conexion);
}

void iniciar_logger(void)
{
	//creemos el log! Este se debe crear con el nombre "tp0.log", se debe mostrar por consola y debe estar en el nivel INFO
		g_logger = log_create("tp0.log", "tp0", 1, LOG_LEVEL_INFO);

	//logeemos el mensaje "Soy un log"
		log_info(g_logger, "Soy un log");



}

void leer_config(void)
{
	char* valor;

	//ahora, vamos a abrir el archivo de configuracion "tp0.config"
		g_config = config_create("tp0.config");

	//tenemos que levantar en valor asociado a la clave "CLAVE" del archivo y asignarselo a la variable valor
		valor = config_get_string_value(g_config, "CLAVE");

	//POR ULTIMO, logeamos el valor obtenido
		log_info(g_logger, valor);
}

void leer_consola(void)
{
	char* leido;

	//El primero te lo dejo de yapa
	leido = readline(">");

	//Los siguiente 4 pasos vas a tener que meterlos en algun tipo de bucle. Te tiro una pista, no es con for ;)

	//Voy a dejar esto por aca porque quizas complemente al enunciado: http://faq.utnso.com/punteros

	while(*leido != '\0')
		{

		//Primero, logeame lo leido
			log_info(g_logger,leido);

		//ahora que ya usamos el string, liberamelo con free
			free(leido);

		//free NO asigna el valor NULL a los punteros que libera, pero es buena practica asignarselo a tenerlo apuntando a basura
			leido = NULL;

		//volvemos a poner el readline para poder seguir loggeando.
			leido = readline(">");
		}
}

void paquete(int conexion)
{
	//Ahora toca lo divertido!

		char* leido;
		t_paquete* paquete = crear_paquete();

	//De nuevo vas a tener que usar readline hasta que te llegue un nulo igual que antes, solo que ahora en lugar de logear, vas a agregar esa cadena al paquete!
	//Recomiendo revisar bien el enunciado del TP ya que ahi está explicado con detalles

		leido = readline(">");

		while(*leido != '\0')
				{

					agregar_a_paquete(paquete, leido, sizeof(paquete));

					free(leido);

					leido = NULL;

					leido = readline(">");
				}



	//Una vez hayamos terminado de ingresar valores, toca enviarlo
	enviar_paquete(paquete, conexion);

	//Y no nos olvidemos de borrar el paquete con eliminar_paquete
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion)
{
	//Y por ultimo, para cerrar, hay que liberar lo que utilizamos (conexion, log y config) con las funciones de las commons y del TP mencionadas en el enunciado
	log_destroy(g_logger);
	config_destroy(g_config);
	eliminar_conexion(conexion);

}
