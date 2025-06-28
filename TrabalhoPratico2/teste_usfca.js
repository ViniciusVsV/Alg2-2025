
function inserirValores(valores){
    const intervaloDeVerificacao = 100;

    const campoDeEntrada = document.querySelector('#AlgorithmSpecificControls input');
    const botaoInserir = document.querySelector('#AlgorithmSpecificControls input[type=Button]');

    function inserirProximoValor(indice) {
        if (indice >= valores.length) {
            console.log("Todos os valores foram inseridos!");
            return;
        }

        // Pega o valor atual da lista.
        const valor = valores[indice];

        // Coloca o valor no campo de texto.
        campoDeEntrada.value = valor;

        // Simula um clique no botão "Insert".
        botaoInserir.click();
        console.log(`Inserindo o valor: ${valor}. Aguardando a animação terminar...`);

        // Cria um intervalo para verificar quando o botão estará habilitado novamente.
        const verificadorDeEstado = setInterval(() => {
            // Se o botão NÃO estiver desabilitado (!botaoInserir.disabled)...
            if (!botaoInserir.disabled) {
            // 1. Para de verificar.
            clearInterval(verificadorDeEstado);
            
            // 2. Chama a função para inserir o próximo valor da lista.
            inserirProximoValor(indice + 1);
            }
        }, intervaloDeVerificacao);
    }

    // Inicia o processo de inserção com o primeiro valor da lista (índice 0).
    inserirProximoValor(0);
}

inserirValores([3,42,73,37,86,50,21,30,24,62,7,90,32,19,94,1,74,14,55,28,92,66,4,99,41,36,35,91,25,2,15,61,33,46,34,26,84,40,10,43,45,77,79,52,16,27,51,47,22,13,83,29,87,57,12,48,97,58,82,20,9,60,49,59,63,78,23,75,6,98,76,80,96,5,93,18,100,89,81,65,56,11,39,54,95,44,64,31,69,67,88,17,68,70,38,8,85,72,53,71])

function removerValores(valores){
    const intervaloDeVerificacao = 100;

    const campoDeEntrada = document.querySelector('#AlgorithmSpecificControls td:nth-child(3) input');
    const botaoRemover = document.querySelector('#AlgorithmSpecificControls td:nth-child(4) input[type=Button]');

    function removerProximoValor(indice) {
        if (indice >= valores.length) {
            console.log("Todos os valores foram removidos!");
            return;
        }

        // Pega o valor atual da lista.
        const valor = valores[indice];

        // Coloca o valor no campo de texto.
        campoDeEntrada.value = valor;

        // Simula um clique no botão "Remove".
        botaoRemover.click();
        console.log(`Removendo o valor: ${valor}. Aguardando a animação terminar...`);

        // Cria um intervalo para verificar quando o botão estará habilitado novamente.
        const verificadorDeEstado = setInterval(() => {
            // Se o botão NÃO estiver desabilitado (!botaoRemover.disabled)...
            if (!botaoRemover.disabled) {
                // 1. Para de verificar.
                clearInterval(verificadorDeEstado);
                
                // 2. Chama a função para remover o próximo valor da lista.
                removerProximoValor(indice + 1);
            }
        }, intervaloDeVerificacao);
    }

    // Inicia o processo de remoção com o primeiro valor da lista (índice 0).
    removerProximoValor(0);
}
removerValores([50,80,81,56,73,86,45,18,91,23,70,99,29,68,40,54,90,60,61,9,94,10,93,87,1,44,12,55,27,5,78,69,63,95,89,14,52,15,98,28,57,38,49,65,82,16,83,20,25,72])