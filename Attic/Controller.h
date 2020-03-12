#ifndef _H_CONTROLLER
#define _H_CONTROLLER

void server_setup_manager(); // Iniciar servidor de configuração de acesso
void server_setup_controller();     // Iniciar servidor de controle

void server_loop_manager();
void server_loop_controller();

void switch_toggle(int status); // Ligar/desligar relé

#endif