# Módulo 0 - Ferramentas

Este módulo apresenta as ferramentas indicadas para a construção do sistema operacional proposto.

## Compilador cruzado

Leituras indicadas:
[Why do I need a Cross Compiler?](https://wiki.osdev.org/Why_do_I_need_a_Cross_Compiler%3F)

Um compilador de uma linguagem de alto nível geralmente produz código para a plataforma (hardware/sistema operacional) que ele está executando. Tendo em vista que este trabalho está sendo produzido em processadores de arquitetura 64 bits e sistemas operacionais com núcleos Linux/OpenBSD, utilizar os compiladores que executam nessas plataformas para gerar código para o novo sistema operacional causa diversos problemas. Portanto, se faz necessária a utilização de um compilador cruzado (Cross-Compile). Um compilador cruzado gera código para uma plataforma diferente da que ele está sendo executado. Para este projeto, o compilador utilizado executa em plataformas de 64 bits Intel/AMD com núcleos Linux/OpenBSD e gera código para a plataforma Intel 80386 (x86). Vale ressaltar que não é especificado o sistema operacional alvo que o compilador cruzado gerará código, pois o objetivo é construir um. Dessa maneira, flags passadas para o compilador indicam esse cenário.

### Compilador cruzado GCC

Quando o código do compilador [GCC](https://gcc.gnu.org/) é compilado, diversas flags são definidas (`gcc -v` para visualizar as flags). Para que não seja necessária a alteração dessas flags no momento da compilação do sistema operacional, a compilação de um novo GCC, com as flags ideais, é preferível. A criação de um compilador cruzado requer o download do código fonte do compilador, definição das flags e compilação do compilador. [i686-elf-gcc-AUR](https://aur.archlinux.org/packages/i686-elf-gcc/) e [i686-elf-binutils-AUR](https://aur.archlinux.org/packages/i686-elf-binutils/) disponibilizam uma forma de obtenção do compilador cruzado para usuários da distribuição Arch Linux e outras distribuições derivadas. Com o objetivo de disponibilizar para outras plataformas, um script escrito em shell, denominado _scripts/i686-elf-install.sh_, foi criado para realizar a construção do compilador cruzado. Este script recebe como parâmetro as versões do GCC e do GNU binutils (conjunto de ferramentas auxiliares utilizado no processo de compilação e linking) desejáveis. A lista de versões do GCC são encontradas em [gnu-ftp-gcc](https://ftp.gnu.org/gnu/gcc/), e a lista de versões do GNU binutils podem ser encontradas em [gnu-ftp-binutils](https://ftp.gnu.org/gnu/binutils/).

Implementado com base em [osdev-cross-compile](https://wiki.osdev.org/GCC_Cross-Compiler), o script baixa os códigos necessários e os compila com as seguintes flags: `--disable-nls` é opcional, mas reduz dependências e tempo de compilação pois ela indica ao GCC e ao binutils não incluírem suporte à língua nativa;`--with-sysroot` habilita o suporte à sysroot, permitindo a mudança do diretório raiz utilizado para localizar arquivos; `--without-headers` informa ao GCC que as bibliotecas C podem não estar presentes para a compilação; `--enable-languages=c,c++` indica que o GCC construído suportará somente linguagens C e C++; `--target=i686-elf` indica que o resultado da compilação padrão utilizando o compilador cruzado será binários ELF para processadores i386; `--prefix=\$PWD/i686-elf/opt/cross` indica que todos os arquivos do compilador cruzado estarão nesse diretório.

Para usuários do OpenBSD que optarem por utilizar o GCC, é recomendado que utilizem os códigos presentes no ports como base.

### Compilador cruzado CLANG

[CLANG](https://clang.llvm.org/) é o compilador de linguagens da família C do projeto [LLVM](https://www.llvm.org/). Por padrão o CLANG é um cross-compile, facilitando o trabalho de compilação de códigos para diferentes plataformas. Esta facilidade, juntamente com a maior compatibilidade com o OpenBSD, fez com que escolhecemos o CLANG como compilador principal, mas também vamos demonstrar como utilizar o GCC.

## QEMU

O [QEMU](https://www.qemu.org/) é utilizado para emular todo o harwere necessário para execução do sistema operacional. Como o processador alvo escolhido para o desenvolvimento do sistema operacional foi o i386, a versão `qemu-system-i386` foi utilizada.

QEMU é um software livre que pode desempenhar o papel de emulador de hardware ou de um virtualizador. Quando executado como emulador de hardware, o QEMU permite, a partir de tradução dinâmica, emular diferentes máquinas. Como virtualizador, o QEMU pode ser executado juntamente com o [xen hypervisor](https://wiki.xenproject.org/wiki/Xen_Project_Software_Overview) ou com o [KVM](https://www.linux-kvm.org/page/Main_Page), permitindo a utilização dos recursos de virtualização do processador.

## GNU MAKE

O [GNU Make](https://www.gnu.org/software/make/) é uma ferramenta que controla a geração de códigos executáveis e outros códigos não fontes de um programa, a partir dos seus códigos fontes. Considerando que a quantidade de comandos a se executar para compilar e executar o sistema operacional cresce na proporção que as funcionalidades são adicionadas, o GNU Make se torna bastante útil em organizar este processo. O GNU Make necessita de um arquivo que define suas regras e suas ações chamado _Makefile_. O Makefile disponibiliza diversas macros que que auxiliam no processo de compilação, tornando-o mais organizado.

## NASM

[NASM](https://www.nasm.us/) é o Assembler utilizado neste projeto, tendo em vista que a sintaxe escolhida para os códigos Assembly foi a Intel.
