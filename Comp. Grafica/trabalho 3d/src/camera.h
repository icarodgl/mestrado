#ifndef CAMERA_H
#define CAMERA_H

// Modos de câmera disponíveis
enum CameraMode {
    CAM_ORBIT,       // órbita livre em volta da origem
    CAM_FIXED,       // perspectiva fixa, olhando para o centro
    CAM_TOP_DOWN,    // vista de cima (útil p/ debug do jogo 2D→3D)
    CAM_COUNT        // sentinela
};

void camera_init(void);
void camera_reshape(int w, int h);
void camera_apply(void);             // chama gluLookAt/projeção — chamar no início do renderScene
void camera_set_mode(CameraMode m);
CameraMode camera_get_mode(void);
void camera_cycle_mode(void);        // tecla 'c'
void camera_set_zoom(float delta);   // +/- no teclado
void camera_orbit(float dxPixels, float dyPixels);  // arrastar com botão direito
void camera_set_target(float x, float y, float z);

#endif