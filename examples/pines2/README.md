# Pines

A aplicação gera uma floresta de pinheiros 3D, e foi aprimorada a partir da realizada na aatividade 2. Para isso, utiliza um modelo simples criado utilizando Adobe Photoshop para os pinheiros (disponível em assets/pine.obj). 

## Implementação

### Renderização dos pinheiros

A renderização dos pinheiros é feita nas linhas 124-149 de openglwindow.cpp. A cada execução do laço abaixo são gerados dois pinheiros, um na posição [i][j], com tamanho 0.2% do modelo original e um de tamanho levemente menor, na posição [i+0.5][j+0.5]. A cor de cada pinheiro era previamente definida através da escolha aleatória de valores para vermelho e verde dentro de uma faixa para cada cor, mas agora pode ser controlada a partir da ImGUI, que possui parâmetros para controle das propriedades de luz e do material.

```c++


for(float i = -5.0; i<5.0; i++){
    for(float j = -5.0; j<5.0; j++){

      model = glm::mat4(1.0);
      model = glm::translate(model, glm::vec3(i, 0.0f, j));
      model = glm::scale(model, glm::vec3(0.002f));

      abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
      abcg::glUniform1f(shininessLoc, m_shininess);
      abcg::glUniform4fv(KaLoc, 1, &m_Ka.x);
      abcg::glUniform4fv(KdLoc, 1, &m_Kd.x);
      abcg::glUniform4fv(KsLoc, 1, &m_Ks.x);
      m_model.render(m_trianglesToDraw);

      model = glm::mat4(1.0);
      model = glm::translate(model, glm::vec3(i+0.5, 0.0f, j+0.5));
      model = glm::scale(model, glm::vec3(0.0018f));

      abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
      abcg::glUniform1f(shininessLoc, m_shininess);
      abcg::glUniform4fv(KaLoc, 1, &m_Ka.x);
      abcg::glUniform4fv(KdLoc, 1, &m_Kd.x);
      abcg::glUniform4fv(KsLoc, 1, &m_Ks.x);
      m_model.render(m_trianglesToDraw);

    }

```

### Shader

Uma versão simplificada dos shaders apresentados nas notas de aula foi utilizada. Como há apenas a possibilidade de carregar o objeto 3d com textura, as opções de mapeameento foram removidas. O espaço de recorte foi reduzido para que a impressão de "pinheiros flutuantes" da atividade 2 não ocorresse mais.

## Correções

- O modelo utilizado na atividade 2 foi recriado e substituído por um novo, melhor, e que possui os arquivos mtl necessários.
- O espaço de recorte foi corrigido para que as árvores não parecessem mais estar flutuando.
- O limite máximo para a visão superior foi alterado.
- O programa agora utiliza a classe Model apresentada nas notas de aula para realizar a renderização do modelo.
- O modal que regula a visão superior foi incorporado ao modal de propriedades, e seu texto não está mais cortado.


Nome: Tamiris Gabriele da Silva Lira RA: 11023914
