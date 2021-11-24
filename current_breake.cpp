#include <iostream>

using namespace std;


typedef enum {

CAN_PACKET_SET_DUTY = 0, //Duty cycle mode
CAN_PACKET_SET_CURRENT, //Current loop mode
CAN_PACKET_SET_CURRENT_BRAKE, // Current brake mode
CAN_PACKET_SET_RPM,
//Velocity mode
CAN_PACKET_SET_POS,
// Position mode
CAN_PACKET_SET_ORIGIN_HERE,
//Set origin mode
CAN_PACKET_SET_POS_SPD,
//Position velocity loop mode
} CAN_PACKET_ID;

int motor_pos;
int16_t motor_spd;
int16_t cur;
int8_t temp;
int8_t error;
float test; 
/*****************************************************************************************************************************************************
 FUNCIONES BASICAS DE COMUNICACION
 ****************************************************************************************************************************************************/
//ENVIO POR CAN 
void comm_can_transmit_eid(uint32_t id, const uint8_t *data, uint8_t len) {
cout<<"\n Entramos en la función de enviar CAN \n";
uint8_t i=0;
if (len > 8) {
len = 8;
}
cout<<"El id es "<<id<<", y la trama es de longitud "<<unsigned(len)<<"\n";
cout<<"La trama a enviar es: \n";
for(i=0;i<len;i++){
cout<<"\n"<<unsigned(data[i]);
}
}

//BUFFER APPEND CON 32 BITS
void buffer_append_int32(uint8_t* buffer, int32_t number, int32_t *index) {

cout<<"\n\n\nEntrada a la funcion del buffer32";

cout<<"\nLa entrada al buffer es: " <<number<<"\n"; 

buffer[(*index)++] = number >> 24;
cout<<"\nValor de index = "<<*index<<"\n"; 
cout<<"Valor del numero guardado  = "<<unsigned(buffer[0])<<"\n";

buffer[(*index)++] = number >> 16;
cout<<"\nValor de index = "<<*index<<"\n"; 
cout<<"Valor del numero guardado  = "<<unsigned(buffer[1])<<"\n";

buffer[(*index)++] = number >> 8;
cout<<"\nValor de index = "<<*index<<"\n"; 
cout<<"Valor del numero guardado  = "<<unsigned(buffer[2])<<"\n";

buffer[(*index)++] = number;
cout<<"\nValor del numero guardado  = "<<unsigned(buffer[3])<<"\n";
cout<<"Valor de index = "<<*index<<"\n"; 
}

//BUFFER APPEND CON 16 BITS
void buffer_append_int16(uint8_t* buffer, int16_t number, int16_t index) {
buffer[(index)++] = number >> 8;
buffer[(index)++] = number;
}




/*****************************************************************************************************************************************************
 FUNCION DE FRENO DE CORRIENTE--- APARENTEMENTE DEPURADA
 ****************************************************************************************************************************************************/

void comm_can_set_cb(uint8_t controller_id, float current) {
cout<<"Entramos en la función de freno corriente \n";
cout<<"Las entradas al freno de corriente son id="<<unsigned(controller_id)<<" y corriente="<<current<<"\n";

int32_t send_index = 0;
uint8_t buffer[4];

cout<<"Llamada a bufer_append32 \n",
//Filtrado del número en los diferentes bytes
buffer_append_int32(buffer, (int32_t)(current * 1000.0), &send_index);

cout<<"\n\n Volvemos a la función de freno corriente \n";
//Printeos por pantalla para depuracion 
uint8_t i=0;
cout<<"Las salidas son id="<<unsigned(controller_id)<<" buffer: "<<buffer<<" y send_index="<<unsigned(send_index)<<"\n";
cout<<"El contenido que se ha guardado en el buffer es:  \n";
for(i=0;i<4;i++){
cout<<"\n"<<unsigned(buffer[i])<<"\n";
std::cout << std::hex << buffer[i];
}
cout<<"\nEl controller id es: "<<unsigned(controller_id)<<" y el otro termino es:"<<((uint32_t)CAN_PACKET_SET_CURRENT_BRAKE <<8)<<"\n";
cout<<"El controller id es: "<<unsigned(controller_id)<<" y el otro termino es:"<<(uint32_t)CAN_PACKET_SET_CURRENT_BRAKE <<"\n";

cout<<"\n Llamada a la funcion de enviar CAN \n\n\n",
//Llamamos a la funcion de enviar por el CAN
comm_can_transmit_eid(controller_id |
((uint32_t)CAN_PACKET_SET_CURRENT_BRAKE << 8), buffer, send_index);
}





/*****************************************************************************************************************************************************
 FUNCION DE VELOCIDAD -- APARENTEMENTE DEPURADA
 ****************************************************************************************************************************************************/
void comm_can_set_rpm(uint8_t controller_id, float rpm) {
  cout<<"Entramos en la función de velocidad\n";
  cout<<"Las entradas a la funcion de velocidad son: "<<unsigned(controller_id)<<" y "<<rpm<<"\n";
  
  
  int32_t send_index = 0;
  uint8_t buffer[4];

//Filtrado del número en los diferentes bytes
  buffer_append_int32(buffer, (int32_t)rpm, &send_index);

//Printeos por pantalla para depuracion 
uint8_t i=0;
cout<<"Las salidas son "<<unsigned(controller_id)<<" buffer: "<<buffer<<" y "<<unsigned(send_index)<<"\n";
for(i=0;i<4;i++){
cout<<"\n"<<unsigned(buffer[i])<<"\n";
}
cout<<"El controller id es: "<<unsigned(controller_id)<<" y el otro termino es:"<<((uint32_t)CAN_PACKET_SET_RPM <<8)<<"\n";
cout<<"El controller id es: "<<unsigned(controller_id)<<" y el otro termino es:"<<(uint32_t)CAN_PACKET_SET_RPM <<"\n";


  //Llamamos a la funcion de enviar por el CAN
  comm_can_transmit_eid(controller_id |
  ((uint32_t)CAN_PACKET_SET_RPM << 8), buffer, send_index);
}





/*****************************************************************************************************************************************************
 FUNCION DE RESET ORIGEN
 ****************************************************************************************************************************************************/

  //Llamamos a la funcion de enviar por el CAN
void comm_can_set_origin(uint8_t controller_id, uint8_t set_origin_mode) {
int send_index = 0;
uint8_t buffer;
//comm_can_transmit_eid(controller_id |
//((uint32_t) CAN_PACKET_SET_ORIGIN_HERE << 8), buffer, send_index);
}





/*****************************************************************************************************************************************************
 FUNCION DE POSICION Y VELOCIDAD
 ****************************************************************************************************************************************************/
void comm_can_set_pos_spd(uint8_t controller_id, float pos,int16_t spd, int16_t RPA ) {
int32_t send_index = 0;
int send_index1 = 0;
uint8_t buffer[4];
buffer_append_int32(buffer, (int32_t)(pos * 10000.0), &send_index);
buffer_append_int16(buffer,spd, send_index1);
buffer_append_int16(buffer,RPA, send_index1);
comm_can_transmit_eid(controller_id |((uint32_t)CAN_PACKET_SET_POS << 8), buffer, send_index);
}

/*****************************************************************************************************************************************************
 FUNCION DE RECEPCION
 ****************************************************************************************************************************************************/

void motor_receive(short int data[8]){
cout<<"Entramos a la recepcion de datos \n";

motor_pos=   (data[0] << 8 | data[1]); //motor position
motor_spd=   (data[2] << 8 | data[3]);// motor speed
cur=   (data[4] << 8 | data[5]);//motor current
temp=  data[6] ;//motor temperature
error= data[7] ;//motor fault code
cout<<"Salimos de la recepcion de datos \n";
}

/*****************************************************************************************************************************************************
 FUNCION DE RSOLO POSICION
 ****************************************************************************************************************************************************/

void comm_can_set_pos(uint8_t controller_id, float pos) {
int32_t send_index = 0;
uint8_t buffer[4];
buffer_append_int32(buffer, (int32_t)(pos * 1000000.0), &send_index);
comm_can_transmit_eid(controller_id |
((uint32_t)CAN_PACKET_SET_POS << 8), buffer, send_index);
}

/*****************************************************************************************************************************************************
 FUNCIONES FLOAT16
 ****************************************************************************************************************************************************/
int16_t buffer_get_int16(const uint8_t *buffer, int32_t *index) {
	int16_t res =	((uint16_t) buffer[*index]) << 8 |
					((uint16_t) buffer[*index + 1]);
	*index += 2;
	return res;
}

float buffer_get_float16(const uint8_t *buffer, float scale, int32_t *index) {
    return (float)buffer_get_int16(buffer, index) / scale;
}




int main() {
  cout<<"Programa depuracion modo servo \n";
  int i; 
//Mandamos un comando de freno de corriente al id 8 de valor 50
 /*  cout<<"Selecciona el ID\n";
  int id; 
  cin>>id; 
  cout<<"Selecciona la corriente\n";
  float current; 
  cin>>current;
  cout<<"Mando un comando de freno de corriente \n\n\n";
  comm_can_set_cb(id, current);

  cout<<"\n\nMando un comando de velocidad\n";
  cout<<"Selecciona el ID\n";
  cin>>id; 
  cout<<"Selecciona la velocidad\n";
  cin>>current;
  cout<<"Mando un comando de velocidad \n\n\n";
  comm_can_set_rpm(8, -60000);

  cout<<"\n\nMando un comando de velocidad y posicion\n";
  cout<<"Selecciona el ID\n";
  cin>>id; 
  float vel, pos,acc; 
  cout<<"Selecciona la posicion\n";
  cin>>pos;
  cout<<"Selecciona la velocidad\n";
  cin>>vel;
  cout<<"Selecciona la aceleració\n";
  cin>>acc;
  cout<<"Mando un comando de velocidad \n\n\n";

  comm_can_set_pos_spd(id,pos,vel,acc );
 */

cout<<"Introduce los bloques de la cadena de menor a mayor\n";

short int data[8]={0x0C, 0x5F, 0x03, 0x88, 0x00, 0x1F, 0x1B, 0X00};
cout<<"introduce dato 0\n";
cin>>hex>>data[0];
cout<<"introduce dato 1\n";
cin>>hex>>data[1];
cout<<"introduce dato 2\n";
cin>>hex>>data[2];
cout<<"introduce dato 3\n";
cin>>hex>>data[3];
cout<<"introduce dato 4\n";
cin>>hex>>data[4];
cout<<"introduce dato 5\n";
cin>>hex>>data[5];
cout<<"introduce dato 6\n";
cin>>hex>>data[6];
cout<<"introduce dato 7\n";
cin>>hex>>data[7];

cout<<"\nCadena introducida con éxito, has introducido la siguiente cadena 0x ";
for(i=0;i<8;i++){
cout<<"  "; 
cout<<hex<<(data[i]);
}
cout<<"\n";
motor_receive(data);
float vel; 
uint8_t buffer[2];
int32_t send_index = 0;
//buffer[0]=data[3];
//buffer[1]=data[4];
//vel= buffer_get_float16(buffer, 10,&send_index ); 
cout<<"Volvemos al main \n";
float pos=(float)motor_pos/10; 
cout<<"La posicion es: "<<pos<<"\n";

float spd=(float)motor_spd*10; 
cout<<"La velocidad es: "<<spd<<"\n";

//cout<<"La velocidad es: "<<vel<<"\n";
//cout<<1e1;
float current=(float)cur/100; 
cout<<"La corriente es: "<<cur<<"\n";
cout<<"La corriente es: "<<current<<"\n";


cout<<"La temperatura es: "<<hex<<temp<<"\n";
cout<<"El error es: "<< std::hex<<error<<"\n";

cout<<"\n El tamaño del float aqui es "<<sizeof( float)<<"\n"; 
  return 0;
}
