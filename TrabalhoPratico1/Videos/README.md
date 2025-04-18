# Vídeos

Criação de vídeos para auxiliar na exemplificação do funcionamento do código. Foi utilizado a biblioteca [`manim`](https://www.manim.community/).

Para o ambiente, foi utilizado a ferramenta [`uv`](https://docs.astral.sh/uv/)

## Instalação do ambiente

```bash
uv sync
```

## Execução dos vídeos

```bash
manim main.py <nome_da_cena>
```

Cenas disponíveis:
- `TreeSort`: Exemplo de execução do algoritmo TreeSort.
- `TreeSortWorstCase`: Exemplo de execução do algoritmo TreeSort no pior caso.