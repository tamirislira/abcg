# Stars

A aplicação gera uma floresta de pinheiros 3D. Para isso, utiliza um modelo simples criado utilizando Adobe Photoshop para os pinheiros (disponível em assets/pinetree.obj). 

## Implementação

Utilizando o código de lookat das notas de aula, foram feitas três mudanças mais signiificativas. O chão da floresta agora possui uma única cor, foi implementada uma nova função view que altera a visão da câmera e o modelo é renderizado várias vezes.

### View

Agora é possível obter uma visão do topo da imagem, utilizando a função view. A altura da câmera é controlada através de um parâmetro topView, definido pelo usuário atravéz de um slider.

```c++
void Camera::view(float topView) {
  m_eye = glm::vec3(0.0f, topView, 2.5f);
  computeViewMatrix();
}
```

### Renderização dos pinheiros

A renderização dos pinheiros é feita nas linhas 191-218. A cada execução do laço abaixo são gerados dois pinheiros, um na posição [i][j], com tamanho 1% do modelo original e um de tamanho levemente menor, na posição [i+0.5][j+0.5]. A cor de cada pinheiro é definida através da escolha aleatória de valores para vermelho e verde dentro de uma faixa pré-definida para cada cor.

```c++

for(float i = -5.0; i<5.0; i++){
    for(float j = -5.0; j<5.0; j++){
  
      rd = red(m_randomEngine)/255.0f;
      gr = green(m_randomEngine)/255.0f;
        
      model = glm::mat4(1.0);
      model = glm::translate(model, glm::vec3(i, 0.0f, j));
      model = glm::scale(model, glm::vec3(0.01f));

      abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
      abcg::glUniform4f(colorLoc, rd, gr, 0.0f, 1.0f);
      abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
                          nullptr);

      rd = red(m_randomEngine)/255.0f;
      gr = green(m_randomEngine)/255.0f;

      model = glm::mat4(1.0);
      model = glm::translate(model, glm::vec3(i+0.5, 0.0f, j+0.5));
      model = glm::scale(model, glm::vec3(0.008f));

      abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
      abcg::glUniform4f(colorLoc, rd, gr, 0.0f, 1.0f);
      abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
                          nullptr);

    }


```

Nome: Tamiris Gabriele da Silva Lira RA: 11023914
