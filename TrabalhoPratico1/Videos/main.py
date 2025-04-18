from manim import *
from manim_dsa import *

def createNodes(arr):
    nodes = []
    for value in arr:
        node = Circle(radius=0.5, color=WHITE)
        node.set_fill(GREEN, opacity=0.5)
        node_label = Text(str(value), font_size=28, font='Cascadia Code').move_to(node.get_center())
        nodes.append(VGroup(node, node_label))
    return nodes

class TreeSort(Scene):
    def construct(self):
        arr = [51, 40, 83, 79, 43, 56, 19, 35] # Essa lista não é dinâmica. Se for modificada, terá que modificar a lógica para a geração
        elems_pos = [
            (0, 0, 0), # 51
            LEFT * 2 + DOWN, # 40
            RIGHT * 2 + DOWN, # 83
            RIGHT * 1 + DOWN * 2, # 79
            LEFT * 1 + DOWN * 2, # 43
            RIGHT * 0 + DOWN * 3, # 56
            LEFT * 3 + DOWN * 2, # 19
            LEFT * 2 + DOWN * 3 # 35
        ]

        elems_travers = [
            [], # 51
            [0], # 40
            [0], # 83
            [0, 2], # 79
            [0, 1], # 43
            [0, 2, 3], # 56
            [0, 1], # 19
            [0, 1, 6] # 35
        ]

        arrows: dict[int,Line] = dict() # Dicionário para armazenar as linhas que ligam os nós. A chave é o índice do nó incidente

        nodes: list[VGroup] = []
        mArr = MArray(arr, style=MArrayStyle.GREEN).add_indexes()

        self.play(Write(Text("Tree Sort", font_size=40, font='Cascadia Code').shift(UP * 3.5 + LEFT * 5.3)))

        # Criação da array
        self.play(Write(mArr), run_time=1.5)
        self.wait()

        # Move a array para o topo
        self.play(mArr.animate.scale(0.6).shift(UP*3))
        self.wait()

        # Seta que aponta para o elemento atual
        arrow = Line(end=mArr.elements[0].get_bottom(), start=mArr.elements[0].get_bottom() + DOWN * 0.8, buff=0.1, color=RED).add_tip(tip_width=0.2, tip_length=0.2)

        # Criação dos nós
        nodes = createNodes(arr)

        self.play(Write(arrow))

        # Criação dos nós do grafo
        # Demonstra a inserção dos nós na árvore
        for i in range(len(nodes)):
            # Próximo elemento no array
            elem: VGroup = mArr.elements[i].copy()

            # Move a seta para o próximo elemento da lista
            arrow.target = Line(end=elem.get_bottom(), start=elem.get_bottom() + DOWN * 0.8, buff=0.1, color=RED).add_tip(tip_width=0.2, tip_length=0.2)

            self.play(MoveToTarget(arrow))
            self.wait(0.5)

            # Cria um nó saindo do elemento atual e posiciona ao lado da lista
            elem.target = nodes[i].copy().move_to((0,0,0)).shift(UP * 1.3)

            nodes[i] = elem

            self.play(MoveToTarget(elem))
            self.wait(0.1)

            # Mostra os nós que estão sendo percorridos para a inserção
            for j in elems_travers[i]:
                # Pisca o nó
                if j in arrows:
                    arrows[j].set_color(RED)
                    self.play(arrows[j].animate.set_color(WHITE), run_time=0.5)
                    self.wait(0.05)
                nodes[j][0].set_fill(RED)
                self.play(nodes[j][0].animate.set_fill(BLUE), run_time=0.5)
                self.wait(0.1)

            # Adiciona o nó na árvore
            self.play(elem.animate.center().shift(elems_pos[i]).scale(0.8), elem[0].animate.set_fill(BLUE).center().shift(elems_pos[i]).scale(0.8))

            # Cria a linha que liga o nó ao pai
            if i > 0:
                line = Line(end=elem.get_center(), start=nodes[elems_travers[i][-1]].get_center(), buff=0.4, color=WHITE).add_tip(tip_width=0.2, tip_length=0.2)
                arrows[i] = line
                self.play(Write(line))
        self.wait()

        # Percorrimento em ordem e Pilha

        # Criação da pilha
        mStack = MStack(style=MStackStyle.PURPLE).scale(0.8).shift(RIGHT * 6)
        mStack._hidden_element.value.font_size = 10

        # Lógica de travessia
        # Uma tupla, onde:
        # 1 - Ordem dos elementos para travessia
        # 2 - Ações:
        #      -1 - Retorna para o nó anterior (remove elemento da pilha)
        #       0 - Nó atual (Chama a função do nó atual)
        #       1 - Nó da esquerda (Chama a função do nó da esquerda)
        #       2 - Nó da direita (Chama a função do nó da direita)
        trav = [
            ([0, 1, 6], [1, 1, 0]),         # 19
            ([6, 7], [2, 0]),               # 35
            ([7, 6, 1], [-1, -1, 0]),       # 40
            ([1, 4], [2, 0]),               # 43
            ([4, 1, 0], [-1, -1, 0]),       # 51
            ([0, 2, 3, 5], [2, 1, 1, 0]),   # 56
            ([5, 3], [-1, 0]),              # 79
            ([3, 2], [-1, 0]),              # 83
            ([0], [-1])                     # 51
        ] 

        elem: VGroup = mArr.elements[0].copy()

        # Move a seta para o primeiro elemento da lista
        arrow.target = Line(end=elem.get_bottom(), start=elem.get_bottom() + DOWN * 0.8, buff=0.1, color=RED).add_tip(tip_width=0.2, tip_length=0.2)

        # Nome das funções em um algoritmo recursivo de percorrimento
        texts = VGroup(
            Text("LeftNode()", font_size=24),
            Text("CurrentNode()", font_size=24),
            Text("RightNode()", font_size=24)
        )
 
        texts.arrange(DOWN, aligned_edge = LEFT)
        texts.move_to(LEFT * 5.8)

        self.play(Write(mStack), MoveToTarget(arrow), Write(texts))
        idx = 0
        last_action = 0

        # Seta que aponta para o elemento atual na árvore
        tree_arrow = Line(end=nodes[0].get_top(), start=nodes[0].get_top() + UP * 0.7, buff=0.1, color=RED).add_tip(tip_width=0.2, tip_length=0.2)

        self.play(Write(tree_arrow))

        # Percorrimento em ordem
        for (nodes_trav, actions) in trav:
            for (node, action) in zip(nodes_trav, actions):
                # Pisca o nó
                if node in arrows and (action != -1 and (last_action == 1 or last_action == 2)):
                    arrows[node].set_color(RED)
                    self.play(arrows[node].animate.set_color(WHITE))
                    self.wait(0.05)

                # Move a seta para o próximo elemento da lista
                tree_arrow.target = Line(end=nodes[node].get_top(), start=nodes[node].get_top() + UP * 0.7, buff=0.1, color=RED).add_tip(tip_width=0.2, tip_length=0.2)
                self.play(MoveToTarget(tree_arrow))

                if action != -1:
                    nodes[node][0].set_fill(RED)
                    self.play(nodes[node][0].animate.set_fill(BLUE))
                    self.wait(0.1)

                # Ação para ser tomada quando cega no nó
                match action:
                    case -1: # Retorna para o nó anterior
                        self.play(mStack.animate.pop()) # Remove a chamada da pilha
                    case 0: # Nó atual
                        texts[1].set_color(RED)

                        # Muda o valor na posição atual do array pelo valor do nó atual
                        self.play(mArr.elements[idx].animate.set_value(arr[node]), texts[1].animate.set_color(WHITE))
                        idx += 1 # Aumenta o índice do array

                        if idx < len(arr): # Move a seta para o próximo elemento da lista
                            arrow.target = Line(end=mArr.elements[idx].get_bottom(), start=mArr.elements[idx].get_bottom() + DOWN * 0.8, buff=0.4, color=RED).add_tip(tip_width=0.2, tip_length=0.2)
                            self.play(MoveToTarget(arrow))

                    case 1: # Nó da esquerda
                        texts[0].set_color(RED)
                        self.play(mStack.animate.append(f"Left\nNode({arr[node]})"), texts[0].animate.set_color(WHITE))
                    case 2: # Nó da direita
                        texts[2].set_color(RED)
                        self.play(mStack.animate.append(f"Right\nNode({arr[node]})"), texts[2].animate.set_color(WHITE))

                last_action = action
        
        # Remove os elementos e mostra o resultado ordenado
        self.play(Unwrite(texts), Unwrite(arrow), Unwrite(tree_arrow), *[Unwrite(x) for x in nodes], Unwrite(mStack), *[Unwrite(x) for x in arrows.values()], mArr.animate.scale(1.5).shift(DOWN*3), run_time=1)
        self.wait(1)
                



class TreeSortWorstCase(Scene):
    def construct(self):
        self.play(Write(Text("Tree Sort", font_size=40, font='Cascadia Code').shift(UP * 3.5 + LEFT * 5.3)))
        self.play(Write(Text("(Pior caso)", font_size=20, font='Cascadia Code').shift(UP * 3 + LEFT * 5.3)))

        arr = [4, 8, 15, 20, 18] # Essa lista não é dinâmica. Se for modificada, terá que modificar a lógica para a geração
        elems_pos = [
            LEFT * 2 + UP, # 4
            LEFT, # 8
            DOWN, # 15
            RIGHT + DOWN * 2, # 20
            DOWN * 3, # 18
        ]

        elems_travers = [
            [], # 4
            [0], # 8
            [0, 1], # 15
            [0, 1, 2], # 20
            [0, 1, 2, 3] # 18
        ]

        arrows: dict[int,Line] = dict() # Dicionário para armazenar as linhas que ligam os nós. A chave é o índice do nó incidente

        nodes: list[VGroup] = []
        mArr = MArray(arr, style=MArrayStyle.GREEN).add_indexes().scale(0.6).shift(UP*3)

        # Criação da array
        self.play(Write(mArr), run_time=1.5)
        self.wait()

        # Seta que aponta para o elemento atual
        arrow = Line(end=mArr.elements[0].get_bottom(), start=mArr.elements[0].get_bottom() + DOWN * 0.8, buff=0.1, color=RED).add_tip(tip_width=0.2, tip_length=0.2)

        # Criação dos nós
        nodes = createNodes(arr)

        self.play(Write(arrow))

        # Criação dos nós do grafo
        # Demonstra a inserção dos nós na árvore
        for i in range(len(nodes)):
            # Próximo elemento no array
            elem: VGroup = mArr.elements[i].copy()

            # Move a seta para o próximo elemento da lista
            arrow.target = Line(end=elem.get_bottom(), start=elem.get_bottom() + DOWN * 0.8, buff=0.1, color=RED).add_tip(tip_width=0.2, tip_length=0.2)

            self.play(MoveToTarget(arrow))
            self.wait(0.5)

            # Cria um nó saindo do elemento atual e posiciona ao lado da lista
            elem.target = nodes[i].copy().move_to((0,0,0)).shift(UP * 1.3)

            nodes[i] = elem

            self.play(MoveToTarget(elem))
            self.wait(0.1)

            # Mostra os nós que estão sendo percorridos para a inserção
            for j in elems_travers[i]:
                # Pisca o nó
                if j in arrows:
                    arrows[j].set_color(RED)
                    self.play(arrows[j].animate.set_color(WHITE), run_time=0.3)
                nodes[j][0].set_fill(RED)
                self.play(nodes[j][0].animate.set_fill(BLUE), run_time=0.3)

            # Adiciona o nó na árvore
            self.play(elem.animate.center().shift(elems_pos[i]).scale(0.8), elem[0].animate.set_fill(BLUE).center().shift(elems_pos[i]).scale(0.8))

            # Cria a linha que liga o nó ao pai
            if i > 0:
                line = Line(end=elem.get_center(), start=nodes[elems_travers[i][-1]].get_center(), buff=0.4, color=WHITE).add_tip(tip_width=0.2, tip_length=0.2)
                arrows[i] = line
                self.play(Write(line))
        self.wait()