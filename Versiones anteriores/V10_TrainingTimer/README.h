/* V8_TrainingTimer
 * ----------------
 * Intentos: 
 * - (DONE en V5) Comunicacion con formato de Actividad a realizar:
 *    - Tipo ('Funcional'  -posibles variantes Tabata, AMRAP, Tiempo/Cantidad- ,'Boxeo', 'Kick'...)
 *    - T_Trabajo
 *    - T_Descanso/Transicion
 *    - Rounds
 *    - Sets 
 *    - T_EntreSets     
 *    - T_Alarma_Trabajo (VER, quizas se pueden poner en una COMM diferente, y/o poner una in/habilitacion de esta alarma)
 *    - T_Alarma_Descanso (VER)
 *    
 * - (DONE en V2) Temporizacion por TimerInterrupt
 * - (DONE en V3) Operacion con minutos/segundos
 * - (DONE en V4) Correccion ciertos errores de la V3, en la reanudacion de actividad
 * - (DONE en V6) Incorporar Sets y correccion de errores cuando comenzaba otra rutina distinta a la anterior (flashaba rounds o sets erroneos)
 * - Pasar control displays a serie
 * - Ver tema de la comunicacion serie del 'subStr' VS strok (el strok si lo quiero usar, cambiar formato de la trama, --> "Datos", separarlos por '-' o utilizar 'copia_trama' en la funcion (esta es la mas simple) para limpiar el programa
 * 
 * - (DONE en V8) Modificaciones basicas, operando con la App en Flutter y modificacion de ficheros de funciones (incorporacion de ficheros por separado, SerialFunctions, Display7s y functions)
 *                Fix tema de TimerISR que disparaba instantaneamente por no limpiar flag previo (TIFR1)
 *                OJO! V8 tiene error, que no me habia dado cuenta, solucionado en V9, por el uso de 'subStr', flasheaba y se reseteaba
 * - (en proceso V9) Incoporacion opciones 'round/setIncrement' ANDANDO
 *                   Fix error mencionado del 'subStr', cambiando por el uso de 'strtok' + 'atoi'
 *                   Se incorporo el fichero 'routineMode', con la intencion de estructurar mejor el programa, mediante el uso de la Clase Routine
 *                   (No se termino de implementar porque surgio la necesidad del Fix del problema del subStr, se deja para mas adelante)
 *                   Incorporacion del uso de variables globales ('extern') incluidas en fichero 'definiciones', lo que me da acceso a todas ellas
 *                   desde cualquier fichero (que tenga #include "definiciones") sin necesidad de pasar punteros a cada variable para su modificacion
 *                   VER si es un problema, si quisiera usar class Routine, a la hora de declararla en el programa y en 'definiciones', por el tema 
 *                   de que al declararla se genera el cosntructor y demás
 *                   (UPD: Fixed, creo) SOLUCIONAR -> Durante el funcionamiento normal, si llega otro "start" se flashea
 *                   
 * - (en proceso v10) Agregar caso de comunicacion "Resincronizar" (por si se desconecto/apago el Bluetooth Timer, desde la app mando a "resincronizar")
 * 
 */
