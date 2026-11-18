#ifndef CAMERA_H
#define CAMERA_H

// Modos de câmera disponíveis
enum CameraMode {
    CAM_TOP_DOWN,     // 1 - Vista de cima (padrão do jogo)
    CAM_FIRST_PERSON, // 2 - Vista do personagem (olhando para frente)
    CAM_THERD_PERSON, // 6 - Vista do personagem (olhando para frente)
    CAM_ENEMY_VIEW,   // 3 - Vista do topo da arena olhando para o personagem
    CAM_ARENA_BASE,   // 4 - Vista da base (fundo) olhando para o topo
    CAM_ORBIT,        // 5 - Órbita livre (mouse direito)
    CAM_COUNT
};

void camera_init(void);
void camera_reshape(int w, int h);
void camera_apply(void);             

void camera_set_mode(CameraMode m);
void camera_set_direcao(float anguloGraus);
CameraMode camera_get_mode(void);
void camera_cycle_mode(void);        
void camera_set_zoom(float delta);   
void camera_orbit(float dxPixels, float dyPixels);  
void camera_set_target(float x, float y, float z);
void camera_set_base_height(float h);
float camera_get_base_height();
void camera_set_pitch(float anguloGraus);

#endif