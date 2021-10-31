# Stars

A aplicação baseia-se em regularpolygons para gerar estrelas de n pontas, com n variando aleatoriamente, de tamanho definido em uma escala entre 10% e 100%, que varia conforme o usuário deseja. Suas cores podem ser sólidas ou com um gradiente radial partindo do centro da estrela, e variam aleatoriamente.

## Implementação

A mudança mais significativa no código a partir de regularpolygons ocorre nas linhas 161-174 de openglwindow.cpp, nas quais os vértices das estrelas são definidos para serem gerados posteriormente usando a função GL_TRIANGLE_FAN.

```c++
// Star vertices

  int sidesDrawn = 1;

   for (int i = 1; i <= sides*2; i++) {
        float angle = i * 360.0f/(sides) * M_PI/180.0f;
        if(sidesDrawn %2 == 0)
          positions.emplace_back(std::cos(angle)/2.0, std::sin(angle)/2.0);
        else
          positions.emplace_back(std::cos(angle), std::sin(angle));
         
        colors.push_back(color2);
        sidesDrawn++;
   }
```

Dessa maneira, cada ponta da estrela é composta por dois triângulos, e a posição dos vértices é definida a partir do ângulo obtido considerando o número de pontas da estrela. Assim, caso estejamos desenhando o "lado ímpar" da ponta da estrela, seus vértices serão definidos pelo cosseno e seno desse ângulo. No caso do "lado par", consideramos metade desse valor. 

A possibilidade de alterar manualmente a escala das figuras desenhadas e a opção de gerar figuras de cores sólidas também foram adicionadas. A janela pode ser limpa tanto ao pressionar o botão "Clear Window", quanto ao pressionar a tecla space.