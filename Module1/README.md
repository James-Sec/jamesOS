# Módulo 1 - Conceitos iniciais

Este módulo apresenta diversos conceitos básicos que são **indispensáveis** para o entendimento dos mecanismos iniciais que serão discutidos. 

## Registradores

Leituras indicadas:
[CPU Registers x86](https://wiki.osdev.org/CPU_Registers_x86)

Registradores são unidades de armazenamento de dados existente dentro do _chipset_ do processador, utilizadas para armazenar informações que necessitam de rápido acesso ou para informações de controle da CPU e do sistema em geral. Em processadores da família x86, existem diferentes grupos de registradores, cada grupo com funções determinadas: registradores de propósitos gerais; registradores ponteiros; registradores de segmento; registrador FLAGS; registradores de controle. Os grupos apresentados, além de alguns complementares, estão apresentados em [CPU Registers x86](https://wiki.osdev.org/CPU_Registers_x86). 

Na família de processadores x86, os registradores podem ter diferentes tamanhos: 8 bits, 16 bits, 32 bits e 64 bits, de maneira que o nome do registrador indica o seu tamanho. Existem extensões em novos processadores que permitem registradores de 128 bits, 256 bits e 512 bits, como é o caso do [SSE](https://en.wikipedia.org/wiki/Streaming_SIMD_Extensions) e do [AVX](https://en.wikipedia.org/wiki/Advanced_Vector_Extensions) por exemplo. Os registradores de tamanhos menores referenciam alguma parte dos registradores maiores. O registrador AX, por exemplo, possui 16 bits. O registrador AL, que possui 8 bits, corresponde aos 8 bits de baixa ordem do registrador AX. O registrador AH, que possui 8 bits, corresponde aos 8 bits de alta ordem do registrador AX. Da mesma maneira, o registrador AX corresponde aos 16 bits de baixa ordem do registrador EAX, que possui 32 bits. 

O conjunto de registradores AX, BX, CX, DX, SI, DI, BP e SP, além de seus respectivos registradores de tamanhos distintos, formam o grupo de registradores de propósito geral. Cada um desses registradores possui uma função em determinadas instruções, como mostra o quadro abaixo:

	
|32 bits | 16 bits | 8 bits altos | 8 bits baixos | Descrição|
|--------|---------|--------------|---------------|----------|
|eax | ax | ah | al | acumulador|
|ebx | bx | bh | bl | base|
|ecx | cx | ch | cl | contador|
|edx | dx | dh | dl | dados|
|esi | si | N/A | N/A | índice de origem|
|edi | di | N/A | N/A | índice de destino|
|ebp | bp | N/A | N/A | ponteiro para base da pilha|
|esp |sp | N/A | N/A | ponteiro para o topo da pilha|


Os registradores ponteiros armazenam o endereço de memória que contem a próxima instrução que deve ser executada pelo processador. Esse registrador não pode ser lido ou escrito diretamente por um programa. O quadro abaixo apresenta os registradores ponteiros disponíveis.

|32 bits | 16 bits | Descrição|
|--------|---------|----------|
|eip | ip | pointeiro de instrução|


Os registradores de segmento são utilizados para formar endereços de memória. Dependendo do modo de operação da CPU, a utilização desses registradores pode ser diferente. O quadro abaixo apresenta os registradores disponíveis para esse fim, além da sua aplicação.

|16 bits | Descrição|
|--------|----------|
|cs | segmento de código|
|ds | segmento de dado|
|es | segmento extra|
|ss | segmento da pilha|
|fs | segmento de propósito geral|
|gs | segmento de propósito geral|

O registrador FLAGS é um registrador que armazena o estado atual do processador. Os registradores EFLAGS e RFLAGS são seus sucessores, tendo 32 bits e 64 bits respectivamente. Algumas instruções fazem uso de algumas flags desse registrador, como a _jz_ (jump if zero), _jc_ (jump if carry) e _jo_ (jump if overflow), por exemplo. As flags existentes nesse registrador são apresentadas no quadro abaixo. Vale ressaltar que os bits que não estão citados são reservados.

	
|Bit | Abreviação | Descrição|
|----|------------|----------|
|0 | cf | Flag Carry |
|2 | pf | Flag Parity |
|4 |  af | Flag Auxiliary |
|6 |  zf | Flag Zero |
|7 |  sf | Flag Sign |
|8 |  tf | Flag Trap |
|9 |  if | Flag Interrupt |
|10 |  df | Flag Direction |
|11 | of | Flag Overflow |
|12-13 | iopl | Nível de privilégio de entrada e saída|
|14 | nt | Flag Nested task |
|16 | rf | Flag Resume |
|17 | vm  |Flag Virtual 8086 mode |
|18 | ac | Flag Alignment check |
|19 | vif | Flag Virtual interrupt |
|20 | vip | Interrupções virtuais pendentes|
|21 | id | Flag Id |
  

Os registradores de controle são registradores responsáveis por modificar ou controlar o comportamento geral da CPU ou de outro dispositivo. O registrador de controle CR0 possui várias flags que modificam as operações básicas da CPU. Para realizar operações de leitura e escrita nesse registrador, é necessário um processo de duas vias (utilização de registradores auxiliares), diferentemente dos registradores comuns que podem ser acessados diretamente. As instruções __lmsw__ e __smsw__ também podem ser utilizadas para esse fim. O Quadro abaixo descreve as flags desse registrador.

	
|Bit | Abreviação | Descrição|
|----|------------|----------|
|0 | pe | protected mode habilitado|
|1 | mp | Monitor co-processor|
|2 | em | Emulação|
|3 | ts | Troca de Task|
|4 | et | Tipo de extensão|
|5 | ne | Erro numérico|
|16 | wp | Proteção de escrita|
|18 | am | Máscara de alinhamento|
|29 | nw | Não escrever|
|30 | cd | cache desabilitada|
|31 | pg | Paginação|
  

O registrador de controle CR1 é reservado. O registrador de controle CR2 contém um valor chamado _Page Fault Linear Address_. Quando uma interrupção _Page Fault_ é gerada, o endereço que o processo tentou acessar é armazenado no registrador CR2. O registrador CR3 é utilizado quando o último bit do registrador CR0 (_Paging bit_) é ativado, sendo utilizado durante a tradução de endereços virtuais para endereços físicos.

O registrador de controle CR4 é utilizado para controlar operações como suporte a virtualização 8086, _I/O breakpoints_, extensões de tamanho de páginas e _machine-check_. Os registradores de controle CR5, CR6 e CR7 são reservados.



## Assembly

Leituras indicadas:
[Aprendendo Assembly - Luiz Felipe](https://mentebinaria.gitbook.io/assembly/)
Organização e Estrutura de Computadores - Andrew S. Tanenbaum (Capítulo 1.1)

A programação de computadores modernos é feita utilizando linguagens de alto nível, que a partir de diversos processos de tradução, são transformadas na linguagem disponibilizada pelo processador. Quando um processador é criado, um conjunto de instruções é aceito. Esse conjunto de instruções, que é o mais baixo nível, é chamado nível de microarquitetura. Processadores do tipo CISC implementam, em instruções que executam no nível de microarquitetura, um microcódigo, que compõe no nível denominado ISA (Instruction Set Architecture). Esse microcódigo é criado para que novas instruções sejam aceitas pelo processador, sem alteração em hardware. Para esse tipo de processador, a linguagem Assembly é uma representação da linguagem do nível ISA. Em processadores RISC, (onde não existe microcódigo), o Assembly é a representação da linguagem de microarquitetura. A programação Assembly é comumente utilizada para softwares de baixo nível e aplicações de tempo real.

O processador Intel 80386 (também conhecido como i386), introduzido no ano de 1985, foi utilizado por vários anos em workstations e computadores pessoais, e introduziu um novo modelo de arquitetura de processadores, baseado em 32 bits, que ainda é utilizado atualmente. Tendo em vista sua importância histórica, a utilização atual da arquitetura introduzida por esse processador e sua vasta documentação, este foi utilizado como processador que executará o sistema operacional proposto.

### Sintaxes do Assembly x86
Como é demonstrado por [Felix Cloutier](https://www.felixcloutier.com/x86/), processadores x86 possuem uma vasta quantidade de instruções disponíveis. Existem duas principais sintaxes para o Assembly de processadores x86: Intel e AT&T. O quadro abaixo apresenta algumas das principais características dessas sintaxes.

| | **AT&T** | **Intel** |
|-|-----------|----------|
|**Ordem dos parâmetros** | Origem antes do destino<br>mov \$5, %eax | Destino antes da origem<br>mov eax, 5 |
|**Tamanho dos parâmetros**  |Mnemônicos são sufixados pela letra que identifica o tamanho dos operandos:__q__ para qword, __l__ para long (dword), __w__ para word e __b__ para byte <br>addl \$4, %esp | Tamanho definido de acordo com o nome do registrador que é usado (Ex.: rax, eax, ax, al implica em q, l, w, b respectivamente)<br>add esp, 4|	
|**Símbolos** |Operandos imediatos são prefixados por "$", registradores são prefixados por "%"<br>addl $4, %esp|Assembler automaticamente detecta o tipo de simbolo<br>add esp, 4|
|**Endereços efetivos** |Sintaxe geral: __DISP(BASE,INDEX,SCALE)__<br>movl mem_location (%ebx, %ecx, 4), %eax| Expressões aritméticas entre colchetes; palavras chave de tamanho como __byte__, __word__ ou __dword__ devem ser adicionadas caso o tamanho não possa ser definido pelos operandos.<br>mov eax, [ebx + ecx*4 + mem_location]|

## Modos de operação do processador x86
Existem alguns modos de operações que os processadores x86 podem assumir. Neste trabalho, trataremos do Real Mode e do Protected Mode. 

### Real Mode

Leituras indicadas:
[Real Mode](https://wiki.osdev.org/Real_Mode)

Real Mode é um modo de operação de 16 bits de uma CPU x86. Dentro desse modo, os operandos da CPU são de 16 bits (os registradores de 32 bits estão disponíveis para utilização, caso existam) , limitando o acesso a RAM em menos de 1MB. Não existe qualquer proteção de segurança e o acesso a endereços de memória maiores que 64KB só pode ser realizado com o auxílio de registradores de segmentos.

O acesso à memória no Real Mode pode ser realizado diretamente a partir de endereços físicos ou utilizando registradores de segmentos. Os registradores de segmentos disponibilizados são CS, DS, ES, FS, GS e SS. Para utilizá-los, basta descrever um endereço de memória a partir da associação do segmento com um deslocamento, como mostrado a seguir:
```
segmento : deslocamento
```
A transcrição para o endereço físico é realizada da seguinte maneira:
```
endereço físico = (segmento * 16) + deslocamento
```
Esse tipo de endereçamento permite o acesso à chamada área alta de memória, aumentando a capacidade de acesso no Real Mode

A pilha no Real Mode é delimitada por dois registradores de 16 bits: BP e SP. BP aponta para a base da pilha e SP para o topo da pilha. Vale ressaltar que a pilha cresce do maior endereço para o menor endereço, portanto, o valor de BP deve ser sempre maior ou igual a SP. O registrador SS pode ser utilizado como registrador de segmento para definir endereços para a pilha.

### Protected Mode

Leituras indicadas:
[Protected Mode](https://wiki.osdev.org/Protected_Mode)

Protected Mode é o modo de operação utilizado comumente pelos processadores de 32 bits. Diferentemente do Real Mode, os operandos da CPU são de 32 bits, o que permite um endereçamento de memória composto por 32 bits, possibilitando o controle de até 4GB de memória RAM; o sistema de acesso a memória é mais sofisticado pois suporta paginação; a segurança é efetuada a partir da utilização de anéis de proteção. A mudança entre o Real mode e o Protected Mode se dá pela mudança do bit menos significativo do registrador CR0 (_protected mode eneabled_), seguido de um far jump. Esse pulo tem como objetivo limpar o Pipe Line, para que não existam instruções de um modo executando em outro.

## Anéis de privilégio

Anéis de privilégio são mecanismos de segurança presentes no Protected Mode dos processadores x86 que tem como objetivo proteger dados e funcionalidades de falhas e comportamentos maliciosos. Nos processadores x86, esse mecanismo é composto por quatro níveis de privilégio, em que quanto menor o nível, mais privilégios. Quando o processador está no __ring 0__ por exemplo, o acesso à qualquer posição de memória é disponível para o código que tem a posse da CPU, assim como a capacidade de executar instruções privilegiadas. Caso o processador esteja em __ring 3__ por exemplo, o código que está em execução no momento está restrito ao seu espaço de memória virtual e a um conjunto restrito de instruções que este pode executar.

Posto isso, é comum que sistemas operacionais executem seus códigos com o processador no __ring 0__, já que estes códigos necessitam, em alguma instancia, de acessos privilegiados. Os processos de usuário são comumente executados em __ring 3__, impedindo que eles acessem diretamente recursos sensíveis do sistema operacional e de outros processos, sendo necessária a comunicação desses com o sistema operacional.

O __ring 1__ e o __ring 2__ não são comumente utilizados por sistemas operacionais modernos. Contudo, tecnologias de virtualização podem usufruir desses níveis para executar sistemas operacionais convidados e hypervisors (processadores atuais fornecem surporte em hardware para virtualização), assim como sistemas operacionais microkernel podem reorganizar partes do seu código entre estes níveis de proteção.

